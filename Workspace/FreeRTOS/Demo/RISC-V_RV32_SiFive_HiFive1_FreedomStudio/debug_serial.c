/*
 * debug_serial.c
 *
 *  Created on: Nov 4, 2019
 *      Author: HoangSHC
 */

// SiFive HiFive1 Revb1
#include <metal/machine/platform.h>
#include <metal/drivers/sifive_uart0.h>
#include <metal/uart.h>
#include <metal/machine.h>

void debug_init(void)
{
	struct metal_uart *uart;
	uart = &__metal_dt_serial_10013000.uart;
	metal_uart_init(uart, 115200);
}

//void debug_putc(int c)
//{
//    metal_uart_putc((struct metal_uart *)&__metal_dt_serial_10013000.uart, (unsigned char) c);
//    // __metal_driver_sifive_uart0_putc((struct metal_uart *)&__metal_dt_serial_10013000.uart, (unsigned char) c);
//}

void debug_putc(int c)
{
#define uart0_txdata    (*(volatile uint32_t*)(0x10013000))
#define UART_TXFULL             (1 << 31)
    while ((uart0_txdata & UART_TXFULL) != 0) { }
    uart0_txdata = c;
}

int debug_puts(const char *s)
{
	int len = 0;

	while (*s) {
		debug_putc(*s);
		s++;
		len++;
	}
	return len;
}
