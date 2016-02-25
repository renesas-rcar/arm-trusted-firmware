/*
 * Copyright (c) 2014, ARM Limited and Contributors. All rights reserved.
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

#ifndef RCAR_DEF_H__
#define RCAR_DEF_H__

#include <tbbr_img_def.h>

/* Firmware Image Package */
#define RCAR_PRIMARY_CPU			0x0

/*
 * Some of the definitions in this file use the 'ull' suffix in order to avoid
 * subtle integer overflow errors due to implicit integer type promotion when
 * working with 32-bit values.
 *
 * The TSP linker script includes some of these definitions to define the BL3-2
 * memory map, but the GNU LD does not support the 'ull' suffix, causing the
 * build process to fail. To solve this problem, the auxiliary macro MAKE_ULL(x)
 * will add the 'ull' suffix only when the macro __LINKER__  is not defined
 * (__LINKER__ is defined in the command line to preprocess the linker script).
 * Constants in the linker script will not have the 'ull' suffix, but this is
 * not a problem since the linker evaluates all constant expressions to 64 bit
 * (assuming the target architecture is 64 bit).
 */
#ifndef __LINKER__
  #define MAKE_ULL(x)			x##ull
#else
  #define MAKE_ULL(x)			x
#endif

/*******************************************************************************
 * RCAR memory map related constants
 ******************************************************************************/

/* The first 4KB of Trusted SRAM are used as shared memory */
#define RCAR_SHARED_MEM_BASE	(RCAR_TRUSTED_SRAM_BASE + RCAR_TRUSTED_SRAM_SIZE)
#define RCAR_SHARED_MEM_SIZE	0x00001000U	/* 4 KB */

/* The remaining Trusted SRAM is used to load the BL images */
#define RCAR_TRUSTED_SRAM_BASE	0x44000000	/* Trusted Firmware Image */
#define RCAR_TRUSTED_SRAM_SIZE	0x0003E000	/* 248 KB */

#define FLASH0_BASE		0x08000000U		/* LBSC RPC address */
#define FLASH0_SIZE		0x04000000U		/* 64MB */

/* Aggregate of all devices in the first GB */
#define DEVICE_RCAR_BASE	0xE6000000U
#define DEVICE_RCAR_SIZE	0x1A000000U

#define DRAM1_BASE		MAKE_ULL(0x40000000)
#define DRAM1_SIZE		MAKE_ULL(0x80000000)

#define DRAM1_NS_BASE		(DRAM1_BASE + 0x8000000U)
#define DRAM1_NS_SIZE		(DRAM1_SIZE - DRAM1_NS_BASE)
#define DRAM1_NS_END		(DRAM1_NS_BASE + DRAM1_NS_SIZE - 1)


#define SYS_CNTCTL_BASE		0xE6080000U



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
#define RCAR_MMAP_ENTRIES		7
#endif
#if IMAGE_BL31
#define RCAR_MMAP_ENTRIES		5
#endif

/* Load address of BL33 in the RCAR port */
#define NS_IMAGE_OFFSET		(DRAM1_BASE + 0x09000000U) /* DRAM */

/* Special value used to verify platform parameters from BL2 to BL3-1 */
#define RCAR_BL31_PLAT_PARAM_VAL	0x0f1e2d3c4b5a6978ULL


/*******************************************************************************
 * CCI-400 related constants
 ******************************************************************************/
#define CCI500_BASE			0xF1200000
#define CCI500_CLUSTER0_SL_IFACE_IX	2
#define CCI500_CLUSTER1_SL_IFACE_IX	3
#define CCI500_CLUSTER0_SL_IFACE_IX_FOR_M3	1
#define CCI500_CLUSTER1_SL_IFACE_IX_FOR_M3	2

/* CCI-500	*/
#define	RCAR_CCI_BASE			(0xF1200000U)					/* The base address of CCI-500						*/

/*******************************************************************************
 * GIC-400 & interrupt handling related constants
 ******************************************************************************/
#define RCAR_GICD_BASE			(0xF1010000U)
#define RCAR_GICD_BASE_A		(0xF1010000)
#define RCAR_GICR_BASE			(0xF1010000U)	/* not use */
#define RCAR_GICC_BASE			(0xF1020000U)
#define RCAR_GICC_BASE_A		(0xF1020000)
#define RCAR_GICH_BASE			(0xF1040000U)	/* not use */
#define RCAR_GICV_BASE			(0xF1060000U)	/* not use */

#define ARM_IRQ_SEC_PHY_TIMER		29U

#define ARM_IRQ_SEC_SGI_0		8U
#define ARM_IRQ_SEC_SGI_1		9U
#define ARM_IRQ_SEC_SGI_2		10U
#define ARM_IRQ_SEC_SGI_3		11U
#define ARM_IRQ_SEC_SGI_4		12U
#define ARM_IRQ_SEC_SGI_5		13U
#define ARM_IRQ_SEC_SGI_6		14U
#define ARM_IRQ_SEC_SGI_7		15U

/****************************************************************************************************************************************************************/
/*																				*/
/*	Interrupt ID for Secure world																*/
/*																				*/
/****************************************************************************************************************************************************************/
#define	ARM_IRQ_SEC_RPC			( 70U)						/* RPC									*/
#define	ARM_IRQ_SEC_TIMER		(166U)						/* Secure timer								*/
#define	ARM_IRQ_SEC_TIMER_UP		(171U)						/* Secure timer UP							*/
#define	ARM_IRQ_SEC_WDT			(173U)						/* System watch dog timer						*/
#define	ARM_IRQ_SEC_WDT_A		(173)						/* System watch dog timer for Assembly language				*/
#define	ARM_IRQ_SEC_CRYPT		(102U)						/* Crypt Engine sec							*/
#define	ARM_IRQ_SEC_CRYPT_SecPKA	( 97U)						/* Crypt Engine PKA sec							*/
#define	ARM_IRQ_SEC_CRYPT_PubPKA	( 98U)						/* Crypt Engine PKA pub							*/

/****************************************************************************************************************************************************************/
/*																				*/
/*	New definition for R-CarGen3																*/
/*																				*/
/****************************************************************************************************************************************************************/
#define	RCAR_DEVICE_BASE		(0xE6000000)					/* Start address of BL31 register area					*/
#define	RCAR_DEVICE_SIZE		(0x1A000000)					/* Size of BL31 register area						*/
#define	RCAR_LOG_RES_SIZE		(512/8)						/* Size of reservation in log area					*/
#define	RCAR_LOG_HEADER_SIZE		(16)						/* Size of LOG header							*/
#define	RCAR_LOG_OTHER_SIZE		(RCAR_LOG_HEADER_SIZE+RCAR_LOG_RES_SIZE)	/* Size of write pointer, log header, reservation			*/
#define	RCAR_BL31_LOG_MAX		(RCAR_BL31_LOG_SIZE - RCAR_LOG_OTHER_SIZE)	/* MAX log size								*/

#define	RCAR_CRASH_STACK		(0x4403F800)					/* Temporary stack for crash						*/

/****************************************************************************************************************************************************************/
/*																				*/
/*	Definition of R-CarGen3 register address														*/
/*																				*/
/****************************************************************************************************************************************************************/
/* Timer	*/
#define	RCAR_CNTC_BASE			(0xE6080000U)					/* The base addess of generic timer control register			*/
/* Reset	*/
#define	RCAR_CPGWPR			(0xE6150900U)					/* CPG write protect register						*/
#define	RCAR_MODEMR			(0xE6160060U)					/* Mode pin register							*/
#define	RCAR_MODEMR_A			(0xE6160060)					/* Mode pin register for Assembly language				*/
#define	RCAR_CA57RESCNT			(0xE6160040U)					/* Reset control register for A57					*/
#define	RCAR_CA53RESCNT			(0xE6160044U)					/* Reset control register for A53					*/
#define	RCAR_SRESCR			(0xE6160110U)					/* Soft Power On Reset Control Register					*/
#define	RCAR_CA53WUPCR			(0xE6151010U)					/* Wake-up control register for A53					*/
#define	RCAR_CA57WUPCR			(0xE6152010U)					/* Wake-up control register for A57					*/
#define	RCAR_CA53PSTR			(0xE6151040U)					/* Power status register for A53					*/
#define	RCAR_CA57PSTR			(0xE6152040U)					/* Power status register for A57					*/
#define	RCAR_CA53CPU0CR			(0xE6151100U)					/* The base address of CPU power status control register for A53	*/
#define	RCAR_CA57CPU0CR			(0xE6152100U)					/* The base address of CPU power status control register for A57	*/
#define	RCAR_CA53CPUCMCR		(0xE6151184U)					/* Common power control register for A53				*/
#define	RCAR_CA57CPUCMCR		(0xE6152184U)					/* Common power control register for A57				*/
#define	RCAR_WUPMSKCA57			(0xE6180014U)					/* Wake-up mask register for A57					*/
#define	RCAR_WUPMSKCA53			(0xE6180018U)					/* Wake-up mask register for A53					*/
/* SYSC		*/
#define	RCAR_PWRSR3			(0xE6180140U)					/* Power status register CA53-SCU					*/
#define	RCAR_PWRSR5			(0xE61801C0U)					/* Power status register CA57-SCU					*/
#define	RCAR_SYSCIER			(0xE618000CU)					/* Interrupt enable register						*/
#define	RCAR_SYSCIMR			(0xE6180010U)					/* Interrupt mask register						*/
#define	RCAR_SYSCSR			(0xE6180000U)					/* SYSC status register							*/
#define	RCAR_PWRONCR3			(0xE618014CU)					/* Power resume control register CA53-SCU				*/
#define	RCAR_PWRONCR5			(0xE61801CCU)					/* Power resume control register CA57-SCU				*/
#define	RCAR_PWROFFCR3			(0xE6180144U)					/* Power shutoff control register CA53-SCU				*/
#define	RCAR_PWROFFCR5			(0xE61801C4U)					/* Power shutoff control register CA57-SCU				*/
#define	RCAR_PWRER3			(0xE6180154U)					/* Power shutoff/resume error register CA53-SCU				*/
#define	RCAR_PWRER5			(0xE61801D4U)					/* Power shutoff/resume error register CA57-SCU				*/
#define	RCAR_SYSCISR			(0xE6180004U)					/* Interrupt status register						*/
#define	RCAR_SYSCISCR			(0xE6180008U)					/* Interrupt status clear register					*/
/* Product register	*/
#define	RCAR_PRR			(0xFFF00044U)					/* Product register							*/

/*******************************************************************************
 *  RCAR product and cut information
 ******************************************************************************/
#define RCAR_PRODUCT_MASK		(0x00007F00U)
#define RCAR_CUT_MASK			(0x000000FFU)
#define RCAR_PRODUCT_H3			(0x00004F00U)
#define RCAR_PRODUCT_M3			(0x00005200U)
#define RCAR_CUT_ES10			(0x00000000U)
#define RCAR_CUT_ES11			(0x00000001U)
#define RCAR_MAJOR_MASK			(0x000000F0U)
#define RCAR_MINOR_MASK			(0x0000000FU)
#define RCAR_PRODUCT_SHIFT		(8U)
#define RCAR_MAJOR_SHIFT		(4U)
#define RCAR_MINOR_SHIFT		(0U)
#define RCAR_MAJOR_OFFSET		(1U)
#define RCAR_PRODUCT_H3_CUT10		(RCAR_PRODUCT_H3 | 0x00U) /* H3 WS1.0 */
#define RCAR_PRODUCT_H3_CUT11		(RCAR_PRODUCT_H3 | 0x01U) /* H3 WS1.1 */
#define RCAR_PRODUCT_M3_CUT10		(RCAR_PRODUCT_M3 | 0x00U) /* M3 */
/****************************************************************************************************************************************************************/
/* Memory mapped Generic timer interfaces  */
/****************************************************************************************************************************************************************/
#define ARM_SYS_CNTCTL_BASE		RCAR_CNTC_BASE

/*******************************************************************************
 *  Boot CPU
 ******************************************************************************/
#define	RCAR_BOOT_CA5X			(0U)	/* Master boot CPU is CA57/53 */
#define	RCAR_BOOT_CR7			(1U)	/* Master boot CPU is CR7 */

/*******************************************************************************
 *  Shared Data
 ******************************************************************************/

#define	RCAR_BL31_MAIL_BASE	(0x4403E000)
#define	RCAR_BL31_MAIL_SIZE	(0x00002000)
#define	RCAR_BL31_WORK_BASE	(0x44022000)
#define	RCAR_BL31_WORK_SIZE	(0x0001C000)

/* Entrypoint mailboxes */
#define MBOX_BASE		RCAR_SHARED_MEM_BASE
#define MBOX_SIZE		0x200

/* Base address where parameters to BL31 are stored */
#define PARAMS_BASE		(MBOX_BASE + MBOX_SIZE)

#endif /* RCAR_DEF_H__ */
