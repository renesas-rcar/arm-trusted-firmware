/*
 * Copyright (c) 2015-2016, Renesas Electronics Corporation
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
#include "bl2_cpg_register.h"
#include "bl2_cpg_write.h"
#include "bl2_dma_register.h"
#include "dma_driver.h"
#include "debug.h"

static void regdump(void);

static void enableDMA(void);
static void setupDMA(void);
static void startDMA(uint32_t dst, uint32_t src, uint32_t len);
static void endDMA(void);
#if DEBUG
static void disableDMA(void);
#endif

static void regdump(void)
{
#if DEBUG
	INFO("BL2:\n"
		"%s(0x%x) : 0x%x\n"
		"%s(0x%x) : 0x%x\n"
		"%s(0x%x) : 0x%x\n"
		"%s(0x%x) : 0x%x\n"
		"%s(0x%x) : 0x%x\n"
		"%s(0x%x) : 0x%x\n"
		"%s(0x%x) : 0x%x\n"
		"%s(0x%x) : 0x%x\n"
		"%s(0x%x) : 0x%x\n"
		"%s(0x%x) : 0x%x\n",
		"CPG_SMSTPCR2",CPG_SMSTPCR2,mmio_read_32(CPG_SMSTPCR2),
		"CPG_SRCR2   ",CPG_SRCR2,mmio_read_32(CPG_SRCR2),
		"CPG_MSTPSR2 ",CPG_MSTPSR2,mmio_read_32(CPG_MSTPSR2),
		"DMAOR2      ",DMA_DMAOR2,mmio_read_16(DMA_DMAOR2),
		"DMASEC2     ",DMA_DMASEC2,mmio_read_32(DMA_DMASEC2),
		"DMACHCLR2   ",DMA_DMACHCLR2,mmio_read_32(DMA_DMACHCLR2),
		"DMASAR32    ",DMA_DMASAR32,mmio_read_32(DMA_DMASAR32),
		"DMADAR32    ",DMA_DMADAR32,mmio_read_32(DMA_DMADAR32),
		"DMATCR32    ",DMA_DMATCR32,mmio_read_32(DMA_DMATCR32),
		"DMACHCR32   ",DMA_DMACHCR32,mmio_read_32(DMA_DMACHCR32)
	);
#endif
}

static void enableDMA(void)
{
	/* Is the clock supply to the CPG disabled ? */
	while((mmio_read_32(CPG_MSTPSR2) & SYS_DMAC2_BIT) != 0U) {
		/* Enables the clock supply to the CPG. */
		cpg_write(CPG_SMSTPCR2,
			mmio_read_32(CPG_SMSTPCR2) & (~SYS_DMAC2_BIT));
	}
}

static void setupDMA(void)
{
	/* DMA operation 2 */
	mmio_write_16(DMA_DMAOR2,0x0000U);
	/* DMA channel clear 2 */
	mmio_write_32(DMA_DMACHCLR2,0x0000FFFFU);
	mmio_write_32(DMA_DMACHCLR2,0x00000000U);
}

static void startDMA(uint32_t dst, uint32_t src, uint32_t len)
{
	/* DMA operation 2 */
	mmio_write_16(DMA_DMAOR2,0x0301U);
	/* DMA destination address 32 */
	mmio_write_32(DMA_DMADAR32,dst);
	/* DMA source address 32 */
	mmio_write_32(DMA_DMASAR32,src);
	/* DMA 64bytes-unit transfer  count 32 */
	mmio_write_32(DMA_DMATCR32,len >> 6);
	/* DMA channel control 32 */
	mmio_write_32(DMA_DMACHCR32,0x00105409U);
}

static void endDMA(void)
{
	/* DMA channel control 32 */
	while((mmio_read_32(DMA_DMACHCR32) & 0x00000002U) == 0x00000000U) {
		/* DMA channel control 32 */
		if ((mmio_read_32(DMA_DMACHCR32) & 0x80000000U) != 0U) {
			ERROR("BL2: DMA - Channel Address Error\n");
			regdump();
			break;
		}
	}
	/* DMA operation 2 */
	mmio_write_16(DMA_DMAOR2,0x0000U);
	/* DMA channel clear 2 */
	mmio_write_32(DMA_DMACHCLR2,0x00000001U);
	mmio_write_32(DMA_DMACHCLR2,0x00000000U);
}

#if DEBUG
static void disableDMA(void)
{
	/* Disable the clock supply to the CPG. */
	cpg_write(CPG_SMSTPCR2,
		mmio_read_32(CPG_SMSTPCR2) | SYS_DMAC2_BIT);
}
#endif

void initDMA(void)
{
	enableDMA();
	setupDMA();
}

void execDMA(uint32_t dst, uint32_t src, uint32_t len)
{
	uint32_t dmalen;
	uint32_t memlen;

	if (((dst & 0x3FU) != 0U) || ((src & 0x3FU) != 0U) ||
		((len & 0xC0000000U) != 0U)) {
		/* dst or src are not 64-bit alignment. */
		dmalen = 0U;
		memlen = len;
	} else {
		/* dst and src are 64-bit alignment. */
		dmalen = len & 0x3FFFFFC0U;
		memlen = len & 0x3FU;
	}
	if (dmalen != 0U) {
		startDMA(dst, src, dmalen);
		endDMA();
		dst += dmalen;
		src += dmalen;
	} else {
		if (memlen == 0U) {
			startDMA(dst, src, 0U);
			endDMA();
			dmalen = 0x01000000U << 6;
			dst += dmalen;
			src += dmalen;
		}
	}
	if (memlen != 0U) {
		(void)memcpy((void*)(uint64_t)dst,
			(const void*)(uint64_t)src,
				(size_t)memlen);
	}
	if (dmalen != 0U) {
		INFO("BL2: DMA    len=%d(0x%x)\n",
			dmalen, dmalen);
	}
	if (memlen != 0U) {
		INFO("BL2: memcpy len=%d(0x%x)\n",
			memlen, memlen);
	}
}

#if DEBUG
void termDMA(void)
{
	disableDMA();
}
#endif
