/*
 * FreeRTOS Kernel V10.2.1
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/******************************************************************************
 * NOTE 1:  This project provides two demo applications.  A simple blinky
 * style project, and a more comprehensive test and demo application.  The
 * mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting in main.c is used to select
 * between the two.  See the notes on using mainCREATE_SIMPLE_BLINKY_DEMO_ONLY
 * in main.c.  This file implements the simply blinky style version.
 *
 * NOTE 2:  This file only contains the source code that is specific to the
 * blinky demo.  Generic functions, such FreeRTOS hook functions, and functions
 * required to configure the hardware are defined in main.c.
 ******************************************************************************
 *
 * main_blinky() creates one queue, and two tasks.  It then starts the
 * scheduler.
 *
 * The Queue Send Task:
 * The queue send task is implemented by the prvQueueSendTask() function in
 * this file.  prvQueueSendTask() sits in a loop that causes it to repeatedly
 * block for 1000 milliseconds, before sending the value 100 to the queue that
 * was created within main_blinky().  Once the value is sent, the task loops
 * back around to block for another 1000 milliseconds...and so on.
 *
 * The Queue Receive Task:
 * The queue receive task is implemented by the prvQueueReceiveTask() function
 * in this file.  prvQueueReceiveTask() sits in a loop where it repeatedly
 * blocks on attempts to read data from the queue that was created within
 * main_blinky().  When data is received, the task checks the value of the
 * data, and if the value equals the expected 100, toggles an LED.  The 'block
 * time' parameter passed to the queue receive function specifies that the task
 * should be held in the Blocked state indefinitely to wait for data to be
 * available on the queue.  The queue receive task will only leave the Blocked
 * state when the queue send task writes to the queue.  As the queue send task
 * writes to the queue every 1000 milliseconds, the queue receive task leaves
 * the Blocked state every 1000 milliseconds, and therefore toggles the LED
 * every 200 milliseconds.
 */

/* Standard includes. */
//#include <stdio.h> // use printf in stdio.h cause error
#include <string.h>
#include <unistd.h>

#include <metal/led.h>
#include <metal/gpio.h>

//#include <metal/uart.h> // serial uart
//#include <metal/machine.h> // serial uart

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "esp32_AT.h"
#include "debug_serial.h"
/* Priorities used by the tasks. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define	mainQUEUE_SEND_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue.  The 200ms value is converted
to ticks using the pdMS_TO_TICKS() macro. */
#define mainQUEUE_SEND_FREQUENCY_MS			pdMS_TO_TICKS( 1000 )

/* The maximum number items the queue can hold.  The priority of the receiving
task is above the priority of the sending task, so the receiving task will
preempt the sending task and remove the queue items each time the sending task
writes to the queue.  Therefore the queue will never have more than one item in
it at any time, and even with a queue length of 1, the sending task will never
find the queue full. */
#define mainQUEUE_LENGTH					( 1 )


#define led_on(x)   metal_led_off(x) // metal_led goes wrong
#define led_off(x)   metal_led_on(x)

enum {
	LED_OFF,
	LED_B_ON,
	LED_G_ON,
	LED_R_ONE,
};
/*-----------------------------------------------------------*/

/*
 * Called by main when mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is set to 1 in
 * main.c.
 */
void main_blinky( void );

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvQueueReceiveTask( void *pvParameters );
static void prvQueueSendTask( void *pvParameters );
static void get_IPD(char* buf);
static int receive_1line(char* dst, char* src, size_t len);
static int process_message(char* msg, int msg_len);
static void init_led(void);
static void led_ctrl(const char led_buf[4]);

/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

static struct metal_led *pxRedLED = NULL;
static struct metal_led *pxGreenLED = NULL;
static struct metal_led *pxBlueLED = NULL;
static struct metal_led *toggleLED = NULL;

static char s_msg[512];
static char s_led_buf[4];
static char s_idf_len[8];
/*-----------------------------------------------------------*/

void main_blinky( void )
{
	/* Create the queue. */
	xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( uint32_t ) );

    init_led();
	esp32_create_tasks();
	if( xQueue != NULL )
	{
		/* Start the two tasks as described in the comments at the top of this
		file. */
		xTaskCreate( prvQueueReceiveTask,				/* The function that implements the task. */
					"Rx", 								/* The text name assigned to the task - for debug only as it is not used by the kernel. */
					configMINIMAL_STACK_SIZE, 			/* The size of the stack to allocate to the task. */
					NULL, 								/* The parameter passed to the task - not used in this case. */
					mainQUEUE_RECEIVE_TASK_PRIORITY, 	/* The priority assigned to the task. */
					NULL );								/* The task handle is not required, so NULL is passed. */

		xTaskCreate( prvQueueSendTask, "TX", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK_PRIORITY, NULL );

		/* Start the tasks and timer running. */
		vTaskStartScheduler();
	}

	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was insufficient FreeRTOS heap memory available for the Idle and/or
	timer tasks to be created.  See the memory management section on the
	FreeRTOS web site for more details on the FreeRTOS heap
	http://www.freertos.org/a00111.html. */
	for( ;; );
}
/*-----------------------------------------------------------*/
static void prvQueueSendTask( void *pvParameters )
{
TickType_t xNextWakeTime;
const unsigned long ulValueToSend = 100UL;
BaseType_t xReturned;

	/* Remove compiler warning about unused parameter. */
	( void ) pvParameters;

	/* Initialise xNextWakeTime - this only needs to be done once. */
	xNextWakeTime = xTaskGetTickCount();

	memset(s_msg, 0, sizeof(s_msg));

	esp32_reset_module();  // reset esp32
	esp32_echo_off();  // switch echo off
	esp32_send_cmd("AT\r\n", 100);  // test AT
	esp32_wifi_conn(WIFI_SSID, WIFI_PASS); // connect to wifi

	esp32_send_cmd("AT+CIPSTART=\"TCP\",\"hifivedemo.s3-ap-southeast-1.amazonaws.com\",80,2\r\n", 1200); // connect to AWS

	esp32_send_cmd("AT+CIPSEND=76\r\n", 100);
	memset(s_msg, 0, sizeof(s_msg));
	esp32_recv(0,0); // clear esp32 queue
	esp32_send_cmd("GET /data.txt HTTP/1.1\r\nHost: hifivedemo.s3-ap-southeast-1.amazonaws.com\r\n\r\n", 2000);

	esp32_recv(s_msg, sizeof(s_msg)); // get esp32 queue
	process_message(s_msg, sizeof(s_msg));

	esp32_send_cmd("AT+CIPCLOSE\r\n", 100);

	led_ctrl(s_led_buf);
	vTaskDelay( ( TickType_t )1000 / portTICK_PERIOD_MS ); //delay ms

	for( ;; )
	{
//		esp32_send_cmd("AT\r\n", 400);
//		memset(s_msg, 0, sizeof(s_msg));
//		esp32_recv(s_msg, 100);
////		debug_puts("main>");debug_puts(s_msg);

///////////////
//		memset(s_msg, 0, sizeof(s_msg));
//		esp32_send_cmd("AT+CIPSTART=\"TCP\",\"hifivedemo.s3-ap-southeast-1.amazonaws.com\",80,2\r\n", 5000);
//
//		esp32_send_cmd("AT+CIPSEND=76\r\n", 100);
//		esp32_recv(0,0); // clear queue
//		esp32_send_cmd("GET /data.txt HTTP/1.1\r\nHost: hifivedemo.s3-ap-southeast-1.amazonaws.com\r\n\r\n", 2000);
//
//		esp32_recv(s_msg, sizeof(s_msg)); // get esp32 msg
//		process_message(s_msg, sizeof(s_msg));
//		esp32_send_cmd("AT+CIPCLOSE\r\n", 100);
//
//		vTaskDelay( ( TickType_t )1000 / portTICK_PERIOD_MS ); //delay ms
//		led_ctrl(s_led_buf);
////////////////
		/* Place this task in the blocked state until it is time to run again. */
		vTaskDelayUntil( &xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS/2 );

		/* Send to the queue - causing the queue receive task to unblock and
		toggle the LED.  0 is used as the block time so the sending operation
		will not block - it shouldn't need to block as the queue should always
		be empty at this point in the code. */
		xReturned = xQueueSend( xQueue, &ulValueToSend, 0U );
		configASSERT( xReturned == pdPASS );
	}
}
/*-----------------------------------------------------------*/

static void prvQueueReceiveTask( void *pvParameters )
{
unsigned long ulReceivedValue;
const unsigned long ulExpectedValue = 100UL;
extern void vToggleLED( void );

	/* Remove compiler warning about unused parameter. */
	( void ) pvParameters;

	for( ;; )
	{
		/* Wait until something arrives in the queue - this task will block
		indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
		FreeRTOSConfig.h. */
		xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );

		/*  To get here something must have been received from the queue, but
		is it the expected value?  If it is, toggle the LED. */
		if( ulReceivedValue == ulExpectedValue )
		{
//			vToggleLED();
			metal_led_toggle( toggleLED );
			ulReceivedValue = 0U;
		}
	}
}

static void get_IPD(char* buf)
{//"+IPD,4:LEDB
	char* c = (char*)&buf[4]; // ',4:LEDB'
	char* start_buf = c+1; // '4:LEDB'
	int len = 0;
	len = 0;

	memset(s_idf_len, 0, sizeof(s_idf_len));
	memset(s_led_buf, 0, sizeof(s_led_buf));

	while (*++c != ':') { len++; }
	*c = 0; /* null character manually added before calling strcpy*/
	strncpy(s_idf_len, (char*)&start_buf[0], len); // 4

	c++;

	len = atoi(s_idf_len);
	start_buf = c;
	*(c + len) = 0; /* null character manually added before calling strcpy*/
	strncpy(s_led_buf, (char*)&start_buf[0], len); // LEDB
}

// Get 1 line in receive buffer
static int receive_1line(char* dst, char* src, size_t len)
{
    size_t i = 0;
    dst[0] = 0;
    for (i = 0; i < len; i++) {
		dst[i] = src[i];
		if(dst[i] == '\n')
		{
			i++;
			break;
		}
		if(dst[i] == 0)
		{
			break;
		}
    }
    dst[i] = 0;
    return i;
}

static int process_message(char* msg, int msg_len)
{
    // Parse AT response to get the result
    int count;
    char *s = msg;
    int len = msg_len;

    do {
    	char tmp[100];// = "+IPD,4:LEDB";
    	count = receive_1line(tmp, s , len);
    	if(count == 0)
    		break;

    	s += count; // move to next line

        // Process each line
       /* if (memcmp(tmp, "OK", 2) == 0) {
        	debug_puts("***OK***\n");
        } else if (memcmp(tmp, "ERROR", 5) == 0) {
        	debug_puts("***ERROR***\n");
        } else if (memcmp(tmp, "ERR CODE:", 9) == 0) {
        	debug_puts("***ERR CODE***\n");
        	// ERR CODE:0x010b0000
        	// ERR CODE:0x01030000
        } else*/ if (memcmp(tmp, "+IPD,4:", 7) == 0)
        {
        	get_IPD(tmp);
        }
        len -= count;
    }
    while (len > 0);
    return 0;
}

static void init_led(void)
{
	/* Initialise the red LED. */
	pxRedLED = metal_led_get_rgb( "LD0", "red" );
	configASSERT( pxRedLED );
	metal_led_enable( pxRedLED );
	metal_led_off( pxRedLED );

    /* Initialise the green LED. */
	pxGreenLED = metal_led_get_rgb( "LD0", "green" );
	configASSERT( pxGreenLED );
	metal_led_enable( pxGreenLED );
	metal_led_off( pxGreenLED );
    
    /* Initialise the blue LED. */
	pxBlueLED = metal_led_get_rgb( "LD0", "blue" );
	configASSERT( pxBlueLED );
	metal_led_enable( pxBlueLED );
	metal_led_off( pxBlueLED );

}

static void led_ctrl(const char led_buf[4])
{
	if(memcmp(led_buf, "LEDR", 4) == 0)
	{
		debug_puts("LED Red on\n");
		led_on( pxRedLED );
		led_off( pxGreenLED );
		led_off( pxBlueLED );
		toggleLED = pxRedLED;
	}
	else if(memcmp(led_buf, "LEDG", 4) == 0)
	{
		debug_puts("LED Green on\n");
		led_off( pxRedLED );
		led_on( pxGreenLED );
		led_off( pxBlueLED );
		toggleLED = pxGreenLED;
	}
	else if(memcmp(led_buf, "LEDB", 4) == 0)
	{
		debug_puts("LED Blue on\n");
		led_off( pxRedLED );
		led_off( pxGreenLED );
		led_on( pxBlueLED );
		toggleLED = pxBlueLED;
	}
	else
	{
		debug_puts("LEDs off\n");
		led_off( pxRedLED );
		led_off( pxGreenLED );
		led_off( pxBlueLED );
		toggleLED = NULL;
	}
}
/*-----------------------------------------------------------*/

