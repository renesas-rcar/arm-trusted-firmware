/*
 * Copyright (c) 2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2019, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RCAR_DEF_H__
#define RCAR_DEF_H__

#include <tbbr_img_def.h>
#include <utils_def.h>

/* Firmware Image Package */
#define RCAR_PRIMARY_CPU			0x0

/*******************************************************************************
 * RCAR memory map related constants
 ******************************************************************************/

/* The first 4KB of Trusted SRAM are used as shared memory */
#define RCAR_SHARED_MEM_BASE	(RCAR_TRUSTED_SRAM_BASE + RCAR_TRUSTED_SRAM_SIZE)
#define RCAR_SHARED_MEM_SIZE	U(0x00001000)	/* 4 KB */

/* The remaining Trusted SRAM is used to load the BL images */
#define RCAR_TRUSTED_SRAM_BASE	U(0x44000000)	/* Trusted Firmware Image */
#define RCAR_TRUSTED_SRAM_SIZE	U(0x0003E000)	/* 248 KB */

#define FLASH0_BASE		U(0x08000000)	/* LBSC RPC address */
#define FLASH0_SIZE		U(0x04000000)	/* 64MB */
#define FLASH_MEMORY_SIZE	U(0x04000000)	/* HyperFlash 64MB */
#define FLASH_TRANS_SIZE_UNIT	U(0x00000100)	/* HyperFlash transfer size units */

/* Aggregate of all devices in the first GB */
#define DEVICE_RCAR_BASE	U(0xE6000000)
#define DEVICE_RCAR_SIZE	U(0x00300000)

#define DEVICE_RCAR_BASE2	U(0xE6360000)
#define DEVICE_RCAR_SIZE2	U(0x19CA0000)

#define DEVICE_SRAM_BASE	U(0xE6300000)
#define DEVICE_SRAM_SIZE	U(0x00002000)

#define DEVICE_SRAM_STACK_BASE	(DEVICE_SRAM_BASE + DEVICE_SRAM_SIZE)
#define DEVICE_SRAM_STACK_SIZE	U(0x00001000)


#define DRAM1_BASE		U(0x40000000)
#define DRAM1_SIZE		U(0x80000000)

#define DRAM1_NS_BASE		(DRAM1_BASE + U(0x8000000))
#define DRAM1_NS_SIZE		(DRAM1_SIZE - DRAM1_NS_BASE)

#define DRAM_LIMIT		ULL(0x0000010000000000)	/* AArch64 */


/* for Loading BL33-BL33x source/destination address range check */
#define	DRAM_40BIT_BASE			ULL(0x0400000000)
#define	DRAM_40BIT_SIZE			ULL(0x0400000000)

#define	DRAM_PROTECTED_BASE		ULL(0x43F00000)
#define	DRAM_40BIT_PROTECTED_BASE	ULL(0x0403F00000)
#define	DRAM_PROTECTED_SIZE		ULL(0x03F00000)

#define	AARCH64_SPACE_BASE		ULL(0x00000000000)
#define	AARCH64_SPACE_SIZE		ULL(0x10000000000)

/*
 * The number of regions like RO(code), coherent and data required by
 * different BL stages which need to be mapped in the MMU.
 */
#if USE_COHERENT_MEM
#define RCAR_BL_REGIONS		3
#else
#define RCAR_BL_REGIONS		2
#endif

/*
 * The RCAR_MAX_MMAP_REGIONS depend on the number of entries in rcar_mmap[]
 * defined for each BL stage in rcar_common.c.
 */
#if IMAGE_BL2
#define RCAR_MMAP_ENTRIES		9
#endif
#if IMAGE_BL31
#define RCAR_MMAP_ENTRIES		9
#endif

#if IMAGE_BL2
#define REG1_BASE		U(0xE6400000)
#define REG1_SIZE		U(0x04C00000)	/* 0xE6400000 - 0xEAFFFFFF */
#define ROM0_BASE		U(0xEB100000)
#define ROM0_SIZE		U(0x00028000)	/* 0xEB100000 - 0xEB127FFF */
#define REG2_BASE		U(0xEC000000)
#define REG2_SIZE		U(0x14000000)	/* 0xEC000000 - 0xFFFFFFFF */
#endif /* IMAGE_BL2 */

/* Load address of BL33 in the RCAR port */
#define NS_IMAGE_OFFSET		(DRAM1_BASE + U(0x09000000)) /* DRAM */


/*******************************************************************************
 * CCI-400 related constants
 ******************************************************************************/
#define CCI500_BASE			0xF1200000
#define CCI500_CLUSTER0_SL_IFACE_IX	2
#define CCI500_CLUSTER1_SL_IFACE_IX	3
#define CCI500_CLUSTER0_SL_IFACE_IX_FOR_M3	1
#define CCI500_CLUSTER1_SL_IFACE_IX_FOR_M3	2

/* CCI-500	*/
#define	RCAR_CCI_BASE			U(0xF1200000)					/* The base address of CCI-500						*/

/*******************************************************************************
 * GIC-400 & interrupt handling related constants
 ******************************************************************************/
#define RCAR_GICD_BASE			U(0xF1010000)
#define RCAR_GICR_BASE			U(0xF1010000)	/* not use */
#define RCAR_GICC_BASE			U(0xF1020000)
#define RCAR_GICH_BASE			U(0xF1040000)	/* not use */
#define RCAR_GICV_BASE			U(0xF1060000)	/* not use */

#define ARM_IRQ_SEC_PHY_TIMER		U(29)

#define ARM_IRQ_SEC_SGI_0		U(8)
#define ARM_IRQ_SEC_SGI_1		U(9)
#define ARM_IRQ_SEC_SGI_2		U(10)
#define ARM_IRQ_SEC_SGI_3		U(11)
#define ARM_IRQ_SEC_SGI_4		U(12)
#define ARM_IRQ_SEC_SGI_5		U(13)
#define ARM_IRQ_SEC_SGI_6		U(14)
#define ARM_IRQ_SEC_SGI_7		U(15)

/****************************************************************************************************************************************************************/
/*																				*/
/*	Interrupt ID for Secure world																*/
/*																				*/
/****************************************************************************************************************************************************************/
#define	ARM_IRQ_SEC_RPC			U( 70)						/* RPC									*/
#define	ARM_IRQ_SEC_TIMER		U(166)						/* Secure timer								*/
#define	ARM_IRQ_SEC_TIMER_UP		U(171)						/* Secure timer UP							*/
#define	ARM_IRQ_SEC_WDT			U(173)						/* System watch dog timer						*/
#define	ARM_IRQ_SEC_CRYPT		U(102)						/* Crypt Engine sec							*/
#define	ARM_IRQ_SEC_CRYPT_SecPKA	U( 97)						/* Crypt Engine PKA sec							*/
#define	ARM_IRQ_SEC_CRYPT_PubPKA	U( 98)						/* Crypt Engine PKA pub							*/

/****************************************************************************************************************************************************************/
/*																				*/
/*	New definition for R-Car Series, 3rd Generation														*/
/*																				*/
/****************************************************************************************************************************************************************/
#define	RCAR_DEVICE_BASE		U(0xE6000000)					/* Start address of BL31 register area					*/
#define	RCAR_DEVICE_SIZE		U(0x1A000000)					/* Size of BL31 register area						*/
#define	RCAR_LOG_RES_SIZE		(U(512)/U(8))					/* Size of reservation in log area					*/
#define	RCAR_LOG_HEADER_SIZE		U(16)						/* Size of LOG header							*/
#define	RCAR_LOG_OTHER_SIZE		(RCAR_LOG_HEADER_SIZE+RCAR_LOG_RES_SIZE)	/* Size of write pointer, log header, reservation			*/
#define	RCAR_BL31_LOG_MAX		(RCAR_BL31_LOG_SIZE - RCAR_LOG_OTHER_SIZE)	/* MAX log size								*/

#define	RCAR_CRASH_STACK		U(0x4403F800)					/* Temporary stack for crash						*/

/****************************************************************************************************************************************************************/
/*																				*/
/*	Definition of R-Car Series, 3rd Generation register address												*/
/*																				*/
/****************************************************************************************************************************************************************/
/* Timer	*/
#define	RCAR_CNTC_BASE			U(0xE6080000)					/* The base addess of generic timer control register			*/
/* Reset	*/
#define	RCAR_CPGWPR			U(0xE6150900)					/* CPG write protect register						*/
#define	RCAR_MODEMR			U(0xE6160060)					/* Mode pin register							*/
#define	RCAR_CA57RESCNT			U(0xE6160040)					/* Reset control register for A57					*/
#define	RCAR_CA53RESCNT			U(0xE6160044)					/* Reset control register for A53					*/
#define	RCAR_SRESCR			U(0xE6160110)					/* Soft Power On Reset Control Register					*/
#define	RCAR_CA53WUPCR			U(0xE6151010)					/* Wake-up control register for A53					*/
#define	RCAR_CA57WUPCR			U(0xE6152010)					/* Wake-up control register for A57					*/
#define	RCAR_CA53PSTR			U(0xE6151040)					/* Power status register for A53					*/
#define	RCAR_CA57PSTR			U(0xE6152040)					/* Power status register for A57					*/
#define	RCAR_CA53CPU0CR			U(0xE6151100)					/* The base address of CPU power status control register for A53	*/
#define	RCAR_CA57CPU0CR			U(0xE6152100)					/* The base address of CPU power status control register for A57	*/
#define	RCAR_CA53CPUCMCR		U(0xE6151184)					/* Common power control register for A53				*/
#define	RCAR_CA57CPUCMCR		U(0xE6152184)					/* Common power control register for A57				*/
#define	RCAR_WUPMSKCA57			U(0xE6180014)					/* Wake-up mask register for A57					*/
#define	RCAR_WUPMSKCA53			U(0xE6180018)					/* Wake-up mask register for A53					*/
/* SYSC		*/
#define	RCAR_PWRSR3			U(0xE6180140)					/* Power status register CA53-SCU					*/
#define	RCAR_PWRSR5			U(0xE61801C0)					/* Power status register CA57-SCU					*/
#define	RCAR_SYSCIER			U(0xE618000C)					/* Interrupt enable register						*/
#define	RCAR_SYSCIMR			U(0xE6180010)					/* Interrupt mask register						*/
#define	RCAR_SYSCSR			U(0xE6180000)					/* SYSC status register							*/
#define	RCAR_PWRONCR3			U(0xE618014C)					/* Power resume control register CA53-SCU				*/
#define	RCAR_PWRONCR5			U(0xE61801CC)					/* Power resume control register CA57-SCU				*/
#define	RCAR_PWROFFCR3			U(0xE6180144)					/* Power shutoff control register CA53-SCU				*/
#define	RCAR_PWROFFCR5			U(0xE61801C4)					/* Power shutoff control register CA57-SCU				*/
#define	RCAR_PWRER3			U(0xE6180154)					/* Power shutoff/resume error register CA53-SCU				*/
#define	RCAR_PWRER5			U(0xE61801D4)					/* Power shutoff/resume error register CA57-SCU				*/
#define	RCAR_SYSCISR			U(0xE6180004)					/* Interrupt status register						*/
#define	RCAR_SYSCISCR			U(0xE6180008)					/* Interrupt status clear register					*/
/* Product register	*/
#define	RCAR_PRR			U(0xFFF00044)					/* Product register							*/

/*******************************************************************************
 *  RCAR product and cut information
 ******************************************************************************/
#define RCAR_PRODUCT_MASK		U(0x00007F00)
#define RCAR_CUT_MASK			U(0x000000FF)
#define RCAR_PRODUCT_H3			U(0x00004F00)
#define RCAR_PRODUCT_M3			U(0x00005200)
#define RCAR_PRODUCT_M3N		U(0x00005500)
#define RCAR_PRODUCT_E3			U(0x00005700)
#define RCAR_CUT_VER10			U(0x00000000)
#define RCAR_CUT_VER11			U(0x00000001)	/* H3/M3N Ver.1.1 */
#define RCAR_M3_CUT_VER11		U(0x00000010)	/* M3 Ver.1.1/Ver.1.2 */
#define RCAR_CUT_VER20			U(0x00000010)
#define RCAR_CUT_VER30			U(0x00000020)
#define RCAR_MAJOR_MASK			U(0x000000F0)
#define RCAR_MINOR_MASK			U(0x0000000F)
#define RCAR_PRODUCT_SHIFT		U(8)
#define RCAR_MAJOR_SHIFT		U(4)
#define RCAR_MINOR_SHIFT		U(0)
#define RCAR_MAJOR_OFFSET		U(1)
#define RCAR_M3_MINOR_OFFSET		U(2)
#define RCAR_PRODUCT_H3_CUT10		(RCAR_PRODUCT_H3 | U(0x00)) /* H3 Ver.1.0 */
#define RCAR_PRODUCT_H3_CUT11		(RCAR_PRODUCT_H3 | U(0x01)) /* H3 Ver.1.1 */
#define RCAR_PRODUCT_H3_CUT20		(RCAR_PRODUCT_H3 | U(0x10)) /* H3 Ver.2.0 */
#define RCAR_PRODUCT_M3_CUT10		(RCAR_PRODUCT_M3 | U(0x00)) /* M3 Ver.1.0 */
#define RCAR_PRODUCT_M3_CUT11		(RCAR_PRODUCT_M3 | U(0x10)) /* M3 Ver.1.1 / Ver.1.2 */
#define RCAR_CPU_MASK_CA57		U(0x80000000)
#define RCAR_CPU_MASK_CA53		U(0x04000000)
#define RCAR_CPU_HAVE_CA57		U(0x00000000)
#define RCAR_CPU_HAVE_CA53		U(0x00000000)
#define RCAR_SSCG_MASK			U(0x1000)  /* MD12 */
#define RCAR_SSCG_ENABLE		U(0x1000)

/*******************************************************************************
 *  RCAR MD pin information
 ******************************************************************************/
#define MODEMR_BOOT_CPU_MASK		U(0x000000C0)
#define MODEMR_BOOT_CPU_CR7		U(0x000000C0)
#define MODEMR_BOOT_CPU_CA57		U(0x00000000)
#define MODEMR_BOOT_CPU_CA53		U(0x00000040)
#define MODEMR_BOOT_DEV_MASK		U(0x0000001E)
#define MODEMR_BOOT_DEV_HYPERFLASH160	U(0x00000004)
#define MODEMR_BOOT_DEV_HYPERFLASH80	U(0x00000006)
#define MODEMR_BOOT_DEV_QSPI_FLASH40	U(0x00000008)
#define MODEMR_BOOT_DEV_QSPI_FLASH80	U(0x0000000C)
#define MODEMR_BOOT_DEV_EMMC_25X1	U(0x0000000A)
#define MODEMR_BOOT_DEV_EMMC_50X8	U(0x0000001A)
#define MODEMR_BOOT_PLL_MASK		U(0x00006000)
#define MODEMR_BOOT_PLL_SHIFT		U(13)

/****************************************************************************************************************************************************************/
/* Memory mapped Generic timer interfaces  */
/****************************************************************************************************************************************************************/
#define ARM_SYS_CNTCTL_BASE		RCAR_CNTC_BASE

/*******************************************************************************
 *  Shared Data
 ******************************************************************************/

#define	RCAR_BL31_CRASH_BASE	U(0x4403F000)
#define	RCAR_BL31_CRASH_SIZE	U(0x00001000)

/* Entrypoint mailboxes */
#define MBOX_BASE		RCAR_SHARED_MEM_BASE
#define MBOX_SIZE		0x200

/* Base address where parameters to BL31 are stored */
#define PARAMS_BASE		(MBOX_BASE + MBOX_SIZE)

/*******************************************************************************
 *  MODEMR PLL masks and bitfield values
 ******************************************************************************/
#define	CHECK_MD13_MD14		U(0x6000)
#define	MD14_MD13_TYPE_0	U(0x0000)	/* MD14=0 MD13=0 */
#define	MD14_MD13_TYPE_1	U(0x2000)	/* MD14=0 MD13=1 */
#define	MD14_MD13_TYPE_2	U(0x4000)	/* MD14=1 MD13=0 */
#define	MD14_MD13_TYPE_3	U(0x6000)	/* MD14=1 MD13=1 */

/*******************************************************************************
*  Frequency of EXTAL(Hz)
 ******************************************************************************/
#define	EXTAL_MD14_MD13_TYPE_0	U(8333300)	/* When MD14=0 MD13=0 */
#define	EXTAL_MD14_MD13_TYPE_1	U(10000000)	/* When MD14=0 MD13=1 */
#define	EXTAL_MD14_MD13_TYPE_2	U(12500000)	/* When MD14=1 MD13=0 */
#define	EXTAL_MD14_MD13_TYPE_3	U(16666600)	/* When MD14=1 MD13=1 */
#define	EXTAL_SALVATOR_XS	U(8320000)	/* When board is Salvator-XS */
#define EXTAL_EBISU		U(24000000)	/* When board is Ebisu */

#endif /* RCAR_DEF_H__ */
