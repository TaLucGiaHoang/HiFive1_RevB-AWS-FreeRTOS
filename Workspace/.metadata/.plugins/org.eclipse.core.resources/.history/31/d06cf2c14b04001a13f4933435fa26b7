/*
 * drv_spi1.h
 *
 *  Created on: Nov 5, 2019
 *      Author: HoangSHC
 */

#ifndef DRV_SPI1_H_
#define DRV_SPI1_H_


/* Register fields */
#define METAL_SPI_SCKDIV_MASK         0xFFF

#define METAL_SPI_SCKMODE_PHA_SHIFT   0
#define METAL_SPI_SCKMODE_POL_SHIFT   1

#define METAL_SPI_CSMODE_MASK         3
#define METAL_SPI_CSMODE_AUTO         0
#define METAL_SPI_CSMODE_HOLD         2
#define METAL_SPI_CSMODE_OFF          3

#define METAL_SPI_PROTO_MASK          3
#define METAL_SPI_PROTO_SINGLE        0
#define METAL_SPI_PROTO_DUAL          1
#define METAL_SPI_PROTO_QUAD          2

#define METAL_SPI_ENDIAN_LSB          4

#define METAL_SPI_DISABLE_RX          8

#define METAL_SPI_FRAME_LEN_SHIFT     16
#define METAL_SPI_FRAME_LEN_MASK      (0xF << METAL_SPI_FRAME_LEN_SHIFT)

#define METAL_SPI_TXDATA_FULL         (1 << 31)
#define METAL_SPI_RXDATA_EMPTY        (1 << 31)
#define METAL_SPI_TXMARK_MASK         7
#define METAL_SPI_TXWM                1
#define METAL_SPI_TXRXDATA_MASK       (0xFF)

#define METAL_SPI_INTERVAL_SHIFT      16

#define METAL_SPI_CONTROL_IO          0
#define METAL_SPI_CONTROL_MAPPED      1

#define METAL_SPI_RXDATA_TIMEOUT      1

#define SPI1_FREQUENCY 79207 // reset spi1 frequency

/* WF_INT pin */
#define WF_INT	10
#define WIFI_INT_PIN_MASK (1 << 10)

/* SPI1 Register Address */
#define spi1_txdata            (*(volatile uint32_t*)(0x10024048))
#define spi1_rxdata            (*(volatile uint32_t*)(0x1002404C))
#define spi1_csmode            (*(volatile uint32_t*)(0x10024018))
// #define gpio_output_val        (*(volatile uint32_t*)(0x1001200C))
// #define spi1_csdef             (*(volatile uint32_t*)(0x10024014))
#define spi1_delay0            (*(volatile uint32_t*)(0x10024028))
#define spi1_txmark            (*(volatile uint32_t*)(0x10024050))
#define spi1_rxmark            (*(volatile uint32_t*)(0x10024054))
#define spi1_ie                (*(volatile uint32_t*)(0x10024070))
#define spi1_ip                (*(volatile uint32_t*)(0x10024074))
#define spi1_delay0            (*(volatile uint32_t*)(0x10024028))
#define spi1_txmark            (*(volatile uint32_t*)(0x10024050))
#define spi1_rxmark            (*(volatile uint32_t*)(0x10024054))
#define spi1_ie                (*(volatile uint32_t*)(0x10024070))
#define spi1_ip                (*(volatile uint32_t*)(0x10024074))

/* GPIO0 Register Address use to control WF_INT pin */
#define gpio0_input_val        (*(volatile uint32_t*)(0x10012000)) // Pin value register
#define gpio0_input_en         (*(volatile uint32_t*)(0x10012004)) // Pin input enable register

/* Function declaration */
#include <string.h>
void driver_spi1_init(void);
int driver_spi1_transfer(size_t len, char *tx_buf, char *rx_buf);

#endif /* DRV_SPI1_H_ */
