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

#ifndef	BL2_LIFEC_REGISTER_H__
#define	BL2_LIFEC_REGISTER_H__

/* LIFEC0 (SECURITY) registers */

/* LIFEC0 (SECURITY) base address */
#define	LIFEC_SEC_BASE	(0xE6110000U)

/* Security attribute setting for master ports */
#define	SEC_SRC		(LIFEC_SEC_BASE + 0x0008U)
/* Security attribute setting for slave ports 0 */
#define	SEC_SEL0	(LIFEC_SEC_BASE + 0x0030U)
/* Security attribute setting for slave ports 1 */
#define	SEC_SEL1	(LIFEC_SEC_BASE + 0x0034U)
/* Security attribute setting for slave ports 2 */
#define	SEC_SEL2	(LIFEC_SEC_BASE + 0x0038U)
/* Security attribute setting for slave ports 3 */
#define	SEC_SEL3	(LIFEC_SEC_BASE + 0x003CU)
/* Security attribute setting for slave ports 4 */
#define	SEC_SEL4	(LIFEC_SEC_BASE + 0x0058U)
/* Security attribute setting for slave ports 5 */
#define	SEC_SEL5	(LIFEC_SEC_BASE + 0x005CU)
/* Security attribute setting for slave ports 6 */
#define SEC_SEL6	(LIFEC_SEC_BASE + 0x0060U)
/* Security attribute setting for slave ports 7 */
#define	SEC_SEL7	(LIFEC_SEC_BASE + 0x0064U)
/* Security attribute setting for slave ports 8 */
#define	SEC_SEL8	(LIFEC_SEC_BASE + 0x0068U)
/* Security attribute setting for slave ports 9 */
#define	SEC_SEL9	(LIFEC_SEC_BASE + 0x006CU)
/* Security attribute setting for slave ports 10 */
#define	SEC_SEL10	(LIFEC_SEC_BASE + 0x0070U)
/* Security attribute setting for slave ports 11 */
#define	SEC_SEL11	(LIFEC_SEC_BASE + 0x0074U)
/* Security attribute setting for slave ports 12 */
#define	SEC_SEL12	(LIFEC_SEC_BASE + 0x0078U)
/* Security attribute setting for slave ports 13 */
#define	SEC_SEL13	(LIFEC_SEC_BASE + 0x007CU)
/* Security attribute setting for slave ports 14 */
#define	SEC_SEL14	(LIFEC_SEC_BASE + 0x0080U)
/* Security attribute setting for slave ports 15 */
#define	SEC_SEL15	(LIFEC_SEC_BASE + 0x0084U)
/* Security group 0 attribute setting for master ports 0 */
#define	SEC_GRP0CR0	(LIFEC_SEC_BASE + 0x0138U)
/* Security group 1 attribute setting for master ports 0 */
#define	SEC_GRP1CR0	(LIFEC_SEC_BASE + 0x013CU)
/* Security group 0 attribute setting for master ports 1 */
#define	SEC_GRP0CR1	(LIFEC_SEC_BASE + 0x0140U)
/* Security group 1 attribute setting for master ports 1 */
#define	SEC_GRP1CR1	(LIFEC_SEC_BASE + 0x0144U)
/* Security group 0 attribute setting for master ports 2 */
#define	SEC_GRP0CR2	(LIFEC_SEC_BASE + 0x0148U)
/* Security group 1 attribute setting for master ports 2 */
#define	SEC_GRP1CR2	(LIFEC_SEC_BASE + 0x014CU)
/* Security group 0 attribute setting for master ports 3 */
#define	SEC_GRP0CR3	(LIFEC_SEC_BASE + 0x0150U)
/* Security group 1 attribute setting for master ports 3 */
#define	SEC_GRP1CR3	(LIFEC_SEC_BASE + 0x0154U)
/* Security group 0 attribute setting for slave ports 0 */
#define	SEC_GRP0COND0	(LIFEC_SEC_BASE + 0x0158U)
/* Security group 1 attribute setting for slave ports 0 */
#define	SEC_GRP1COND0	(LIFEC_SEC_BASE + 0x015CU)
/* Security group 0 attribute setting for slave ports 1 */
#define	SEC_GRP0COND1	(LIFEC_SEC_BASE + 0x0160U)
/* Security group 1 attribute setting for slave ports 1 */
#define	SEC_GRP1COND1	(LIFEC_SEC_BASE + 0x0164U)
/* Security group 0 attribute setting for slave ports 2 */
#define	SEC_GRP0COND2	(LIFEC_SEC_BASE + 0x0168U)
/* Security group 1 attribute setting for slave ports 2 */
#define	SEC_GRP1COND2	(LIFEC_SEC_BASE + 0x016CU)
/* Security group 0 attribute setting for slave ports 3 */
#define	SEC_GRP0COND3	(LIFEC_SEC_BASE + 0x0170U)
/* Security group 1 attribute setting for slave ports 3 */
#define	SEC_GRP1COND3	(LIFEC_SEC_BASE + 0x0174U)
/* Security group 0 attribute setting for slave ports 4 */
#define	SEC_GRP0COND4	(LIFEC_SEC_BASE + 0x0178U)
/* Security group 1 attribute setting for slave ports 4 */
#define	SEC_GRP1COND4	(LIFEC_SEC_BASE + 0x017CU)
/* Security group 0 attribute setting for slave ports 5 */
#define	SEC_GRP0COND5	(LIFEC_SEC_BASE + 0x0180U)
/* Security group 1 attribute setting for slave ports 5 */
#define	SEC_GRP1COND5	(LIFEC_SEC_BASE + 0x0184U)
/* Security group 0 attribute setting for slave ports 6 */
#define	SEC_GRP0COND6	(LIFEC_SEC_BASE + 0x0188U)
/* Security group 1 attribute setting for slave ports 6 */
#define	SEC_GRP1COND6	(LIFEC_SEC_BASE + 0x018CU)
/* Security group 0 attribute setting for slave ports 7 */
#define	SEC_GRP0COND7	(LIFEC_SEC_BASE + 0x0190U)
/* Security group 1 attribute setting for slave ports 7 */
#define	SEC_GRP1COND7	(LIFEC_SEC_BASE + 0x0194U)
/* Security group 0 attribute setting for slave ports 8 */
#define	SEC_GRP0COND8	(LIFEC_SEC_BASE + 0x0198U)
/* Security group 1 attribute setting for slave ports 8 */
#define	SEC_GRP1COND8	(LIFEC_SEC_BASE + 0x019CU)
/* Security group 0 attribute setting for slave ports 9 */
#define	SEC_GRP0COND9	(LIFEC_SEC_BASE + 0x01A0U)
/* Security group 1 attribute setting for slave ports 9 */
#define	SEC_GRP1COND9	(LIFEC_SEC_BASE + 0x01A4U)
/* Security group 0 attribute setting for slave ports 10 */
#define	SEC_GRP0COND10	(LIFEC_SEC_BASE + 0x01A8U)
/* Security group 1 attribute setting for slave ports 10 */
#define	SEC_GRP1COND10	(LIFEC_SEC_BASE + 0x01ACU)
/* Security group 0 attribute setting for slave ports 11 */
#define	SEC_GRP0COND11	(LIFEC_SEC_BASE + 0x01B0U)
/* Security group 1 attribute setting for slave ports 11 */
#define	SEC_GRP1COND11	(LIFEC_SEC_BASE + 0x01B4U)
/* Security group 0 attribute setting for slave ports 12 */
#define	SEC_GRP0COND12	(LIFEC_SEC_BASE + 0x01B8U)
/* Security group 1 attribute setting for slave ports 12 */
#define	SEC_GRP1COND12	(LIFEC_SEC_BASE + 0x01BCU)
/* Security group 0 attribute setting for slave ports 13 */
#define	SEC_GRP0COND13	(LIFEC_SEC_BASE + 0x01C0U)
/* Security group 1 attribute setting for slave ports 13 */
#define	SEC_GRP1COND13	(LIFEC_SEC_BASE + 0x01C4U)
/* Security group 0 attribute setting for slave ports 14 */
#define	SEC_GRP0COND14	(LIFEC_SEC_BASE + 0x01C8U)
/* Security group 1 attribute setting for slave ports 14 */
#define	SEC_GRP1COND14	(LIFEC_SEC_BASE + 0x01CCU)
/* Security group 0 attribute setting for slave ports 15 */
#define	SEC_GRP0COND15	(LIFEC_SEC_BASE + 0x01D0U)
/* Security group 1 attribute setting for slave ports 15 */
#define	SEC_GRP1COND15	(LIFEC_SEC_BASE + 0x01D4U)
/* Security write protection attribute setting for slave ports 0 */
#define	SEC_READONLY0	(LIFEC_SEC_BASE + 0x01D8U)
/* Security write protection attribute setting for slave ports 1 */
#define	SEC_READONLY1	(LIFEC_SEC_BASE + 0x01DCU)
/* Security write protection attribute setting for slave ports 2 */
#define	SEC_READONLY2	(LIFEC_SEC_BASE + 0x01E0U)
/* Security write protection attribute setting for slave ports 3 */
#define	SEC_READONLY3	(LIFEC_SEC_BASE + 0x01E4U)
/* Security write protection attribute setting for slave ports 4 */
#define	SEC_READONLY4	(LIFEC_SEC_BASE + 0x01E8U)
/* Security write protection attribute setting for slave ports 5 */
#define	SEC_READONLY5	(LIFEC_SEC_BASE + 0x01ECU)
/* Security write protection attribute setting for slave ports 6 */
#define	SEC_READONLY6	(LIFEC_SEC_BASE + 0x01F0U)
/* Security write protection attribute setting for slave ports 7 */
#define	SEC_READONLY7	(LIFEC_SEC_BASE + 0x01F4U)
/* Security write protection attribute setting for slave ports 8 */
#define	SEC_READONLY8	(LIFEC_SEC_BASE + 0x01F8U)
/* Security write protection attribute setting for slave ports 9 */
#define	SEC_READONLY9	(LIFEC_SEC_BASE + 0x01FCU)
/* Security write protection attribute setting for slave ports 10 */
#define	SEC_READONLY10	(LIFEC_SEC_BASE + 0x0200U)
/* Security write protection attribute setting for slave ports 11 */
#define	SEC_READONLY11	(LIFEC_SEC_BASE + 0x0204U)
/* Security write protection attribute setting for slave ports 12 */
#define	SEC_READONLY12	(LIFEC_SEC_BASE + 0x0208U)
/* Security write protection attribute setting for slave ports 13 */
#define	SEC_READONLY13	(LIFEC_SEC_BASE + 0x020CU)
/* Security write protection attribute setting for slave ports 14 */
#define	SEC_READONLY14	(LIFEC_SEC_BASE + 0x0210U)
/* Security write protection attribute setting for slave ports 15 */
#define	SEC_READONLY15	(LIFEC_SEC_BASE + 0x0214U)

/* LIFEC1 (SAFETY) registers */

/* LIFEC1 (SAFETY) base address */
#define	LIFEC_SAFE_BASE	(0xE6120000U)

/* Safety group 0 attribute setting for master ports 0 */
#define	SAFE_GRP0CR0	(LIFEC_SAFE_BASE + 0x0138U)
/* Safety group 1 attribute setting for master ports 0 */
#define	SAFE_GRP1CR0	(LIFEC_SAFE_BASE + 0x013CU)
/* Safety group 0 attribute setting for master ports 1 */
#define	SAFE_GRP0CR1	(LIFEC_SAFE_BASE + 0x0140U)
/* Safety group 1 attribute setting for master ports 1 */
#define	SAFE_GRP1CR1	(LIFEC_SAFE_BASE + 0x0144U)
/* Safety group 0 attribute setting for master ports 2 */
#define	SAFE_GRP0CR2	(LIFEC_SAFE_BASE + 0x0148U)
/* Safety group 1 attribute setting for master ports 2 */
#define	SAFE_GRP1CR2	(LIFEC_SAFE_BASE + 0x014CU)
/* Safety group 0 attribute setting for master ports 3 */
#define	SAFE_GRP0CR3	(LIFEC_SAFE_BASE + 0x0150U)
/* Safety group 1 attribute setting for master ports 3 */
#define	SAFE_GRP1CR3	(LIFEC_SAFE_BASE + 0x0154U)
/* Safety group 0 attribute setting for slave ports 0 */
#define	SAFE_GRP0COND0	(LIFEC_SAFE_BASE + 0x0158U)
/* Safety group 1 attribute setting for slave ports 0 */
#define	SAFE_GRP1COND0	(LIFEC_SAFE_BASE + 0x015CU)
/* Safety group 0 attribute setting for slave ports 1 */
#define	SAFE_GRP0COND1	(LIFEC_SAFE_BASE + 0x0160U)
/* Safety group 1 attribute setting for slave ports 1 */
#define	SAFE_GRP1COND1	(LIFEC_SAFE_BASE + 0x0164U)
/* Safety group 0 attribute setting for slave ports 2 */
#define	SAFE_GRP0COND2	(LIFEC_SAFE_BASE + 0x0168U)
/* Safety group 1 attribute setting for slave ports 2 */
#define	SAFE_GRP1COND2	(LIFEC_SAFE_BASE + 0x016CU)
/* Safety group 0 attribute setting for slave ports 3 */
#define	SAFE_GRP0COND3	(LIFEC_SAFE_BASE + 0x0170U)
/* Safety group 1 attribute setting for slave ports 3 */
#define	SAFE_GRP1COND3	(LIFEC_SAFE_BASE + 0x0174U)
/* Safety group 0 attribute setting for slave ports 4 */
#define	SAFE_GRP0COND4	(LIFEC_SAFE_BASE + 0x0178U)
/* Safety group 1 attribute setting for slave ports 4 */
#define	SAFE_GRP1COND4	(LIFEC_SAFE_BASE + 0x017CU)
/* Safety group 0 attribute setting for slave ports 5 */
#define	SAFE_GRP0COND5	(LIFEC_SAFE_BASE + 0x0180U)
/* Safety group 1 attribute setting for slave ports 5 */
#define	SAFE_GRP1COND5	(LIFEC_SAFE_BASE + 0x0184U)
/* Safety group 0 attribute setting for slave ports 6 */
#define	SAFE_GRP0COND6	(LIFEC_SAFE_BASE + 0x0188U)
/* Safety group 1 attribute setting for slave ports 6 */
#define	SAFE_GRP1COND6	(LIFEC_SAFE_BASE + 0x018CU)
/* Safety group 0 attribute setting for slave ports 7 */
#define	SAFE_GRP0COND7	(LIFEC_SAFE_BASE + 0x0190U)
/* Safety group 1 attribute setting for slave ports 7 */
#define	SAFE_GRP1COND7	(LIFEC_SAFE_BASE + 0x0194U)
/* Safety group 0 attribute setting for slave ports 8 */
#define	SAFE_GRP0COND8	(LIFEC_SAFE_BASE + 0x0198U)
/* Safety group 1 attribute setting for slave ports 8 */
#define	SAFE_GRP1COND8	(LIFEC_SAFE_BASE + 0x019CU)
/* Safety group 0 attribute setting for slave ports 9 */
#define	SAFE_GRP0COND9	(LIFEC_SAFE_BASE + 0x01A0U)
/* Safety group 1 attribute setting for slave ports 9 */
#define	SAFE_GRP1COND9	(LIFEC_SAFE_BASE + 0x01A4U)
/* Safety group 0 attribute setting for slave ports 10 */
#define	SAFE_GRP0COND10	(LIFEC_SAFE_BASE + 0x01A8U)
/* Safety group 1 attribute setting for slave ports 10 */
#define	SAFE_GRP1COND10	(LIFEC_SAFE_BASE + 0x01ACU)
/* Safety group 0 attribute setting for slave ports 11 */
#define	SAFE_GRP0COND11	(LIFEC_SAFE_BASE + 0x01B0U)
/* Safety group 1 attribute setting for slave ports 11 */
#define	SAFE_GRP1COND11	(LIFEC_SAFE_BASE + 0x01B4U)
/* Safety group 0 attribute setting for slave ports 12 */
#define	SAFE_GRP0COND12	(LIFEC_SAFE_BASE + 0x01B8U)
/* Safety group 1 attribute setting for slave ports 12 */
#define	SAFE_GRP1COND12	(LIFEC_SAFE_BASE + 0x01BCU)
/* Safety group 0 attribute setting for slave ports 13 */
#define	SAFE_GRP0COND13	(LIFEC_SAFE_BASE + 0x01C0U)
/* Safety group 1 attribute setting for slave ports 13 */
#define	SAFE_GRP1COND13	(LIFEC_SAFE_BASE + 0x01C4U)
/* Safety group 0 attribute setting for slave ports 14 */
#define	SAFE_GRP0COND14	(LIFEC_SAFE_BASE + 0x01C8U)
/* Safety group 1 attribute setting for slave ports 14 */
#define	SAFE_GRP1COND14	(LIFEC_SAFE_BASE + 0x01CCU)
/* Safety group 0 attribute setting for slave ports 15 */
#define	SAFE_GRP0COND15	(LIFEC_SAFE_BASE + 0x01D0U)
/* Safety group 1 attribute setting for slave ports 15 */
#define	SAFE_GRP1COND15	(LIFEC_SAFE_BASE + 0x01D4U)
/* Safety write protection attribute setting for slave ports 0 */
#define	SAFE_READONLY0	(LIFEC_SAFE_BASE + 0x01D8U)
/* Safety write protection attribute setting for slave ports 1 */
#define	SAFE_READONLY1	(LIFEC_SAFE_BASE + 0x01DCU)
/* Safety write protection attribute setting for slave ports 2 */
#define	SAFE_READONLY2	(LIFEC_SAFE_BASE + 0x01E0U)
/* Safety write protection attribute setting for slave ports 3 */
#define	SAFE_READONLY3	(LIFEC_SAFE_BASE + 0x01E4U)
/* Safety write protection attribute setting for slave ports 4 */
#define	SAFE_READONLY4	(LIFEC_SAFE_BASE + 0x01E8U)
/* Safety write protection attribute setting for slave ports 5 */
#define	SAFE_READONLY5	(LIFEC_SAFE_BASE + 0x01ECU)
/* Safety write protection attribute setting for slave ports 6 */
#define	SAFE_READONLY6	(LIFEC_SAFE_BASE + 0x01F0U)
/* Safety write protection attribute setting for slave ports 7 */
#define	SAFE_READONLY7	(LIFEC_SAFE_BASE + 0x01F4U)
/* Safety write protection attribute setting for slave ports 8 */
#define	SAFE_READONLY8	(LIFEC_SAFE_BASE + 0x01F8U)
/* Safety write protection attribute setting for slave ports 9 */
#define	SAFE_READONLY9	(LIFEC_SAFE_BASE + 0x01FCU)
/* Safety write protection attribute setting for slave ports 10 */
#define	SAFE_READONLY10	(LIFEC_SAFE_BASE + 0x0200U)
/* Safety write protection attribute setting for slave ports 11 */
#define	SAFE_READONLY11	(LIFEC_SAFE_BASE + 0x0204U)
/* Safety write protection attribute setting for slave ports 12 */
#define	SAFE_READONLY12	(LIFEC_SAFE_BASE + 0x0208U)
/* Safety write protection attribute setting for slave ports 13 */
#define	SAFE_READONLY13	(LIFEC_SAFE_BASE + 0x020CU)
/* Safety write protection attribute setting for slave ports 14 */
#define	SAFE_READONLY14	(LIFEC_SAFE_BASE + 0x0210U)
/* Safety write protection attribute setting for slave ports 15 */
#define	SAFE_READONLY15	(LIFEC_SAFE_BASE + 0x0214U)

#endif	/* BL2_LIFEC_REGISTER_H__ */
