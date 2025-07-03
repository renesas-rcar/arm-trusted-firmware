/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RCAR_DEF_H
#define RCAR_DEF_H

#include <common/tbbr/tbbr_img_def.h>
#include <lib/utils_def.h>

#define RCAR_DOMAIN			UL(0x0)

#define RCAR_TRUSTED_SRAM_BASE		UL(0x8C200000) /* DRAM */
#define RCAR_TRUSTED_SRAM_SIZE		UL(0x00040000) /* 256kB */
#define RCAR_SHARED_MEM_BASE		(RCAR_TRUSTED_SRAM_BASE + \
					RCAR_TRUSTED_SRAM_SIZE)
#define RCAR_SHARED_MEM_SIZE		UL(0x00002000) /* 8kB */
#define	RCAR_BL31_CRASH_BASE		(RCAR_SHARED_MEM_BASE + RCAR_SHARED_MEM_SIZE)
#define RCAR_BL31_CRASH_SIZE		UL(0x00001000)
#define DEVICE_RCAR_BASE1		UL(0x10000000)
#define DEVICE_RCAR_SIZE1		UL(0x30000000)
#define DEVICE_RCAR_BASE2		UL(0xC0000000)
#define DEVICE_RCAR_SIZE2		UL(0x00C00000)
#define DEVICE_SRAM_BASE		UL(0xE9042000)
#define DEVICE_SRAM_SIZE		UL(0x00002000)
#define DEVICE_SRAM_DATA_BASE		(DEVICE_SRAM_BASE + DEVICE_SRAM_SIZE)
#define DEVICE_SRAM_DATA_SIZE		UL(0x00000100)
#define DEVICE_SRAM_STACK_BASE		(DEVICE_SRAM_DATA_BASE + DEVICE_SRAM_DATA_SIZE)
#define DEVICE_SRAM_STACK_SIZE		(UL(0x00001000) - DEVICE_SRAM_DATA_SIZE)
#define DEVICE_RCAR_BASE3		UL(0xE5000000)
#define DEVICE_RCAR_SIZE3		UL(0x1B000000)
/* Entrypoint mailboxes */
#define MBOX_BASE			RCAR_SHARED_MEM_BASE
#define MBOX_SIZE			UL(0x800) /* 2kB: 32 cores */
/* Base address where parameters to BL31 are stored */
#define PARAMS_BASE			(RCAR_TRUSTED_SRAM_BASE - UL(0x100000))
#define PARAMS_SIZE			UL(0x8000) /* 32kB */
#define BOOT_KIND_BASE			(PARAMS_BASE + UL(0x1700))

/*
 * The number of regions like RO(code), coherent and data required by
 * different BL stages which need to be mapped in the MMU
 */
#define RCAR_BL_REGIONS			(2)
/*
 * The RCAR_MAX_MMAP_REGIONS depends on the number of entries in rcar_mmap[]
 * defined for each BL stage in platform_common.c.
 */
#define RCAR_MMAP_ENTRIES		(10)
/* BL31 */
#define RCAR_CRASH_STACK		RCAR_BL31_CRASH_BASE

/* CCI related constants */
#define CCI500_BASE			UL(0xF1200000)
#define CCI500_CLUSTER0_SL_IFACE_IX	(0)
#define CCI500_CLUSTER1_SL_IFACE_IX	(1)
#define CCI500_CLUSTER2_SL_IFACE_IX	(2)
#define CCI500_CLUSTER3_SL_IFACE_IX	(3)
#define RCAR_CCI_BASE			CCI500_BASE
/* GIC */
#define RCAR_GICD_BASE			UL(0x39000000) /* GICD base address for View 1 */
#define RCAR_GICR_BASE			UL(0x38080000)

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
#define RCAR_CNTC_BASE			UL(0x1C000000)
#define RCAR_CNTC_EXTAL			U(1066666667)


/* Counter Count Value Lower register */
#define RCAR_CNTCVL_OFF			UL(0x008)
/* Counter Count Value Upper register */
#define RCAR_CNTCVU_OFF			UL(0x00C)
/* Conversion value from seconds to micro seconds */
#define RCAR_CONV_MICROSEC		UL(1000000)


// Workaround to ensure only RSIPM IPL output log firstly
#define BOOT_BASE_ADDRESS	U(0x1003FC00)
#define BOOT_READY_CR52_FLAG	U(0x01234200)
#define BOOT_BL31_REG		(BOOT_BASE_ADDRESS + U(0x200))


/* Memory mapped Generic timer interfaces */
#define ARM_SYS_CNTCTL_BASE		RCAR_CNTC_BASE

/* MPIDR_EL1 */
#define	RCAR_MPIDR_AFFMASK		U(0x00FFFF00)

/* CPUPWRCTLR */
#define CPUPWRCTLR_PWDN			U(0x00000001)

/* For SCMI message */
#define RCAR_SCMI_CHANNEL_MMU_BASE		UL(0xC1060000) // align 4kB
#define RCAR_SCMI_CHANNEL_SIZE		UL(0x00001000) // align 4kB (SCP FW defines 0x100)
#define RCAR_SCMI_CHANNEL_BASE		UL(0xC1060E00) //  for A2P PSCI Command (SCP FW defines)

#define MFIS_SCP_COMMON_BASE            UL(0x189E1000)
#define MFIS_MFISWACNTR_SCP             (MFIS_SCP_COMMON_BASE + UL(0x00000904))
#define MFISWACNTR_SCP_CODEVALUE_SET    UL(0xACC00000)
#define MFISWACNTR_SCP_REGISTERADDRESS_MASK	UL(0x000FFFFF)
/* MFIS CPU communication control register AP System core[k] to SCP Core(k=0-31) MFISASEICR[k=0] */
#define RCAR_SCMI_MFIS_ADDR		UL(0x18840004)
#define RCAR_SCMI_MFIS_MOD_MASK		U(0x00000001)
#define RCAR_SCMI_MFIS_PRV_MASK		(~RCAR_SCMI_MFIS_MOD_MASK)

#endif /* RCAR_DEF_H */
