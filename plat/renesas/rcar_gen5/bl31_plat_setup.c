/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stddef.h>

#include <arch.h>
#include <arch_helpers.h>
#include <bl31/bl31.h>
#include <common/bl_common.h>
#include <common/debug.h>
#include <drivers/arm/cci.h>
#include <drivers/console.h>
#include <lib/mmio.h>
#include <plat/common/platform.h>



#include "pwrc.h"
#include "rcar_def.h"
#include "rcar_private.h"
#include "rcar_version.h"



static u_register_t rcar_boot_mpidr;

struct entry_point_info *bl31_plat_get_next_image_ep_info(uint32_t type)
{
	bl2_to_bl31_params_mem_t *from_bl2 = (bl2_to_bl31_params_mem_t *)
					     PARAMS_BASE;

	entry_point_info_t *next_image_info;

	next_image_info = (type == NON_SECURE) ?
		&from_bl2->bl33_ep_info : &from_bl2->bl32_ep_info;

	return (next_image_info->pc != 0U) ? next_image_info : NULL;
}

void bl31_early_platform_setup2(u_register_t arg0, u_register_t arg1,
				u_register_t arg2, u_register_t arg3)
{
	rcar_console_boot_init();
	NOTICE("BL3-1 : Rev.%s\n", version_of_renesas);
}

/**
 *
 *
 */
#define SYSSS_TOP_Region_0	UL(0xC6480000)
#define SASYNCRTCKCR_OFFSET	UL(0x1064)
#define SASYNCRTCKCR		(SYSSS_TOP_Region_0 + SASYNCRTCKCR_OFFSET)
#define CKSTP_BIT              BIT_32(7)

// Base Address Macros
#define CLK_CONTROL_SCP_BASE			(0xC1330000uL) // SYSSS_SCP_PS0_Region_0 base Address
#define CLK_CONTROL_PERE_BASE			(0xC08F0000uL) // SYSSS_PERE_Region_0 base Address
#define CLK_CONTROL_TOP_BASE			(0xC6480000uL) // SYSSS_TOP_Region_0 base Address
#define CLK_CONTROL_HSCS_BASE			(0xDE200000uL) // SYSSS_HSCS_Region_0 base Address

// Protection Register Macros
#define CLK_CONTROL_CLKTOPPKCPROT0		(CLK_CONTROL_TOP_BASE + (0x00001370uL))
#define CLK_CONTROL_CLKPEREPKCPROT0		(CLK_CONTROL_PERE_BASE + (0x00001370uL))
#define CLK_CONTROL_CLKHSCSPKCPROT0		(CLK_CONTROL_HSCS_BASE + (0x00001370uL))
#define CLK_CONTROL_CLKSCPPKCPROT0		(CLK_CONTROL_SCP_BASE + (0x00001370uL))

// Mask and Key Code Macros
#define CLK_CONTROL_BIT32_16_BASE_ADDRESS_MASK (0xFFFF0000uL)
#define CLK_PKCPROT0_KCPROT_SET               (0xA5A5A500uL)
#define CLK_PKCPROT0_KCE_DISABLE_SET          (0x00000000uL)
#define CLK_PKCPROT0_KCE_ENABLE_SET           (0x00000001uL)

// Flag Macros
#define FLAG_ON				(0x5A5A0000u) // Flag On Value
#define FLAG_OFF			(0xD4D47171u) // Flag Off Value

void rd_WriteClockControlRegister(uint32_t reg_address, uint32_t set_value)
{
	uint32_t base_address;
	uint32_t base_address_check_flag;
	uint32_t pkc_prot_register;
	uint32_t write_key_code;

	base_address = (CLK_CONTROL_BIT32_16_BASE_ADDRESS_MASK & reg_address);

	/* Select the Protect register for Corresponding Power domain */
	switch (base_address) {
	case CLK_CONTROL_TOP_BASE:
		pkc_prot_register = CLK_CONTROL_CLKTOPPKCPROT0;
		base_address_check_flag = FLAG_OFF;
		break;

	case CLK_CONTROL_PERE_BASE:
		pkc_prot_register = CLK_CONTROL_CLKPEREPKCPROT0;
		base_address_check_flag = FLAG_OFF;
		break;

	case CLK_CONTROL_HSCS_BASE:
		pkc_prot_register = CLK_CONTROL_CLKHSCSPKCPROT0;
		base_address_check_flag = FLAG_OFF;
		break;

	case CLK_CONTROL_SCP_BASE:
		pkc_prot_register = CLK_CONTROL_CLKSCPPKCPROT0;
		base_address_check_flag = FLAG_OFF;
		break;

	default:
		pkc_prot_register = 0uL;
		base_address_check_flag = FLAG_ON;
		break;
	}

	if (base_address_check_flag == FLAG_OFF) {
		write_key_code = (CLK_PKCPROT0_KCPROT_SET | CLK_PKCPROT0_KCE_ENABLE_SET);
		mmio_write_32(pkc_prot_register, write_key_code);
		mmio_write_32(reg_address, set_value);
		write_key_code = (CLK_PKCPROT0_KCPROT_SET | CLK_PKCPROT0_KCE_DISABLE_SET);
		mmio_write_32(pkc_prot_register, write_key_code);
	}
}

static void enable_sasyncck_rt_clk(void)
{
	uint32_t register_value;

	// Read the current register value
	register_value = mmio_read_32(SASYNCRTCKCR);

	// Clear the CKSTP_BIT to enable the SASYNCCK_RT clock
	register_value &= ~CKSTP_BIT;

	// Write the updated value back to the register using the wrapper
	rd_WriteClockControlRegister(SASYNCRTCKCR, register_value);
}
void bl31_plat_arch_setup(void)
{
	static const uintptr_t BL31_RO_BASE = BL_CODE_BASE;
	static const uintptr_t BL31_RO_LIMIT = BL_CODE_END;

	/* Enable SASYNCCK_RT gate clock */
	enable_sasyncck_rt_clk();

	rcar_configure_mmu_el3(BL31_BASE,
			       BL31_LIMIT - BL31_BASE,
			       BL31_RO_BASE, BL31_RO_LIMIT);

}

void bl31_platform_setup(void)
{
	uint32_t reg_cntfid;

	plat_rcar_gic_driver_init();
	plat_rcar_gic_init();

	/* initialize generic timer */
	reg_cntfid = RCAR_CNTC_EXTAL;
	/* Update memory mapped and register based frequency */
	write_cntfrq_el0((u_register_t)reg_cntfid);
	mmio_write_32(ARM_SYS_CNTCTL_BASE + (uintptr_t)CNTFID_OFF, reg_cntfid);

	/* enable the system level generic timer */
	mmio_write_32(RCAR_CNTC_BASE + CNTCR_OFF,
			CNTCR_FCREQ((uint32_t)(0)) | CNTCR_EN);

	/*
	 * Preserve plat_secondary_reset symbol for secondary CPU boot.
	 * Store function address to prevent linker dead code elimination.
	*/
	volatile uintptr_t secondary_reset_func = (uintptr_t)plat_secondary_reset;
	(void)secondary_reset_func;

	plat_rcar_scmi_setup();

	/*
	 * mask should match the kernel's MPIDR_HWID_BITMASK so the core can be
	 * identified during cpuhotplug (check the kernel's psci migrate set of
	 * functions
	 */
	rcar_boot_mpidr = read_mpidr_el1() & RCAR_MPIDR_AFFMASK;
}

uint32_t bl31_plat_boot_mpidr_chk(void)
{
	uint32_t rc = RCAR_MPIDRCHK_NOT_BOOTCPU;
	uint64_t tmp_mpidr;

	tmp_mpidr = read_mpidr_el1() & RCAR_MPIDR_AFFMASK;

	if (tmp_mpidr == rcar_boot_mpidr) {
		rc = RCAR_MPIDRCHK_BOOTCPU;
	}
	return rc;
}

void bl31_plat_runtime_setup(void)
{
	rcar_console_runtime_init();
	console_switch_state(CONSOLE_FLAG_RUNTIME);
}
