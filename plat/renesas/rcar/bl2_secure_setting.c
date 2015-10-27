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

#include "bl2_lifec_register.h"
#include "bl2_axi_register.h"
#include "bl2_secure_setting.h"
#include "mmio.h"

typedef struct {
	uint32_t	adr;
	uint32_t	val;
} reg_setting_t;

static const reg_setting_t lifec_reg_setting[] = {

	/* LIFEC0 (SECURITY) settings */

	/* Security attribute setting for master ports */
	{SEC_SRC,		0x0000001EU},
	/** Security attribute setting for slave ports 0 */
	{SEC_SEL0,		0xFFFFFFFFU},
	/** Security attribute setting for slave ports 1 */
	{SEC_SEL1,		0xFFFFFFFFU},
	/** Security attribute setting for slave ports 2 */
	{SEC_SEL2,		0xFFFFFFFFU},
	/** Security attribute setting for slave ports 3 */
	{SEC_SEL3,		0xFFF7FDFFU},
	/** Security attribute setting for slave ports 4 */
	{SEC_SEL4,		0xFFFFFFFFU},
	/** Security attribute setting for slave ports 5 */
	{SEC_SEL5,		0xFFFFFFBFU},
	/** Security attribute setting for slave ports 6 */
	{SEC_SEL6,		0xFFFFDBFFU},
	/** Security attribute setting for slave ports 7 */
	{SEC_SEL7,		0xFFFFFFFFU},
	/** Security attribute setting for slave ports 8 */
	{SEC_SEL8,		0xFFFFFFFFU},
	/** Security attribute setting for slave ports 9 */
	{SEC_SEL9,		0xFFFFFFFFU},
	/** Security attribute setting for slave ports 10 */
	{SEC_SEL10,		0xFFFFFFFFU},
	/** Security attribute setting for slave ports 11 */
	{SEC_SEL11,		0xFFFFFFFFU},
	/** Security attribute setting for slave ports 12 */
	{SEC_SEL12,		0xFFFFFFFFU},
	/** Security attribute setting for slave ports 13 */
	{SEC_SEL13,		0xFFBFFFEFU},
	/** Security attribute setting for slave ports 14 */
	{SEC_SEL14,		0xF3FFFFFFU},
	/** Security attribute setting for slave ports 15 */
	{SEC_SEL15,		0xFFFFDE7FU},
	/** Security group 0 attribute setting for master ports 0 */
	{SEC_GRP0CR0,		0x00000000U},
	/** Security group 1 attribute setting for master ports 0 */
	{SEC_GRP1CR0,		0x00000000U},
	/** Security group 0 attribute setting for master ports 1 */
	{SEC_GRP0CR1,		0x00000000U},
	/** Security group 1 attribute setting for master ports 1 */
	{SEC_GRP1CR1,		0x00000000U},
	/** Security group 0 attribute setting for master ports 2 */
	{SEC_GRP0CR2,		0x00020000U},
	/** Security group 1 attribute setting for master ports 2 */
	{SEC_GRP1CR2,		0x00020000U},
	/** Security group 0 attribute setting for master ports 3 */
	{SEC_GRP0CR3,		0x00000500U},
	/** Security group 1 attribute setting for master ports 3 */
	{SEC_GRP1CR3,		0x00000500U},
	/** Security group 0 attribute setting for slave ports 0 */
	{SEC_GRP0COND0,		0x00000000U},
	/** Security group 1 attribute setting for slave ports 0 */
	{SEC_GRP1COND0,		0x00000000U},
	/** Security group 0 attribute setting for slave ports 1 */
	{SEC_GRP0COND1,		0x00000000U},
	/** Security group 1 attribute setting for slave ports 1 */
	{SEC_GRP1COND1,		0x00000000U},
	/** Security group 0 attribute setting for slave ports 2 */
	{SEC_GRP0COND2,		0x00000000U},
	/** Security group 1 attribute setting for slave ports 2 */
	{SEC_GRP1COND2,		0x00000000U},
	/** Security group 0 attribute setting for slave ports 3 */
	{SEC_GRP0COND3,		0x00080200U},
	/** Security group 1 attribute setting for slave ports 3 */
	{SEC_GRP1COND3,		0x00080200U},
	/** Security group 0 attribute setting for slave ports 4 */
	{SEC_GRP0COND4,		0x00000000U},
	/** Security group 1 attribute setting for slave ports 4 */
	{SEC_GRP1COND4,		0x00000000U},
	/** Security group 0 attribute setting for slave ports 5 */
	{SEC_GRP0COND5,		0x00000040U},
	/** Security group 1 attribute setting for slave ports 5 */
	{SEC_GRP1COND5,		0x00000040U},
	/** Security group 0 attribute setting for slave ports 6 */
	{SEC_GRP0COND6,		0x00002400U},
	/** Security group 1 attribute setting for slave ports 6 */
	{SEC_GRP1COND6,		0x00002400U},
	/** Security group 0 attribute setting for slave ports 7 */
	{SEC_GRP0COND7,		0x00000000U},
	/** Security group 1 attribute setting for slave ports 7 */
	{SEC_GRP1COND7,		0x00000000U},
	/** Security group 0 attribute setting for slave ports 8 */
	{SEC_GRP0COND8,		0x00000000U},
	/** Security group 1 attribute setting for slave ports 8 */
	{SEC_GRP1COND8,		0x00000000U},
	/** Security group 0 attribute setting for slave ports 9 */
	{SEC_GRP0COND9,		0x00000000U},
	/** Security group 1 attribute setting for slave ports 9 */
	{SEC_GRP1COND9,		0x00000000U},
	/** Security group 0 attribute setting for slave ports 10 */
	{SEC_GRP0COND10,	0x00000000U},
	/** Security group 1 attribute setting for slave ports 10 */
	{SEC_GRP1COND10,	0x00000000U},
	/** Security group 0 attribute setting for slave ports 11 */
	{SEC_GRP0COND11,	0x00000000U},
	/** Security group 1 attribute setting for slave ports 11 */
	{SEC_GRP1COND11,	0x00000000U},
	/** Security group 0 attribute setting for slave ports 12 */
	{SEC_GRP0COND12,	0x00000000U},
	/** Security group 1 attribute setting for slave ports 12 */
	{SEC_GRP1COND12,	0x00000000U},
	/** Security group 0 attribute setting for slave ports 13 */
	{SEC_GRP0COND13,	0x00400010U},
	/** Security group 1 attribute setting for slave ports 13 */
	{SEC_GRP1COND13,	0x00400010U},
	/** Security group 0 attribute setting for slave ports 14 */
	{SEC_GRP0COND14,	0x0C000000U},
	/** Security group 1 attribute setting for slave ports 14 */
	{SEC_GRP1COND14,	0x0C000000U},
	/** Security group 0 attribute setting for slave ports 15 */
	{SEC_GRP0COND15,	0x00000180U},
	/** Security group 1 attribute setting for slave ports 15 */
	{SEC_GRP1COND15,	0x00000180U},
	/** Security write protection attribute setting for slave ports 0 */
	{SEC_READONLY0,		0x00000000U},
	/** Security write protection attribute setting for slave ports 1 */
	{SEC_READONLY1,		0x00000000U},
	/** Security write protection attribute setting for slave ports 2 */
	{SEC_READONLY2,		0x00000000U},
	/** Security write protection attribute setting for slave ports 3 */
	{SEC_READONLY3,		0x00000000U},
	/** Security write protection attribute setting for slave ports 4 */
	{SEC_READONLY4,		0x00000000U},
	/** Security write protection attribute setting for slave ports 5 */
	{SEC_READONLY5,		0x00000000U},
	/** Security write protection attribute setting for slave ports 6 */
	{SEC_READONLY6,		0x00000000U},
	/** Security write protection attribute setting for slave ports 7 */
	{SEC_READONLY7,		0x00000000U},
	/** Security write protection attribute setting for slave ports 8 */
	{SEC_READONLY8,		0x00000000U},
	/** Security write protection attribute setting for slave ports 9 */
	{SEC_READONLY9,		0x00000000U},
	/** Security write protection attribute setting for slave ports 10 */
	{SEC_READONLY10,	0x00000000U},
	/** Security write protection attribute setting for slave ports 11 */
	{SEC_READONLY11,	0x00000000U},
	/** Security write protection attribute setting for slave ports 12 */
	{SEC_READONLY12,	0x00000000U},
	/** Security write protection attribute setting for slave ports 13 */
	{SEC_READONLY13,	0x00000000U},
	/** Security write protection attribute setting for slave ports 14 */
	{SEC_READONLY14,	0x00000000U},
	/** Security write protection attribute setting for slave ports 15 */
	{SEC_READONLY15,	0x00000000U},

	/* LIFEC1 (SAFETY) settings */

	/* Safety group 0 attribute setting for master ports 0 */
	{SAFE_GRP0CR0,		0x00000000U},
	/* Safety group 1 attribute setting for master ports 0 */
	{SAFE_GRP1CR0,		0x00000000U},
	/* Safety group 0 attribute setting for master ports 1 */
	{SAFE_GRP0CR1,		0x00000000U},
	/* Safety group 1 attribute setting for master ports 1 */
	{SAFE_GRP1CR1,		0x00000000U},
	/* Safety group 0 attribute setting for master ports 2 */
	{SAFE_GRP0CR2,		0x00000000U},
	/* Safety group 1 attribute setting for master ports 2 */
	{SAFE_GRP1CR2,		0x00000000U},
	/* Safety group 0 attribute setting for master ports 3 */
	{SAFE_GRP0CR3,		0x00000000U},
	/* Safety group 1 attribute setting for master ports 3 */
	{SAFE_GRP1CR3,		0x00000000U},
	/* Safety group 0 attribute setting for slave ports 0 */
	{SAFE_GRP0COND0,	0x00000000U},
	/* Safety group 1 attribute setting for slave ports 0 */
	{SAFE_GRP1COND0,	0x00000000U},
	/* Safety group 0 attribute setting for slave ports 1 */
	{SAFE_GRP0COND1,	0x00000000U},
	/* Safety group 1 attribute setting for slave ports 1 */
	{SAFE_GRP1COND1,	0x00000000U},
	/* Safety group 0 attribute setting for slave ports 2 */
	{SAFE_GRP0COND2,	0x00000000U},
	/* Safety group 1 attribute setting for slave ports 2 */
	{SAFE_GRP1COND2,	0x00000000U},
	/* Safety group 0 attribute setting for slave ports 3 */
	{SAFE_GRP0COND3,	0x00000000U},
	/* Safety group 1 attribute setting for slave ports 3 */
	{SAFE_GRP1COND3,	0x00000000U},
	/* Safety group 0 attribute setting for slave ports 4 */
	{SAFE_GRP0COND4,	0x00000000U},
	/* Safety group 1 attribute setting for slave ports 4 */
	{SAFE_GRP1COND4,	0x00000000U},
	/* Safety group 0 attribute setting for slave ports 5 */
	{SAFE_GRP0COND5,	0x00000000U},
	/* Safety group 1 attribute setting for slave ports 5 */
	{SAFE_GRP1COND5,	0x00000000U},
	/* Safety group 0 attribute setting for slave ports 6 */
	{SAFE_GRP0COND6,	0x00000000U},
	/* Safety group 1 attribute setting for slave ports 6 */
	{SAFE_GRP1COND6,	0x00000000U},
	/* Safety group 0 attribute setting for slave ports 7 */
	{SAFE_GRP0COND7,	0x00000000U},
	/* Safety group 1 attribute setting for slave ports 7 */
	{SAFE_GRP1COND7,	0x00000000U},
	/* Safety group 0 attribute setting for slave ports 8 */
	{SAFE_GRP0COND8,	0x00000000U},
	/* Safety group 1 attribute setting for slave ports 8 */
	{SAFE_GRP1COND8,	0x00000000U},
	/* Safety group 0 attribute setting for slave ports 9 */
	{SAFE_GRP0COND9,	0x00000000U},
	/* Safety group 1 attribute setting for slave ports 9 */
	{SAFE_GRP1COND9,	0x00000000U},
	/* Safety group 0 attribute setting for slave ports 10 */
	{SAFE_GRP0COND10,	0x00000000U},
	/* Safety group 1 attribute setting for slave ports 10 */
	{SAFE_GRP1COND10,	0x00000000U},
	/* Safety group 0 attribute setting for slave ports 11 */
	{SAFE_GRP0COND11,	0x00000000U},
	/* Safety group 1 attribute setting for slave ports 11 */
	{SAFE_GRP1COND11,	0x00000000U},
	/* Safety group 0 attribute setting for slave ports 12 */
	{SAFE_GRP0COND12,	0x00000000U},
	/* Safety group 1 attribute setting for slave ports 12 */
	{SAFE_GRP1COND12,	0x00000000U},
	/* Safety group 0 attribute setting for slave ports 13 */
	{SAFE_GRP0COND13,	0x00000000U},
	/* Safety group 1 attribute setting for slave ports 13 */
	{SAFE_GRP1COND13,	0x00000000U},
	/* Safety group 0 attribute setting for slave ports 14 */
	{SAFE_GRP0COND14,	0x00000000U},
	/* Safety group 1 attribute setting for slave ports 14 */
	{SAFE_GRP1COND14,	0x00000000U},
	/* Safety group 0 attribute setting for slave ports 15 */
	{SAFE_GRP0COND15,	0x00000000U},
	/* Safety group 1 attribute setting for slave ports 15 */
	{SAFE_GRP1COND15,	0x00000000U},
	/* Safety write protection attribute setting for slave ports 0 */
	{SAFE_READONLY0,	0x00000000U},
	/* Safety write protection attribute setting for slave ports 1 */
	{SAFE_READONLY1,	0x00000000U},
	/* Safety write protection attribute setting for slave ports 2 */
	{SAFE_READONLY2,	0x00000000U},
	/* Safety write protection attribute setting for slave ports 3 */
	{SAFE_READONLY3,	0x00000000U},
	/* Safety write protection attribute setting for slave ports 4 */
	{SAFE_READONLY4,	0x00000000U},
	/* Safety write protection attribute setting for slave ports 5 */
	{SAFE_READONLY5,	0x00000000U},
	/* Safety write protection attribute setting for slave ports 6 */
	{SAFE_READONLY6,	0x00000000U},
	/* Safety write protection attribute setting for slave ports 7 */
	{SAFE_READONLY7,	0x00000000U},
	/* Safety write protection attribute setting for slave ports 8 */
	{SAFE_READONLY8,	0x00000000U},
	/* Safety write protection attribute setting for slave ports 9 */
	{SAFE_READONLY9,	0x00000000U},
	/* Safety write protection attribute setting for slave ports 10 */
	{SAFE_READONLY10,	0x00000000U},
	/* Safety write protection attribute setting for slave ports 11 */
	{SAFE_READONLY11,	0x00000000U},
	/* Safety write protection attribute setting for slave ports 12 */
	{SAFE_READONLY12,	0x00000000U},
	/* Safety write protection attribute setting for slave ports 13 */
	{SAFE_READONLY13,	0x00000000U},
	/* Safety write protection attribute setting for slave ports 14 */
	{SAFE_READONLY14,	0x00000000U},
	/* Safety write protection attribute setting for slave ports 15 */
	{SAFE_READONLY15,	0x00000000U}
};

static const reg_setting_t axi_reg_setting[] = {

	/* AXI settings */

	/* DRAM protection */

	/* AXI dram protected area division 0 */
	{AXI_DPTDIVCR0,		0xEE0407E0U},
	/* AXI dram protected area division 1 */
	{AXI_DPTDIVCR1,		0xEE080000U},
	/* AXI dram protected area division 2 */
	{AXI_DPTDIVCR2,		0xEE080000U},
	/* AXI dram protected area division 3 */
	{AXI_DPTDIVCR3,		0xEE080000U},
	/* AXI dram protected area division 4 */
	{AXI_DPTDIVCR4,		0xEE080000U},
	/* AXI dram protected area division 5 */
	{AXI_DPTDIVCR5,		0xEE080000U},
	/* AXI dram protected area division 6 */
	{AXI_DPTDIVCR6,		0xEE080000U},
	/* AXI dram protected area division 7 */
	{AXI_DPTDIVCR7,		0xEE080000U},
	/* AXI dram protected area division 8 */
	{AXI_DPTDIVCR8,		0xEE080000U},
	/* AXI dram protected area division 9 */
	{AXI_DPTDIVCR9,		0xEE080000U},
	/* AXI dram protected area division 10 */
	{AXI_DPTDIVCR10,	0xEE080000U},
	/* AXI dram protected area division 11 */
	{AXI_DPTDIVCR11,	0xEE080000U},
	/* AXI dram protected area division 12 */
	{AXI_DPTDIVCR12,	0xEE080000U},
	/* AXI dram protected area division 13 */
	{AXI_DPTDIVCR13,	0xEE080000U},
	/* AXI dram protected area division 14 */
	{AXI_DPTDIVCR14,	0xEE080000U},

	/* AXI dram protected area setting 0 */
	{AXI_DPTCR0,		0xEE00EEEEU},
	/* AXI dram protected area setting 1 */
	{AXI_DPTCR1,		0xEE000000U},
	/* AXI dram protected area setting 2 */
	{AXI_DPTCR2,		0xEE000000U},
	/* AXI dram protected area setting 3 */
	{AXI_DPTCR3,		0xEE000000U},
	/* AXI dram protected area setting 4 */
	{AXI_DPTCR4,		0xEE000000U},
	/* AXI dram protected area setting 5 */
	{AXI_DPTCR5,		0xEE000000U},
	/* AXI dram protected area setting 6 */
	{AXI_DPTCR6,		0xEE000000U},
	/* AXI dram protected area setting 7 */
	{AXI_DPTCR7,		0xEE000000U},
	/* AXI dram protected area setting 8 */
	{AXI_DPTCR8,		0xEE000000U},
	/* AXI dram protected area setting 9 */
	{AXI_DPTCR9,		0xEE000000U},
	/* AXI dram protected area setting 10 */
	{AXI_DPTCR10,		0xEE000000U},
	/* AXI dram protected area setting 11 */
	{AXI_DPTCR11,		0xEE000000U},
	/* AXI dram protected area setting 12 */
	{AXI_DPTCR12,		0xEE000000U},
	/* AXI dram protected area setting 13 */
	{AXI_DPTCR13,		0xEE000000U},
	/* AXI dram protected area setting 14 */
	{AXI_DPTCR14,		0xEE000000U},
	/* AXI dram protected area setting 15 */
	{AXI_DPTCR15,		0xEE000000U}

#if 0
	/* SRAM ptotection */
	,
	/* AXI sram protected area division 0 */
	{AXI_SPTDIVCR0,		0xEE0E6330U},
	/* AXI sram protected area division 1 */
	{AXI_SPTDIVCR1,		0xEE0E6360U},
	/* AXI sram protected area division 2 */
	{AXI_SPTDIVCR2,		0xEE0E6360U},
	/* AXI sram protected area division 3 */
	{AXI_SPTDIVCR3,		0xEE0E6360U},
	/* AXI sram protected area division 4 */
	{AXI_SPTDIVCR4,		0xEE0E6360U},
	/* AXI sram protected area division 5 */
	{AXI_SPTDIVCR5,		0xEE0E6360U},
	/* AXI sram protected area division 6 */
	{AXI_SPTDIVCR6,		0xEE0E6360U},
	/* AXI sram protected area division 7 */
	{AXI_SPTDIVCR7,		0xEE0E6360U},
	/* AXI sram protected area division 8 */
	{AXI_SPTDIVCR8,		0xEE0E6360U},
	/* AXI sram protected area division 9 */
	{AXI_SPTDIVCR9,		0xEE0E6360U},
	/* AXI sram protected area division 10 */
	{AXI_SPTDIVCR10,	0xEE0E6360U},
	/* AXI sram protected area division 11 */
	{AXI_SPTDIVCR11,	0xEE0E6360U},
	/* AXI sram protected area division 12 */
	{AXI_SPTDIVCR12,	0xEE0E6360U},
	/* AXI sram protected area division 13 */
	{AXI_SPTDIVCR13,	0xEE0E6360U},
	/* AXI sram protected area division 14 */
	{AXI_SPTDIVCR14,	0xEE0E6360U},

	/* AXI sram protected area setting 0 */
	{AXI_SPTCR0,		0xEE00EEEEU},
	/* AXI sram protected area setting 1 */
	{AXI_SPTCR1,		0xEE000000U},
	/* AXI sram protected area setting 2 */
	{AXI_SPTCR2,		0xEE000000U},
	/* AXI sram protected area setting 3 */
	{AXI_SPTCR3,		0xEE000000U},
	/* AXI sram protected area setting 4 */
	{AXI_SPTCR4,		0xEE000000U},
	/* AXI sram protected area setting 5 */
	{AXI_SPTCR5,		0xEE000000U},
	/* AXI sram protected area setting 6 */
	{AXI_SPTCR6,		0xEE000000U},
	/* AXI sram protected area setting 7 */
	{AXI_SPTCR7,		0xEE000000U},
	/* AXI sram protected area setting 8 */
	{AXI_SPTCR8,		0xEE000000U},
	/* AXI sram protected area setting 9 */
	{AXI_SPTCR9,		0xEE000000U},
	/* AXI sram protected area setting 10 */
	{AXI_SPTCR10,		0xEE000000U},
	/* AXI sram protected area setting 11 */
	{AXI_SPTCR11,		0xEE000000U},
	/* AXI sram protected area setting 12 */
	{AXI_SPTCR12,		0xEE000000U},
	/* AXI sram protected area setting 13 */
	{AXI_SPTCR13,		0xEE000000U},
	/* AXI sram protected area setting 14 */
	{AXI_SPTCR14,		0xEE000000U},
	/* AXI sram protected area setting 15 */
	{AXI_SPTCR15,		0xEE000000U}
#endif
};

void bl2_secure_setting(void)
{
#if 0	/* TEST */
	uint32_t		i, ie;
	const reg_setting_t	*reg_setting;

	reg_setting = &lifec_reg_setting[0];
	i = 0;
	ie = sizeof(*reg_setting) / sizeof(reg_setting_t);
	for (; i < ie; i++) {
		mmio_write_32(reg_setting[i].adr, reg_setting[i].val);
	}

	reg_setting = &axi_reg_setting[0];
	i = 0;
	ie = sizeof(*reg_setting) / sizeof(reg_setting_t);
	for (; i < ie; i++) {
		mmio_write_32(reg_setting[i].adr, reg_setting[i].val);
	}
#endif	/* TEST */
}
