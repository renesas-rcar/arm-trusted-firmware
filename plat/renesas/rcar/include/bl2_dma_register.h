/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BL2_DMA_REGISTER_H__
#define BL2_DMA_REGISTER_H__

#if RCAR_LSI == RCAR_V3M	/* V3M */
#define	DMACH	16	/* DMA CH setting (0/16/32) */
#else
#define	DMACH	0	/* DMA CH setting (0/16/32) */
#endif

#if (DMACH==0)		/* SYS-DMAC0 (CH0) */
#define SYS_DMAC_BIT	((uint32_t)1U << 19U)
#define DMA_BASE	(0xE6700000U)
#elif (DMACH==16)	/* SYS-DMAC1 (CH16) */
#define SYS_DMAC_BIT	((uint32_t)1U << 18U)
#define DMA_BASE	(0xE7300000U)
#elif (DMACH==32)	/* SYS-DMAC2 (CH32) */
#define SYS_DMAC_BIT	((uint32_t)1U << 17U)
#define DMA_BASE	(0xE7320000U)
#else
#define SYS_DMAC_BIT	((uint32_t)1U << 19U)
#define DMA_BASE	(0xE6700000U)
#endif

/* DMA operation */
#define DMA_DMAOR	(DMA_BASE + 0x0060U)
/* DMA secure control */
#define	DMA_DMASEC	(DMA_BASE + 0x0030U)
/* DMA channel clear */
#define DMA_DMACHCLR	(DMA_BASE + 0x0080U)
/* DMA source address */
#define DMA_DMASAR	(DMA_BASE + 0x8000U)
/* DMA destination address */
#define DMA_DMADAR	(DMA_BASE + 0x8004U)
/* DMA transfer count */
#define DMA_DMATCR	(DMA_BASE + 0x8008U)
/* DMA channel control */
#define DMA_DMACHCR	(DMA_BASE + 0x800CU)
/* DMA fixed destination address */
#define DMA_DMAFIXDAR	(DMA_BASE + 0x8014U)

#endif	/* BL2_DMA_REGISTER_H__ */
