/*
 * Copyright (c) 2013-2020, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <asm_macros.S>
#include <common/bl_common.h>
#include <common/runtime_svc.h>
#include <cortex_a720_ae.h>
#include <platform_def.h>

#include "rcar_def.h"

	.globl	plat_secondary_reset
	/* ---------------------------------------------
	* plat_secondary_reset
	*
	* ---------------------------------------------
	*/
func plat_secondary_reset
	mrs	x0, sctlr_el3
	bic	x0, x0, #SCTLR_EE_BIT
	msr	sctlr_el3, x0
	isb

	mrs	x0, cptr_el3
	bic	w0, w0, #TCPAC_BIT
	bic	w0, w0, #TTA_BIT
	bic	w0, w0, #TFP_BIT
	msr	cptr_el3, x0

	mov_imm	x0, PARAMS_BASE
	mov_imm	x2, BL31_BASE
	ldr x3, =BOOT_KIND_BASE
	mov x1, #0x1
	str x1, [x3]
	br	x2	/* jump to BL31 */
	nop
	nop
	nop
endfunc plat_secondary_reset
