/*
 * Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2019, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch_helpers.h>
#include <gicv2.h>
#include <bl_common.h>
#include <bakery_lock.h>
#include <cci.h>
#include <debug.h>
#include <mmio.h>
#include <platform.h>
#include <platform_def.h>
#include <psci.h>
#include <errno.h>
#include "drivers/pwrc/rcar_pwrc.h"
#include "drivers/iic_dvfs/iic_dvfs.h"
#include "rcar_def.h"
#include "rcar_private.h"
#include "rcar_pm.h"
#include "ulcb_cpld.h"

static void rcar_program_mailbox(uint64_t mpidr, uint64_t address);
static int32_t rcar_do_plat_actions(unsigned int afflvl, unsigned int state);
static void rcar_cpu_pwrdwn_common(void);
static void rcar_cluster_pwrdwn_common(void);
static void __dead2 rcar_system_off(void);
static void __dead2 rcar_system_reset(void);
static int32_t cpu_on_check(uint64_t mpidr) __unused;

#define	RCAR_GENERIC_TIMER_STACK	(0x300)
#define	RCAR_BOOT_MODE			(0x01U)
#define	RCAR_BOOT_COLD			(0x00U)

#define	RCAR_MPIDR_CA57_CPU0		((uint64_t)0x0000U)
#define	RCAR_MPIDR_CA53_CPU0		((uint64_t)0x0100U)

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
	gicv2_cpuif_disable();

	/* Program the power controller to power off this cpu. */
	rcar_pwrc_cpuoff (read_mpidr_el1());
}

/*******************************************************************************
 * Function which implements the common RCAR specific operations to power down a
 * cluster in response to a CPU_OFF or CPU_SUSPEND request.
 ******************************************************************************/
static void rcar_cluster_pwrdwn_common(void)
{
	uint32_t cluster_type;
	uint64_t mpidr = read_mpidr_el1();

	cluster_type = rcar_bl31_get_cluster();
	if (RCAR_CLUSTER_A53A57 == cluster_type) {
		/* Disable coherency if this cluster is to be turned off */
		rcar_cci_disable();
	}

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

	if (state != PSCI_STATE_OFF) {
		return -EAGAIN;
	}

	/*
	 * Find the highest affinity level which will be suspended and postpone
	 * all the platform specific actions until that level is hit.
	 */
	max_phys_off_afflvl = psci_get_max_phys_off_afflvl();
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
	uint32_t scr_el3;

	scr_el3 = read_scr_el3();
	write_scr_el3(scr_el3 | SCR_IRQ_BIT);
	dsb();
	wfi();
	write_scr_el3(scr_el3);
}

/*******************************************************************************
 * RCAR handler called when an affinity instance is about to be turned on. The
 * level and mpidr determine the affinity instance.
 ******************************************************************************/
int rcar_affinst_on(unsigned long mpidr, unsigned long sec_entrypoint,
		unsigned int afflvl, unsigned int state)
{
	int rc = PSCI_E_SUCCESS;

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
	uint32_t cluster_type;
	unsigned long mpidr;

	/* Determine if any platform actions need to be executed. */
	if (rcar_do_plat_actions(afflvl, state) == -EAGAIN) {
		return;
	}

	/* Get the mpidr for this cpu */
	mpidr = read_mpidr_el1();

	cluster_type = rcar_bl31_get_cluster();

	/* Perform the common cluster specific operations */
	if ((afflvl != MPIDR_AFFLVL0) && (RCAR_CLUSTER_A53A57 == cluster_type)) {
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
	gicv2_cpuif_enable();

	/* Program the gic per-cpu distributor or re-distributor interface */
	gicv2_pcpu_distif_init();
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
	uint32_t cluster_type;

	if ((uint32_t)afflvl >= (uint32_t)PLATFORM_MAX_AFFLVL) {
		plat_arm_gic_driver_init();
		plat_arm_gic_init();

		cluster_type = rcar_bl31_get_cluster();
		if (RCAR_CLUSTER_A53A57 == cluster_type) {
			rcar_cci_init();
		}

		rcar_bl31_restore_timer_state();
		rcar_pwrc_setup();
		rcar_bl31_code_copy_to_system_ram(RCAR_DYNAMIC_REGION_EXIST);
#if RCAR_SYSTEM_SUSPEND
		rcar_bl31_init_suspend_to_ram();
#endif /* RCAR_SYSTEM_SUSPEND */
	}

	rcar_affinst_on_finish(afflvl, state);

}

/*******************************************************************************
 * RCAR handlers to shutdown/reboot the system
 ******************************************************************************/
static void __dead2 rcar_system_off(void)
{
#if PMIC_ROHM_BD9571
#if PMIC_LEVEL_MODE
	int32_t error;

	error = rcar_iic_dvfs_send(SLAVE_ADDR_PMIC
					,REG_ADDR_DVFS_SetVID
					,REG_DATA_DVFS_SetVID_0V);
	if (error != 0) {
		ERROR("BL3-1:Failed the SYSTEM-OFF.\n");
	}
#else /* pulse mode */
	int32_t error;

	error = rcar_iic_dvfs_send(SLAVE_ADDR_PMIC
					,REG_ADDR_BKUP_Mode_Cnt
					,REG_DATA_P_ALL_OFF);
	if (error != 0) {
		ERROR("BL3-1:Failed the SYSTEM-RESET.\n");
	}
#endif
#else /* PMIC_ROHM_BD9571 */
	uint64_t my_cpu;
	uint32_t boot_mpidr_ret;
	int32_t rtn_on;

	my_cpu = read_mpidr_el1();
	boot_mpidr_ret = bl31_plat_boot_mpidr_chk();
	rtn_on = cpu_on_check(my_cpu);
	if ((RCAR_MPIDRCHK_BOOTCPU == boot_mpidr_ret) && (rtn_on == 0)) {
		rcar_pwrc_cpuoff(my_cpu);
		rcar_pwrc_clusteroff(my_cpu);
	} else {
		panic();
	}
#endif /* PMIC_ROHM_BD9571 */
	wfi();
	ERROR("RCAR System Off: operation not handled.\n");
	panic();
}

static void __dead2 rcar_system_reset(void)
{
#if PMIC_ROHM_BD9571
#if PMIC_LEVEL_MODE
	int32_t error;

#if RCAR_SYSTEM_RESET_KEEPON_DDR
	uint8_t		mode;

	error = rcar_iic_dvfs_send(SLAVE_ADDR_PMIC
					,REG_ADDR_REG_KEEP10
					,REG_DATA_REG_KEEP10_MAGIC);
	if (error != 0) {
		ERROR("Failed send KEEP10 magic ret=%d \n",error);
	} else {

		error = rcar_iic_dvfs_recieve(SLAVE_ADDR_PMIC
					,REG_ADDR_BKUP_Mode_Cnt, &mode);
		if (error != 0) {
			ERROR("Failed recieve BKUP_Mode_Cnt ret=%d \n",error);
		} else {
			mode = ( mode | REG_DATA_KEEPON_DDR1C |
					REG_DATA_KEEPON_DDR0C |
					REG_DATA_KEEPON_DDR1  |
					REG_DATA_KEEPON_DDR0 );

			error = rcar_iic_dvfs_send(SLAVE_ADDR_PMIC
						,REG_ADDR_BKUP_Mode_Cnt
						,mode);
			if (error != 0) {
				ERROR("Failed send KEEPON_DDRx ret=%d \n",error);
			} else {
				rcar_bl31_set_suspend_to_ram();
			}
		}
	}
#else /* RCAR_SYSTEM_RESET_KEEPON_DDR */
	error = rcar_iic_dvfs_send(SLAVE_ADDR_PMIC
					,REG_ADDR_BKUP_Mode_Cnt
					,REG_DATA_P_ALL_OFF);
	if (error != 0) {
		ERROR("BL3-1:Failed the SYSTEM-RESET.\n");
	}
#endif /* RCAR_SYSTEM_RESET_KEEPON_DDR */
#else /* pulse mode */
	#if (RCAR_GEN3_ULCB==1)
	/* Starter Kit */
		cpld_reset_cpu();
	#endif
#endif
#else /* PMIC_ROHM_BD9571 */
	rcar_pwrc_system_reset();
#endif /* PMIC_ROHM_BD9571 */
	wfi();
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
	uint32_t my_cluster_type;

	const uint32_t cluster_type[PLATFORM_CLUSTER_COUNT] = {
			RCAR_CLUSTER_CA53,
			RCAR_CLUSTER_CA57
	};
	const uintptr_t registerPSTR[PLATFORM_CLUSTER_COUNT] = {
			RCAR_CA53PSTR,
			RCAR_CA57PSTR
	};

	my_cluster_type = rcar_bl31_get_mpidr_cluster(mpidr);

	rtn = 0;
	my_cpu = mpidr & ((uint64_t)(MPIDR_CPU_MASK));
	for (i = 0U; i < ((uint64_t)(PLATFORM_CLUSTER_COUNT)); i++) {
		cpu_count = rcar_bl31_get_cpu_num(cluster_type[i]);
		reg_PSTR = registerPSTR[i];
		for (j = 0U; j < cpu_count; j++) {
			if ((my_cluster_type != cluster_type[i]) || (my_cpu != j)) {
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
		if (psci_get_pstate_pwrlvl(power_state) != PSCI_CPU_PWR_LVL) {
			return PSCI_E_INVALID_PARAMS;
		}
	} else {
		/*
		 * System suspend is only supported via PSCI SYSTEM_SUSPEND.
		 */
		if (psci_get_pstate_pwrlvl(power_state) == PLAT_MAX_PWR_LVL) {
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
#if RCAR_SYSTEM_SUSPEND
uint32_t rcar_get_sys_suspend_power_state(void)
{
	return psci_make_powerstate(0, PSTATE_TYPE_POWERDOWN,
			PLATFORM_MAX_AFFLVL);
}
#endif /* RCAR_SYSTEM_SUSPEND */
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
#if RCAR_SYSTEM_SUSPEND
	.get_sys_suspend_power_state = rcar_get_sys_suspend_power_state
#endif /* RCAR_SYSTEM_SUSPEND */
};

/*******************************************************************************
 * Export the platform specific power ops & initialize the rcar power controller
 ******************************************************************************/
int platform_setup_pm(const plat_pm_ops_t **plat_ops)
{
	*plat_ops = &rcar_plat_pm_ops;

#if RCAR_SYSTEM_SUSPEND
	rcar_bl31_init_suspend_to_ram();
#endif /* RCAR_SYSTEM_SUSPEND */

	return 0;
}
