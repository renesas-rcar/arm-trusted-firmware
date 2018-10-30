/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Renesas nor the names of its contributors may be
 *     used to endorse or promote products derived from this software without
 *     specific prior written permission.
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

#include <stdint.h>		/* for uint32_t */
#include <mmio.h>
#include "pfc_init_v3h.h"


/* GPIO base address */
#define	GPIO_BASE		(0xE6050000U)

/* GPIO registers */
#define	GPIO_IOINTSEL0		(GPIO_BASE + 0x0000U)
#define	GPIO_INOUTSEL0		(GPIO_BASE + 0x0004U)
#define	GPIO_OUTDT0		(GPIO_BASE + 0x0008U)
#define	GPIO_INDT0		(GPIO_BASE + 0x000CU)
#define	GPIO_INTDT0		(GPIO_BASE + 0x0010U)
#define	GPIO_INTCLR0		(GPIO_BASE + 0x0014U)
#define	GPIO_INTMSK0		(GPIO_BASE + 0x0018U)
#define	GPIO_MSKCLR0		(GPIO_BASE + 0x001CU)
#define	GPIO_POSNEG0		(GPIO_BASE + 0x0020U)
#define	GPIO_EDGLEVEL0		(GPIO_BASE + 0x0024U)
#define	GPIO_FILONOFF0		(GPIO_BASE + 0x0028U)
#define	GPIO_INTMSKS0		(GPIO_BASE + 0x0038U)
#define	GPIO_MSKCLRS0		(GPIO_BASE + 0x003CU)
#define	GPIO_OUTDTSEL0		(GPIO_BASE + 0x0040U)
#define	GPIO_OUTDTH0		(GPIO_BASE + 0x0044U)
#define	GPIO_OUTDTL0		(GPIO_BASE + 0x0048U)
#define	GPIO_BOTHEDGE0		(GPIO_BASE + 0x004CU)
#define	GPIO_IOINTSEL1		(GPIO_BASE + 0x1000U)
#define	GPIO_INOUTSEL1		(GPIO_BASE + 0x1004U)
#define	GPIO_OUTDT1		(GPIO_BASE + 0x1008U)
#define	GPIO_INDT1		(GPIO_BASE + 0x100CU)
#define	GPIO_INTDT1		(GPIO_BASE + 0x1010U)
#define	GPIO_INTCLR1		(GPIO_BASE + 0x1014U)
#define	GPIO_INTMSK1		(GPIO_BASE + 0x1018U)
#define	GPIO_MSKCLR1		(GPIO_BASE + 0x101CU)
#define	GPIO_POSNEG1		(GPIO_BASE + 0x1020U)
#define	GPIO_EDGLEVEL1		(GPIO_BASE + 0x1024U)
#define	GPIO_FILONOFF1		(GPIO_BASE + 0x1028U)
#define	GPIO_INTMSKS1		(GPIO_BASE + 0x1038U)
#define	GPIO_MSKCLRS1		(GPIO_BASE + 0x103CU)
#define	GPIO_OUTDTSEL1		(GPIO_BASE + 0x1040U)
#define	GPIO_OUTDTH1		(GPIO_BASE + 0x1044U)
#define	GPIO_OUTDTL1		(GPIO_BASE + 0x1048U)
#define	GPIO_BOTHEDGE1		(GPIO_BASE + 0x104CU)
#define	GPIO_IOINTSEL2		(GPIO_BASE + 0x2000U)
#define	GPIO_INOUTSEL2		(GPIO_BASE + 0x2004U)
#define	GPIO_OUTDT2		(GPIO_BASE + 0x2008U)
#define	GPIO_INDT2		(GPIO_BASE + 0x200CU)
#define	GPIO_INTDT2		(GPIO_BASE + 0x2010U)
#define	GPIO_INTCLR2		(GPIO_BASE + 0x2014U)
#define	GPIO_INTMSK2		(GPIO_BASE + 0x2018U)
#define	GPIO_MSKCLR2		(GPIO_BASE + 0x201CU)
#define	GPIO_POSNEG2		(GPIO_BASE + 0x2020U)
#define	GPIO_EDGLEVEL2		(GPIO_BASE + 0x2024U)
#define	GPIO_FILONOFF2		(GPIO_BASE + 0x2028U)
#define	GPIO_INTMSKS2		(GPIO_BASE + 0x2038U)
#define	GPIO_MSKCLRS2		(GPIO_BASE + 0x203CU)
#define	GPIO_OUTDTSEL2		(GPIO_BASE + 0x2040U)
#define	GPIO_OUTDTH2		(GPIO_BASE + 0x2044U)
#define	GPIO_OUTDTL2		(GPIO_BASE + 0x2048U)
#define	GPIO_BOTHEDGE2		(GPIO_BASE + 0x204CU)
#define	GPIO_IOINTSEL3		(GPIO_BASE + 0x3000U)
#define	GPIO_INOUTSEL3		(GPIO_BASE + 0x3004U)
#define	GPIO_OUTDT3		(GPIO_BASE + 0x3008U)
#define	GPIO_INDT3		(GPIO_BASE + 0x300CU)
#define	GPIO_INTDT3		(GPIO_BASE + 0x3010U)
#define	GPIO_INTCLR3		(GPIO_BASE + 0x3014U)
#define	GPIO_INTMSK3		(GPIO_BASE + 0x3018U)
#define	GPIO_MSKCLR3		(GPIO_BASE + 0x301CU)
#define	GPIO_POSNEG3		(GPIO_BASE + 0x3020U)
#define	GPIO_EDGLEVEL3		(GPIO_BASE + 0x3024U)
#define	GPIO_FILONOFF3		(GPIO_BASE + 0x3028U)
#define	GPIO_INTMSKS3		(GPIO_BASE + 0x3038U)
#define	GPIO_MSKCLRS3		(GPIO_BASE + 0x303CU)
#define	GPIO_OUTDTSEL3		(GPIO_BASE + 0x3040U)
#define	GPIO_OUTDTH3		(GPIO_BASE + 0x3044U)
#define	GPIO_OUTDTL3		(GPIO_BASE + 0x3048U)
#define	GPIO_BOTHEDGE3		(GPIO_BASE + 0x304CU)
#define	GPIO_IOINTSEL4		(GPIO_BASE + 0x4000U)
#define	GPIO_INOUTSEL4		(GPIO_BASE + 0x4004U)
#define	GPIO_OUTDT4		(GPIO_BASE + 0x4008U)
#define	GPIO_INDT4		(GPIO_BASE + 0x400CU)
#define	GPIO_INTDT4		(GPIO_BASE + 0x4010U)
#define	GPIO_INTCLR4		(GPIO_BASE + 0x4014U)
#define	GPIO_INTMSK4		(GPIO_BASE + 0x4018U)
#define	GPIO_MSKCLR4		(GPIO_BASE + 0x401CU)
#define	GPIO_POSNEG4		(GPIO_BASE + 0x4020U)
#define	GPIO_EDGLEVEL4		(GPIO_BASE + 0x4024U)
#define	GPIO_FILONOFF4		(GPIO_BASE + 0x4028U)
#define	GPIO_INTMSKS4		(GPIO_BASE + 0x4038U)
#define	GPIO_MSKCLRS4		(GPIO_BASE + 0x403CU)
#define	GPIO_OUTDTSEL4		(GPIO_BASE + 0x4040U)
#define	GPIO_OUTDTH4		(GPIO_BASE + 0x4044U)
#define	GPIO_OUTDTL4		(GPIO_BASE + 0x4048U)
#define	GPIO_BOTHEDGE4		(GPIO_BASE + 0x404CU)
#define	GPIO_IOINTSEL5		(GPIO_BASE + 0x5000U)
#define	GPIO_INOUTSEL5		(GPIO_BASE + 0x5004U)
#define	GPIO_OUTDT5		(GPIO_BASE + 0x5008U)
#define	GPIO_INDT5		(GPIO_BASE + 0x500CU)
#define	GPIO_INTDT5		(GPIO_BASE + 0x5010U)
#define	GPIO_INTCLR5		(GPIO_BASE + 0x5014U)
#define	GPIO_INTMSK5		(GPIO_BASE + 0x5018U)
#define	GPIO_MSKCLR5		(GPIO_BASE + 0x501CU)
#define	GPIO_POSNEG5		(GPIO_BASE + 0x5020U)
#define	GPIO_EDGLEVEL5		(GPIO_BASE + 0x5024U)
#define	GPIO_FILONOFF5		(GPIO_BASE + 0x5028U)
#define	GPIO_INTMSKS5		(GPIO_BASE + 0x5038U)
#define	GPIO_MSKCLRS5		(GPIO_BASE + 0x503CU)
#define	GPIO_OUTDTSEL5		(GPIO_BASE + 0x5040U)
#define	GPIO_OUTDTH5		(GPIO_BASE + 0x5044U)
#define	GPIO_OUTDTL5		(GPIO_BASE + 0x5048U)
#define	GPIO_BOTHEDGE5		(GPIO_BASE + 0x504CU)

/* Pin functon base address */
#define	PFC_BASE		(0xE6060000U)

/* Pin functon registers */
#define	PFC_PMMR		(PFC_BASE + 0x0000U)
#define PFC_GPSR0		(PFC_BASE + 0x0100U)
#define PFC_GPSR1		(PFC_BASE + 0x0104U)
#define PFC_GPSR2		(PFC_BASE + 0x0108U)
#define PFC_GPSR3		(PFC_BASE + 0x010CU)
#define PFC_GPSR4		(PFC_BASE + 0x0110U)
#define	PFC_GPSR5		(PFC_BASE + 0x0114U)
#define	PFC_IPSR0		(PFC_BASE + 0x0200U)
#define	PFC_IPSR1		(PFC_BASE + 0x0204U)
#define	PFC_IPSR2		(PFC_BASE + 0x0208U)
#define	PFC_IPSR3		(PFC_BASE + 0x020CU)
#define	PFC_IPSR4		(PFC_BASE + 0x0210U)
#define	PFC_IPSR5		(PFC_BASE + 0x0214U)
#define	PFC_IPSR6		(PFC_BASE + 0x0218U)
#define	PFC_IPSR7		(PFC_BASE + 0x021CU)
#define	PFC_IPSR8		(PFC_BASE + 0x0220U)
#define	PFC_IPSR9		(PFC_BASE + 0x0224U)
#define	PFC_IPSR10		(PFC_BASE + 0x0228U)
#define PFC_IOCTRL0		(PFC_BASE + 0x0300U)
#define PFC_IOCTRL1		(PFC_BASE + 0x0304U)
#define PFC_IOCTRL2		(PFC_BASE + 0x0308U)
#define PFC_IOCTRL3		(PFC_BASE + 0x030CU)
#define PFC_IOCTRL4		(PFC_BASE + 0x0310U)
#define PFC_IOCTRL5		(PFC_BASE + 0x0314U)
#define PFC_IOCTRL6		(PFC_BASE + 0x0318U)
#define PFC_IOCTRL7		(PFC_BASE + 0x031CU)
#define PFC_IOCTRL8		(PFC_BASE + 0x0320U)
#define PFC_IOCTRL9		(PFC_BASE + 0x0324U)
#define PFC_IOCTRL10		(PFC_BASE + 0x0328U)
#define PFC_IOCTRL11		(PFC_BASE + 0x032CU)
#define PFC_IOCTRL12		(PFC_BASE + 0x0330U)
#define PFC_IOCTRL13		(PFC_BASE + 0x0334U)
#define PFC_IOCTRL14		(PFC_BASE + 0x0338U)
#define PFC_IOCTRL15		(PFC_BASE + 0x033CU)
#define PFC_IOCTRL16		(PFC_BASE + 0x0340U)
#define PFC_IOCTRL17		(PFC_BASE + 0x0344U)
#define PFC_IOCTRL18		(PFC_BASE + 0x0348U)
#define PFC_IOCTRL19		(PFC_BASE + 0x034CU)
#define PFC_IOCTRL30		(PFC_BASE + 0x0380U)
#define PFC_IOCTRL31		(PFC_BASE + 0x0384U)
#define PFC_IOCTRL32		(PFC_BASE + 0x0388U)
#define PFC_IOCTRL33		(PFC_BASE + 0x038CU)
#define PFC_IOCTRL40		(PFC_BASE + 0x03C0U)
#define	PFC_TSREG		(PFC_BASE + 0x03E4U)
#define	PFC_PUEN0		(PFC_BASE + 0x0400U)
#define	PFC_PUEN1		(PFC_BASE + 0x0404U)
#define	PFC_PUEN2		(PFC_BASE + 0x0408U)
#define	PFC_PUEN3		(PFC_BASE + 0x040CU)
#define	PFC_PUEN4		(PFC_BASE + 0x0410U)
#define	PFC_PUD0		(PFC_BASE + 0x0440U)
#define	PFC_PUD1		(PFC_BASE + 0x0444U)
#define	PFC_PUD2		(PFC_BASE + 0x0448U)
#define	PFC_PUD3		(PFC_BASE + 0x044CU)
#define	PFC_PUD4		(PFC_BASE + 0x0450U)
#define	PFC_MOD_SEL0		(PFC_BASE + 0x0500U)

#define	GPSR0_DU_EXODDF_DU_ODDF_DISP_CDE	((uint32_t)1U << 21U)
#define	GPSR0_DU_EXVSYNC_DU_VSYNC		((uint32_t)1U << 20U)
#define	GPSR0_DU_EXHSYNC_DU_HSYNC		((uint32_t)1U << 19U)
#define	GPSR0_DU_EXHSYNC_DU_HSYNC_A		((uint32_t)0U << 19U)
#define	GPSR0_DU_DOTCLKOUT	((uint32_t)1U << 18U)
#define	GPSR0_DU_DB7		((uint32_t)1U << 17U)
#define	GPSR0_DU_DB6		((uint32_t)1U << 16U)
#define	GPSR0_DU_DB5		((uint32_t)1U << 15U)
#define	GPSR0_DU_DB4		((uint32_t)1U << 14U)
#define	GPSR0_DU_DB3		((uint32_t)1U << 13U)
#define	GPSR0_DU_DB2		((uint32_t)1U << 12U)
#define	GPSR0_DU_DG7		((uint32_t)1U << 11U)
#define	GPSR0_DU_DG6		((uint32_t)1U << 10U)
#define	GPSR0_DU_DG5		((uint32_t)1U << 9U)
#define	GPSR0_DU_DG4		((uint32_t)1U << 8U)
#define	GPSR0_DU_DG3		((uint32_t)1U << 7U)
#define	GPSR0_DU_DG2		((uint32_t)1U << 6U)
#define	GPSR0_DU_DR7		((uint32_t)1U << 5U)
#define	GPSR0_DU_DR6		((uint32_t)1U << 4U)
#define	GPSR0_DU_DR5		((uint32_t)1U << 3U)
#define	GPSR0_DU_DR4		((uint32_t)1U << 2U)
#define	GPSR0_DU_DR3		((uint32_t)1U << 1U)
#define	GPSR0_DU_DR2		((uint32_t)1U << 0U)
#define	GPSR1_DIGRF_CLKOUT	((uint32_t)1U << 27U)
#define	GPSR1_DIGRF_CLKIN	((uint32_t)1U << 26U)
#define	GPSR1_CANFD_CLK_A	((uint32_t)1U << 25U)
#define	GPSR1_CANFD1_RX		((uint32_t)1U << 24U)
#define	GPSR1_CANFD1_TX		((uint32_t)1U << 23U)
#define	GPSR1_CANFD0_RX_A	((uint32_t)1U << 22U)
#define	GPSR1_CANFD0_TX_A	((uint32_t)1U << 21U)
#define	GPSR1_AVB_AVTP_CAPTURE		((uint32_t)1U << 20U)
#define	GPSR1_AVB_AVTP_MATCH		((uint32_t)1U << 19U)
#define	GPSR1_AVB_LINK		((uint32_t)1U << 18U)
#define	GPSR1_AVB_PHY_INT	((uint32_t)1U << 17U)
#define	GPSR1_AVB_MAGIC		((uint32_t)1U << 16U)
#define	GPSR1_AVB_MDC		((uint32_t)1U << 15U)
#define	GPSR1_AVB_MDIO		((uint32_t)1U << 14U)
#define	GPSR1_AVB_TXCREFCLK	((uint32_t)1U << 13U)
#define	GPSR1_AVB_TD3		((uint32_t)1U << 12U)
#define	GPSR1_AVB_TD2		((uint32_t)1U << 11U)
#define	GPSR1_AVB_TD1		((uint32_t)1U << 10U)
#define	GPSR1_AVB_TD0		((uint32_t)1U << 9U)
#define	GPSR1_AVB_TXC		((uint32_t)1U << 8U)
#define	GPSR1_AVB_TX_CTL	((uint32_t)1U << 7U)
#define	GPSR1_AVB_RD3		((uint32_t)1U << 6U)
#define	GPSR1_AVB_RD2		((uint32_t)1U << 5U)
#define	GPSR1_AVB_RD1		((uint32_t)1U << 4U)
#define	GPSR1_AVB_RD0		((uint32_t)1U << 3U)
#define	GPSR1_AVB_RXC		((uint32_t)1U << 2U)
#define	GPSR1_AVB_RX_CTL	((uint32_t)1U << 1U)
#define	GPSR1_IRQ0		((uint32_t)1U << 0U)
#define	GPSR2_FSO_TOE		((uint32_t)1U << 29U)
#define	GPSR2_FSO_CFE_1		((uint32_t)1U << 28U)
#define	GPSR2_FSO_CFE_0		((uint32_t)1U << 27U)
#define	GPSR2_SDA3			((uint32_t)1U << 26U)
#define	GPSR2_SCL3			((uint32_t)1U << 25U)
#define	GPSR2_MSIOF0_SS2	((uint32_t)1U << 24U)
#define	GPSR2_MSIOF0_SS1	((uint32_t)1U << 23U)
#define	GPSR2_MSIOF0_SYNC	((uint32_t)1U << 22U)
#define	GPSR2_MSIOF0_SCK	((uint32_t)1U << 21U)
#define	GPSR2_MSIOF0_TXD	((uint32_t)1U << 20U)
#define	GPSR2_MSIOF0_RXD	((uint32_t)1U << 19U)
#define	GPSR2_IRQ5			((uint32_t)1U << 18U)
#define	GPSR2_IRQ4			((uint32_t)1U << 17U)
#define	GPSR2_VI0_FIELD		((uint32_t)1U << 16U)
#define	GPSR2_VI0_DATA11	((uint32_t)1U << 15U)
#define	GPSR2_VI0_DATA10	((uint32_t)1U << 14U)
#define	GPSR2_VI0_DATA9		((uint32_t)1U << 13U)
#define	GPSR2_VI0_DATA8		((uint32_t)1U << 12U)
#define	GPSR2_VI0_DATA7		((uint32_t)1U << 11U)
#define	GPSR2_VI0_DATA6		((uint32_t)1U << 10U)
#define	GPSR2_VI0_DATA5		((uint32_t)1U << 9U)
#define	GPSR2_VI0_DATA4		((uint32_t)1U << 8U)
#define	GPSR2_VI0_DATA3		((uint32_t)1U << 7U)
#define	GPSR2_VI0_DATA2		((uint32_t)1U << 6U)
#define	GPSR2_VI0_DATA1		((uint32_t)1U << 5U)
#define	GPSR2_VI0_DATA0		((uint32_t)1U << 4U)
#define	GPSR2_VI0_VSYNC		((uint32_t)1U << 3U)
#define	GPSR2_VI0_HSYNC		((uint32_t)1U << 2U)
#define	GPSR2_VI0_CLKENB	((uint32_t)1U << 1U)
#define	GPSR2_VI0_CLK		((uint32_t)1U << 0U)
#define	GPSR3_VI1_FIELD		((uint32_t)1U << 16U)
#define	GPSR3_VI1_DATA11	((uint32_t)1U << 15U)
#define	GPSR3_VI1_DATA10	((uint32_t)1U << 14U)
#define	GPSR3_VI1_DATA9		((uint32_t)1U << 13U)
#define	GPSR3_VI1_DATA8		((uint32_t)1U << 12U)
#define	GPSR3_VI1_DATA7		((uint32_t)1U << 11U)
#define	GPSR3_VI1_DATA6		((uint32_t)1U << 10U)
#define	GPSR3_VI1_DATA5		((uint32_t)1U << 9U)
#define	GPSR3_VI1_DATA4		((uint32_t)1U << 8U)
#define	GPSR3_VI1_DATA3		((uint32_t)1U << 7U)
#define	GPSR3_VI1_DATA2		((uint32_t)1U << 6U)
#define	GPSR3_VI1_DATA1		((uint32_t)1U << 5U)
#define	GPSR3_VI1_DATA0		((uint32_t)1U << 4U)
#define	GPSR3_VI1_VSYNC		((uint32_t)1U << 3U)
#define	GPSR3_VI1_HSYNC		((uint32_t)1U << 2U)
#define	GPSR3_VI1_CLKENB	((uint32_t)1U << 1U)
#define	GPSR3_VI1_CLK		((uint32_t)1U << 0U)
#define	GPSR4_GETHER_LINK_A	((uint32_t)1U << 24U)
#define	GPSR4_GETHER_PHY_INT_A		((uint32_t)1U << 23U)
#define	GPSR4_GETHER_MAGIC	((uint32_t)1U << 22U)
#define	GPSR4_GETHER_MDC_A	((uint32_t)1U << 21U)
#define	GPSR4_GETHER_MDIO_A	((uint32_t)1U << 20U)
#define	GPSR4_GETHER_TXCREFCLK_MEGA		((uint32_t)1U << 19U)
#define	GPSR4_GETHER_TXCREFCLK		((uint32_t)1U << 18U)
#define	GPSR4_GETHER_TD3	((uint32_t)1U << 17U)
#define	GPSR4_GETHER_TD2	((uint32_t)1U << 16U)
#define	GPSR4_GETHER_TD1	((uint32_t)1U << 15U)
#define	GPSR4_GETHER_TD0	((uint32_t)1U << 14U)
#define	GPSR4_GETHER_TXC	((uint32_t)1U << 13U)
#define	GPSR4_GETHER_TX_CTL	((uint32_t)1U << 12U)
#define	GPSR4_GETHER_RD3	((uint32_t)1U << 11U)
#define	GPSR4_GETHER_RD2	((uint32_t)1U << 10U)
#define	GPSR4_GETHER_RD1	((uint32_t)1U << 9U)
#define	GPSR4_GETHER_RD0	((uint32_t)1U << 8U)
#define	GPSR4_GETHER_RXC	((uint32_t)1U << 7U)
#define	GPSR4_GETHER_RX_CTL	((uint32_t)1U << 6U)
#define	GPSR4_SDA2			((uint32_t)1U << 5U)
#define	GPSR4_SCL2			((uint32_t)1U << 4U)
#define	GPSR4_SDA1			((uint32_t)1U << 3U)
#define	GPSR4_SCL1			((uint32_t)1U << 2U)
#define	GPSR4_SDA0			((uint32_t)1U << 1U)
#define	GPSR4_SCL0			((uint32_t)1U << 0U)
#define	GPSR5_RPC_INT		((uint32_t)1U << 14U)
#define	GPSR5_RPC_WP		((uint32_t)1U << 13U)
#define	GPSR5_RPC_RESET		((uint32_t)1U << 12U)
#define	GPSR5_QSPI1_SSL		((uint32_t)1U << 11U)
#define	GPSR5_QSPI1_IO3		((uint32_t)1U << 10U)
#define	GPSR5_QSPI1_IO2		((uint32_t)1U << 9U)
#define	GPSR5_QSPI1_MISO_IO1		((uint32_t)1U << 8U)
#define	GPSR5_QSPI1_MOSI_IO0		((uint32_t)1U << 7U)
#define	GPSR5_QSPI1_SPCLK	((uint32_t)1U << 6U)
#define	GPSR5_QSPI0_SSL		((uint32_t)1U << 5U)
#define	GPSR5_QSPI0_IO3		((uint32_t)1U << 4U)
#define	GPSR5_QSPI0_IO2		((uint32_t)1U << 3U)
#define	GPSR5_QSPI0_MISO_IO1		((uint32_t)1U << 2U)
#define	GPSR5_QSPI0_MOSI_IO0		((uint32_t)1U << 1U)
#define	GPSR5_QSPI0_SPCLK	((uint32_t)1U << 0U)

#define	IPSR_28_FUNC(x)		((uint32_t)(x) << 28U)
#define	IPSR_24_FUNC(x)		((uint32_t)(x) << 24U)
#define	IPSR_20_FUNC(x)		((uint32_t)(x) << 20U)
#define	IPSR_16_FUNC(x)		((uint32_t)(x) << 16U)
#define	IPSR_12_FUNC(x)		((uint32_t)(x) << 12U)
#define	IPSR_8_FUNC(x)		((uint32_t)(x) << 8U)
#define	IPSR_4_FUNC(x)		((uint32_t)(x) << 4U)
#define	IPSR_0_FUNC(x)		((uint32_t)(x) << 0U)

#define IOCTRL0_MASK								(0x00000000U)
#define IOCTRL1_MASK								(0x00000000U)
#define IOCTRL2_MASK								(0x00000000U)
#define IOCTRL3_MASK								(0x00000000U)
#define IOCTRL4_MASK								(0x00000000U)
#define IOCTRL5_MASK								(0x00000000U)
#define IOCTRL6_MASK								(0x00000000U)
#define IOCTRL7_MASK								(0x00000000U)
#define IOCTRL8_MASK								(0x00000000U)
#define IOCTRL9_MASK								(0x00000000U)
#define IOCTRL10_MASK								(0x00000000U)
#define IOCTRL11_MASK								(0x00000000U)
#define IOCTRL12_MASK								(0x00000000U)
#define IOCTRL13_MASK								(0x00000000U)
#define IOCTRL14_MASK								(0x00000000U)
#define IOCTRL15_MASK								(0x00000000U)
#define IOCTRL16_MASK								(0x00000000U)
#define IOCTRL17_MASK								(0x00000000U)
#define IOCTRL18_MASK								(0x00000000U)
#define IOCTRL19_MASK								(0x00000000U)

#define IOCTRL0_DRV3_GETHER_DR2						((uint32_t)1U << 30U)
#define IOCTRL0_DRV2_GETHER_DR2						((uint32_t)1U << 29U)
#define IOCTRL0_DRV1_GETHER_DR2						((uint32_t)1U << 28U)
#define IOCTRL0_DRV3_GETHER_DR3						((uint32_t)1U << 26U)
#define IOCTRL0_DRV2_GETHER_DR3						((uint32_t)1U << 25U)
#define IOCTRL0_DRV1_GETHER_DR3						((uint32_t)1U << 24U)
#define IOCTRL0_DRV3_GETHER_DR4						((uint32_t)1U << 22U)
#define IOCTRL0_DRV2_GETHER_DR4						((uint32_t)1U << 21U)
#define IOCTRL0_DRV1_GETHER_DR4						((uint32_t)1U << 20U)
#define IOCTRL0_DRV3_GETHER_DR5						((uint32_t)1U << 18U)
#define IOCTRL0_DRV2_GETHER_DR5						((uint32_t)1U << 17U)
#define IOCTRL0_DRV1_GETHER_DR5						((uint32_t)1U << 16U)
#define IOCTRL0_DRV3_GETHER_DR6						((uint32_t)1U << 14U)
#define IOCTRL0_DRV2_GETHER_DR6						((uint32_t)1U << 13U)
#define IOCTRL0_DRV1_GETHER_DR6						((uint32_t)1U << 12U)
#define IOCTRL0_DRV3_GETHER_DR7						((uint32_t)1U << 10U)
#define IOCTRL0_DRV2_GETHER_DR7						((uint32_t)1U << 9U)
#define IOCTRL0_DRV1_GETHER_DR7						((uint32_t)1U << 8U)
#define IOCTRL0_DRV3_GETHER_DG2						((uint32_t)1U << 6U)
#define IOCTRL0_DRV2_GETHER_DG2						((uint32_t)1U << 5U)
#define IOCTRL0_DRV1_GETHER_DG2						((uint32_t)1U << 4U)
#define IOCTRL0_DRV3_GETHER_DG3						((uint32_t)1U << 2U)
#define IOCTRL0_DRV2_GETHER_DG3						((uint32_t)1U << 1U)
#define IOCTRL0_DRV1_GETHER_DG3						((uint32_t)1U << 0U)
#define IOCTRL1_DRV3_GETHER_DG4						((uint32_t)1U << 30U)
#define IOCTRL1_DRV2_GETHER_DG4						((uint32_t)1U << 29U)
#define IOCTRL1_DRV1_GETHER_DG4						((uint32_t)1U << 28U)
#define IOCTRL1_DRV3_GETHER_DG5						((uint32_t)1U << 26U)
#define IOCTRL1_DRV2_GETHER_DG5						((uint32_t)1U << 25U)
#define IOCTRL1_DRV1_GETHER_DG5						((uint32_t)1U << 24U)
#define IOCTRL1_DRV3_GETHER_DG6						((uint32_t)1U << 22U)
#define IOCTRL1_DRV2_GETHER_DG6						((uint32_t)1U << 21U)
#define IOCTRL1_DRV1_GETHER_DG6						((uint32_t)1U << 20U)
#define IOCTRL1_DRV3_GETHER_DG7						((uint32_t)1U << 18U)
#define IOCTRL1_DRV2_GETHER_DG7						((uint32_t)1U << 17U)
#define IOCTRL1_DRV1_GETHER_DG7						((uint32_t)1U << 16U)
#define IOCTRL1_DRV3_GETHER_DB2						((uint32_t)1U << 14U)
#define IOCTRL1_DRV2_GETHER_DB2						((uint32_t)1U << 13U)
#define IOCTRL1_DRV1_GETHER_DB2						((uint32_t)1U << 12U)
#define IOCTRL1_DRV3_GETHER_DB3						((uint32_t)1U << 10U)
#define IOCTRL1_DRV2_GETHER_DB3						((uint32_t)1U << 9U)
#define IOCTRL1_DRV1_GETHER_DB3						((uint32_t)1U << 8U)
#define IOCTRL1_DRV3_GETHER_DB4						((uint32_t)1U << 6U)
#define IOCTRL1_DRV2_GETHER_DB4						((uint32_t)1U << 5U)
#define IOCTRL1_DRV1_GETHER_DB4						((uint32_t)1U << 4U)
#define IOCTRL1_DRV3_GETHER_DB5						((uint32_t)1U << 2U)
#define IOCTRL1_DRV2_GETHER_DB5						((uint32_t)1U << 1U)
#define IOCTRL1_DRV1_GETHER_DB5						((uint32_t)1U << 0U)
#define IOCTRL2_DRV3_GETHER_DB6						((uint32_t)1U << 30U)
#define IOCTRL2_DRV2_GETHER_DB6						((uint32_t)1U << 29U)
#define IOCTRL2_DRV1_GETHER_DB6						((uint32_t)1U << 28U)
#define IOCTRL2_DRV3_GETHER_DB7						((uint32_t)1U << 26U)
#define IOCTRL2_DRV2_GETHER_DB7						((uint32_t)1U << 25U)
#define IOCTRL2_DRV1_GETHER_DB7						((uint32_t)1U << 24U)
#define IOCTRL2_DRV3_DU_DOTCLKOUT					((uint32_t)1U << 22U)
#define IOCTRL2_DRV2_DU_DOTCLKOUT					((uint32_t)1U << 21U)
#define IOCTRL2_DRV1_DU_DOTCLKOUT					((uint32_t)1U << 20U)
#define IOCTRL2_DRV3_DU_EXHSYNC_DU_HSYNC				((uint32_t)1U << 18U)
#define IOCTRL2_DRV2_DU_EXHSYNC_DU_HSYNC				((uint32_t)1U << 17U)
#define IOCTRL2_DRV1_DU_EXHSYNC_DU_HSYNC				((uint32_t)1U << 16U)
#define IOCTRL2_DRV3_DU_EXHSYNC_DU_VSYNC				((uint32_t)1U << 14U)
#define IOCTRL2_DRV2_DU_EXHSYNC_DU_VSYNC				((uint32_t)1U << 13U)
#define IOCTRL2_DRV1_DU_EXHSYNC_DU_VSYNC				((uint32_t)1U << 12U)
#define IOCTRL2_DRV3_DU_EXODDF_DU_ODDF_DISP_CDE				((uint32_t)1U << 10U)
#define IOCTRL2_DRV2_DU_EXODDF_DU_ODDF_DISP_CDE				((uint32_t)1U << 9U)
#define IOCTRL2_DRV1_DU_EXODDF_DU_ODDF_DISP_CDE				((uint32_t)1U << 8U)
#define IOCTRL3_DRV2_DU_DOTCLKIN					((uint32_t)1U << 29U)
#define IOCTRL3_DRV1_DU_DOTCLKIN					((uint32_t)1U << 28U)
#define IOCTRL3_DRV3_PRESETOUT						((uint32_t)1U << 22U)
#define IOCTRL3_DRV2_PRESETOUT						((uint32_t)1U << 21U)
#define IOCTRL3_DRV1_PRESETOUT						((uint32_t)1U << 20U)
#define IOCTRL3_DRV2_FSCLKST						((uint32_t)1U << 1U)
#define IOCTRL3_DRV1_FSCLKST						((uint32_t)1U << 0U)
#define IOCTRL4_DRV2_FSCLKST2						((uint32_t)1U << 29U)
#define IOCTRL4_DRV1_FSCLKST2						((uint32_t)1U << 28U)
#define IOCTRL4_DRV3_IRQ0						((uint32_t)1U << 22U)
#define IOCTRL4_DRV2_IRQ0						((uint32_t)1U << 21U)
#define IOCTRL4_DRV1_IRQ0						((uint32_t)1U << 20U)
#define IOCTRL4_DRV2_DCUTMS						((uint32_t)1U << 9U)
#define IOCTRL4_DRV1_DCUTMS						((uint32_t)1U << 8U)
#define IOCTRL4_DRV2_DCUTDO_LPDO					((uint32_t)1U << 1U)
#define IOCTRL4_DRV1_DCUTDO_LPDO					((uint32_t)1U << 0U)
#define IOCTRL5_DRV2_DCURDY_LPDCLKOUT					((uint32_t)1U << 29U)
#define IOCTRL5_DRV1_DCURDY_LPDCLKOUT					((uint32_t)1U << 28U)
#define IOCTRL5_DRV3_VI0_CLK						((uint32_t)1U << 26U)
#define IOCTRL5_DRV2_VI0_CLK						((uint32_t)1U << 25U)
#define IOCTRL5_DRV1_VI0_CLK						((uint32_t)1U << 24U)
#define IOCTRL5_DRV3_VI0_CLKENB						((uint32_t)1U << 22U)
#define IOCTRL5_DRV2_VI0_CLKENB						((uint32_t)1U << 21U)
#define IOCTRL5_DRV1_VI0_CLKENB						((uint32_t)1U << 20U)
#define IOCTRL5_DRV3_VI0_HSYNC						((uint32_t)1U << 18U)
#define IOCTRL5_DRV2_VI0_HSYNC						((uint32_t)1U << 17U)
#define IOCTRL5_DRV1_VI0_HSYNC						((uint32_t)1U << 16U)
#define IOCTRL5_DRV3_VI0_VSYNC						((uint32_t)1U << 14U)
#define IOCTRL5_DRV2_VI0_VSYNC						((uint32_t)1U << 13U)
#define IOCTRL5_DRV1_VI0_VSYNC						((uint32_t)1U << 12U)
#define IOCTRL5_DRV3_VI0_DATA0						((uint32_t)1U << 10U)
#define IOCTRL5_DRV2_VI0_DATA0						((uint32_t)1U << 9U)
#define IOCTRL5_DRV1_VI0_DATA0						((uint32_t)1U << 8U)
#define IOCTRL5_DRV3_VI0_DATA1						((uint32_t)1U << 6U)
#define IOCTRL5_DRV2_VI0_DATA1						((uint32_t)1U << 5U)
#define IOCTRL5_DRV1_VI0_DATA1						((uint32_t)1U << 4U)
#define IOCTRL5_DRV3_VI0_DATA2						((uint32_t)1U << 2U)
#define IOCTRL5_DRV2_VI0_DATA2						((uint32_t)1U << 1U)
#define IOCTRL5_DRV1_VI0_DATA2						((uint32_t)1U << 0U)
#define IOCTRL6_DRV3_VI0_DATA3						((uint32_t)1U << 30U)
#define IOCTRL6_DRV2_VI0_DATA3						((uint32_t)1U << 29U)
#define IOCTRL6_DRV1_VI0_DATA3						((uint32_t)1U << 28U)
#define IOCTRL6_DRV3_VI0_DATA4						((uint32_t)1U << 26U)
#define IOCTRL6_DRV2_VI0_DATA4						((uint32_t)1U << 25U)
#define IOCTRL6_DRV1_VI0_DATA4						((uint32_t)1U << 24U)
#define IOCTRL6_DRV3_VI0_DATA5						((uint32_t)1U << 22U)
#define IOCTRL6_DRV2_VI0_DATA5						((uint32_t)1U << 21U)
#define IOCTRL6_DRV1_VI0_DATA5						((uint32_t)1U << 20U)
#define IOCTRL6_DRV3_VI0_DATA6						((uint32_t)1U << 18U)
#define IOCTRL6_DRV2_VI0_DATA6						((uint32_t)1U << 17U)
#define IOCTRL6_DRV1_VI0_DATA6						((uint32_t)1U << 16U)
#define IOCTRL6_DRV3_VI0_DATA7						((uint32_t)1U << 14U)
#define IOCTRL6_DRV2_VI0_DATA7						((uint32_t)1U << 13U)
#define IOCTRL6_DRV1_VI0_DATA7						((uint32_t)1U << 12U)
#define IOCTRL6_DRV3_VI0_DATA8						((uint32_t)1U << 10U)
#define IOCTRL6_DRV2_VI0_DATA8						((uint32_t)1U << 9U)
#define IOCTRL6_DRV1_VI0_DATA8						((uint32_t)1U << 8U)
#define IOCTRL6_DRV3_VI0_DATA9						((uint32_t)1U << 6U)
#define IOCTRL6_DRV2_VI0_DATA9						((uint32_t)1U << 5U)
#define IOCTRL6_DRV1_VI0_DATA9						((uint32_t)1U << 4U)
#define IOCTRL6_DRV3_VI0_DATA10						((uint32_t)1U << 2U)
#define IOCTRL6_DRV2_VI0_DATA10						((uint32_t)1U << 1U)
#define IOCTRL6_DRV1_VI0_DATA10						((uint32_t)1U << 0U)
#define IOCTRL7_DRV3_VI0_DATA11						((uint32_t)1U << 30U)
#define IOCTRL7_DRV2_VI0_DATA11						((uint32_t)1U << 29U)
#define IOCTRL7_DRV1_VI0_DATA11						((uint32_t)1U << 28U)
#define IOCTRL7_DRV3_VI0_FIELD						((uint32_t)1U << 26U)
#define IOCTRL7_DRV2_VI0_FIELD						((uint32_t)1U << 25U)
#define IOCTRL7_DRV1_VI0_FIELD						((uint32_t)1U << 24U)
#define IOCTRL7_DRV3_VI1_CLK						((uint32_t)1U << 22U)
#define IOCTRL7_DRV2_VI1_CLK						((uint32_t)1U << 21U)
#define IOCTRL7_DRV1_VI1_CLK						((uint32_t)1U << 20U)
#define IOCTRL7_DRV3_VI1_CLKENB						((uint32_t)1U << 18U)
#define IOCTRL7_DRV2_VI1_CLKENB						((uint32_t)1U << 17U)
#define IOCTRL7_DRV1_VI1_CLKENB						((uint32_t)1U << 16U)
#define IOCTRL7_DRV3_VI1_HSYNC						((uint32_t)1U << 14U)
#define IOCTRL7_DRV2_VI1_HSYNC						((uint32_t)1U << 13U)
#define IOCTRL7_DRV1_VI1_HSYNC						((uint32_t)1U << 12U)
#define IOCTRL7_DRV3_VI1_VSYNC						((uint32_t)1U << 10U)
#define IOCTRL7_DRV2_VI1_VSYNC						((uint32_t)1U << 9U)
#define IOCTRL7_DRV1_VI1_VSYNC						((uint32_t)1U << 8U)
#define IOCTRL7_DRV3_VI1_DATA0						((uint32_t)1U << 6U)
#define IOCTRL7_DRV2_VI1_DATA0						((uint32_t)1U << 5U)
#define IOCTRL7_DRV1_VI1_DATA0						((uint32_t)1U << 4U)
#define IOCTRL7_DRV3_VI1_DATA1						((uint32_t)1U << 2U)
#define IOCTRL7_DRV2_VI1_DATA1						((uint32_t)1U << 1U)
#define IOCTRL7_DRV1_VI1_DATA1						((uint32_t)1U << 0U)
#define IOCTRL8_DRV3_VI1_DATA2						((uint32_t)1U << 30U)
#define IOCTRL8_DRV2_VI1_DATA2						((uint32_t)1U << 29U)
#define IOCTRL8_DRV1_VI1_DATA2						((uint32_t)1U << 28U)
#define IOCTRL8_DRV3_VI1_DATA3						((uint32_t)1U << 26U)
#define IOCTRL8_DRV2_VI1_DATA3						((uint32_t)1U << 25U)
#define IOCTRL8_DRV1_VI1_DATA3						((uint32_t)1U << 24U)
#define IOCTRL8_DRV3_VI1_DATA4						((uint32_t)1U << 22U)
#define IOCTRL8_DRV2_VI1_DATA4						((uint32_t)1U << 21U)
#define IOCTRL8_DRV1_VI1_DATA4						((uint32_t)1U << 20U)
#define IOCTRL8_DRV3_VI1_DATA5						((uint32_t)1U << 18U)
#define IOCTRL8_DRV2_VI1_DATA5						((uint32_t)1U << 17U)
#define IOCTRL8_DRV1_VI1_DATA5						((uint32_t)1U << 16U)
#define IOCTRL8_DRV3_VI1_DATA6						((uint32_t)1U << 14U)
#define IOCTRL8_DRV2_VI1_DATA6						((uint32_t)1U << 13U)
#define IOCTRL8_DRV1_VI1_DATA6						((uint32_t)1U << 12U)
#define IOCTRL8_DRV3_VI1_DATA7						((uint32_t)1U << 10U)
#define IOCTRL8_DRV2_VI1_DATA7						((uint32_t)1U << 9U)
#define IOCTRL8_DRV1_VI1_DATA7						((uint32_t)1U << 8U)
#define IOCTRL8_DRV3_VI1_DATA8						((uint32_t)1U << 6U)
#define IOCTRL8_DRV2_VI1_DATA8						((uint32_t)1U << 5U)
#define IOCTRL8_DRV1_VI1_DATA8						((uint32_t)1U << 4U)
#define IOCTRL8_DRV3_VI1_DATA9						((uint32_t)1U << 2U)
#define IOCTRL8_DRV2_VI1_DATA9						((uint32_t)1U << 1U)
#define IOCTRL8_DRV1_VI1_DATA9						((uint32_t)1U << 0U)
#define IOCTRL9_DRV3_VI1_DATA10						((uint32_t)1U << 30U)
#define IOCTRL9_DRV2_VI1_DATA10						((uint32_t)1U << 29U)
#define IOCTRL9_DRV1_VI1_DATA10						((uint32_t)1U << 28U)
#define IOCTRL9_DRV3_VI1_DATA11						((uint32_t)1U << 26U)
#define IOCTRL9_DRV2_VI1_DATA11						((uint32_t)1U << 25U)
#define IOCTRL9_DRV1_VI1_DATA11						((uint32_t)1U << 24U)
#define IOCTRL9_DRV3_VI1_FIELD						((uint32_t)1U << 22U)
#define IOCTRL9_DRV2_VI1_FIELD						((uint32_t)1U << 21U)
#define IOCTRL9_DRV1_VI1_FIELD						((uint32_t)1U << 20U)
#define IOCTRL9_DRV3_VI1_SCL0						((uint32_t)1U << 18U)
#define IOCTRL9_DRV2_VI1_SCL0						((uint32_t)1U << 17U)
#define IOCTRL9_DRV1_VI1_SCL0						((uint32_t)1U << 16U)
#define IOCTRL9_DRV3_VI1_SDA0						((uint32_t)1U << 14U)
#define IOCTRL9_DRV2_VI1_SDA0						((uint32_t)1U << 13U)
#define IOCTRL9_DRV1_VI1_SDA0						((uint32_t)1U << 12U)
#define IOCTRL9_DRV3_VI1_SCL1						((uint32_t)1U << 10U)
#define IOCTRL9_DRV2_VI1_SCL1						((uint32_t)1U << 9U)
#define IOCTRL9_DRV1_VI1_SCL1						((uint32_t)1U << 8U)
#define IOCTRL9_DRV3_VI1_SDA1						((uint32_t)1U << 6U)
#define IOCTRL9_DRV2_VI1_SDA1						((uint32_t)1U << 5U)
#define IOCTRL9_DRV1_VI1_SDA1						((uint32_t)1U << 4U)
#define IOCTRL9_DRV3_VI1_SCL2						((uint32_t)1U << 2U)
#define IOCTRL9_DRV2_VI1_SCL2						((uint32_t)1U << 1U)
#define IOCTRL9_DRV1_VI1_SCL2						((uint32_t)1U << 0U)
#define IOCTRL10_DRV3_VI1_SDA2						((uint32_t)1U << 30U)
#define IOCTRL10_DRV2_VI1_SDA2						((uint32_t)1U << 29U)
#define IOCTRL10_DRV1_VI1_SDA2						((uint32_t)1U << 28U)
#define IOCTRL10_DRV3_AVB_RX_CTL					((uint32_t)1U << 26U)
#define IOCTRL10_DRV2_AVB_RX_CTL					((uint32_t)1U << 25U)
#define IOCTRL10_DRV1_AVB_RX_CTL					((uint32_t)1U << 24U)
#define IOCTRL10_DRV3_AVB_RX_RXC					((uint32_t)1U << 22U)
#define IOCTRL10_DRV2_AVB_RX_RXC					((uint32_t)1U << 21U)
#define IOCTRL10_DRV1_AVB_RX_RXC					((uint32_t)1U << 20U)
#define IOCTRL10_DRV3_AVB_RX_RD0					((uint32_t)1U << 18U)
#define IOCTRL10_DRV2_AVB_RX_RD0					((uint32_t)1U << 17U)
#define IOCTRL10_DRV1_AVB_RX_RD0					((uint32_t)1U << 16U)
#define IOCTRL10_DRV3_AVB_RX_RD1					((uint32_t)1U << 14U)
#define IOCTRL10_DRV2_AVB_RX_RD1					((uint32_t)1U << 13U)
#define IOCTRL10_DRV1_AVB_RX_RD1					((uint32_t)1U << 12U)
#define IOCTRL10_DRV3_AVB_RX_RD2					((uint32_t)1U << 10U)
#define IOCTRL10_DRV2_AVB_RX_RD2					((uint32_t)1U << 9U)
#define IOCTRL10_DRV1_AVB_RX_RD2					((uint32_t)1U << 8U)
#define IOCTRL10_DRV3_AVB_RX_RD3					((uint32_t)1U << 6U)
#define IOCTRL10_DRV2_AVB_RX_RD3					((uint32_t)1U << 5U)
#define IOCTRL10_DRV1_AVB_RX_RD3					((uint32_t)1U << 4U)
#define IOCTRL10_DRV3_AVB_TX_CTL					((uint32_t)1U << 2U)
#define IOCTRL10_DRV2_AVB_TX_CTL					((uint32_t)1U << 1U)
#define IOCTRL10_DRV1_AVB_TX_CTL					((uint32_t)1U << 0U)
#define IOCTRL11_DRV3_AVB_TXC						((uint32_t)1U << 30U)
#define IOCTRL11_DRV2_AVB_TXC						((uint32_t)1U << 29U)
#define IOCTRL11_DRV1_AVB_TXC						((uint32_t)1U << 28U)
#define IOCTRL11_DRV3_AVB_TD0						((uint32_t)1U << 26U)
#define IOCTRL11_DRV2_AVB_TD0						((uint32_t)1U << 25U)
#define IOCTRL11_DRV1_AVB_TD0						((uint32_t)1U << 24U)
#define IOCTRL11_DRV3_AVB_TD1						((uint32_t)1U << 22U)
#define IOCTRL11_DRV2_AVB_TD1						((uint32_t)1U << 21U)
#define IOCTRL11_DRV1_AVB_TD1						((uint32_t)1U << 20U)
#define IOCTRL11_DRV3_AVB_TD2						((uint32_t)1U << 18U)
#define IOCTRL11_DRV2_AVB_TD2						((uint32_t)1U << 17U)
#define IOCTRL11_DRV1_AVB_TD2						((uint32_t)1U << 16U)
#define IOCTRL11_DRV3_AVB_TD3						((uint32_t)1U << 14U)
#define IOCTRL11_DRV2_AVB_TD3						((uint32_t)1U << 13U)
#define IOCTRL11_DRV1_AVB_TD3						((uint32_t)1U << 12U)
#define IOCTRL11_DRV3_AVB_TXCREFCLK					((uint32_t)1U << 10U)
#define IOCTRL11_DRV2_AVB_TXCREFCLK					((uint32_t)1U << 9U)
#define IOCTRL11_DRV1_AVB_TXCREFCLK					((uint32_t)1U << 8U)
#define IOCTRL11_DRV3_AVB_MDIO						((uint32_t)1U << 6U)
#define IOCTRL11_DRV2_AVB_MDIO						((uint32_t)1U << 5U)
#define IOCTRL11_DRV1_AVB_MDIO						((uint32_t)1U << 4U)
#define IOCTRL11_DRV3_AVB_MDC						((uint32_t)1U << 2U)
#define IOCTRL11_DRV2_AVB_MDC						((uint32_t)1U << 1U)
#define IOCTRL11_DRV1_AVB_MDC						((uint32_t)1U << 0U)
#define IOCTRL12_DRV3_AVB_MAGIC						((uint32_t)1U << 30U)
#define IOCTRL12_DRV2_AVB_MAGIC						((uint32_t)1U << 29U)
#define IOCTRL12_DRV1_AVB_MAGIC						((uint32_t)1U << 28U)
#define IOCTRL12_DRV3_AVB_PHY_INT					((uint32_t)1U << 26U)
#define IOCTRL12_DRV2_AVB_PHY_INT					((uint32_t)1U << 25U)
#define IOCTRL12_DRV1_AVB_PHY_INT					((uint32_t)1U << 24U)
#define IOCTRL12_DRV3_AVB_LINK						((uint32_t)1U << 22U)
#define IOCTRL12_DRV2_AVB_LINK						((uint32_t)1U << 21U)
#define IOCTRL12_DRV1_AVB_LINK						((uint32_t)1U << 20U)
#define IOCTRL12_DRV3_AVB_AVTP_MATCH					((uint32_t)1U << 18U)
#define IOCTRL12_DRV2_AVB_AVTP_MATCH					((uint32_t)1U << 17U)
#define IOCTRL12_DRV1_AVB_AVTP_MATCH					((uint32_t)1U << 16U)
#define IOCTRL12_DRV3_AVB_AVTP_CAPTURE					((uint32_t)1U << 14U)
#define IOCTRL12_DRV2_AVB_AVTP_CAPTURE					((uint32_t)1U << 13U)
#define IOCTRL12_DRV1_AVB_AVTP_CAPTURE					((uint32_t)1U << 12U)
#define IOCTRL12_DRV3_GETHER_RX_CTL					((uint32_t)1U << 10U)
#define IOCTRL12_DRV2_GETHER_RX_CTL					((uint32_t)1U << 9U)
#define IOCTRL12_DRV1_GETHER_RX_CTL					((uint32_t)1U << 8U)
#define IOCTRL12_DRV3_GETHER_RXC					((uint32_t)1U << 6U)
#define IOCTRL12_DRV2_GETHER_RXC					((uint32_t)1U << 5U)
#define IOCTRL12_DRV1_GETHER_RXC					((uint32_t)1U << 4U)
#define IOCTRL12_DRV3_GETHER_RD0					((uint32_t)1U << 2U)
#define IOCTRL12_DRV2_GETHER_RD0					((uint32_t)1U << 1U)
#define IOCTRL12_DRV1_GETHER_RD0					((uint32_t)1U << 0U)
#define IOCTRL13_DRV3_GETHER_RD1					((uint32_t)1U << 30U)
#define IOCTRL13_DRV2_GETHER_RD1					((uint32_t)1U << 29U)
#define IOCTRL13_DRV1_GETHER_RD1					((uint32_t)1U << 28U)
#define IOCTRL13_DRV3_GETHER_RD2					((uint32_t)1U << 26U)
#define IOCTRL13_DRV2_GETHER_RD2					((uint32_t)1U << 25U)
#define IOCTRL13_DRV1_GETHER_RD2					((uint32_t)1U << 24U)
#define IOCTRL13_DRV3_GETHER_RD3					((uint32_t)1U << 22U)
#define IOCTRL13_DRV2_GETHER_RD3					((uint32_t)1U << 21U)
#define IOCTRL13_DRV1_GETHER_RD3					((uint32_t)1U << 20U)
#define IOCTRL13_DRV3_GETHER_TX_CTL					((uint32_t)1U << 18U)
#define IOCTRL13_DRV2_GETHER_TX_CTL					((uint32_t)1U << 17U)
#define IOCTRL13_DRV1_GETHER_TX_CTL					((uint32_t)1U << 16U)
#define IOCTRL13_DRV3_GETHER_TXC					((uint32_t)1U << 14U)
#define IOCTRL13_DRV2_GETHER_TXC					((uint32_t)1U << 13U)
#define IOCTRL13_DRV1_GETHER_TXC					((uint32_t)1U << 12U)
#define IOCTRL13_DRV3_GETHER_TD0					((uint32_t)1U << 10U)
#define IOCTRL13_DRV2_GETHER_TD0					((uint32_t)1U << 9U)
#define IOCTRL13_DRV1_GETHER_TD0					((uint32_t)1U << 8U)
#define IOCTRL13_DRV3_GETHER_TD1					((uint32_t)1U << 6U)
#define IOCTRL13_DRV2_GETHER_TD1					((uint32_t)1U << 5U)
#define IOCTRL13_DRV1_GETHER_TD1					((uint32_t)1U << 4U)
#define IOCTRL13_DRV3_GETHER_TD2					((uint32_t)1U << 2U)
#define IOCTRL13_DRV2_GETHER_TD2					((uint32_t)1U << 1U)
#define IOCTRL13_DRV1_GETHER_TD2					((uint32_t)1U << 0U)
#define IOCTRL13_DRV3_GETHER_TD3					((uint32_t)1U << 30U)
#define IOCTRL13_DRV2_GETHER_TD3					((uint32_t)1U << 29U)
#define IOCTRL13_DRV1_GETHER_TD3					((uint32_t)1U << 28U)
#define IOCTRL14_DRV3_GETHER_TXCREFCLK					((uint32_t)1U << 26U)
#define IOCTRL14_DRV2_GETHER_TXCREFCLK					((uint32_t)1U << 25U)
#define IOCTRL14_DRV1_GETHER_TXCREFCLK					((uint32_t)1U << 24U)
#define IOCTRL14_DRV3_GETHER_TXCREFCLK_MEGA				((uint32_t)1U << 22U)
#define IOCTRL14_DRV2_GETHER_TXCREFCLK_MEGA				((uint32_t)1U << 21U)
#define IOCTRL14_DRV1_GETHER_TXCREFCLK_MEGA				((uint32_t)1U << 20U)
#define IOCTRL14_DRV3_GETHER_MDIO					((uint32_t)1U << 18U)
#define IOCTRL14_DRV2_GETHER_MDIO					((uint32_t)1U << 17U)
#define IOCTRL14_DRV1_GETHER_MDIO					((uint32_t)1U << 16U)
#define IOCTRL14_DRV3_GETHER_MDC					((uint32_t)1U << 14U)
#define IOCTRL14_DRV2_GETHER_MDC					((uint32_t)1U << 13U)
#define IOCTRL14_DRV1_GETHER_MDC					((uint32_t)1U << 12U)
#define IOCTRL14_DRV3_GETHER_MAGIC					((uint32_t)1U << 10U)
#define IOCTRL14_DRV2_GETHER_MAGIC					((uint32_t)1U << 9U)
#define IOCTRL14_DRV1_GETHER_MAGIC					((uint32_t)1U << 8U)
#define IOCTRL14_DRV3_GETHER_PHY_INT					((uint32_t)1U << 6U)
#define IOCTRL14_DRV2_GETHER_PHY_INT					((uint32_t)1U << 5U)
#define IOCTRL14_DRV1_GETHER_PHY_INT					((uint32_t)1U << 4U)
#define IOCTRL14_DRV3_GETHER_LINK					((uint32_t)1U << 2U)
#define IOCTRL14_DRV2_GETHER_LINK					((uint32_t)1U << 1U)
#define IOCTRL14_DRV1_GETHER_LINK					((uint32_t)1U << 0U)
#define IOCTRL15_DRV3_CANFD0_TX						((uint32_t)1U << 30U)
#define IOCTRL15_DRV2_CANFD0_TX						((uint32_t)1U << 29U)
#define IOCTRL15_DRV1_CANFD0_TX						((uint32_t)1U << 28U)
#define IOCTRL15_DRV3_CANFD0_RX						((uint32_t)1U << 26U)
#define IOCTRL15_DRV2_CANFD0_RX						((uint32_t)1U << 25U)
#define IOCTRL15_DRV1_CANFD0_RX						((uint32_t)1U << 24U)
#define IOCTRL15_DRV3_CANFD1_TX						((uint32_t)1U << 22U)
#define IOCTRL15_DRV2_CANFD1_TX						((uint32_t)1U << 21U)
#define IOCTRL15_DRV1_CANFD1_TX						((uint32_t)1U << 20U)
#define IOCTRL15_DRV3_CANFD1_RX						((uint32_t)1U << 18U)
#define IOCTRL15_DRV2_CANFD1_RX						((uint32_t)1U << 17U)
#define IOCTRL15_DRV1_CANFD1_RX						((uint32_t)1U << 16U)
#define IOCTRL15_DRV3_CAN_CLK						((uint32_t)1U << 14U)
#define IOCTRL15_DRV2_CAN_CLK						((uint32_t)1U << 13U)
#define IOCTRL15_DRV1_CAN_CLK						((uint32_t)1U << 12U)
#define IOCTRL15_DRV2_QSPI0_SPCLK					((uint32_t)1U << 9U)
#define IOCTRL15_DRV1_QSPI0_SPCLK					((uint32_t)1U << 8U)
#define IOCTRL15_DRV2_QSPI0_MOSI_IO0					((uint32_t)1U << 5U)
#define IOCTRL15_DRV1_QSPI0_MOSI_IO0					((uint32_t)1U << 4U)
#define IOCTRL15_DRV2_QSPI0_MOSI_IO1					((uint32_t)1U << 1U)
#define IOCTRL15_DRV1_QSPI0_MOSI_IO1					((uint32_t)1U << 0U)
#define IOCTRL16_DRV2_QSPI0_MOSI_IO2					((uint32_t)1U << 29U)
#define IOCTRL16_DRV1_QSPI0_MOSI_IO2					((uint32_t)1U << 28U)
#define IOCTRL16_DRV2_QSPI0_MOSI_IO3					((uint32_t)1U << 25U)
#define IOCTRL16_DRV1_QSPI0_MOSI_IO3					((uint32_t)1U << 24U)
#define IOCTRL16_DRV2_QSPI0_SSL						((uint32_t)1U << 21U)
#define IOCTRL16_DRV1_QSPI0_SSL						((uint32_t)1U << 20U)
#define IOCTRL16_DRV2_QSPI1_SPCLK					((uint32_t)1U << 17U)
#define IOCTRL16_DRV1_QSPI1_SPCLK					((uint32_t)1U << 16U)
#define IOCTRL16_DRV2_QSPI1_MOSI_IO0					((uint32_t)1U << 13U)
#define IOCTRL16_DRV1_QSPI1_MOSI_IO0					((uint32_t)1U << 12U)
#define IOCTRL16_DRV2_QSPI1_MOSI_IO1					((uint32_t)1U << 9U)
#define IOCTRL16_DRV1_QSPI1_MOSI_IO1					((uint32_t)1U << 8U)
#define IOCTRL16_DRV2_QSPI1_IO2						((uint32_t)1U << 5U)
#define IOCTRL16_DRV1_QSPI1_IO2						((uint32_t)1U << 4U)
#define IOCTRL16_DRV2_QSPI1_IO3						((uint32_t)1U << 1U)
#define IOCTRL16_DRV1_QSPI1_IO3						((uint32_t)1U << 0U)
#define IOCTRL17_DRV2_QSPI1_SSL						((uint32_t)1U << 29U)
#define IOCTRL17_DRV1_QSPI1_SSL						((uint32_t)1U << 28U)
#define IOCTRL17_DRV2_QSPI1_RPC_RESET					((uint32_t)1U << 25U)
#define IOCTRL17_DRV1_QSPI1_RPC_RESET					((uint32_t)1U << 24U)
#define IOCTRL17_DRV2_RPC_WP						((uint32_t)1U << 21U)
#define IOCTRL17_DRV1_RPC_WP						((uint32_t)1U << 20U)
#define IOCTRL17_DRV2_RPC_INT						((uint32_t)1U << 17U)
#define IOCTRL17_DRV1_RPC_INT						((uint32_t)1U << 16U)
#define IOCTRL17_DRV2_DIGRF_CLKIN					((uint32_t)1U << 13U)
#define IOCTRL17_DRV1_DIGRF_CLKIN					((uint32_t)1U << 12U)
#define IOCTRL17_DRV2_DIGRF_CLKOUT					((uint32_t)1U << 9U)
#define IOCTRL17_DRV1_DIGRF_CLKOUT					((uint32_t)1U << 8U)
#define IOCTRL17_DRV2_RPC_IRQ4						((uint32_t)1U << 5U)
#define IOCTRL17_DRV1_RPC_IRQ4						((uint32_t)1U << 4U)
#define IOCTRL17_DRV2_RPC_IRQ5						((uint32_t)1U << 1U)
#define IOCTRL17_DRV1_RPC_IRQ5						((uint32_t)1U << 0U)
#define IOCTRL18_DRV3_SCL3						((uint32_t)1U << 30U)
#define IOCTRL18_DRV2_SCL3						((uint32_t)1U << 29U)
#define IOCTRL18_DRV1_SCL3						((uint32_t)1U << 28U)
#define IOCTRL18_DRV3_SDA3						((uint32_t)1U << 26U)
#define IOCTRL18_DRV2_SDA3						((uint32_t)1U << 25U)
#define IOCTRL18_DRV1_SDA3						((uint32_t)1U << 24U)
#define IOCTRL18_DRV3_MSIOF0_RXD					((uint32_t)1U << 22U)
#define IOCTRL18_DRV2_MSIOF0_RXD					((uint32_t)1U << 21U)
#define IOCTRL18_DRV1_MSIOF0_RXD					((uint32_t)1U << 20U)
#define IOCTRL18_DRV3_MSIOF0_TXD					((uint32_t)1U << 18U)
#define IOCTRL18_DRV2_MSIOF0_TXD					((uint32_t)1U << 17U)
#define IOCTRL18_DRV1_MSIOF0_TXD					((uint32_t)1U << 16U)
#define IOCTRL18_DRV3_MSIOF0_SCK					((uint32_t)1U << 14U)
#define IOCTRL18_DRV2_MSIOF0_SCK					((uint32_t)1U << 13U)
#define IOCTRL18_DRV1_MSIOF0_SCK					((uint32_t)1U << 12U)
#define IOCTRL18_DRV3_MSIOF0_SYNC					((uint32_t)1U << 10U)
#define IOCTRL18_DRV2_MSIOF0_SYNC					((uint32_t)1U << 9U)
#define IOCTRL18_DRV1_MSIOF0_SYNC					((uint32_t)1U << 8U)
#define IOCTRL18_DRV3_MSIOF0_SS1					((uint32_t)1U << 6U)
#define IOCTRL18_DRV2_MSIOF0_SS1					((uint32_t)1U << 5U)
#define IOCTRL18_DRV1_MSIOF0_SS1					((uint32_t)1U << 4U)
#define IOCTRL18_DRV3_MSIOF0_SS2					((uint32_t)1U << 2U)
#define IOCTRL18_DRV2_MSIOF0_SS2					((uint32_t)1U << 1U)
#define IOCTRL18_DRV1_MSIOF0_SS2					((uint32_t)1U << 0U)
#define IOCTRL19_DRV3_FSO_CFE_0						((uint32_t)1U << 30U)
#define IOCTRL19_DRV2_FSO_CFE_0						((uint32_t)1U << 29U)
#define IOCTRL19_DRV1_FSO_CFE_0						((uint32_t)1U << 28U)
#define IOCTRL19_DRV3_FSO_CFE_1						((uint32_t)1U << 26U)
#define IOCTRL19_DRV2_FSO_CFE_1						((uint32_t)1U << 25U)
#define IOCTRL19_DRV1_FSO_CFE_1						((uint32_t)1U << 24U)
#define IOCTRL19_DRV3_FSO_TOE						((uint32_t)1U << 22U)
#define IOCTRL19_DRV2_FSO_TOE						((uint32_t)1U << 21U)
#define IOCTRL19_DRV1_FSO_TOE						((uint32_t)1U << 20U)

#define IOCTRL30_POC_VI0_DATA5						((uint32_t)1U << 31U)
#define IOCTRL30_POC_VI0_DATA4						((uint32_t)1U << 30U)
#define IOCTRL30_POC_VI0_DATA3						((uint32_t)1U << 29U)
#define IOCTRL30_POC_VI0_DATA2						((uint32_t)1U << 28U)
#define IOCTRL30_POC_VI0_DATA1						((uint32_t)1U << 27U)
#define IOCTRL30_POC_VI0_DATA0						((uint32_t)1U << 26U)
#define IOCTRL30_POC_VI0_VSYNC_N					((uint32_t)1U << 25U)
#define IOCTRL30_POC_VI0_HSYNC_N					((uint32_t)1U << 24U)
#define IOCTRL30_POC_VI0_CLKENB						((uint32_t)1U << 23U)
#define IOCTRL30_POC_VI0_CLK						((uint32_t)1U << 22U)
#define IOCTRL30_POC_DU_EXODDF_DU_ODDF_DISP_CDE				((uint32_t)1U << 21U)
#define IOCTRL30_POC_DU_EXVSYNC_DU_VSYNC				((uint32_t)1U << 20U)
#define IOCTRL30_POC_DU_EXHSYNC_DU_HSYNC				((uint32_t)1U << 19U)
#define IOCTRL30_POC_DU_DOTCLKOUT					((uint32_t)1U << 18U)
#define IOCTRL30_POC_DU_DB7						((uint32_t)1U << 17U)
#define IOCTRL30_POC_DU_DB6						((uint32_t)1U << 16U)
#define IOCTRL30_POC_DU_DB5						((uint32_t)1U << 15U)
#define IOCTRL30_POC_DU_DB4						((uint32_t)1U << 14U)
#define IOCTRL30_POC_DU_DB3						((uint32_t)1U << 13U)
#define IOCTRL30_POC_DU_DB2						((uint32_t)1U << 12U)
#define IOCTRL30_POC_DU_DG7						((uint32_t)1U << 11U)
#define IOCTRL30_POC_DU_DG6						((uint32_t)1U << 10U)
#define IOCTRL30_POC_DU_DG5						((uint32_t)1U << 9U)
#define IOCTRL30_POC_DU_DG4						((uint32_t)1U << 8U)
#define IOCTRL30_POC_DU_DG3						((uint32_t)1U << 7U)
#define IOCTRL30_POC_DU_DG2						((uint32_t)1U << 6U)
#define IOCTRL30_POC_DU_DR7						((uint32_t)1U << 5U)
#define IOCTRL30_POC_DU_DR6						((uint32_t)1U << 4U)
#define IOCTRL30_POC_DU_DR5						((uint32_t)1U << 3U)
#define IOCTRL30_POC_DU_DR4						((uint32_t)1U << 2U)
#define IOCTRL30_POC_DU_DR3						((uint32_t)1U << 1U)
#define IOCTRL30_POC_DU_DR2						((uint32_t)1U << 0U)

#define IOCTRL31_POC_MSIOF0_SS2						((uint32_t)1U << 31U)
#define IOCTRL31_POC_MSIOF0_SS1						((uint32_t)1U << 30U)
#define IOCTRL31_POC_MSIOF0_SYNC					((uint32_t)1U << 29U)
#define IOCTRL31_POC_MSIOF0_SCK						((uint32_t)1U << 28U)
#define IOCTRL31_POC_MSIOF0_TXD						((uint32_t)1U << 27U)
#define IOCTRL31_POC_MSIOF0_RXD						((uint32_t)1U << 26U)
#define IOCTRL31_POC_MSIOF0_IRQ5					((uint32_t)1U << 25U)
#define IOCTRL31_POC_MSIOF0_IRQ4					((uint32_t)1U << 24U)
#define IOCTRL31_POC_VI1_FIELD						((uint32_t)1U << 23U)
#define IOCTRL31_POC_VI1_DATA11						((uint32_t)1U << 22U)
#define IOCTRL31_POC_VI1_DATA10						((uint32_t)1U << 21U)
#define IOCTRL31_POC_VI1_DATA9						((uint32_t)1U << 20U)
#define IOCTRL31_POC_VI1_DATA8						((uint32_t)1U << 19U)
#define IOCTRL31_POC_VI1_DATA7						((uint32_t)1U << 18U)
#define IOCTRL31_POC_VI1_DATA6						((uint32_t)1U << 17U)
#define IOCTRL31_POC_VI1_DATA5						((uint32_t)1U << 16U)
#define IOCTRL31_POC_VI1_DATA4						((uint32_t)1U << 15U)
#define IOCTRL31_POC_VI1_DATA3						((uint32_t)1U << 14U)
#define IOCTRL31_POC_VI1_DATA2						((uint32_t)1U << 13U)
#define IOCTRL31_POC_VI1_DATA1						((uint32_t)1U << 12U)
#define IOCTRL31_POC_VI1_DATA0						((uint32_t)1U << 11U)
#define IOCTRL31_POC_VI1_VSYNC						((uint32_t)1U << 10U)
#define IOCTRL31_POC_VI1_HSYNC						((uint32_t)1U << 9U)
#define IOCTRL31_POC_VI1_CLKENB						((uint32_t)1U << 8U)
#define IOCTRL31_POC_VI1_CLK						((uint32_t)1U << 7U)
#define IOCTRL31_POC_VI0_FIELD						((uint32_t)1U << 6U)
#define IOCTRL31_POC_VI0_DATA11						((uint32_t)1U << 5U)
#define IOCTRL31_POC_VI0_DATA10						((uint32_t)1U << 4U)
#define IOCTRL31_POC_VI0_DATA9						((uint32_t)1U << 3U)
#define IOCTRL31_POC_VI0_DATA8						((uint32_t)1U << 2U)
#define IOCTRL31_POC_VI0_DATA7						((uint32_t)1U << 1U)
#define IOCTRL31_POC_VI0_DATA6						((uint32_t)1U << 0U)
#define IOCTRL32_POC_FSO_TOE						((uint32_t)1U << 4U)
#define IOCTRL32_POC_FSO_CFE_1						((uint32_t)1U << 3U)
#define IOCTRL32_POC_FSO_CFE_0						((uint32_t)1U << 2U)
#define IOCTRL32_POC_SDA3						((uint32_t)1U << 1U)
#define IOCTRL32_POC_SCL3						((uint32_t)1U << 0U)
#define IOCTRL40_SD0TDSEL1						((uint32_t)1U << 1U)
#define IOCTRL40_SD0TDSEL0						((uint32_t)1U << 0U)

#define	MOD_sel_canfd0_A		((uint32_t)0U << 11U)
#define	MOD_sel_canfd0_B		((uint32_t)1U << 11U)
#define	MOD_sel_gether_A		((uint32_t)0U << 10U)
#define	MOD_sel_gether_B		((uint32_t)1U << 10U)
#define	MOD_sel_hscif0_A		((uint32_t)0U << 9U)
#define	MOD_sel_hscif0_B		((uint32_t)1U << 9U)
#define	MOD_sel_pwm0_A			((uint32_t)0U << 8U)
#define	MOD_sel_pwm0_B			((uint32_t)1U << 8U)
#define	MOD_sel_pwm1_A			((uint32_t)0U << 7U)
#define	MOD_sel_pwm1_B			((uint32_t)1U << 7U)
#define	MOD_sel_pwm2_A			((uint32_t)0U << 6U)
#define	MOD_sel_pwm2_B			((uint32_t)1U << 6U)
#define	MOD_sel_pwm3_A			((uint32_t)0U << 5U)
#define	MOD_sel_pwm3_B			((uint32_t)1U << 5U)
#define	MOD_sel_pwm4_A			((uint32_t)0U << 4U)
#define	MOD_sel_pwm4_B			((uint32_t)1U << 4U)
#define	MOD_sel_rsp_A			((uint32_t)0U << 2U)
#define	MOD_sel_rsp_B			((uint32_t)1U << 2U)
#define	MOD_sel_scif1_A			((uint32_t)0U << 1U)
#define	MOD_sel_scif1_B			((uint32_t)1U << 1U)
#define	MOD_sel_tmu_A			((uint32_t)0U << 0U)
#define	MOD_sel_tmu_B			((uint32_t)1U << 0U)


static void pfc_reg_write(uint32_t addr, uint32_t data);

static void pfc_reg_write(uint32_t addr, uint32_t data)
{
	mmio_write_32(PFC_PMMR, ~data);
	mmio_write_32((uintptr_t)addr, data);
}


void pfc_init_v3h(void)
{

	/* initialize module select */
	pfc_reg_write(PFC_MOD_SEL0, MOD_sel_canfd0_A
				  | MOD_sel_gether_A
				  | MOD_sel_hscif0_B
				  | MOD_sel_pwm0_A
				  | MOD_sel_pwm1_A
				  | MOD_sel_pwm2_A
				  | MOD_sel_pwm3_A
				  | MOD_sel_pwm4_A
				  | MOD_sel_rsp_A
				  | MOD_sel_scif1_A
				  | MOD_sel_tmu_A);

	/* initialize peripheral function select */
	pfc_reg_write(PFC_IPSR0, IPSR_28_FUNC(0)
			       | IPSR_24_FUNC(0)
			       | IPSR_20_FUNC(0)
			       | IPSR_16_FUNC(0)
			       | IPSR_12_FUNC(0)
			       | IPSR_8_FUNC(0)
			       | IPSR_4_FUNC(0)
			       | IPSR_0_FUNC(0));
	pfc_reg_write(PFC_IPSR1, IPSR_28_FUNC(0)
			       | IPSR_24_FUNC(0)
			       | IPSR_20_FUNC(0)
			       | IPSR_16_FUNC(0)
			       | IPSR_12_FUNC(0)
			       | IPSR_8_FUNC(0)
			       | IPSR_4_FUNC(0)
			       | IPSR_0_FUNC(0));
	pfc_reg_write(PFC_IPSR2, IPSR_28_FUNC(0)
			       | IPSR_24_FUNC(0)
			       | IPSR_20_FUNC(0)
			       | IPSR_16_FUNC(0)
			       | IPSR_12_FUNC(0)
			       | IPSR_8_FUNC(0)
			       | IPSR_4_FUNC(0)
			       | IPSR_0_FUNC(0));
	pfc_reg_write(PFC_IPSR3, IPSR_28_FUNC(0)
			       | IPSR_24_FUNC(0)
			       | IPSR_20_FUNC(0)
			       | IPSR_16_FUNC(0)
			       | IPSR_12_FUNC(0)
			       | IPSR_8_FUNC(0)
			       | IPSR_4_FUNC(0)
			       | IPSR_0_FUNC(0));
	pfc_reg_write(PFC_IPSR4, IPSR_28_FUNC(0)
			       | IPSR_24_FUNC(0)
			       | IPSR_20_FUNC(0)
			       | IPSR_16_FUNC(0)
			       | IPSR_12_FUNC(0)
			       | IPSR_8_FUNC(0)
			       | IPSR_4_FUNC(0)
			       | IPSR_0_FUNC(0));
	pfc_reg_write(PFC_IPSR5, IPSR_28_FUNC(4)
			       | IPSR_24_FUNC(4)
			       | IPSR_20_FUNC(0)
			       | IPSR_16_FUNC(0)
			       | IPSR_12_FUNC(0)
			       | IPSR_8_FUNC(0)
			       | IPSR_4_FUNC(0)
			       | IPSR_0_FUNC(0));
	pfc_reg_write(PFC_IPSR6, IPSR_28_FUNC(4)
			       | IPSR_24_FUNC(4)
			       | IPSR_20_FUNC(4)
			       | IPSR_16_FUNC(4)
			       | IPSR_12_FUNC(4)
			       | IPSR_8_FUNC(4)
			       | IPSR_4_FUNC(4)
			       | IPSR_0_FUNC(4));
	pfc_reg_write(PFC_IPSR7, IPSR_28_FUNC(0)
			       | IPSR_24_FUNC(4)
			       | IPSR_20_FUNC(4)
			       | IPSR_16_FUNC(0)
			       | IPSR_12_FUNC(0)
			       | IPSR_8_FUNC(0)
			       | IPSR_4_FUNC(0)
			       | IPSR_0_FUNC(4));
	pfc_reg_write(PFC_IPSR8, IPSR_28_FUNC(0)
			       | IPSR_24_FUNC(0)
			       | IPSR_20_FUNC(4)
			       | IPSR_16_FUNC(0)
			       | IPSR_12_FUNC(0)
			       | IPSR_8_FUNC(0)
			       | IPSR_4_FUNC(0)
			       | IPSR_0_FUNC(0));
	pfc_reg_write(PFC_IPSR9, IPSR_28_FUNC(0)
			       | IPSR_24_FUNC(0)
			       | IPSR_20_FUNC(0)
			       | IPSR_16_FUNC(0)
			       | IPSR_12_FUNC(0)
			       | IPSR_8_FUNC(0)
			       | IPSR_4_FUNC(0)
			       | IPSR_0_FUNC(0));
	pfc_reg_write(PFC_IPSR10, IPSR_28_FUNC(0)
				| IPSR_24_FUNC(0)
				| IPSR_20_FUNC(0)
				| IPSR_16_FUNC(0)
				| IPSR_12_FUNC(0)
				| IPSR_8_FUNC(0)
				| IPSR_4_FUNC(0)
				| IPSR_0_FUNC(0));

	/* initialize GPIO/perihperal function select */
	pfc_reg_write(PFC_GPSR0, 0x00000000);

	pfc_reg_write(PFC_GPSR1, GPSR1_DIGRF_CLKOUT
			       | GPSR1_DIGRF_CLKIN
			       | GPSR1_CANFD_CLK_A
			       | GPSR1_CANFD0_RX_A
			       | GPSR1_CANFD0_TX_A
			       | GPSR1_AVB_LINK
			       | GPSR1_AVB_PHY_INT
			       | GPSR1_AVB_MDC
			       | GPSR1_AVB_MDIO
			       | GPSR1_AVB_TXCREFCLK
			       | GPSR1_AVB_TD3
			       | GPSR1_AVB_TD2
			       | GPSR1_AVB_TD1
			       | GPSR1_AVB_TD0
			       | GPSR1_AVB_TXC
			       | GPSR1_AVB_TX_CTL
			       | GPSR1_AVB_RD3
			       | GPSR1_AVB_RD2
			       | GPSR1_AVB_RD1
			       | GPSR1_AVB_RD0
			       | GPSR1_AVB_RXC
			       | GPSR1_AVB_RX_CTL
			       | GPSR1_IRQ0);

	pfc_reg_write(PFC_GPSR2, 0x00000000);

	pfc_reg_write(PFC_GPSR3, GPSR3_VI1_FIELD
			       | GPSR3_VI1_DATA11
			       | GPSR3_VI1_DATA10
			       | GPSR3_VI1_DATA9
			       | GPSR3_VI1_DATA8
			       | GPSR3_VI1_DATA7
			       | GPSR3_VI1_DATA6
			       | GPSR3_VI1_DATA5
			       | GPSR3_VI1_DATA4
			       | GPSR3_VI1_DATA3
			       | GPSR3_VI1_DATA2);

	pfc_reg_write(PFC_GPSR4, GPSR4_GETHER_LINK_A
			       | GPSR4_GETHER_PHY_INT_A
			       | GPSR4_GETHER_MDC_A
			       | GPSR4_GETHER_MDIO_A
			       | GPSR4_GETHER_TXCREFCLK_MEGA
			       | GPSR4_GETHER_TXCREFCLK
			       | GPSR4_GETHER_TD3
			       | GPSR4_GETHER_TD2
			       | GPSR4_GETHER_TD1
			       | GPSR4_GETHER_TD0
			       | GPSR4_GETHER_TXC
			       | GPSR4_GETHER_TX_CTL
			       | GPSR4_GETHER_RD3
			       | GPSR4_GETHER_RD2
			       | GPSR4_GETHER_RD1
			       | GPSR4_GETHER_RD0
			       | GPSR4_GETHER_RXC
			       | GPSR4_GETHER_RX_CTL
			       | GPSR4_SDA2
			       | GPSR4_SCL2
			       | GPSR4_SDA1
			       | GPSR4_SCL1
			       | GPSR4_SDA0
			       | GPSR4_SCL0);

	pfc_reg_write(PFC_GPSR5, GPSR5_QSPI1_SSL
			       | GPSR5_QSPI1_IO3
			       | GPSR5_QSPI1_IO2
			       | GPSR5_QSPI1_MISO_IO1
			       | GPSR5_QSPI1_MOSI_IO0
			       | GPSR5_QSPI1_SPCLK
			       | GPSR5_QSPI0_SSL
			       | GPSR5_QSPI0_IO3
			       | GPSR5_QSPI0_IO2
			       | GPSR5_QSPI0_MISO_IO1
			       | GPSR5_QSPI0_MOSI_IO0
			       | GPSR5_QSPI0_SPCLK);

	
	
	/* initialize POC Control */

	pfc_reg_write(PFC_IOCTRL30, IOCTRL30_POC_VI0_DATA5
			       | IOCTRL30_POC_VI0_DATA4
			       | IOCTRL30_POC_VI0_DATA3
			       | IOCTRL30_POC_VI0_DATA2
			       | IOCTRL30_POC_VI0_DATA1
			       | IOCTRL30_POC_VI0_DATA0
			       | IOCTRL30_POC_VI0_VSYNC_N
			       | IOCTRL30_POC_VI0_HSYNC_N
			       | IOCTRL30_POC_VI0_CLKENB
			       | IOCTRL30_POC_VI0_CLK
			       | IOCTRL30_POC_DU_EXODDF_DU_ODDF_DISP_CDE
			       | IOCTRL30_POC_DU_EXVSYNC_DU_VSYNC
			       | IOCTRL30_POC_DU_EXHSYNC_DU_HSYNC
			       | IOCTRL30_POC_DU_DOTCLKOUT
			       | IOCTRL30_POC_DU_DB7
			       | IOCTRL30_POC_DU_DB6
			       | IOCTRL30_POC_DU_DB5
			       | IOCTRL30_POC_DU_DB4
			       | IOCTRL30_POC_DU_DB3
			       | IOCTRL30_POC_DU_DB2
			       | IOCTRL30_POC_DU_DG7
			       | IOCTRL30_POC_DU_DG6
			       | IOCTRL30_POC_DU_DG5
			       | IOCTRL30_POC_DU_DG4
			       | IOCTRL30_POC_DU_DG3
			       | IOCTRL30_POC_DU_DG2
			       | IOCTRL30_POC_DU_DR7
			       | IOCTRL30_POC_DU_DR6
			       | IOCTRL30_POC_DU_DR5
			       | IOCTRL30_POC_DU_DR4
			       | IOCTRL30_POC_DU_DR3
			       | IOCTRL30_POC_DU_DR2);

	pfc_reg_write(PFC_IOCTRL31, IOCTRL31_POC_MSIOF0_SS2
			       | IOCTRL31_POC_MSIOF0_SS1
			       | IOCTRL31_POC_MSIOF0_SYNC
			       | IOCTRL31_POC_MSIOF0_SCK
			       | IOCTRL31_POC_MSIOF0_TXD
			       | IOCTRL31_POC_MSIOF0_RXD
			       | IOCTRL31_POC_MSIOF0_IRQ5
			       | IOCTRL31_POC_MSIOF0_IRQ4
			       | IOCTRL31_POC_VI1_FIELD
			       | IOCTRL31_POC_VI1_DATA11
			       | IOCTRL31_POC_VI1_DATA10
			       | IOCTRL31_POC_VI1_DATA9
			       | IOCTRL31_POC_VI1_DATA8
			       | IOCTRL31_POC_VI1_DATA7
			       | IOCTRL31_POC_VI1_DATA6
			       | IOCTRL31_POC_VI1_DATA5
			       | IOCTRL31_POC_VI1_DATA4
			       | IOCTRL31_POC_VI1_DATA3
			       | IOCTRL31_POC_VI1_DATA2
			       | IOCTRL31_POC_VI1_DATA1
			       | IOCTRL31_POC_VI1_DATA0
			       | IOCTRL31_POC_VI1_VSYNC
			       | IOCTRL31_POC_VI1_HSYNC
			       | IOCTRL31_POC_VI1_CLKENB
			       | IOCTRL31_POC_VI1_CLK
			       | IOCTRL31_POC_VI0_FIELD
			       | IOCTRL31_POC_VI0_DATA11
			       | IOCTRL31_POC_VI0_DATA10
			       | IOCTRL31_POC_VI0_DATA9
			       | IOCTRL31_POC_VI0_DATA8
			       | IOCTRL31_POC_VI0_DATA7
			       | IOCTRL31_POC_VI0_DATA6
			       | IOCTRL31_POC_VI0_DATA6);

	pfc_reg_write(PFC_IOCTRL32, IOCTRL32_POC_FSO_TOE
			       | IOCTRL32_POC_FSO_CFE_1
			       | IOCTRL32_POC_FSO_CFE_0
			       | IOCTRL32_POC_SDA3
			       | IOCTRL32_POC_SCL3);

	pfc_reg_write(PFC_IOCTRL33,0x00000000);

	pfc_reg_write(PFC_IOCTRL40,0x00000000);
	

	/* initialize LSI pin pull-up/down control */
	pfc_reg_write(PFC_PUD0,0x80000000U);
	pfc_reg_write(PFC_PUD1,0x1B01C77CU);
	pfc_reg_write(PFC_PUD2,0x00000000U);
	pfc_reg_write(PFC_PUD3,0x0F800008U);
	pfc_reg_write(PFC_PUD4,0x03807C00U);

	/* initialize LSI pin pull-enable register */
	pfc_reg_write(PFC_PUEN0,0x0035F721U);
	pfc_reg_write(PFC_PUEN1,0x7E01C700U);
	pfc_reg_write(PFC_PUEN2,0x003F0000U);
	pfc_reg_write(PFC_PUEN3,0x07000000U);
	pfc_reg_write(PFC_PUEN4,0x0381E800U);
	
	/* initialize positive/negative logic select */
	mmio_write_32(GPIO_POSNEG0, 0x00000000U);
	mmio_write_32(GPIO_POSNEG1, 0x00000000U);
	mmio_write_32(GPIO_POSNEG2, 0x00000000U);
	mmio_write_32(GPIO_POSNEG3, 0x00000000U);
	mmio_write_32(GPIO_POSNEG4, 0x00000000U);
	mmio_write_32(GPIO_POSNEG5, 0x00000000U);

	/* initialize general IO/interrupt switching */
	mmio_write_32(GPIO_IOINTSEL0, 0x00000000U);
	mmio_write_32(GPIO_IOINTSEL1, 0x00000000U);
	mmio_write_32(GPIO_IOINTSEL2, 0x00000000U);
	mmio_write_32(GPIO_IOINTSEL3, 0x00000000U);
	mmio_write_32(GPIO_IOINTSEL4, 0x00000000U);
	mmio_write_32(GPIO_IOINTSEL5, 0x00000000U);

	/* initialize general output register */
	mmio_write_32(GPIO_OUTDT1, 0x00010000U);
	mmio_write_32(GPIO_OUTDT4, 0x00400000U);
	mmio_write_32(GPIO_OUTDT5, 0x00007000U);

	/* initialize general input/output switching */
	mmio_write_32(GPIO_INOUTSEL0, 0x00000000U);
	mmio_write_32(GPIO_INOUTSEL1, 0x00010000U);
	mmio_write_32(GPIO_INOUTSEL2, 0x00000000U);
	mmio_write_32(GPIO_INOUTSEL3, 0x00000000U);
	mmio_write_32(GPIO_INOUTSEL4, 0x00400000U);
	mmio_write_32(GPIO_INOUTSEL5, 0x00007000U);
}
