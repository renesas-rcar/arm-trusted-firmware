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
