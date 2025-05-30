/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "uart.h"
#include <lib/mmio.h>

/* UART0 (PL011) */
#define UART0_BASE              (0xE5C00000)
#define UARTDR                  (UART0_BASE + 0x00U)
#define UARTFR                  (UART0_BASE + 0x18U)
#define UARTIBRD                (UART0_BASE + 0x24U)
#define UARTFBRD                (UART0_BASE + 0x28U)
#define UARTLCR_H               (UART0_BASE + 0x2CU)
#define UARTCR                  (UART0_BASE + 0x30U)
#define UARTIFLS                (UART0_BASE + 0x34U)
#define UARTIMSC                (UART0_BASE + 0x38U)
#define UARTMIS                 (UART0_BASE + 0x40U)
#define UARTDMACR               (UART0_BASE + 0x48U)

// MIS REGISTER
#define UART_OEMIS              0x0400
#define UART_BEMIS              0x0200
#define UART_PEMIS              0x0100
#define UART_FEMIS              0x0080
#define UART_RTMIS              0x0040
#define UART_TXMIS              0x0020
#define UART_RXMIS              0x0010
#define UART_DSRMMIS            0x0008
#define UART_DCDMMIS            0x0004
#define UART_CTSMMIS            0x0002
#define UART_RIMMIS             0x0001
#define UART_NO_INT_PENDING     0x0000

//  IMSC REGISTER
#define UART_OEIM               0x0400
#define UART_BEIM               0x0200
#define UART_PEIM               0x0100
#define UART_FEIM               0x0080
#define UART_RTIM               0x0040
#define UART_TXIM               0x0020
#define UART_RXIM               0x0010
#define UART_DSRMIM             0x0008
#define UART_DCDMIM             0x0004
#define UART_CTSMIM             0x0002
#define UART_RIMIM              0x0001
#define UART_DISABLED           0x0000

// FR REGISTER
#define UART_RI_ACTIVE          0x0100
#define UART_RI_INACTIVE        0x0000
#define UART_TX_FIFO_EMPTY      0x0080
#define UART_RX_FIFO_FULL       0x0040
#define UART_TX_FIFO_FULL       0x0020
#define UART_RX_FIFO_EMPTY      0x0010
#define UART_BUSY               0x0008
#define UART_DCD_DETECTED       0x0004
#define UART_DSR_DETECTED       0x0002
#define UART_CTS_DETECTED       0x0001

// DMACR REGISTER
#define UART_TX_DMA_ENABLE      0x2
#define UART_TX_DMA_DISABLE     0x0
#define UART_RX_DMA_ENABLE      0x1
#define UART_RX_DMA_DISABLE     0x0

//  ICR Register
#define UART_OEIC               0x0400
#define UART_BEIC               0x0200
#define UART_PEIC               0x0100
#define UART_FEIC               0x0080
#define UART_RTIC               0x0040
#define UART_TXIC               0x0020
#define UART_RXIC               0x0010
#define UART_DSRMIC             0x0008
#define UART_DCDMIC             0x0004
#define UART_CTSMIC             0x0002
#define UART_RIMIC              0x0001

void uart_init()
{
    mmio_write_16(UARTIBRD, 0x10U);
    mmio_write_8(UARTFBRD, 0x10U);
    mmio_write_8(UARTLCR_H, 0x70U);
    mmio_write_16(UARTCR, (UART_BEIC | UART_PEIC | UART_RIMIC));
    mmio_write_8(UARTIFLS, 0x00U);
    mmio_write_8(UARTDMACR, (UART_TX_DMA_DISABLE | UART_RX_DMA_DISABLE));
    mmio_write_16(UARTIMSC, (UART_TXIM | UART_RXIM));
}

void uart_console_putc(uint8_t outchar)
{
    uint16_t reg;

    do {
        /* Wait unitl send */
        reg = mmio_read_16(UARTFR);
    } while ((reg & UART_TX_FIFO_FULL) == UART_TX_FIFO_FULL);

    mmio_write_8(UARTDR, outchar);

    if('\n' == outchar)
    {
        /* Convert line feed code from LF to LF + CR */
        uart_console_putc('\r');
    }
}

uint8_t uart_console_getc()
{
    uint16_t reg1;
    uint8_t LucChar = 0;

    do {
        /* Wait until received */
        reg1 = mmio_read_16(UARTFR);
    } while ((reg1 & UART_RX_FIFO_EMPTY) == UART_RX_FIFO_EMPTY);

    LucChar = mmio_read_8(UARTDR);

    if ('\r' == LucChar)
    {
        /* Convert line feed code from CR to LF */
        LucChar = '\n';
    }

    /* Print to console */
    uart_console_putc(LucChar);

    return LucChar;
}

uint8_t uart_console_scan_rd(void)
{
    uint16_t    reg;
    uint8_t     result;

    /* Check RDF flag */
    reg = mmio_read_16(UARTFR);
    if ((reg & UART_RX_FIFO_EMPTY) != UART_RX_FIFO_EMPTY)
    {
        result = 1u;
    }
    else
    {
        result = 0u;
    }

    return result;
}

