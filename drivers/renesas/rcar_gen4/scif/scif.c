/*
 * Copyright (c) 2021, Renesas Electronics Corporation. All rights reserved.
 */

#include <stdint.h>
#include <stddef.h>
#include <lib/mmio.h>
#include <drivers/console.h>
#include "rcar_def.h"
#include "scif.h"

/* RST */
#define RST_BASE		(UL(0xE6160000) + (RCAR_DOMAIN * UL(0x4000)))
#define RST_MODEMR0		RST_BASE
#define RST_MODEMR1		(RST_BASE + UL(4))
#define RST_MODEMR0_MD31	((uint32_t)(1) << U(31))
#define RST_MODEMR1_MD32	((uint32_t)(1) << U(0))

/* SCIF/HSCIF */
#define SCIF0_BASE		UL(0xE6E60000)
#define SCIF3_BASE		UL(0xE6C50000)
#define HSCIF0_BASE		UL(0xE6540000)
#define SCIF_SCFSR_TEND		(uint16_t)((uint16_t)1U << 6U)
#define SCIF_SCFSR_TDFE		(uint16_t)((uint16_t)1U << 5U)
#define TRANS_END_CHECK		(uint16_t)(SCIF_SCFSR_TEND | SCIF_SCFSR_TDFE)

/* SCIF */
#if (RCAR_LSI == RCAR_S4) /* S4 */
#define SCIF_BASE	SCIF3_BASE
#else
#define SCIF_BASE	SCIF0_BASE
#endif
#define SCIF_SCFTDR	(SCIF_BASE + 0x000CU)	/*  8 Transmit FIFO data register */
#define SCIF_SCFSR	(SCIF_BASE + 0x0010U)	/* 16 Serial status register */

/* HSCIF */
#define HSCIF_BASE	HSCIF0_BASE
#define HSCIF_HSFTDR	(HSCIF_BASE + 0x000CU) /*  8 Transmit FIFO data register */
#define HSCIF_HSFSR	(HSCIF_BASE + 0x0010U) /* 16 Serial status register */

/* mode */
#define MODEMR_SCIF_DLMODE		U(0x00000000)
#define MODEMR_HSCIF_DLMODE_921600	U(0x00000001)
#define MODEMR_HSCIF_DLMODE_1843200	U(0x00000002)
#define MODEMR_HSCIF_DLMODE_3000000	U(0x00000003)

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

	uint32_t modemr;

	modemr = (((mmio_read_32(RST_MODEMR0) & RST_MODEMR0_MD31) >> 31U)
		| ((mmio_read_32(RST_MODEMR1) & RST_MODEMR1_MD32) << 1U));

	scif_console_init(modemr);

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
	case MODEMR_HSCIF_DLMODE_3000000:
	case MODEMR_HSCIF_DLMODE_1843200:
	case MODEMR_HSCIF_DLMODE_921600:
		/* Set the pointer to a function that outputs one character. */
		rcar_putc = hscif_console_putc;
		break;
	case MODEMR_SCIF_DLMODE:
	default:
		/* Set the pointer to a function that outputs one character. */
		rcar_putc = scif_console_putc;
		break;
	}
}

static void scif_console_putc(uint8_t outchar)
{
	uint16_t reg;

	/* Check that transfer of SCIF is completed */
	while (!((TRANS_END_CHECK & mmio_read_16(SCIF_SCFSR)) == TRANS_END_CHECK)) {
		;
	}

	mmio_write_8(SCIF_SCFTDR, outchar); /* Transfer one character */
	reg = mmio_read_16(SCIF_SCFSR);
	reg &= (uint16_t) (~(TRANS_END_CHECK)); /* TEND,TDFE clear */
	mmio_write_16(SCIF_SCFSR, reg);

	/* Check that transfer of SCIF is completed */
	while (!((TRANS_END_CHECK & mmio_read_16(SCIF_SCFSR)) == TRANS_END_CHECK)) {
		;
	}
}

static void hscif_console_putc(uint8_t outchar)
{
	uint16_t reg;

	/* Check that transfer of HSCIF is completed */
	while (!((TRANS_END_CHECK & mmio_read_16(HSCIF_HSFSR)) == TRANS_END_CHECK)) {
		;
	}

	mmio_write_8(HSCIF_HSFTDR, outchar); /* Transfer one character */
	reg = mmio_read_16(HSCIF_HSFSR);
	reg &= (uint16_t) (~(TRANS_END_CHECK)); /* TEND,TDFE clear */
	mmio_write_16(HSCIF_HSFSR, reg);

	/* Check that transfer of HSCIF is completed */
	while (!((TRANS_END_CHECK & mmio_read_16(HSCIF_HSFSR)) == TRANS_END_CHECK)) {
		;
	}
}
