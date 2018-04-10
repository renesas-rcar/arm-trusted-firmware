/*
 * Copyright (c) 2015-2018, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <arch_helpers.h>
#include <string.h>
#include <mmio.h>
#include "rcar_def.h"
#include "bl2_cpg_register.h"
#include "bl2_cpg_init.h"
#include "bl2_dma_register.h"
#include "dma_driver.h"
#include "debug.h"
#include "rcar_private.h"

#define	DMA_USE_CHANNEL		(0x00000001U)	/* DMA CH setting (0/16/32) */
#define	DMAOR_INITIAL		(0x0301U)	/* PR[1:0]=11, DME=1 */
#define	DMACHCLR_CH_ALL		(0x0000FFFFU)	/* CLR[15:0] */
#define	DMAFIXDAR_32BIT_SHIFT	(32U)
#define	DMAFIXDAR_DAR_MASK	(0x000000FFU)	/* DAR[39:32] */
#define	DMADAR_BOUNDARY_ADDR	(0x100000000ULL)
#define	DMATCR_CNT_SHIFT	(6U)		/* TS is 64-byte units */
#define	DMATCR_MAX		(0x00FFFFFFU)	/* Max of TCR */
#define	DMACHCR_TRN_MODE	(0x00105409U)	/* TS=0101, DM=01, SM=01, RS=0100, DE=1 */
#define	DMACHCR_DE_BIT		(0x00000001U)	/* DMA Enable */
#define	DMACHCR_TE_BIT		(0x00000002U)	/* Transfer End Flag */
#define	DMACHCR_CHE_BIT		(0x80000000U)	/* Channel Address Error Flag */

#define	DMA_SIZE_UNIT		FLASH_TRANS_SIZE_UNIT	/* transfer size units */
#define	DMA_FRACTION_MASK	(0xFFU)		/* fraction mask for 256-byte units */
#define DMA_DST_LIMIT		(0x10000000000ULL)	/* 40bit address area */
/* transfer length limit */
#define DMA_LENGTH_LIMIT	((DMATCR_MAX \
				* ((uint32_t)1U << DMATCR_CNT_SHIFT)) \
				& ~((uint32_t)DMA_FRACTION_MASK))

static void enableDMA(void);
static void setupDMA(void);
static void startDMA(uintptr_t dst, uint32_t src, uint32_t len);
static void endDMA(void);


static void enableDMA(void)
{
	/* Enable clock supply to DMAC. */
	mstpcr_write(CPG_SMSTPCR2, CPG_MSTPSR2, SYS_DMAC_BIT);
}

static void setupDMA(void)
{
	/* DMA operation */
	mmio_write_16(DMA_DMAOR,0x0000U);
	/* DMA channel clear */
	mmio_write_32(DMA_DMACHCLR,DMACHCLR_CH_ALL);
}

static void startDMA(uintptr_t dst, uint32_t src, uint32_t len)
{
	/* DMA operation */
	mmio_write_16(DMA_DMAOR,DMAOR_INITIAL);
	/* DMA fixed destination address */
	mmio_write_32(DMA_DMAFIXDAR,
		(uint32_t)((dst >> DMAFIXDAR_32BIT_SHIFT) &
					DMAFIXDAR_DAR_MASK));
	/* DMA destination address */
	mmio_write_32(DMA_DMADAR,
		(uint32_t)(dst & UINT32_MAX));
	/* DMA source address */
	mmio_write_32(DMA_DMASAR,src);
	/* DMA 64bytes-unit transfer count */
	mmio_write_32(DMA_DMATCR,len >> DMATCR_CNT_SHIFT);
	/* DMA DMA Secure Control Register */
	mmio_write_32(DMA_DMASEC,DMA_USE_CHANNEL);
	/* DMA channel control */
	mmio_write_32(DMA_DMACHCR,DMACHCR_TRN_MODE);
}

static void endDMA(void)
{
	/* DMA channel control */
	while((mmio_read_32(DMA_DMACHCR) & DMACHCR_TE_BIT) == 0x00000000U) {
		/* DMA channel control */
		if ((mmio_read_32(DMA_DMACHCR) & DMACHCR_CHE_BIT) != 0U) {
			ERROR("BL2: DMA - Channel Address Error\n");
			panic();
			break;
		}
	}
	/* DMA transfer Disable*/
	mmio_write_32(DMA_DMACHCR,
		mmio_read_32(DMA_DMACHCR) & ~((uint32_t)DMACHCR_DE_BIT));
	while ((mmio_read_32(DMA_DMACHCR) & DMACHCR_DE_BIT) != 0U) {
	}
	/* DMA DMA Secure Control Register */
	mmio_write_32(DMA_DMASEC,0x00000000U);
	/* DMA operation */
	mmio_write_16(DMA_DMAOR,0x0000U);
	/* DMA channel clear */
	mmio_write_32(DMA_DMACHCLR,DMA_USE_CHANNEL);
}

void initDMA(void)
{
	enableDMA();
	setupDMA();
}

/* execDMA */
void execDMA(uintptr_t dst, uint32_t src, uint32_t len)
{
	uint32_t	dmalen;

	if ((len & DMA_FRACTION_MASK) != 0U) {
		dmalen = (len + DMA_SIZE_UNIT) & ~((uint32_t)DMA_FRACTION_MASK);
	} else {
		dmalen = len;
	}

	if (((src + dmalen) < src) ||
		((src & DMA_FRACTION_MASK) != 0U)) {
		/* source address invalid */
		ERROR("BL2: DMA - Source address invalid\n" \
		      "           source address  = 0x%x\n" \
		      "           transfer length = 0x%x\n",
			src, dmalen);
		panic();
	}
	if ((dst >= DMA_DST_LIMIT) ||
	    ((dst + (uintptr_t)dmalen) > DMA_DST_LIMIT) ||
		(((dst & UINT32_MAX) + dmalen) > DMADAR_BOUNDARY_ADDR) ||
		((dst & DMA_FRACTION_MASK) != 0U)) {
		/* destination address invalid */
		ERROR("BL2: DMA - Destination address invalid\n" \
		      "           destination address = 0x%lx\n" \
		      "           transfer length     = 0x%x\n",
			dst, dmalen);
		panic();
	}
	if ((dmalen > DMA_LENGTH_LIMIT) || (dmalen == 0U)) {
		/* transfer length exceeded the limit or zero. */
		ERROR("BL2: DMA - Transfer size invalid\n" \
		      "           transfer length     = 0x%x\n",
			dmalen);
		panic();
	}

	startDMA(dst, src, dmalen);
	endDMA();
}
