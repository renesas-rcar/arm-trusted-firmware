/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2016, Renesas Electronics Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
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

#include <arch_helpers.h>
#include <arm_gic.h>
#include <assert.h>
#include <bakery_lock.h>
#include <cci.h>
#include <debug.h>
#include <mmio.h>
#include <platform.h>
#include <platform_def.h>
#include <psci.h>
#include <errno.h>
#include "drivers/pwrc/rcar_pwrc.h"
#include "rcar_def.h"
#include "rcar_private.h"

static void rcar_program_mailbox(uint64_t mpidr, uint64_t address);
static int32_t rcar_do_plat_actions(unsigned int afflvl, unsigned int state);
static void rcar_cpu_pwrdwn_common(void);
static void rcar_cluster_pwrdwn_common(void);
static void __dead2 rcar_system_off(void);
static void __dead2 rcar_system_reset(void);

static int32_t cpu_on_check(uint64_t mpidr);

extern int32_t platform_is_primary_cpu(uint64_t mpidr);

/*******************************************************************************
 * Private RCAR function to program the mailbox for a cpu before it is released
 * from reset.
 ******************************************************************************/
static void rcar_program_mailbox(uint64_t mpidr, uint64_t address)
{
	uint64_t linear_id;
	mailbox_t *rcar_mboxes;

	linear_id = platform_get_core_pos(mpidr);
	rcar_mboxes = (mailbox_t *)MBOX_BASE;
	rcar_mboxes[linear_id].value = address;
	flush_dcache_range((unsigned long)&rcar_mboxes[linear_id],
			sizeof(unsigned long));
}

/*******************************************************************************
 * Function which implements the common RCAR specific operations to power down a
 * cpu in response to a CPU_OFF or CPU_SUSPEND request.
 ******************************************************************************/
static void rcar_cpu_pwrdwn_common(void)
{
	/* Prevent interrupts from spuriously waking up this cpu */
	arm_gic_cpuif_deactivate();

	/* Program the power controller to power off this cpu. */
	rcar_pwrc_cpuoff (read_mpidr_el1());
}

/*******************************************************************************
 * Function which implements the common RCAR specific operations to power down a
 * cluster in response to a CPU_OFF or CPU_SUSPEND request.
 ******************************************************************************/
static void rcar_cluster_pwrdwn_common(void)
{
	uint64_t mpidr = read_mpidr_el1();

	/* Disable coherency if this cluster is to be turned off */
	rcar_cci_disable();

	/* Program the power controller to turn the cluster off */
	rcar_pwrc_clusteroff(mpidr);
}

/*******************************************************************************
 * Private RCAR function which is used to determine if any platform actions
 * should be performed for the specified affinity instance given its
 * state. Nothing needs to be done if the 'state' is not off or if this is not
 * the highest affinity level which will enter the 'state'.
 ******************************************************************************/
static int32_t rcar_do_plat_actions(unsigned int afflvl, unsigned int state)
{
	unsigned int max_phys_off_afflvl;

	assert(afflvl <= MPIDR_AFFLVL1);

	if (state != PSCI_STATE_OFF) {
		return -EAGAIN;
	}

	/*
	 * Find the highest affinity level which will be suspended and postpone
	 * all the platform specific actions until that level is hit.
	 */
	max_phys_off_afflvl = psci_get_max_phys_off_afflvl();
	assert(max_phys_off_afflvl != PSCI_INVALID_DATA);
	if (afflvl != max_phys_off_afflvl) {
		return -EAGAIN;
	}

	return 0;
}

/*******************************************************************************
 * RCAR handler called when an affinity instance is about to enter standby.
 ******************************************************************************/
void rcar_affinst_standby(unsigned int power_state)
{
	/*
	 * Enter standby state
	 * dsb is good practice before using wfi to enter low power states
	 */
	dsb();
	wfi();
}

/*******************************************************************************
 * RCAR handler called when an affinity instance is about to be turned on. The
 * level and mpidr determine the affinity instance.
 ******************************************************************************/
int rcar_affinst_on(unsigned long mpidr, unsigned long sec_entrypoint,
		unsigned int afflvl, unsigned int state)
{
	int rc = PSCI_E_SUCCESS;

#if PSCI_DISABLE_BIGLITTLE_IN_CA57BOOT
	uint64_t boot_cluster = read_mpidr_el1() & ((uint64_t)MPIDR_CLUSTER_MASK);
	if (boot_cluster == 0x0000U) {
		if ((mpidr & ((uint64_t)MPIDR_CLUSTER_MASK)) != boot_cluster) {
			return PSCI_E_INTERN_FAIL;
		}
	}
#endif
	/*
	 * It's possible to turn on only affinity level 0 i.e. a cpu
	 * on the RCAR. Ignore any other affinity level.
	 */
	if (afflvl != MPIDR_AFFLVL0) {
		return rc;
	}

	/*
	 * Ensure that we do not cancel an inflight power off request
	 * for the target cpu. That would leave it in a zombie wfi.
	 * Wait for it to power off, program the jump address for the
	 * target cpu and then program the power controller to turn
	 * that cpu on
	 */

	rcar_program_mailbox(mpidr, sec_entrypoint);
	rcar_pwrc_cpuon(mpidr);

	return rc;
}

/*******************************************************************************
 * RCAR handler called when an affinity instance is about to be turned off. The
 * level and mpidr determine the affinity instance. The 'state' arg. allows the
 * platform to decide whether the cluster is being turned off and take apt
 * actions.
 *
 * CAUTION: There is no guarantee that caches will remain turned on across calls
 * to this function as each affinity level is dealt with. So do not write & read
 * global variables across calls. It will be wise to do flush a write to the
 * global to prevent unpredictable results.
 ******************************************************************************/
void rcar_affinst_off(unsigned int afflvl, unsigned int state)
{
	/* Determine if any platform actions need to be executed */
	if (rcar_do_plat_actions(afflvl, state) == -EAGAIN) {
		return;
	}

	/*
	 * If execution reaches this stage then this affinity level will be
	 * suspended. Perform at least the cpu specific actions followed the
	 * cluster specific operations if applicable.
	 */
	rcar_cpu_pwrdwn_common();

	if (afflvl != MPIDR_AFFLVL0) {
		rcar_cluster_pwrdwn_common();
	}
}

/*******************************************************************************
 * RCAR handler called when an affinity instance is about to be suspended. The
 * level and mpidr determine the affinity instance. The 'state' arg. allows the
 * platform to decide whether the cluster is being turned off and take apt
 * actions.
 *
 * CAUTION: There is no guarantee that caches will remain turned on across calls
 * to this function as each affinity level is dealt with. So do not write & read
 * global variables across calls. It will be wise to do flush a write to the
 * global to prevent unpredictable results.
 ******************************************************************************/
void rcar_affinst_suspend(unsigned long sec_entrypoint, unsigned int afflvl,
		unsigned int state)
{
	unsigned long mpidr;

	/* Determine if any platform actions need to be executed. */
	if (rcar_do_plat_actions(afflvl, state) == -EAGAIN) {
		return;
	}

	/* Get the mpidr for this cpu */
	mpidr = read_mpidr_el1();

	/* Program the jump address for the this cpu */
	rcar_program_mailbox(mpidr, sec_entrypoint);

	/* Program the power controller to enable wakeup interrupts. */
	rcar_pwrc_enable_interrupt_wakeup(mpidr);

	/* Perform the common cpu specific operations */
	rcar_cpu_pwrdwn_common();

	/* Perform the common cluster specific operations */
	if (afflvl != MPIDR_AFFLVL0) {
		rcar_cluster_pwrdwn_common();
	}
}

/*******************************************************************************
 * RCAR handler called when an affinity instance has just been powered on after
 * being turned off earlier. The level and mpidr determine the affinity
 * instance. The 'state' arg. allows the platform to decide whether the cluster
 * was turned off prior to wakeup and do what's necessary to setup it up
 * correctly.
 ******************************************************************************/
void rcar_affinst_on_finish(unsigned int afflvl, unsigned int state)
{
	unsigned long mpidr;

	/* Determine if any platform actions need to be executed. */
	if (rcar_do_plat_actions(afflvl, state) == -EAGAIN) {
		return;
	}

	/* Get the mpidr for this cpu */
	mpidr = read_mpidr_el1();

	/* Perform the common cluster specific operations */
	if (afflvl != MPIDR_AFFLVL0) {
		/* Enable coherency if this cluster was off */
		rcar_cci_enable();
	}

	/*
	 * Prohibit cpu wake up by interrupt
	 */
	rcar_pwrc_disable_interrupt_wakeup(mpidr);

	/* Zero the jump address in the mailbox for this cpu */
	rcar_program_mailbox(mpidr, 0U);

	/* Enable the gic cpu interface */
	arm_gic_cpuif_setup();

	/* TODO: This setup is needed only after a cold boot */
	arm_gic_pcpu_distif_setup();
}

/*******************************************************************************
 * RCAR handler called when an affinity instance has just been powered on after
 * having been suspended earlier. The level and mpidr determine the affinity
 * instance.
 * TODO: At the moment we reuse the on finisher and reinitialize the secure
 * context. Need to implement a separate suspend finisher.
 ******************************************************************************/
void rcar_affinst_suspend_finish(unsigned int afflvl, unsigned int state)
{
	rcar_affinst_on_finish(afflvl, state);
}

/*******************************************************************************
 * RCAR handlers to shutdown/reboot the system
 ******************************************************************************/
static void __dead2 rcar_system_off(void)
{
	uint64_t my_cpu;
	int32_t rtn_primary;
	int32_t rtn_on;

	my_cpu = read_mpidr_el1();
	rtn_primary = platform_is_primary_cpu(my_cpu);
	rtn_on = cpu_on_check(my_cpu);
	if ((rtn_primary != 0) && (rtn_on == 0)) {
		rcar_pwrc_cpuoff(my_cpu);
		rcar_pwrc_clusteroff(my_cpu);
	} else {
		panic();
	}
	wfi();

	ERROR("RCAR System Off: operation not handled.\n");
	panic();
}

static void __dead2 rcar_system_reset(void)
{
	rcar_pwrc_system_reset();

	ERROR("RCAR System Reset: operation not handled.\n");
	panic();
}

static int32_t cpu_on_check(uint64_t mpidr)
{
	uint64_t i;
	uint64_t j;
	uint64_t cpu_count;
	uintptr_t reg_PSTR;
	uint32_t status;
	uint64_t my_cpu;
	int32_t rtn;

	const uint64_t cpu_num_in_core[PLATFORM_MAX_AFFLVL + 1] = {
			(uint64_t)PLATFORM_CLUSTER0_CORE_COUNT,
			(uint64_t)PLATFORM_CLUSTER1_CORE_COUNT
	};
	const uintptr_t registerPSTR[PLATFORM_MAX_AFFLVL + 1] = {
			RCAR_CA57PSTR,
			RCAR_CA53PSTR
	};

	rtn = 0;
	my_cpu = mpidr & ((uint64_t)((MPIDR_CLUSTER_MASK) | (MPIDR_CPU_MASK)));
	for (i = 0U; i < ((uint64_t)(PLATFORM_MAX_AFFLVL + 1U)); i++) {
		cpu_count = cpu_num_in_core[i];
		reg_PSTR = registerPSTR[i];
		for (j = 0U; j < cpu_count; j++) {
			if (my_cpu != ((i * 0x100U) + j)) {
				status = mmio_read_32(reg_PSTR) >> (j * 4U);
				if ((status & 0x00000003U) == 0U) {
					rtn--;
				}
			}
		}
	}
	return (rtn);

}

/*******************************************************************************
 * RCAR handler called to check the validity of the power state parameter.
 ******************************************************************************/
int rcar_validate_power_state(unsigned int power_state)
{
	/* Sanity check the requested state */
	if (psci_get_pstate_type(power_state) == PSTATE_TYPE_STANDBY) {
		/*
		 * It's possible to enter standby only on affinity level 0
		 * i.e. a cpu on the rcar. Ignore any other affinity level.
		 */
		if (psci_get_pstate_afflvl(power_state) != MPIDR_AFFLVL0) {
			return PSCI_E_INVALID_PARAMS;
		}
	}

	/*
	 * We expect the 'state id' to be zero.
	 */
	if (psci_get_pstate_id(power_state) != 0U) {
		return PSCI_E_INVALID_PARAMS;
	}

	return PSCI_E_SUCCESS;
}

unsigned int rcar_get_sys_suspend_power_state(void)
{
	return psci_make_powerstate(0, PSTATE_TYPE_POWERDOWN,
			PLATFORM_MAX_AFFLVL);
}
/*******************************************************************************
 * Export the platform handlers to enable psci to invoke them
 ******************************************************************************/
static const plat_pm_ops_t rcar_plat_pm_ops = {
	.affinst_standby = rcar_affinst_standby,
	.affinst_on = rcar_affinst_on,
	.affinst_off = rcar_affinst_off,
	.affinst_suspend = rcar_affinst_suspend,
	.affinst_on_finish = rcar_affinst_on_finish,
	.affinst_suspend_finish = rcar_affinst_suspend_finish,
	.system_off = rcar_system_off,
	.system_reset = rcar_system_reset,
	.validate_power_state = rcar_validate_power_state,
	.get_sys_suspend_power_state = rcar_get_sys_suspend_power_state
};

/*******************************************************************************
 * Export the platform specific power ops & initialize the rcar power controller
 ******************************************************************************/
int platform_setup_pm(const plat_pm_ops_t **plat_ops)
{
	*plat_ops = &rcar_plat_pm_ops;
	return 0;
}
