/*
 * Copyright (c) 2018, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef RCAR_PLAT_LD_S
#define RCAR_PLAT_LD_S

#include <lib/xlat_tables/xlat_tables_defs.h>
#include <platform_def.h>

MEMORY {
    SRAM      (rwx): ORIGIN = DEVICE_SRAM_BASE, LENGTH = DEVICE_SRAM_SIZE
    SRAM_DATA (rwx): ORIGIN = DEVICE_SRAM_DATA_BASE, LENGTH = DEVICE_SRAM_DATA_SIZE
    PRAM      (r)  : ORIGIN = BL31_LIMIT - (DEVICE_SRAM_SIZE + DEVICE_SRAM_DATA_SIZE),
                     LENGTH = DEVICE_SRAM_SIZE + DEVICE_SRAM_DATA_SIZE
    #if VDK_ENV == 1
		DRAM_WAKEUP (rw): ORIGIN = 0x40650800, LENGTH = 0x1000 /* Reserve 4KB for wakeup factor */
	#endif
}

SECTIONS
{
	/* SRAM_COPY is in PRAM (RO) */
	. = BL31_LIMIT - (DEVICE_SRAM_SIZE + DEVICE_SRAM_DATA_SIZE);
	__SRAM_COPY_START__ = .;

	.system_ram : {
		/* system ram start is in SRAM */
		__system_ram_start__ = .;
		*(.system_ram)
	        __system_ram_end__ = .;
	} >SRAM AT>PRAM

	/* SRAM variable is in PRAM (RW) */
	. = BL31_LIMIT - DEVICE_SRAM_DATA_SIZE;

	.system_ram_data : {
		__system_ram_data_start__ = .;
		*(.system_ram_data)
	        __system_ram_data_end__ = .;
	} >SRAM_DATA AT>PRAM

	#if VDK_ENV == 1
		/* Wakeup factor section in DRAM */
		.wakeup_factor : {
			__wakeup_factor_start__ = .;
			KEEP(*(.wakeup_factor)) /* Ensure linker doesn't discard this section */
			__wakeup_factor_end__ = .;
		} >DRAM_WAKEUP
	#endif


    ASSERT(__BL31_END__ <= (BL31_LIMIT - (DEVICE_SRAM_SIZE + DEVICE_SRAM_DATA_SIZE)),
    "BL31 image too large - writing on top of SRAM!")

}

#endif /* RCAR_PLAT_LD_S */
