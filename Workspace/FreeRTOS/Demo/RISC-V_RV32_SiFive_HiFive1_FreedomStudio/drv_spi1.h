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

//typedef struct {
//    volatile uint32_t sckdiv;           /* 0x00 Serial clock divisor register */
//    volatile uint32_t sckmode;          /* 0x04 Serial clock mode register */
//    volatile uint32_t reserved0[2];     /* 0x08 ~ 0x0c Reserved  */
//    volatile uint32_t csid;             /* 0x10 Chip select ID register */
//    volatile uint32_t csdef;            /* 0x14 Chip select default register */
//    volatile uint32_t csmode;           /* 0x18 Chip select mode register */
//    volatile uint32_t reserved1[3];     /* 0x1c ~ 0x24 Reserved  */
//    volatile uint32_t delay0;           /* 0x28 Delay control 0 register */
//    volatile uint32_t delay1;           /* 0x2c Delay control 1 register */
//    volatile uint32_t reserved2[4];     /* 0x30 ~ 0x3c Reserved  */
//    volatile uint32_t fmt;              /* 0x40 Frame format register */
//    volatile uint32_t reserved3[1];     /* 0x44 Reserved  */
//    volatile uint32_t txdata;           /* 0x48 Tx FIFO Data register */
//    volatile uint32_t rxdata;           /* 0x4c Rx FIFO data register */
//    volatile uint32_t txmark;           /* 0x50 Tx FIFO watermark register */
//    volatile uint32_t rxmark;           /* 0x54 Rx FIFO watermark register */
//    volatile uint32_t reserved4[2];     /* 0x58 ~ 0x5c Reserved  */
//    volatile uint32_t fctrl;            /* 0x60 SPI flash interface control register */
//    volatile uint32_t ffmt;             /* 0x64 SPI flash instruction format register */
//    volatile uint32_t reserved5[2];     /* 0x68 ~ 0x6c Reserved  */
//    volatile uint32_t ie;               /* 0x70 SPI interrupt enable register */
//    volatile uint32_t ip;               /* 0x74 SPI interrupt pending register */
//} SPI_RegDef;
//#define SPI1_BASE               (volatile uint32_t*)(0x10024000)
//#define DEV_SPI1              ((SPI_RegDef *)  SPI1_BASE)


/* Function declaration */
#include <string.h>
void driver_spi1_init(void);
int driver_spi1_transfer(size_t len, char *tx_buf, char *rx_buf);

#endif /* DRV_SPI1_H_ */
