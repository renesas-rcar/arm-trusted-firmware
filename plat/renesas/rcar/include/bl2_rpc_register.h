/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BL2_RPC_REGISTER_H__
#define BL2_RPC_REGISTER_H__

/* RPC base address */
#define RPC_BASE	(0xEE200000U)

/* RPC common control */
#define RPC_CMNCR	(RPC_BASE + 0x0000U)
/* RPC ssl delay */
#define RPC_SSLDR	(RPC_BASE + 0x0004U)
/* RPC data read control */
#define RPC_DRCR	(RPC_BASE + 0x000CU)
/* RPC data read command setting */
#define RPC_DRCMR	(RPC_BASE + 0x0010U)
/* RPC data read enable setting */
#define RPC_DRENR	(RPC_BASE + 0x001CU)

/* RPC manual mode control */
#define RPC_SMCR	(RPC_BASE + 0x0020U)
/* RPC manual mode command setting */
#define RPC_SMCMR	(RPC_BASE + 0x0024U)
/* RPC manual mode enable setting */
#define RPC_SMENR	(RPC_BASE + 0x0030U)

/* RPC common status */
#define RPC_CMNSR	(RPC_BASE + 0x0048U)
/* RPC data read dummy cycle setting */
#define RPC_DRDMCR	(RPC_BASE + 0x0058U)
/* RPC data read ddr enable */
#define RPC_DRDRENR	(RPC_BASE + 0x005CU)

/* RPC phy control */
#define RPC_PHYCNT	(RPC_BASE + 0x007CU)
/* RPC phy interrupt */
#define RPC_PHYINT	(RPC_BASE + 0x0088U)

#endif	/* BL2_RPC_REGISTER_H__ */
