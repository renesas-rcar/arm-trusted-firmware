/*
 * Copyright (c) 2015-2021, Renesas Electronics Corporation. All rights reserved.
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

#include "pwrc.h"
#include "rcar_def.h"
#include "rcar_private.h"

/*
 * Someday there will be a generic power controller api. At the moment each
 * platform has its own pwrc so just exporting functions should be acceptable.
 */
static RCAR_INSTANTIATE_LOCK


/* APSREG boot configuration */
static inline uintptr_t APSREG_AP_CLUSTER_AUX0(uint32_t n)
{
	return (APSREG_BASE + UL(0x10) + (((n) & UL(0x3)) * UL(0x1000)));
}

/* APMU */
static inline uintptr_t RCAR_APMU_CLUSTER_BASE(uint32_t n)
{
	return (RCAR_APMU_BASE + UL(0x400) + (((n) & UL(0x3)) * UL(0x40)));
}
static inline uintptr_t RCAR_APMU_CPU_BASE(uint32_t n)
{
	return (RCAR_APMU_BASE + UL(0x800) + (((n) & UL(0x6)) * UL(0x100))
			+ (((n) & UL(0x1)) * UL(0x40)));
}
static inline uintptr_t RCAR_APMU_PWRCTRLCL(uint32_t n)
{
	return RCAR_APMU_CLUSTER_BASE(n);
}
static inline uintptr_t RCAR_APMU_PWRCTRLC(uint32_t n)
{
	return RCAR_APMU_CPU_BASE(n);
}
static inline uintptr_t RCAR_APMU_SAFECTRLC(uint32_t n)
{
	return (RCAR_APMU_CPU_BASE(n) + UL(0x20));
}
static inline uintptr_t RCAR_APMU_RVBARPLC(uint32_t n)
{
	return (RCAR_APMU_CPU_BASE(n) + UL(0x38));
}
static inline uintptr_t RCAR_APMU_RVBARPHC(uint32_t n)
{
	return (RCAR_APMU_CPU_BASE(n) + UL(0x3C));
}

/* Product register */
static inline uint32_t PRR_CA55_XX_EN_CPU(uint32_t n)
{
	return ((uint32_t)(1) << ((n) & U(1)));
}
static inline uint32_t PRR_CA55_XX_EN_SHIFT(uint32_t c)
{
	return ((uint32_t)(20) + (((c) & U(3)) * U(3)));
}

static inline void write_cpupwrctlr(u_register_t v)
{
	__asm__ volatile ("msr S3_0_C15_C2_7, %0" : : "r" (v));
}

static void rcar_pwrc_cpuoff_sub(uint32_t cpu);
static uint32_t rcar_pwrc_get_cpu_num(uint32_t c);
static uint32_t rcar_pwrc_get_cpu_num_sub(uint32_t c);
static uint32_t rcar_pwrc_core_pos(u_register_t mpidr);


void rcar_pwrc_cpuon(u_register_t mpidr)
{
	uint32_t cpu;
	uint32_t cluster;

	rcar_lock_get();

	cpu = rcar_pwrc_core_pos(mpidr);

	cluster = rcar_pwrc_get_mpidr_cluster(mpidr);

	/* clear Cluster OFF bit */
	mmio_clrbits_32(RCAR_APMU_PWRCTRLCL(cluster),
			RCAR_APMU_PWRCTRLCL_PCHPDNEN);

	/* clear Core OFF bit */
	mmio_clrbits_32(RCAR_APMU_PWRCTRLC(cpu), RCAR_APMU_PWRCTRLC_PCHPDNEN);
	while ((mmio_read_32(RCAR_APMU_PWRCTRLC(cpu)) &
			RCAR_APMU_PWRCTRLC_PCHPDNEN) != 0U)
		;

	mmio_setbits_32(RCAR_APMU_PWRCTRLC(cpu), RCAR_APMU_PWRCTRLC_WUP_REQ);

	/* Wait until CA55 wake up sequence finishes */
	while ((mmio_read_32(RCAR_APMU_PWRCTRLC(cpu)) &
			RCAR_APMU_PWRCTRLC_WUP_REQ)
			== RCAR_APMU_PWRCTRLC_WUP_REQ)
		;

	rcar_lock_release();
}

void rcar_pwrc_cpuoff(u_register_t mpidr)
{
	uint32_t cpu;

	rcar_lock_get();

	cpu = rcar_pwrc_core_pos(mpidr);

	rcar_pwrc_cpuoff_sub(cpu);

	rcar_lock_release();
}

void rcar_pwrc_enable_interrupt_wakeup(u_register_t mpidr)
{
	uint32_t cpu;

	rcar_lock_get();

	cpu = rcar_pwrc_core_pos(mpidr);

	mmio_setbits_32(RCAR_APMU_PWRCTRLC(cpu), RCAR_APMU_PWRCTRLC_IWUP_EN);

	rcar_lock_release();
}

void rcar_pwrc_disable_interrupt_wakeup(u_register_t mpidr)
{
	uint32_t cpu;

	rcar_lock_get();

	cpu = rcar_pwrc_core_pos(mpidr);

	mmio_clrbits_32(RCAR_APMU_PWRCTRLC(cpu), RCAR_APMU_PWRCTRLC_IWUP_EN);

	rcar_lock_release();
}

void rcar_pwrc_clusteroff(u_register_t mpidr)
{
	uint32_t cluster;
	uint32_t cpu;

	rcar_lock_get();

	cpu = rcar_pwrc_core_pos(mpidr);

	cluster = rcar_pwrc_get_mpidr_cluster(mpidr);

	/* for Cluster OFF */
	mmio_setbits_32(RCAR_APMU_PWRCTRLCL(cluster),
			RCAR_APMU_PWRCTRLCL_PCHPDNEN);

	rcar_pwrc_cpuoff_sub(cpu);

	rcar_lock_release();
}

void rcar_pwrc_setup(void)
{
	uintptr_t rst_barh;
	uintptr_t rst_barl;
	uint32_t i, j;
	uint32_t reg;
	uint32_t cnum;
	uint64_t reset = (uint64_t) (&plat_secondary_reset) & 0xFFFFFFFFU;

	reset &= RCAR_APMU_RVBARPLC_MASK;
	reset |= RCAR_APMU_RVBARPL_VLD;

	reg = mmio_read_32(RCAR_PRR) >> 17;
	for (i = 0; i < PLATFORM_CLUSTER_COUNT; i++) {
		reg >>= 3;
		if ((reg & PRR_CA55_XX_EN_CLUSTER_MASK) != RCAR_CPU_HAVE_CA55) {
			continue;
		}

		mmio_setbits_32(APSREG_AP_CLUSTER_AUX0(i),
			      APSREG_AP_CLUSTER_AUX0_INIT);
		cnum = rcar_pwrc_get_cpu_num(i + 1U);
		for (j = 0; j < cnum; j++) {
			uint32_t cpu = (i * PLATFORM_MAX_CPUS_PER_CLUSTER) + j;

			if ((reg & PRR_CA55_XX_EN_CPU(cpu)) != RCAR_CPU_HAVE_CA55) {
				continue;
			}

			rst_barh = RCAR_APMU_RVBARPHC(cpu);
			rst_barl = RCAR_APMU_RVBARPLC(cpu);
			mmio_write_32(rst_barh, 0);
			mmio_write_32(rst_barl, (uint32_t)reset);
		}
	}
	mmio_setbits_32(APSREG_CCI500_AUX, APSREG_CCI500_AUX_INIT);
	mmio_setbits_32(APSREG_P_CCI500_AUX, APSREG_P_CCI500_AUX_INIT);

	rcar_lock_init();
}

uint32_t rcar_pwrc_get_mpidr_cluster(u_register_t mpidr)
{
	int32_t cluster = rcar_cluster_pos_by_mpidr(mpidr);

	if (cluster < 0) {
		ERROR("BL3-1 : The value of passed MPIDR is invalid.");
		panic();
	}

	return (uint32_t)cluster;
}

static uint32_t rcar_pwrc_get_cpu_num(uint32_t c)
{
	uint32_t i;
	uint32_t count = 0;

	if (c == 0U) {
		for (i = 0; i < PLATFORM_CLUSTER_COUNT; i++) {
			count += rcar_pwrc_get_cpu_num_sub(i);
		}
	} else {
		count = rcar_pwrc_get_cpu_num_sub(c - 1U);
	}

	return count;
}

static uint32_t rcar_pwrc_get_cpu_num_sub(uint32_t c)
{
	uint32_t i;
	uint32_t count = 0;
	uint32_t reg = mmio_read_32(RCAR_PRR);

	reg >>= PRR_CA55_XX_EN_SHIFT(c);
	if ((reg & PRR_CA55_XX_EN_CLUSTER_MASK) != RCAR_CPU_HAVE_CA55) {
		return 0;
	}

	for (i = 0; i < PLATFORM_MAX_CPUS_PER_CLUSTER; i++) {
		if ((reg & PRR_CA55_XX_EN_CPU(i)) == RCAR_CPU_HAVE_CA55) {
			count++;
		}
	}

	return count;
}

static void rcar_pwrc_cpuoff_sub(uint32_t cpu)
{
	/* Clear DBGGEN_PPDN bit for core down to 'OFF' mode */
	mmio_clrbits_32(RCAR_APMU_SAFECTRLC(cpu), RCAR_APMU_SAFECTRLC_DBGGEN);
	/* for Core OFF */
	mmio_setbits_32(RCAR_APMU_PWRCTRLC(cpu), RCAR_APMU_PWRCTRLC_PCHPDNEN);

	write_cpupwrctlr(CPUPWRCTLR_PWDN);
}

static uint32_t rcar_pwrc_core_pos(u_register_t mpidr)
{
	int cpu;

	cpu = plat_core_pos_by_mpidr(mpidr);
	if (cpu < 0) {
		ERROR("BL3-1 : The value of passed MPIDR is invalid.");
		panic();
	}

	return (uint32_t)cpu;
}
