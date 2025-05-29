/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <string.h>

#include <arch.h>
#include <arch_helpers.h>
#include <common/debug.h>
#include <lib/bakery_lock.h>
#include <lib/mmio.h>
#include <lib/xlat_tables/xlat_tables_v2.h>
#include <plat/common/platform.h>
#include <lib/libc/errno.h>

#include "pwrc.h"
#include "rcar_def.h"
#include "rcar_private.h"


#ifndef __ASSEMBLER__
IMPORT_SYM(uintptr_t, __system_ram_start__, SYSTEM_RAM_START);
IMPORT_SYM(uintptr_t, __system_ram_end__, SYSTEM_RAM_END);
IMPORT_SYM(uintptr_t, __SRAM_COPY_START__, SRAM_COPY_START);
#endif

#define	RCAR_CODE_COPY_DONE	U(0x01)
#define	RCAR_CODE_COPY_NONE	U(0x00)


static uint32_t rcar_pwrc_code_copy_state;



static void rcar_pwrc_go_suspend_to_ram(void);

static void rcar_pwrc_go_suspend_to_ram(void)
{

}

static uint64_t rcar_pwrc_saved_cntpct_el0;
static uint32_t rcar_pwrc_saved_cntfid;


uint32_t rcar_pwrc_get_mpidr_cluster(u_register_t mpidr)
{
	int32_t cluster = rcar_cluster_pos_by_mpidr(mpidr);

	if (cluster < 0) {
		ERROR("BL3-1 : The value of passed MPIDR is invalid.");
		panic();
	}

	return (uint32_t)cluster;
}

static void rcar_pwrc_save_timer_state(void)
{
	rcar_pwrc_saved_cntpct_el0 = read_cntpct_el0();

	rcar_pwrc_saved_cntfid =
		mmio_read_32((uintptr_t)(RCAR_CNTC_BASE + CNTFID_OFF));
}

void rcar_pwrc_restore_timer_state(void)
{
	/* Stop timer before restoring counter value */
	mmio_write_32((uintptr_t)(RCAR_CNTC_BASE + CNTCR_OFF), 0U);

	/* restore lower counter value */
	mmio_write_32((uintptr_t)(RCAR_CNTC_BASE + RCAR_CNTCVL_OFF),
		(uint32_t)(rcar_pwrc_saved_cntpct_el0 & 0xFFFFFFFFU));
	/* restore upper counter value */
	mmio_write_32((uintptr_t)(RCAR_CNTC_BASE + RCAR_CNTCVU_OFF),
		(uint32_t)(rcar_pwrc_saved_cntpct_el0 >> 32U));
	/* restore counter frequency setting */
	mmio_write_32((uintptr_t)(RCAR_CNTC_BASE + CNTFID_OFF),
		rcar_pwrc_saved_cntfid);

	/* Start generic timer back */
	write_cntfrq_el0((u_register_t)plat_get_syscnt_freq2());

	mmio_write_32((uintptr_t)(RCAR_CNTC_BASE + CNTCR_OFF),
			CNTCR_FCREQ((uint32_t)(0)) | CNTCR_EN);
}
void rcar_pwrc_suspend_to_ram(void)
{
	rcar_pwrc_save_timer_state();

	/* Clear code copy state to execute copy on next boot time */
	rcar_pwrc_code_copy_state = RCAR_CODE_COPY_NONE;

	/* disable MMU */
	disable_mmu_el3();

	/* cache flush */
	dcsw_op_all(DCCISW);


	rcar_pwrc_go_suspend_to_ram();
}
void rcar_pwrc_code_copy_to_system_ram(void)
{
	//TODO: Dummy now
}

uint32_t rcar_pwrc_cpu_on_check(u_register_t mpidr)
{
	//TODO: Dummy now
	return 0;
}

void rcar_pwrc_enable_interrupt_wakeup(u_register_t mpidr)
{
	//TODO: Dummy now
}

void rcar_pwrc_disable_interrupt_wakeup(u_register_t mpidr)
{
	//TODO: Dummy now
}


