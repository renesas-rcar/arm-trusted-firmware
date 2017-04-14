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

#include <mmio.h>
#include "rcar_def.h"
#include "bl2_cpg_register.h"
#include "bl2_cpg_init.h"

typedef struct {
	uintptr_t	adr;
	uint32_t	val;
} reg_setting_t;

static const reg_setting_t reg_setting[] = {

	/* CPG (SECURITY) registers */

	/* Secure Module Stop Control Register 0 */
	{SCMSTPCR0,		0xFFFFFFFFU},
	/* Secure Module Stop Control Register 1 */
	{SCMSTPCR1,		0xFFFFFFFFU},
	/* Secure Module Stop Control Register 2 */
	{SCMSTPCR2,		0xEBFFFFFFU},
	/* Secure Module Stop Control Register 3 */
	{SCMSTPCR3,		0xFFFFFFFFU},
	/* Secure Module Stop Control Register 4 */
	{SCMSTPCR4,		0x7FFFFFFFU},
	/* Secure Module Stop Control Register 5 */
	{SCMSTPCR5,		0xBFFFFFFFU},
	/* Secure Module Stop Control Register 6 */
	{SCMSTPCR6,		0xFFFFFFFFU},
	/* Secure Module Stop Control Register 7 */
	{SCMSTPCR7,		0xFFFFFFFFU},
	/* Secure Module Stop Control Register 8 */
	{SCMSTPCR8,		0xFFFFFFFFU},
	/* Secure Module Stop Control Register 9 */
	{SCMSTPCR9,		0xFFFDFFFFU},
	/* Secure Module Stop Control Register 10 */
	{SCMSTPCR10,		0xFFFFFFFFU},
	/* Secure Module Stop Control Register 11 */
	{SCMSTPCR11,		0xFFFFFFFFU},
	/* Secure Software Reset Access Enable Control Register 0 */
	{SCSRSTECR0,		0x00000000U},
	/* Secure Software Reset Access Enable Control Register 1 */
	{SCSRSTECR1,		0x00000000U},
	/* Secure Software Reset Access Enable Control Register 2 */
	{SCSRSTECR2,		0x14000000U},
	/* Secure Software Reset Access Enable Control Register 3 */
	{SCSRSTECR3,		0x00000000U},
	/* Secure Software Reset Access Enable Control Register 4 */
	{SCSRSTECR4,		0x80000003U},
	/* Secure Software Reset Access Enable Control Register 5 */
	{SCSRSTECR5,		0x40000000U},
	/* Secure Software Reset Access Enable Control Register 6 */
	{SCSRSTECR6,		0x00000000U},
	/* Secure Software Reset Access Enable Control Register 7 */
	{SCSRSTECR7,		0x00000000U},
	/* Secure Software Reset Access Enable Control Register 8 */
	{SCSRSTECR8,		0x00000000U},
	/* Secure Software Reset Access Enable Control Register 9 */
	{SCSRSTECR9,		0x00020000U},
	/* Secure Software Reset Access Enable Control Register 10 */
	{SCSRSTECR10,		0x00000000U},
	/* Secure Software Reset Access Enable Control Register 11 */
	{SCSRSTECR11,		0x00000000U}
};

static const reg_setting_t reg_setting_rt[] = {

	/* CPG (REALTIME) registers */

	/* Realtime Module Stop Control Register 0 */
	{RMSTPCR0,		0x00200000U},
	/* Realtime Module Stop Control Register 1 */
	{RMSTPCR1,		0xFFFFFFFFU},
	/* Realtime Module Stop Control Register 2 */
	{RMSTPCR2,		0x340E0FDCU},
	/* Realtime Module Stop Control Register 3 */
	{RMSTPCR3,		0xFFFFFFDFU},
	/* Realtime Module Stop Control Register 4 */
	{RMSTPCR4,		0x80000184U},
	/* Realtime Module Stop Control Register 5 */
	{RMSTPCR5,		0xC3FFFFFFU},
	/* Realtime Module Stop Control Register 6 */
	{RMSTPCR6,		0xFFFFFFFFU},
	/* Realtime Module Stop Control Register 7 */
	{RMSTPCR7,		0xFFFFFFFFU},
	/* Realtime Module Stop Control Register 8 */
	{RMSTPCR8,		0x01F1FFF4U},
	/* Realtime Module Stop Control Register 9 */
	{RMSTPCR9,		0xFFFFFFFEU},
	/* Realtime Module Stop Control Register 10 */
	{RMSTPCR10,		0xFFFEFFE0U},
	/* Realtime Module Stop Control Register 11 */
	{RMSTPCR11,		0x000000B7U}
};

static const reg_setting_t reg_setting_system[] = {

	/* CPG (SYSTEM) registers */

	/* System Module Stop Control Register 0 */
	{SMSTPCR0,		0x00210000U},
	/* System Module Stop Control Register 1 */
	{SMSTPCR1,		0xFFFFFFFFU},
	/* System Module Stop Control Register 2 */
	{SMSTPCR2,		0x340E2FDCU},
	/* System Module Stop Control Register 3 */
	{SMSTPCR3,		0xFFFFFBDFU},
	/* System Module Stop Control Register 4 */
	{SMSTPCR4,		0x80000004U},
	/* System Module Stop Control Register 5 */
	{SMSTPCR5,		0xC3FFFFFFU},
	/* System Module Stop Control Register 6 */
	{SMSTPCR6,		0xFFFFFFFFU},
	/* System Module Stop Control Register 7 */
	{SMSTPCR7,		0xFFFFFFFFU},
	/* System Module Stop Control Register 8 */
	{SMSTPCR8,		0x01F1FFF5U},
	/* System Module Stop Control Register 9 */
	{SMSTPCR9,		0xFFFFFFFEU},
	/* System Module Stop Control Register 10 */
	{SMSTPCR10,		0xFFFEFFE0U},
	/* System Module Stop Control Register 11 */
	{SMSTPCR11,		0x000000B7U}
};

void bl2_cpg_init(void)
{
	uint32_t	i;
	uint32_t	ie;
	uint32_t	modemr;

	/* CPG (SECURITY) registers */
	ie = (uint32_t)(sizeof(reg_setting) / sizeof(reg_setting_t));

	for (i = 0U; i < ie; i++) {
		cpg_write(reg_setting[i].adr, reg_setting[i].val);
	}

	modemr = mmio_read_32(RCAR_MODEMR);
	modemr &= MODEMR_BOOT_CPU_MASK;

	/* CPG (REALTIME) registers */
	if((modemr == MODEMR_BOOT_CPU_CA57) ||
	   (modemr == MODEMR_BOOT_CPU_CA53)) {
		ie = (uint32_t)(sizeof(reg_setting_rt) / sizeof(reg_setting_t));

		for (i = 0U; i < ie; i++) {
			cpg_write(reg_setting_rt[i].adr, reg_setting_rt[i].val);
		}
	}
}

void bl2_system_cpg_init(void)
{
	uint32_t	i;
	uint32_t	ie;

	/* CPG (SYSTEM) registers */
	ie = (uint32_t)(sizeof(reg_setting_system) / sizeof(reg_setting_t));

	for (i = 0U; i < ie; i++) {
		cpg_write(reg_setting_system[i].adr, reg_setting_system[i].val);
	}
}

void cpg_write(uintptr_t regadr, uint32_t regval)
{
	uint32_t value = (regval);
	mmio_write_32((uintptr_t)CPG_CPGWPR,~value);
	mmio_write_32(regadr,value);
}
