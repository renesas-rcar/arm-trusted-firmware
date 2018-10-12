/*
 * Copyright (c) 2018, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch.h>
#include <arch_helpers.h>
#include "micro_wait.h"

#define RCAR_CONV_MICROSEC			(1000000U)

void
#if IMAGE_BL31
	__attribute__ ((section (".system_ram")))
#endif
	micro_wait(uint64_t micro_sec)
{
	uint64_t freq;
	uint64_t base_count;
	uint64_t get_count;
	uint64_t wait_time = 0U;

	freq = read_cntfrq_el0();
	base_count = (uint64_t)read_cntpct_el0();
	while(micro_sec > wait_time) {
		get_count = (uint64_t)read_cntpct_el0();
		wait_time = ((get_count - base_count) * RCAR_CONV_MICROSEC) / freq;
	}
}

/*******************************************************************************
 * This function Returns the snapshot value of count of the Generic Timer.
 * To use this function, the "bl2_init_generic_timer" function needs
 * to be executed.
 * To convert count value into time, use the following formula.
 *
 *   count value / frequency = time[sec]
 *
 * The "frequency" is set in cntfrq_el0.
 ******************************************************************************/
uint64_t snapshot_CNTPCT_EL0(void)
{
	return (uint64_t)read_cntpct_el0();
}
