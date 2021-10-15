/*
 * Copyright (c) 2015-2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RCAR_DEF_H
#define RCAR_DEF_H

#include <common/tbbr/tbbr_img_def.h>
#include <lib/utils_def.h>

#define RCAR_DOMAIN			UL(0x0)

#define RCAR_TRUSTED_SRAM_BASE		UL(0x46400000)
#define RCAR_TRUSTED_SRAM_SIZE		UL(0x00022000)
#define RCAR_SHARED_MEM_BASE		(RCAR_TRUSTED_SRAM_BASE + \
					RCAR_TRUSTED_SRAM_SIZE)
#define RCAR_SHARED_MEM_SIZE		UL(0x00001000)
#define DEVICE_RCAR_BASE		UL(0xE6000000)
#define DEVICE_RCAR_SIZE		UL(0x1A000000)
#define	RCAR_BL31_CRASH_BASE		(RCAR_TRUSTED_SRAM_BASE + UL(0x3F000))
#define RCAR_BL31_CRASH_SIZE		UL(0x00001000)
/* Entrypoint mailboxes */
#define MBOX_BASE			RCAR_SHARED_MEM_BASE
#define MBOX_SIZE			UL(0x200)
/* Base address where parameters to BL31 are stored */
#define PARAMS_BASE			(MBOX_BASE + MBOX_SIZE)
#define BOOT_KIND_BASE			(RCAR_SHARED_MEM_BASE + \
					RCAR_SHARED_MEM_SIZE - UL(0x100))
/*
 * The number of regions like RO(code), coherent and data required by
 * different BL stages which need to be mapped in the MMU
 */
#if USE_COHERENT_MEM
#define RCAR_BL_REGIONS			(3)
#else
#define RCAR_BL_REGIONS			(2)
#endif
/*
 * The RCAR_MAX_MMAP_REGIONS depends on the number of entries in rcar_mmap[]
 * defined for each BL stage in platform_common.c.
 */
#define RCAR_MMAP_ENTRIES		(4)
/* BL31 */
#define RCAR_CRASH_STACK		RCAR_BL31_CRASH_BASE

/* CCI related constants */
#define CCI500_BASE			UL(0xF1200000)
#define CCI500_CLUSTER0_SL_IFACE_IX	(0)
#define CCI500_CLUSTER1_SL_IFACE_IX	(1)
#define CCI500_CLUSTER2_SL_IFACE_IX	(2)
#define CCI500_CLUSTER3_SL_IFACE_IX	(3)
#define RCAR_CCI_BASE			CCI500_BASE

/* APSREG boot configuration */
#define APSREG_BASE			UL(0xE6280000)
#define APSREG_CCI500_AUX		(APSREG_BASE + UL(0x9010))
#define APSREG_P_CCI500_AUX		(APSREG_BASE + UL(0x29010))

#define APSREG_AP_CLUSTER_AUX0_INIT	U(0x00000003)
#define APSREG_CCI500_AUX_INIT		U(0x00000001)
#define APSREG_P_CCI500_AUX_INIT	U(0x00000002)

/* GIC */
#define RCAR_GICD_BASE			UL(0xF1000000)
#define RCAR_GICR_BASE			UL(0xF1060000)
#define ARM_IRQ_SEC_PHY_TIMER		U(29)
#define ARM_IRQ_SEC_SGI_0		U(8)
#define ARM_IRQ_SEC_SGI_1		U(9)
#define ARM_IRQ_SEC_SGI_2		U(10)
#define ARM_IRQ_SEC_SGI_3		U(11)
#define ARM_IRQ_SEC_SGI_4		U(12)
#define ARM_IRQ_SEC_SGI_5		U(13)
#define ARM_IRQ_SEC_SGI_6		U(14)
#define ARM_IRQ_SEC_SGI_7		U(15)

/* Timer control */
#define RCAR_CNTC_BASE			UL(0xE6080000)
#define RCAR_CNTC_EXTAL			U(16666600)

/* APMU */
#define RCAR_APMU_BASE			(UL(0xE6170000) + (RCAR_DOMAIN * UL(0x1000)))
#define RCAR_APMU_PWRCTRLCL_PCHPDNEN	((uint32_t)(1) << U(17))

#define RCAR_APMU_PWRCTRLC_WUP_REQ	((uint32_t)(1) << U(0))
#define RCAR_APMU_PWRCTRLC_IWUP_EN	((uint32_t)(1) << U(4))
#define RCAR_APMU_PWRCTRLC_PCHPDNEN	((uint32_t)(1) << U(17))

#define RCAR_APMU_SAFECTRLC_DBGGEN	((uint32_t)(1) << U(13))

#define RCAR_APMU_RVBARPLC_MASK		U(0xFFFFFFFC)
#define RCAR_APMU_RVBARPL_VLD		U(0x00000001)

/* Product register */
#define RCAR_PRR			UL(0xFFF00044)
#define RCAR_CPU_HAVE_CA55		U(0x00000000)
#define PRR_CA55_XX_EN_CLUSTER_MASK	U(0x00000004)

/* Memory mapped Generic timer interfaces */
#define ARM_SYS_CNTCTL_BASE		RCAR_CNTC_BASE

/* MPIDR_EL1 */
#define	RCAR_MPIDR_AFFMASK		U(0x00FFFF00)

/* CPUPWRCTLR */
#define CPUPWRCTLR_PWDN			U(0x00000001)

#endif /* RCAR_DEF_H */
