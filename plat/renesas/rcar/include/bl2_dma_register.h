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

#ifndef BL2_DMA_REGISTER_H__
#define BL2_DMA_REGISTER_H__

#define	DMACH	0	/* DMA CH setting (0/16/32) */

#if (DMACH==0)		/* SYS-DMAC0 (CH0) */
#define SYS_DMAC2_BIT	(0x00080000U)
#define DMA_BASE	(0xE6700000U)
#elif (DMACH==16)	/* SYS-DMAC1 (CH16) */
#define SYS_DMAC2_BIT	(0x00040000U)
#define DMA_BASE	(0xE7300000U)
#elif (DMACH==32)	/* SYS-DMAC2 (CH32) */
#define SYS_DMAC2_BIT	(0x00020000U)
#define DMA_BASE	(0xE7320000U)
#else
#define SYS_DMAC2_BIT	(0x00080000U)
#define DMA_BASE	(0xE6700000U)
#endif

/* DMA operation 0 */
#define DMA_DMAOR2	(DMA_BASE + 0x0060U)
/* DMA secure control */
#define	DMA_DMASEC2	(DMA_BASE + 0x0030U)
/* DMA channel clear */
#define DMA_DMACHCLR2	(DMA_BASE + 0x0080U)
/* DMA source address */
#define DMA_DMASAR32	(DMA_BASE + 0x8000U)
/* DMA destination address */
#define DMA_DMADAR32	(DMA_BASE + 0x8004U)
/* DMA transfer count */
#define DMA_DMATCR32	(DMA_BASE + 0x8008U)
/* DMA channel control */
#define DMA_DMACHCR32	(DMA_BASE + 0x800CU)

#endif	/* BL2_DMA_REGISTER_H__ */

