/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stddef.h>
#include <lib/mmio.h>
#include <drivers/console.h>
#include <drivers/delay_timer.h>
#include "rcar_def.h"
#include "scif.h"
#include "mfis.h"

#include "hscif_register.h"
#include "scif_register.h"

/* RST */
/* RCarX5H_TS_RegisterDescription_Operating_Mode_20230920.xlsm */
#define RST_MODEMR0         (0xC1321000U)    /* Mode0 */
#define RST_MODEMR1         (0x34000400U)    /* Mode1 (palladium DF1.5 special)*/
#define RST_MODEMR1_MD10_MD09    (0x3U << 9U)

/* SCIF/HSCIF */
#define SCIF_SCFSR_TEND		(uint16_t)((uint16_t)1U << 6U)
#define SCIF_SCFSR_TDFE		(uint16_t)((uint16_t)1U << 5U)
#define TRANS_END_CHECK		(uint16_t)(SCIF_SCFSR_TEND | SCIF_SCFSR_TDFE)

/* SCIF / HSCIF clock speed */
#define MODEMR_SCIF_DLMODE          (0x00000000U)
#define MODEMR_HSCIF_DLMODE_921600  (0x00000200U)
#define MODEMR_HSCIF_DLMODE_1843200 (0x00000400U)
#define MODEMR_HSCIF_DLMODE_3000000 (0x00000600U)

/* CR/LF char code */
#define RCAR_CHR_CR	(0x0A)
#define RCAR_CHR_LF	(0x0D)

static void scif_console_init(uint32_t modemr);

static void scif_console_putc(uint8_t outchar);
static void hscif_console_putc(uint8_t outchar);

static void (*rcar_putc)(uint8_t outchar);

int console_rcar_init(uintptr_t base_addr, uint32_t uart_clk,
		uint32_t baud_rate)
{
	(void)base_addr;
	(void)uart_clk;
	(void)baud_rate;

	rcar_mfis_init();
	scif_console_init(MODEMR_HSCIF_DLMODE_3000000); /* force use HSCIF */

	return 1;
}

int console_rcar_putc(int c, console_t *pconsole)
{
	(void)pconsole;

	int ret = -1;

	if (rcar_putc != NULL) {
		rcar_putc((uint8_t)c);
		if (c == RCAR_CHR_CR) {
			/* add 'LF' after 'CR' */
			rcar_putc(RCAR_CHR_LF);
		}
		ret = c;
	}

	return (ret);
}

int console_rcar_flush(console_t *pconsole)
{
	(void)pconsole;
	/* Nothing to do */
	return 0;
}


static void scif_console_init(uint32_t modemr)
{
	switch (modemr) {
	case MODEMR_HSCIF_DLMODE_3000000: /* 0x00000600U */
	case MODEMR_HSCIF_DLMODE_1843200: /* 0x00000400U */
	case MODEMR_HSCIF_DLMODE_921600:  /* 0x00000200U */
		/* Set the pointer to a function that outputs one character. */
		rcar_putc = hscif_console_putc;
		break;
	case MODEMR_SCIF_DLMODE: /* 0x00000000 */
		/* Set the pointer to a function that outputs one character. */
		rcar_putc = scif_console_putc;
		break;
	default:
		break;
	}
}
static void scif_console_putc(uint8_t outchar)
{
	static uint8_t remain;

	/* Check that transfer of SCIF0 is completed */
	while (remain == 0) {
		remain = 128-(mmio_read_16(SCIF_SCFDR)>>8);
		if (remain < 64) {
			remain = 0;
		} else {
			remain -= 64;
		}
	}

	mmio_write_8(SCIF_SCFTDR, outchar);  /* Transfer one character */
	remain--;
}
/* End of function scif_console_putc(uint8_t outchar) */

static void hscif_console_putc(uint8_t outchar)
{
	static uint8_t remain;

	/* Check that transfer of SCIF0 is completed */
	while (remain == 0) {
		remain = 128-(mmio_read_16(HSCIF_HSFDR)>>8);
		if (remain < 64) {
			remain = 0;
		} else {
			remain -= 64;
		}
	}

	mmio_write_8(HSCIF_HSFTDR, outchar);  /* Transfer one character */
	remain--;
}
/* End of function hscif_console_putc(uint8_t outchar) */
