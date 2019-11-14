/*
 * drv_esp32.c
 *
 *  Created on: Nov 7, 2019
 *      Author: HoangSHC
 */

#include "drv_esp32.h"
#include "drv_spi1.h"
#include "debug_serial.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#define delay_ms(ms)    vTaskDelay( ( TickType_t )ms / portTICK_PERIOD_MS ) //delay ms

void drv_esp32_init(void)
{
	drr_esp32_init();
}

int drv_esp32_send(const char* cmd)
{
	int send_len = strlen(cmd);
	char at_flag_buf[4] = {0x02, 0x00, 0x00, 0x00};
	char len_buf[4] = {0x00, 0x00, 0x00, 'A'};
	debug_puts(cmd);

	/* Send SEND request */
	driver_spi1_transfer(4, at_flag_buf, 0);
	delay_ms(5);

	/* Slice the length if it over than 127 */
    if (send_len <= ESP_AT_BUFFER_SIZE) {
        len_buf[0] = send_len & 127;
        len_buf[1] = send_len >> 7;
        len_buf[3] = 'A'; // check info
    } else {
        return -1; // Too large data
    }

	/* Send data length */
	driver_spi1_transfer(4, len_buf, 0);
	delay_ms(5);

	/* Send data */
	driver_spi1_transfer(send_len, cmd, 0);
	delay_ms(15);
	return 0;
}

//int drv_esp32_is_ready(int wait_ms)
//{
//	TickType_t endwait = xTaskGetTickCount() + wait_ms;
//    while (!(gpio0_input_val & WIFI_INT_PIN_MASK)) {
//       if (xTaskGetTickCount() > endwait) {
//           return 0; /* If timeout */
//       }
//    }
//	return 1;
//}

int drv_esp32_recv(char* recvbuf, int* len)
{
	char at_flag_buf[4] = {0x01, 0x00, 0x00, 0x00};
	char len_buf[4] = {0x00, 0x00, 0x00, 0x00};
	int recv_len = 0;

	/* Wait for handsake pin is set to high */
//	if(drv_esp32_is_ready(500) == 0)
//	{
//		/* re-Send SEND request */
//		delay_ms(50);
//		at_flag_buf[0] = 0x2;
//		driver_spi1_transfer(4, at_flag_buf, 0);
//		delay_ms(50);
//	}

	/* Send RECV request */
	at_flag_buf[0] = 0x1;
	if(driver_spi1_transfer(4, at_flag_buf, 0) != 0)
	{
		return -1;
	}
	delay_ms(5);

	/* Receive data length */
	if(driver_spi1_transfer(4, 0, len_buf) != 0)
	{
		return -1;
	}
	delay_ms(5);

	/* Check info */
	if(len_buf[3] == 'B')
	{
		recv_len = (len_buf[1] << 7) + len_buf[0];
	}

	/* Receive data */
	if (recv_len < ESP_AT_BUFFER_SIZE) {
		if(driver_spi1_transfer(recv_len, 0, recvbuf) != 0)
		{
			return -1;
		}
		recvbuf[recv_len] = 0;
//		debug_puts(recvbuf);
	} else {
		debug_puts("recv_len over size\n");
	}

	// return length
	if(len) {
		*len = recv_len;
	}
	delay_ms(15);
	return 0;
}
