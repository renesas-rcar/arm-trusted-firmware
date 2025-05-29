/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef SCIF_REGISTER_H_
#define SCIF_REGISTER_H_

/* SCIF ch0 base address */
#define SCIF_BASE   (0xC0700000U)

#define SCIF_SCSMR  (SCIF_BASE + 0x0000U)   /*  16  Serial mode register */
#define SCIF_SCBRR  (SCIF_BASE + 0x0004U)   /*  8   Bit rate register */
#define SCIF_SCSCR  (SCIF_BASE + 0x0008U)   /*  16  Serial control register */
#define SCIF_SCFTDR (SCIF_BASE + 0x000CU)   /*  8   Transmit FIFO data register */
#define SCIF_SCFSR  (SCIF_BASE + 0x0010U)   /*  16  Serial status register */
#define SCIF_SCFCR  (SCIF_BASE + 0x0018U)   /*  16  FIFO control register */
#define SCIF_SCFDR  (SCIF_BASE + 0x001CU)   /*  16  FIFO Data count register (palladium debug) */
#define SCIF_SCLSR  (SCIF_BASE + 0x0024U)   /*  16  Line status register */
#define SCIF_CKS    (SCIF_BASE + 0x0034U)   /*  16  Clock Select register */

#endif /* SCIF_REGISTER_H_ */
