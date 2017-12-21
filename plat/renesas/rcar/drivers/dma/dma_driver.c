/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
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

#define	DMA_USE_CHANNEL		(0x00000001U)	/* DMA CH setting (0/16/32) */
#define	DMAOR_INITIAL		(0x0301U)	/* PR[1:0]=11, DME=1 */
#define	DMACHCLR_CH_ALL		(0x0000FFFFU)	/* CLR[15:0] */
#define	DMAFIXDAR_32BIT_SHIFT	(32U)
#define	DMAFIXDAR_DAR_MASK	(0x000000FFU)	/* DAR[39:32] */
#define	DMADAR_BOUNDARY_ADDR	(0x100000000ULL)
#define	DMATCR_CNT_SHIFT	(6U)		/* TS is 64-byte units */
#define	DMACHCR_TRN_MODE	(0x00105409U)	/* TS=0101, DM=01, SM=01, RS=0100, DE=1 */
#define	DMACHCR_TE_BIT		(0x00000002U)	/* Transfer End Flag */
#define	DMACHCR_CHE_BIT		(0x80000000U)	/* Channel Address Error Flag */

#define DMA_LENGTH_LIMIT	(0x40000000U)
#define DMA_LENGTH_MASK		(0x3FFFFFFFU)
#define DMA_LEN_ALIGN_MASK	(0x3FFFFFC0U)
#define	DMA_FRACTION_MASK	(0x3FU)
#define DMA_EXCEED_LEN_LIMIT	(0xC0000000U)

static void enableDMA(void);
static void setupDMA(void);
static void startDMA(uintptr_t dst, uint32_t src, uint32_t len);
static void endDMA(void);


static void enableDMA(void)
{
	/* Is the clock supply to the CPG disabled ? */
	while((mmio_read_32(CPG_MSTPSR2) & SYS_DMAC_BIT) != 0U) {
		/* Enables the clock supply to the CPG. */
		cpg_write(CPG_SMSTPCR2,
			mmio_read_32(CPG_SMSTPCR2) & (~SYS_DMAC_BIT));
	}
}

static void setupDMA(void)
{
	/* DMA operation */
	mmio_write_16(DMA_DMAOR,0x0000U);
	/* DMA channel clear */
	mmio_write_32(DMA_DMACHCLR,DMACHCLR_CH_ALL);
	mmio_write_32(DMA_DMACHCLR,0x00000000U);
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
	/* DMA DMA Secure Control Register */
	mmio_write_32(DMA_DMASEC,0x00000000U);
	/* DMA operation */
	mmio_write_16(DMA_DMAOR,0x0000U);
	/* DMA channel clear */
	mmio_write_32(DMA_DMACHCLR,DMA_USE_CHANNEL);
	mmio_write_32(DMA_DMACHCLR,0x00000000U);
}

void initDMA(void)
{
	enableDMA();
	setupDMA();
}

/* execDMA */
/* note) Parameter len is interpret 0x40000000, If len is 0. */
void execDMA(uintptr_t dst, uint32_t src, uint32_t len)
{
	uint32_t	dmalen = 0U;
	uint32_t	memlen = 0U;
	uintptr_t	dst_l = 0U;
	uint32_t	divlen = 0U;

	/* fail safe */
	if (((src + len) < src) ||
	    ((len == 0U) && ((src + DMA_LENGTH_LIMIT) < src))) {
		/* source address invalid */
		if (len == 0U) {
			len = DMA_LENGTH_LIMIT;
		}
		ERROR("BL2: DMA - Source address invalid\n" \
		      "           source address  = 0x%x\n," \
		      "           transfer length = 0x%x\n",
			src, len);
		panic();
	}
	/* fail safe */
	if ((dst >= DRAM_LIMIT) ||
	    ((dst + (uintptr_t)len) >= DRAM_LIMIT) ||
	    ((len == 0U) &&
	     ((dst + DMA_LENGTH_LIMIT) >= DRAM_LIMIT))) {
		/* destination address invalid */
		if (len == 0U) {
			len = DMA_LENGTH_LIMIT;
		}
		ERROR("BL2: DMA - Destination address invalid\n" \
		      "           destination address = 0x%lx\n," \
		      "           transfer length     = 0x%x\n",
			dst, len);
		panic();
	}
	if (((dst & DMA_FRACTION_MASK) != 0U) ||
		((src & DMA_FRACTION_MASK) != 0U) ||
		((len & DMA_EXCEED_LEN_LIMIT) != 0U)) {
		/* dst or src are not 64-byte alignment. */
		dmalen = 0U;
		memlen = len;
	} else {
		/* dst and src are 64-byte alignment. */
		dmalen = len & DMA_LEN_ALIGN_MASK;
		memlen = len & DMA_FRACTION_MASK;
	}
	if (dmalen != 0U) {
		dst_l = dst & UINT32_MAX;
		if ((dst_l + (uintptr_t)dmalen) >= DMADAR_BOUNDARY_ADDR) {
			/* transfer will over than the DMADAR range. */
			/* divide dma transfer */
			divlen = (uint32_t)(DMADAR_BOUNDARY_ADDR - dst_l);
			startDMA(dst, src, divlen);
			endDMA();
			dst += (uintptr_t)divlen;
			src += divlen;
			dmalen -= divlen;
		}
		startDMA(dst, src, dmalen);
		endDMA();
		dst += dmalen;
		src += dmalen;
	} else {
		if (memlen == 0U) {
			dmalen = DMA_LENGTH_LIMIT;
			dst_l = dst & UINT32_MAX;
			if ((dst_l + (uintptr_t)dmalen) >= DMADAR_BOUNDARY_ADDR) {
				/* transfer will over than the DMADAR range. */
				/* divide dma transfer */
				divlen = (uint32_t)(DMADAR_BOUNDARY_ADDR - dst_l);
				startDMA(dst, src, divlen);
				endDMA();
				dst += (uintptr_t)divlen;
				src += divlen;
				dmalen -= divlen;
			}
			startDMA(dst, src, dmalen & DMA_LENGTH_MASK);
			endDMA();
			dst += dmalen;
			src += dmalen;
		}
	}
	if (memlen != 0U) {
		(void)memcpy((void*)dst,
			(const void*)((uintptr_t)src),
				(size_t)memlen);
#if RCAR_BL2_DCACHE == 1
		flush_dcache_range(dst, (size_t)memlen);
#endif /* RCAR_BL2_DCACHE == 1 */
	}
}
