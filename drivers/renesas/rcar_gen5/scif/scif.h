/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SCIF_H
#define SCIF_H

int console_rcar_register(uintptr_t base, uint32_t clk, uint32_t baud,
		console_t *pconsole);
int console_rcar_init(uintptr_t base_addr, uint32_t uart_clk,
		uint32_t baud_rate);
int console_rcar_putc(int c, console_t *pconsole);
int console_rcar_flush(console_t *pconsole);

/* Define  */
#define SCIF_SCSCR_TE_EN        (uint16_t)((uint16_t)1U << 5U)
#define SCIF_SCSCR_RE_EN        (uint16_t)((uint16_t)1U << 4U)
#define SCIF_SCSCR_CKE_EXT_CLK  (uint16_t)((uint16_t)2U << 0U)
#define SCIF_SCSCR_INIT_DATA    (uint16_t)(SCIF_SCSCR_TE_EN | SCIF_SCSCR_RE_EN)
#define SCIF_SCSCR_HW_INIT      (uint16_t)(0x0000U)

#define SCIF_SCFCR_TFRST_EN     (uint16_t)((uint16_t)1U << 2U)
#define SCIF_SCFCR_RFRS_EN      (uint16_t)((uint16_t)1U << 1U)
#define SCIF_SCFCR_RESET_FIFO   (uint16_t)(SCIF_SCFCR_TFRST_EN | SCIF_SCFCR_RFRS_EN)

#define SCIF_SCFSR_TEND         (uint16_t)((uint16_t)1U << 6U)
#define SCIF_SCFSR_TDFE         (uint16_t)((uint16_t)1U << 5U)
#define TRANS_END_CHECK         (uint16_t)(SCIF_SCFSR_TEND | SCIF_SCFSR_TDFE)
#define SCIF_SCFSR_INIT_DATA    (uint16_t)(0x0000U)

#define SCIF_SCLSR_INIT_DATA    (uint16_t)(0x0000U)

#define SCIF_SCSMR_CHR          (uint16_t)((uint16_t)1U << 6U)
#define SCIF_SCSMR_PE           (uint16_t)((uint16_t)1U << 5U)
#define SCIF_SCSMR_STOP         (uint16_t)((uint16_t)1U << 3U)
#define SCIF_SCSMR_CKS          (uint16_t)((uint16_t)3U << 0U)
#define SCIF_SCSMR_INIT_DATA    ~((uint16_t)(SCIF_SCSMR_CHR | SCIF_SCSMR_PE \
					     | SCIF_SCSMR_STOP | SCIF_SCSMR_CKS))

/* Pclk(66MHz)/1, 115.2kbps*/
/* N = 66/(66/2*115200)*10^4-1 =17=> 0x11 */
#define SCIF_SCBRR_115200BPS    (uint8_t)(0x11U)
/* Pclk(266MHz)/1, 921.6kbps*/
/* N = 266/(8*2*921600)*10^6-1 =17=> 0x11 */
#define HSCIF_SCBRR_921600BPS   (uint8_t)(0x11U)
/* Pclk(266MHz)/1, 1.8432Mbps*/
/* N = 266/(8*2*1843200)*10^6-1 =8=> 0x08 */
#define HSCIF_SCBRR_1843200BPS  (uint8_t)(0x08U)

#define HSCIF_HSSRR_SRE         (uint16_t)(1U << 15U)
#define HSCIF_HSSRR_SRCYC       (uint16_t)(0x1FU << 0U)
#define HSCIF_HSSRR_SRCYC8      (uint16_t)(7U << 0U)    /* Sampling rate 8-1 */
#define HSCIF_HSSRR_VAL         (uint16_t)(HSCIF_HSSRR_SRE | HSCIF_HSSRR_SRCYC8)

#define HSCIF_DL_DIV1           (uint16_t)(1U << 0U)
#define HSCIF_CKS_CKS           (uint16_t)(1U << 15U)
#define HSCIF_CKS_XIN           (uint16_t)(1U << 14U)
#define HSCIF_CKS_SC_CLK_EXT    ~((uint16_t)(HSCIF_CKS_CKS | HSCIF_CKS_XIN))

#endif /* SCIF_H */
