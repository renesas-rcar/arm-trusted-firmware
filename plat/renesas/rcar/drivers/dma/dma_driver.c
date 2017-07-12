/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Renesas nor the names of its contributors may be
 *     used to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <string.h>
#include <mmio.h>
#include "rcar_def.h"
#include "bl2_cpg_register.h"
#include "bl2_cpg_init.h"
#include "bl2_dma_register.h"
#include "dma_driver.h"
#include "debug.h"


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
	mmio_write_32(DMA_DMACHCLR,0x0000FFFFU);
	mmio_write_32(DMA_DMACHCLR,0x00000000U);
}

static void startDMA(uintptr_t dst, uint32_t src, uint32_t len)
{
	/* DMA operation */
	mmio_write_16(DMA_DMAOR,0x0301U);
	/* DMA fixed destination address */
	mmio_write_32(DMA_DMAFIXDAR,
		(uint32_t)((dst >> 32ULL) & 0x000000FFULL));
	/* DMA destination address */
	mmio_write_32(DMA_DMADAR,
		(uint32_t)(dst & 0x0FFFFFFFFULL));
	/* DMA source address */
	mmio_write_32(DMA_DMASAR,src);
	/* DMA 64bytes-unit transfer count */
	mmio_write_32(DMA_DMATCR,len >> 6);
	/* DMA DMA Secure Control Register */
	mmio_write_32(DMA_DMASEC,0x00000001U);
	/* DMA channel control */
	mmio_write_32(DMA_DMACHCR,0x00105409U);
}

static void endDMA(void)
{
	/* DMA channel control */
	while((mmio_read_32(DMA_DMACHCR) & 0x00000002U) == 0x00000000U) {
		/* DMA channel control */
		if ((mmio_read_32(DMA_DMACHCR) & 0x80000000U) != 0U) {
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
	mmio_write_32(DMA_DMACHCLR,0x00000001U);
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
	    ((len == 0U) && ((src + 0x40000000U) < src))) {
		/* source address invalid */
		if (len == 0U) {
			len = 0x40000000U;
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
	     ((dst + 0x40000000ULL) >= DRAM_LIMIT))) {
		/* destination address invalid */
		if (len == 0U) {
			len = 0x40000000U;
		}
		ERROR("BL2: DMA - Destination address invalid\n" \
		      "           destination address = 0x%lx\n," \
		      "           transfer length     = 0x%x\n",
			dst, len);
		panic();
	}
	if (((dst & 0x3FU) != 0U) || ((src & 0x3FU) != 0U) ||
		((len & 0xC0000000U) != 0U)) {
		/* dst or src are not 64-byte alignment. */
		dmalen = 0U;
		memlen = len;
	} else {
		/* dst and src are 64-byte alignment. */
		dmalen = len & 0x3FFFFFC0U;
		memlen = len & 0x3FU;
	}
	if (dmalen != 0U) {
		dst_l = dst & 0x0FFFFFFFFULL;
		if ((dst_l + (uintptr_t)dmalen) >= 0x100000000ULL) {
			/* transfer will over than the DMADAR range. */
			/* divide dma transfer */
			divlen = (uint32_t)(0x100000000ULL - dst_l);
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
			dmalen = 0x40000000U;
			dst_l = dst & 0x0FFFFFFFFULL;
			if ((dst_l + (uintptr_t)dmalen) >= 0x100000000ULL) {
				/* transfer will over than the DMADAR range. */
				/* divide dma transfer */
				divlen = (uint32_t)(0x100000000ULL - dst_l);
				startDMA(dst, src, divlen);
				endDMA();
				dst += (uintptr_t)divlen;
				src += divlen;
				dmalen -= divlen;
			}
			startDMA(dst, src, dmalen & 0x3FFFFFFFU);
			endDMA();
			dst += dmalen;
			src += dmalen;
		}
	}
	if (memlen != 0U) {
		(void)memcpy((void*)dst,
			(const void*)((uintptr_t)src),
				(size_t)memlen);
	}
}
