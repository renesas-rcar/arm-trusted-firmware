/*
 * Copyright (c) 2015-2016, Renesas Electronics Corporation
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

#include <debug.h>
#include <mmio.h>
#include <arch_helpers.h>
#include <bl_common.h>
#include "bl2_sdram_init.h"
#include "bl2_phy_init.h"



#define	BIT3			(0x08U)

/* #define	DEBUG_SDRA_REG_PRINT */	/* print dbsc4 registers */

#define	EFUSE_DUMMY5		(0xE60603E8U)
#define	GET_DUMMY5_PKG(x)	(((x) & 0x00000001) >> 0U)	/* bit 0 */
#define	DUMMY5_PKG_SIP		((uint32_t)0U)
#define	DUMMY5_PKG_DISCRETE	((uint32_t)1U)
#define	GET_DUMMY5_VENDOR(x)	(((x) & 0x00000000U) >> 1U)	/* bit 2:1 */
#define	DUMMY5_VENDOR_SAMSUNG	((uint32_t)0U)
#define	DUMMY5_VENDOR_MICRON	((uint32_t)0U)
#define	DUMMY5_VENDOR_HYNIX	((uint32_t)0U)
#define	GET_DUMMY5_MEMCONFIG(x)	(((x) & 0x00000078U) >> 3U)	/* bit 6:3 */
#define	DUMMY5_MEMCONFIG_1GB_4	((uint32_t)0U)
#define	DUMMY5_MEMCONFIG_2GB_4	((uint32_t)1U)
#define	GET_DUMMY5_MEMTYPE(x)	(((x) & 0x00000380U) >> 7U)	/* bit 9:7 */
#define	DUMMY5_MEMTYPE_LPDDR4	((uint32_t)0U)
#define	DUMMY5_MEMTYPE_LPDDR3	((uint32_t)1U)
#define	DUMMY5_MEMTYPE_DDR3L	((uint32_t)2U)
#define	GET_DUMMY5_MEMRATE(x)	(((x) & 0x00000C00U) >> 10U)	/* bit 11:10 */
#define	DUMMY5_MEMRATE_3200	((uint32_t)0U)
#define	DUMMY5_MEMRATE_2667	((uint32_t)1U)

#define	CPG_BASE		(0xE6150000U)
#define	CPG_PLL3CR		(CPG_BASE + 0x00DCU)
#define	CPG_SRCR4		(CPG_BASE + 0x00BCU)
#define	CPG_PLLECR		(CPG_BASE + 0x00D0U)
#define	CPG_CPGWPR		(CPG_BASE + 0x0900U)
#define	CPG_SRSTCLR4		(CPG_BASE + 0x0950U)
#define	PLLECR_PLL3ST		((uint32_t)1U << 11U)
#define	PLLECR_PLL3E		((uint32_t)1U << 3U)

#define	RST_BASE		(0xE6160000U)
#define	RST_MODEMR		(RST_BASE + 0x0060U)
#define	MODEMR_MD19		((uint32_t)1U << 19U)
#define	MODEMR_MD17		((uint32_t)1U << 17U)
#define MODEMR_DDR_CLOCK_MASK	((uint32_t)(~(MODEMR_MD19 | MODEMR_MD17)))
#define MODEMR_DDR_CLOCK_3200	(0x00000000U)
#define MODEMR_DDR_CLOCK_1600	((uint32_t)(MODEMR_MD19 | MODEMR_MD17))


#define	DBSC_BASE		(0xE6790000U)
#define	DBSC_DBSYSCONF0		(DBSC_BASE + 0x0000U)
#define	DBSC_DBSYSCONF1		(DBSC_BASE + 0x0004U)
#define	DBSC_DBPHYCONF0		(DBSC_BASE + 0x0010U)
#define	DBSC_DBKIND		(DBSC_BASE + 0x0020U)
#define	DBSC_DBMEMCONF_0_0	(DBSC_BASE + 0x0030U)
#define	DBSC_DBMEMCONF_0_1	(DBSC_BASE + 0x0034U)
#define	DBSC_DBMEMCONF_0_2	(DBSC_BASE + 0x0038U)
#define	DBSC_DBMEMCONF_0_3	(DBSC_BASE + 0x003CU)
#define	DBSC_DBMEMCONF_1_0	(DBSC_BASE + 0x0040U)
#define	DBSC_DBMEMCONF_1_1	(DBSC_BASE + 0x0044U)
#define	DBSC_DBMEMCONF_1_2	(DBSC_BASE + 0x0048U)
#define	DBSC_DBMEMCONF_1_3	(DBSC_BASE + 0x004CU)
#define	DBSC_DBMEMCONF_2_0	(DBSC_BASE + 0x0050U)
#define	DBSC_DBMEMCONF_2_1	(DBSC_BASE + 0x0054U)
#define	DBSC_DBMEMCONF_2_2	(DBSC_BASE + 0x0058U)
#define	DBSC_DBMEMCONF_2_3	(DBSC_BASE + 0x005CU)
#define	DBSC_DBMEMCONF_3_0	(DBSC_BASE + 0x0060U)
#define	DBSC_DBMEMCONF_3_1	(DBSC_BASE + 0x0064U)
#define	DBSC_DBMEMCONF_3_2	(DBSC_BASE + 0x0068U)
#define	DBSC_DBMEMCONF_3_3	(DBSC_BASE + 0x006CU)
#define	DBSC_DBINTEN		(DBSC_BASE + 0x0180U)
#define	DBSC_DBINTSTAT0		(DBSC_BASE + 0x0184U)
#define	DBSC_DBACEN		(DBSC_BASE + 0x0200U)
#define	DBSC_DBRFEN		(DBSC_BASE + 0x0204U)
#define	DBSC_DBCMD		(DBSC_BASE + 0x0208U)
#define	DBSC_DBWAIT		(DBSC_BASE + 0x0210U)
#define	DBSC_DBSYSCTRL0		(DBSC_BASE + 0x0280U)
#define	DBSC_DBTR0		(DBSC_BASE + 0x0300U)
#define	DBSC_DBTR1		(DBSC_BASE + 0x0304U)
#define	DBSC_DBTR2		(DBSC_BASE + 0x0308U)
#define	DBSC_DBTR3		(DBSC_BASE + 0x030CU)
#define	DBSC_DBTR4		(DBSC_BASE + 0x0310U)
#define	DBSC_DBTR5		(DBSC_BASE + 0x0314U)
#define	DBSC_DBTR6		(DBSC_BASE + 0x0318U)
#define	DBSC_DBTR7		(DBSC_BASE + 0x031CU)
#define	DBSC_DBTR8		(DBSC_BASE + 0x0320U)
#define	DBSC_DBTR9		(DBSC_BASE + 0x0324U)
#define	DBSC_DBTR10		(DBSC_BASE + 0x0328U)
#define	DBSC_DBTR11		(DBSC_BASE + 0x032CU)
#define	DBSC_DBTR12		(DBSC_BASE + 0x0330U)
#define	DBSC_DBTR13		(DBSC_BASE + 0x0334U)
#define	DBSC_DBTR14		(DBSC_BASE + 0x0338U)
#define	DBSC_DBTR15		(DBSC_BASE + 0x033CU)
#define	DBSC_DBTR16		(DBSC_BASE + 0x0340U)
#define	DBSC_DBTR17		(DBSC_BASE + 0x0344U)
#define	DBSC_DBTR18		(DBSC_BASE + 0x0348U)
#define	DBSC_DBTR19		(DBSC_BASE + 0x034CU)
#define	DBSC_DBTR20		(DBSC_BASE + 0x0350U)
#define	DBSC_DBTR21		(DBSC_BASE + 0x0354U)
#define	DBSC_DBTR22		(DBSC_BASE + 0x0358U)
#define	DBSC_DBTR23		(DBSC_BASE + 0x035CU)
#define	DBSC_DBBL		(DBSC_BASE + 0x0400U)
#define	DBSC_DBRFCNF1		(DBSC_BASE + 0x0414U)
#define	DBSC_DBRFCNF2		(DBSC_BASE + 0x0418U)
#define	DBSC_DBRFCNF3		(DBSC_BASE + 0x041CU)
#define	DBSC_DBTSPCNF		(DBSC_BASE + 0x0420U)
#define	DBSC_DBCALCNF		(DBSC_BASE + 0x0424U)
#define	DBSC_DBCALTR		(DBSC_BASE + 0x0428U)
#define	DBSC_DBRNK0		(DBSC_BASE + 0x0430U)
#define	DBSC_DBRNK1		(DBSC_BASE + 0x0434U)
#define	DBSC_DBRNK2		(DBSC_BASE + 0x0438U)
#define	DBSC_DBRNK3		(DBSC_BASE + 0x043CU)
#define	DBSC_DBRNK4		(DBSC_BASE + 0x0440U)
#define	DBSC_DBRNK5		(DBSC_BASE + 0x0444U)
#define	DBSC_DBRNK6		(DBSC_BASE + 0x0448U)
#define	DBSC_DBPDNCNF		(DBSC_BASE + 0x0450U)
#define	DBSC_DBODT0		(DBSC_BASE + 0x0460U)
#define	DBSC_DBODT1		(DBSC_BASE + 0x0464U)
#define	DBSC_DBODT2		(DBSC_BASE + 0x0468U)
#define	DBSC_DBODT3		(DBSC_BASE + 0x046CU)
#define	DBSC_DBODT4		(DBSC_BASE + 0x0470U)
#define	DBSC_DBODT5		(DBSC_BASE + 0x0474U)
#define	DBSC_DBODT6		(DBSC_BASE + 0x0478U)
#define	DBSC_DBODT7		(DBSC_BASE + 0x047CU)
#define	DBSC_DBMRRDR		(DBSC_BASE + 0x0480U)
#define	DBSC_DBTSPDR		(DBSC_BASE + 0x0484U)
#define	DBSC_DBADJ0		(DBSC_BASE + 0x0500U)
#define	DBSC_DBADJ2		(DBSC_BASE + 0x0508U)
#define	DBSC_DBDBICNT		(DBSC_BASE + 0x0518U)
#define	DBSC_DBDFIPMSTRCNF	(DBSC_BASE + 0x0520U)
#define	DBSC_DBDFIPMSTRSTAT	(DBSC_BASE + 0x0524U)
#define	DBSC_DBDFILPCNF		(DBSC_BASE + 0x0528U)
#define	DBSC_DBDFISTAT0		(DBSC_BASE + 0x0600U)
#define	DBSC_DBDFISTAT1		(DBSC_BASE + 0x0640U)
#define	DBSC_DBDFISTAT2		(DBSC_BASE + 0x0680U)
#define	DBSC_DBDFISTAT3		(DBSC_BASE + 0x06C0U)
#define	DBSC_DBDFICNT_0		(DBSC_BASE + 0x0604U)
#define	DBSC_DBDFICNT_1		(DBSC_BASE + 0x0644U)
#define	DBSC_DBDFICNT_2		(DBSC_BASE + 0x0684U)
#define	DBSC_DBDFICNT_3		(DBSC_BASE + 0x06C4U)
#define	DBSC_DBDFICNT(x)	(DBSC_BASE + 0x0604U + (uint32_t)((x) * 0x40U))
#define	DBSC_DBPDCNT00		(DBSC_BASE + 0x0610U)
#define	DBSC_DBPDCNT10		(DBSC_BASE + 0x0650U)
#define	DBSC_DBPDCNT20		(DBSC_BASE + 0x0690U)
#define	DBSC_DBPDCNT30		(DBSC_BASE + 0x06D0U)
#define	DBSC_DBPDCNT01		(DBSC_BASE + 0x0614U)
#define	DBSC_DBPDCNT11		(DBSC_BASE + 0x0654U)
#define	DBSC_DBPDCNT21		(DBSC_BASE + 0x0694U)
#define	DBSC_DBPDCNT31		(DBSC_BASE + 0x06D4U)
#define	DBSC_DBPDCNT02		(DBSC_BASE + 0x0618U)
#define	DBSC_DBPDCNT12		(DBSC_BASE + 0x0658U)
#define	DBSC_DBPDCNT22		(DBSC_BASE + 0x0698U)
#define	DBSC_DBPDCNT32		(DBSC_BASE + 0x06D8U)
#define	DBSC_DBPDCNT03		(DBSC_BASE + 0x061CU)
#define	DBSC_DBPDCNT13		(DBSC_BASE + 0x065CU)
#define	DBSC_DBPDCNT23		(DBSC_BASE + 0x069CU)
#define	DBSC_DBPDCNT33		(DBSC_BASE + 0x06DCU)
#define	DBSC_DBPDLK_0		(DBSC_BASE + 0x0620U)
#define	DBSC_DBPDLK_1		(DBSC_BASE + 0x0660U)
#define	DBSC_DBPDLK_2		(DBSC_BASE + 0x06A0U)
#define	DBSC_DBPDLK_3		(DBSC_BASE + 0x06E0U)
#define	DBSC_DBPDRGA_0		(DBSC_BASE + 0x0624U)
#define	DBSC_DBPDRGA_1		(DBSC_BASE + 0x0664U)
#define	DBSC_DBPDRGA_2		(DBSC_BASE + 0x06A4U)
#define	DBSC_DBPDRGA_3		(DBSC_BASE + 0x06E4U)
#define	DBSC_DBPDRGD_0		(DBSC_BASE + 0x0628U)
#define	DBSC_DBPDRGD_1		(DBSC_BASE + 0x0668U)
#define	DBSC_DBPDRGD_2		(DBSC_BASE + 0x06A8U)
#define	DBSC_DBPDRGD_3		(DBSC_BASE + 0x06E8U)
#define	DBSC_DBBUS0CNF0		(DBSC_BASE + 0x0800U)
#define	DBSC_DBBUS0CNF1		(DBSC_BASE + 0x0804U)
#define	DBSC_DBCAM0CNF0		(DBSC_BASE + 0x0900U)
#define	DBSC_DBCAM0CNF1		(DBSC_BASE + 0x0904U)
#define	DBSC_DBCAM0CNF2		(DBSC_BASE + 0x0908U)
#define	DBSC_DBCAM0CNF3		(DBSC_BASE + 0x090CU)
#define	DBSC_DBBCAMSWAP		(DBSC_BASE + 0x09F0U)
#define	DBSC_DBBCAMDIS		(DBSC_BASE + 0x09FCU)
#define	DBSC_DBSCHCNT0		(DBSC_BASE + 0x1000U)
#define	DBSC_DBSCHCNT1		(DBSC_BASE + 0x1004U)
#define	DBSC_DBSCHSZ0		(DBSC_BASE + 0x1010U)
#define	DBSC_DBSCHRW0		(DBSC_BASE + 0x1020U)
#define	DBSC_DBSCHRW1		(DBSC_BASE + 0x1024U)
#define	DBSC_DBSCHQOS_0_0	(DBSC_BASE + 0x1030U)
#define	DBSC_DBSCHQOS_1_0	(DBSC_BASE + 0x1040U)
#define	DBSC_DBSCHQOS_2_0	(DBSC_BASE + 0x1050U)
#define	DBSC_DBSCHQOS_3_0	(DBSC_BASE + 0x1060U)
#define	DBSC_DBSCHQOS_4_0	(DBSC_BASE + 0x1070U)
#define	DBSC_DBSCHQOS_5_0	(DBSC_BASE + 0x1080U)
#define	DBSC_DBSCHQOS_6_0	(DBSC_BASE + 0x1090U)
#define	DBSC_DBSCHQOS_7_0	(DBSC_BASE + 0x10A0U)
#define	DBSC_DBSCHQOS_8_0	(DBSC_BASE + 0x10B0U)
#define	DBSC_DBSCHQOS_9_0	(DBSC_BASE + 0x10C0U)
#define	DBSC_DBSCHQOS_10_0	(DBSC_BASE + 0x10D0U)
#define	DBSC_DBSCHQOS_11_0	(DBSC_BASE + 0x10E0U)
#define	DBSC_DBSCHQOS_12_0	(DBSC_BASE + 0x10F0U)
#define	DBSC_DBSCHQOS_13_0	(DBSC_BASE + 0x1100U)
#define	DBSC_DBSCHQOS_14_0	(DBSC_BASE + 0x1110U)
#define	DBSC_DBSCHQOS_15_0	(DBSC_BASE + 0x1120U)
#define	DBSC_DBSCHQOS_0_1	(DBSC_BASE + 0x1034U)
#define	DBSC_DBSCHQOS_1_1	(DBSC_BASE + 0x1044U)
#define	DBSC_DBSCHQOS_2_1	(DBSC_BASE + 0x1054U)
#define	DBSC_DBSCHQOS_3_1	(DBSC_BASE + 0x1064U)
#define	DBSC_DBSCHQOS_4_1	(DBSC_BASE + 0x1074U)
#define	DBSC_DBSCHQOS_5_1	(DBSC_BASE + 0x1084U)
#define	DBSC_DBSCHQOS_6_1	(DBSC_BASE + 0x1094U)
#define	DBSC_DBSCHQOS_7_1	(DBSC_BASE + 0x10A4U)
#define	DBSC_DBSCHQOS_8_1	(DBSC_BASE + 0x10B4U)
#define	DBSC_DBSCHQOS_9_1	(DBSC_BASE + 0x10C4U)
#define	DBSC_DBSCHQOS_10_1	(DBSC_BASE + 0x10D4U)
#define	DBSC_DBSCHQOS_11_1	(DBSC_BASE + 0x10E4U)
#define	DBSC_DBSCHQOS_12_1	(DBSC_BASE + 0x10F4U)
#define	DBSC_DBSCHQOS_13_1	(DBSC_BASE + 0x1104U)
#define	DBSC_DBSCHQOS_14_1	(DBSC_BASE + 0x1114U)
#define	DBSC_DBSCHQOS_15_1	(DBSC_BASE + 0x1124U)
#define	DBSC_DBSCHQOS_0_2	(DBSC_BASE + 0x1038U)
#define	DBSC_DBSCHQOS_1_2	(DBSC_BASE + 0x1048U)
#define	DBSC_DBSCHQOS_2_2	(DBSC_BASE + 0x1058U)
#define	DBSC_DBSCHQOS_3_2	(DBSC_BASE + 0x1068U)
#define	DBSC_DBSCHQOS_4_2	(DBSC_BASE + 0x1078U)
#define	DBSC_DBSCHQOS_5_2	(DBSC_BASE + 0x1088U)
#define	DBSC_DBSCHQOS_6_2	(DBSC_BASE + 0x1098U)
#define	DBSC_DBSCHQOS_7_2	(DBSC_BASE + 0x10A8U)
#define	DBSC_DBSCHQOS_8_2	(DBSC_BASE + 0x10B8U)
#define	DBSC_DBSCHQOS_9_2	(DBSC_BASE + 0x10C8U)
#define	DBSC_DBSCHQOS_10_2	(DBSC_BASE + 0x10D8U)
#define	DBSC_DBSCHQOS_11_2	(DBSC_BASE + 0x10E8U)
#define	DBSC_DBSCHQOS_12_2	(DBSC_BASE + 0x10F8U)
#define	DBSC_DBSCHQOS_13_2	(DBSC_BASE + 0x1108U)
#define	DBSC_DBSCHQOS_14_2	(DBSC_BASE + 0x1118U)
#define	DBSC_DBSCHQOS_15_2	(DBSC_BASE + 0x1128U)
#define	DBSC_DBSCHQOS_0_3	(DBSC_BASE + 0x103CU)
#define	DBSC_DBSCHQOS_1_3	(DBSC_BASE + 0x104CU)
#define	DBSC_DBSCHQOS_2_3	(DBSC_BASE + 0x105CU)
#define	DBSC_DBSCHQOS_3_3	(DBSC_BASE + 0x106CU)
#define	DBSC_DBSCHQOS_4_3	(DBSC_BASE + 0x107CU)
#define	DBSC_DBSCHQOS_5_3	(DBSC_BASE + 0x108CU)
#define	DBSC_DBSCHQOS_6_3	(DBSC_BASE + 0x109CU)
#define	DBSC_DBSCHQOS_7_3	(DBSC_BASE + 0x10ACU)
#define	DBSC_DBSCHQOS_8_3	(DBSC_BASE + 0x10BCU)
#define	DBSC_DBSCHQOS_9_3	(DBSC_BASE + 0x10CCU)
#define	DBSC_DBSCHQOS_10_3	(DBSC_BASE + 0x10DCU)
#define	DBSC_DBSCHQOS_11_3	(DBSC_BASE + 0x10ECU)
#define	DBSC_DBSCHQOS_12_3	(DBSC_BASE + 0x10FCU)
#define	DBSC_DBSCHQOS_13_3	(DBSC_BASE + 0x110CU)
#define	DBSC_DBSCHQOS_14_3	(DBSC_BASE + 0x111CU)
#define	DBSC_DBSCHQOS_15_3	(DBSC_BASE + 0x112CU)
#define	DBSC_DBSCTR0		(DBSC_BASE + 0x1700U)
#define	DBSC_DBSCTR1		(DBSC_BASE + 0x1708U)
#define	DBSC_DBSCHRW2		(DBSC_BASE + 0x170CU)
#define	DBSC_DBMRRDR0		(DBSC_BASE + 0x1800U)
#define	DBSC_DBMRRDR1		(DBSC_BASE + 0x1804U)
#define	DBSC_DBMRRDR2		(DBSC_BASE + 0x1808U)
#define	DBSC_DBMRRDR3		(DBSC_BASE + 0x180CU)
#define	DBSC_DBDTMP0		(DBSC_BASE + 0x1820U)
#define	DBSC_DBDTMP1		(DBSC_BASE + 0x1824U)
#define	DBSC_DBDTMP2		(DBSC_BASE + 0x1828U)
#define	DBSC_DBDTMP3		(DBSC_BASE + 0x182CU)
#define	DBSC_DBDQSOSC00		(DBSC_BASE + 0x1840U)
#define	DBSC_DBDQSOSC10		(DBSC_BASE + 0x1848U)
#define	DBSC_DBDQSOSC20		(DBSC_BASE + 0x1850U)
#define	DBSC_DBDQSOSC30		(DBSC_BASE + 0x1858U)
#define	DBSC_DBDQSOSC01		(DBSC_BASE + 0x1844U)
#define	DBSC_DBDQSOSC11		(DBSC_BASE + 0x184CU)
#define	DBSC_DBDQSOSC21		(DBSC_BASE + 0x1854U)
#define	DBSC_DBDQSOSC31		(DBSC_BASE + 0x185CU)
#define	DBSC_DBOSCTHH00		(DBSC_BASE + 0x1880U)
#define	DBSC_DBOSCTHH10		(DBSC_BASE + 0x1888U)
#define	DBSC_DBOSCTHH20		(DBSC_BASE + 0x1890U)
#define	DBSC_DBOSCTHH30		(DBSC_BASE + 0x1898U)
#define	DBSC_DBOSCTHH01		(DBSC_BASE + 0x1884U)
#define	DBSC_DBOSCTHH11		(DBSC_BASE + 0x188CU)
#define	DBSC_DBOSCTHH21		(DBSC_BASE + 0x1894U)
#define	DBSC_DBOSCTHH31		(DBSC_BASE + 0x189CU)
#define	DBSC_DBOSCTHL00		(DBSC_BASE + 0x18C0U)
#define	DBSC_DBOSCTHL10		(DBSC_BASE + 0x18C8U)
#define	DBSC_DBOSCTHL20		(DBSC_BASE + 0x18D0U)
#define	DBSC_DBOSCTHL30		(DBSC_BASE + 0x18D8U)
#define	DBSC_DBOSCTHL01		(DBSC_BASE + 0x18C4U)
#define	DBSC_DBOSCTHL11		(DBSC_BASE + 0x18CCU)
#define	DBSC_DBOSCTHL21		(DBSC_BASE + 0x18D4U)
#define	DBSC_DBOSCTHL31		(DBSC_BASE + 0x18DCU)
#define	DBSC_DBFSFCONF0		(DBSC_BASE + 0x2000U)

#define	DBSC_DBSTATE0		(DBSC_BASE + 0x0108U)
#define	DBSC_PLL_LOCK(x)	(DBSC_BASE + 0x4054U \
					   + (uint32_t)((x) * 0x0100U))
#define	DBSC_PLL_LOCK_0		(DBSC_BASE + 0x4054U)
#define	DBSC_PLL_LOCK_1		(DBSC_BASE + 0x4154U)
#define	DBSC_PLL_LOCK_2		(DBSC_BASE + 0x4254U)
#define	DBSC_PLL_LOCK_3		(DBSC_BASE + 0x4354U)
#if 1
#define DBSC_DBPDCNT0_0		DBSC_DBPDCNT00
#define DBSC_DBPDCNT0_1		DBSC_DBPDCNT10
#define DBSC_DBPDCNT0_2		DBSC_DBPDCNT20
#define DBSC_DBPDCNT0_3		DBSC_DBPDCNT30
#define DBSC_DBPDCNT_0		DBSC_DBPDCNT03
#define DBSC_DBPDCNT_1		DBSC_DBPDCNT13
#define DBSC_DBPDCNT_2		DBSC_DBPDCNT23
#define DBSC_DBPDCNT_3		DBSC_DBPDCNT33
#define DBSC_INITCOMP_0		DBSC_DBDFISTAT0
#define DBSC_INITCOMP_1		DBSC_DBDFISTAT1
#define DBSC_INITCOMP_2		DBSC_DBDFISTAT2
#define DBSC_INITCOMP_3		DBSC_DBDFISTAT3
#define DBSC_DFI_FREQ_0		DBSC_DBPDCNT01
#define DBSC_DFI_FREQ_1		DBSC_DBPDCNT11
#define DBSC_DFI_FREQ_2		DBSC_DBPDCNT21
#define DBSC_DFI_FREQ_3		DBSC_DBPDCNT31
#define DBSC_FREQ_CHG_ACK_0	DBSC_DBPDCNT02
#define DBSC_FREQ_CHG_ACK_1	DBSC_DBPDCNT12
#define DBSC_FREQ_CHG_ACK_2	DBSC_DBPDCNT22
#define DBSC_FREQ_CHG_ACK_3	DBSC_DBPDCNT32
#endif


#define	DDCG_LPDDR3		(0x0008U)
#define	DDCG_LPDDR4		(0x000AU)

#define	DBMEMCONF_DENS_2N	(0x00000000U)
#define	DBMEMCONF_DENS_2N3	(0x40000000U)
#define	DBMEMCONF_AWRW_12	(0x0C000000U)
#define	DBMEMCONF_AWRW_13	(0x0D000000U)
#define	DBMEMCONF_AWRW_14	(0x0E000000U)
#define	DBMEMCONF_AWRW_15	(0x0F000000U)
#define	DBMEMCONF_AWRW_16	(0x10000000U)
#define	DBMEMCONF_AWBK		(0x00030000U)
#define	DBMEMCONF_AWCL_9	(0x00000900U)
#define	DBMEMCONF_AWCL_10	(0x00000A00U)
#define	DBMEMCONF_AWCL_11	(0x00000B00U)
#define	DBMEMCONF_DW_10		(0x00000002U)

#define	DBPHYCONF0_PHY_DFI	(0x01U)

#define	DBTR0_CL(x)		((uint32_t)(x) << 0U)
#define	DBTR1_CWL(x)		((uint32_t)(x) << 0U)
#define	DBTR2_AL_0		(0x00U)
#define	DBTR3_TRCD(x)		((uint32_t)(x) << 0U)
#define	DBTR4_TRPA(x)		((uint32_t)(x) << 16U)
#define	DBTR4_TRP(x)		((uint32_t)(x) << 0U)
#define	DBTR5_TRC(x)		((uint32_t)(x) << 0U)
#define	DBTR6_TRAS(x)		((uint32_t)(x) << 0U)
#define	DBTR7_TRRD_S(x)		((uint32_t)(x) << 16U)
#define	DBTR7_TRRD(x)		((uint32_t)(x) << 0U)
#define	DBTR8_TFAW(x)		((uint32_t)(x) << 0U)
#define	DBTR9_TRDPR(x)		((uint32_t)(x) << 0U)
#define	DBTR10_TWR(x)		((uint32_t)(x) << 0U)
#define	DBTR11_TWRTWR(x)	((uint32_t)(x) << 0U)
#define	DBTR12_TWRRD_S(x)	((uint32_t)(x) << 16U)
#define	DBTR12_TWRRD(x)		((uint32_t)(x) << 0U)
#define	DBTR13_TRFCPB(x)	((uint32_t)(x) << 16U)
#define	DBTR13_TRFC_TRFCAB(x)	((uint32_t)(x) << 0U)
#define	DBTR14_TCKEHDLL(x)	((uint32_t)(x) << 16U)
#define	DBTR14_TCKEH(x)		((uint32_t)(x) << 0U)
#define	DBTR15_TCKESR(x)	((uint32_t)(x) << 16U)
#define	DBTR15_TCKEL(x)		((uint32_t)(x) << 0U)
#define	DBTR16_DQIENLTNCY(x)	((uint32_t)(x) << 24U)
#define	DBTR16_DQL(x)		((uint32_t)(x) << 16U)
#define	DBTR16_DQENLTNCY(x)	((uint32_t)(x) << 8U)
#define	DBTR16_WDQL(x)		((uint32_t)(x) << 0U)
#define	DBTR17_TMODRD(x)	((uint32_t)(x) << 24U)
#define	DBTR17_TMOD(x)		((uint32_t)(x) << 16U)
#define	DBTR17_TRDMR(x)		((uint32_t)(x) << 0U)
#define	DBTR18_RODTL(x)		((uint32_t)(x) << 24U)
#define	DBTR18_RODTA(x)		((uint32_t)(x) << 16U)
#define	DBTR18_WODTL(x)		((uint32_t)(x) << 8U)
#define	DBTR18_WODTA(x)		((uint32_t)(x) << 0U)
#define	DBTR19_TZQCL(x)		((uint32_t)(x) << 16U)
#define	DBTR19_TZQCS(x)		((uint32_t)(x) << 0U)
#define	DBTR20_TXSDLL(x)	((uint32_t)(x) << 16U)
#define	DBTR20_TXS(x)		((uint32_t)(x) << 0U)
#define DBTR21_TCCD_S(x)	((uint32_t)(x) << 16U)
#define DBTR21_TCCD(x)		((uint32_t)(x) << 0U)
#define DBTR22_TZQCAL(x)	((uint32_t)(x) << 16U)
#define	DBTR22_TZQLAT(x)	((uint32_t)(x) << 0U)

#define	DBBL_BL8		((uint32_t)0x00)
#define	DBBL_BL16		((uint32_t)0x02)

#define	DBRNK1_RKRP(x)		((uint32_t)(x) << 16U)
#define DBRNK1_RKRF(x)		((uint32_t)(x) << 0U)
#define	DBRNK2_RKRR7(x)		((uint32_t)(x) << 28U)
#define	DBRNK2_RKRR6(x)		((uint32_t)(x) << 24U)
#define	DBRNK2_RKRR5(x)		((uint32_t)(x) << 20U)
#define	DBRNK2_RKRR4(x)		((uint32_t)(x) << 16U)
#define	DBRNK2_RKRR3(x)		((uint32_t)(x) << 12U)
#define	DBRNK2_RKRR2(x)		((uint32_t)(x) << 8U)
#define	DBRNK2_RKRR1(x)		((uint32_t)(x) << 4U)
#define	DBRNK2_RKRR0(x)		((uint32_t)(x) << 0U)
#define	DBRNK3_RKRW7(x)		((uint32_t)(x) << 28U)
#define	DBRNK3_RKRW6(x)		((uint32_t)(x) << 24U)
#define	DBRNK3_RKRW5(x)		((uint32_t)(x) << 20U)
#define	DBRNK3_RKRW4(x)		((uint32_t)(x) << 16U)
#define	DBRNK3_RKRW3(x)		((uint32_t)(x) << 12U)
#define	DBRNK3_RKRW2(x)		((uint32_t)(x) << 8U)
#define	DBRNK3_RKRW1(x)		((uint32_t)(x) << 4U)
#define	DBRNK3_RKRW0(x)		((uint32_t)(x) << 0U)
#define	DBRNK4_RKWR7(x)		((uint32_t)(x) << 28U)
#define	DBRNK4_RKWR6(x)		((uint32_t)(x) << 24U)
#define	DBRNK4_RKWR5(x)		((uint32_t)(x) << 20U)
#define	DBRNK4_RKWR4(x)		((uint32_t)(x) << 16U)
#define	DBRNK4_RKWR3(x)		((uint32_t)(x) << 12U)
#define	DBRNK4_RKWR2(x)		((uint32_t)(x) << 8U)
#define	DBRNK4_RKWR1(x)		((uint32_t)(x) << 4U)
#define	DBRNK4_RKWR0(x)		((uint32_t)(x) << 0U)
#define	DBRNK5_RKWW7(x)		((uint32_t)(x) << 28U)
#define	DBRNK5_RKWW6(x)		((uint32_t)(x) << 24U)
#define	DBRNK5_RKWW5(x)		((uint32_t)(x) << 20U)
#define	DBRNK5_RKWW4(x)		((uint32_t)(x) << 16U)
#define	DBRNK5_RKWW3(x)		((uint32_t)(x) << 12U)
#define	DBRNK5_RKWW2(x)		((uint32_t)(x) << 8U)
#define	DBRNK5_RKWW1(x)		((uint32_t)(x) << 4U)
#define	DBRNK5_RKWW0(x)		((uint32_t)(x) << 0U)
#define	DBRNK6_REFMODE_ALL	(0x00U)
#define	DBRNK6_REFMODE_RANK	(0x00U)

#define	DBADJ0_CAMODE_1_CLK	(0x00U)
#define	DBADJ0_CAMODE_2_CLK	(0x01U)

#define	DBPDCNT0_DFIRESETN	(0x01U)
#define	DBPDCNT3_CNTREG(x)	((uint32_t)(x) << 16U)
#define	DBPDCNT3_DLLRST		(0xCF01U)

#define	DBCAM0CNF1_WBKWAIT(x)		((uint32_t)(x) << 16U)
#define	DBCAM0CNF1_SWPINPRI3(x)		((uint32_t)(x) << 12U)
#define	DBCAM0CNF1_SWPINPRI2(x)		((uint32_t)(x) << 8U)
#define	DBCAM0CNF1_SWPINPRI1(x)		((uint32_t)(x) << 4U)
#define	DBCAM0CNF1_SWPINPRI1F(x)	((uint32_t)(x) << 0U)

#define	DBCAM0CNF2_FILLUNIT_64		(0x0000U)
#define	DBCAM0CNF2_FILLUNIT_128		(0x0100U)
#define	DBCAM0CNF2_FILLUNIT_256		(0x0200U)
#define	DBCAM0CNF2_FCDIRTYMAX(x)	((uint32_t)(x) << 4U)
#define	DBCAM0CNF2_FCDIRTYMIN(x)	((uint32_t)(x) << 0U)

#define DBCAM0CNF3_RDFULL(x)		((uint32_t)(x) << 0U)

#define	DBSCHCNT0_SCWBPRI_3		((uint32_t)8U << 24U)
#define	DBSCHCNT0_SCWBPRI_2		((uint32_t)4U << 24U)
#define	DBSCHCNT0_SCWBPRI_1		((uint32_t)2U << 24U)
#define	DBSCHCNT0_SCWBPRI_0		((uint32_t)1U << 24U)
#define	DBSCHCNT0_SCQTZEN_3		((uint32_t)1U << 19U)
#define	DBSCHCNT0_SCQTZEN_2		((uint32_t)1U << 18U)
#define	DBSCHCNT0_SCQTZEN_1		((uint32_t)1U << 17U)
#define	DBSCHCNT0_SCQTZEN_0		((uint32_t)1U << 16U)
#define	DBSCHCNT0_SCSZEN_EN		(0x20U)
#define	DBSCHCNT0_SCSZEN_DIS		(0x00U)
#define	DBSCHCNT0_SCBAEN_EN		(0x10U)
#define	DBSCHCNT0_SCBAEN_DIS		(0x00U)
#define	DBSCHCNT0_SCPGEN_EN		(0x04U)
#define	DBSCHCNT0_SCPGEN_DIS		(0x00U)
#define	DBSCHCNT0_SCRWEN_EN		(0x02U)
#define	DBSCHCNT0_SCRWEN_DIS		(0x00U)
#define	DBSCHCNT0_SCQOSEN_EN		(0x01U)
#define	DBSCHCNT0_SCQOSEN_DIS		(0x00U)

#define	DBSCHCNT1_SCHCH3(x)		((uint32_t)(x) << 12U)
#define	DBSCHCNT1_SCHCH2(x)		((uint32_t)(x) << 8U)
#define	DBSCHCNT1_SCHCH1(x)		((uint32_t)(x) << 4U)
#define	DBSCHCNT1_SCHCH0(x)		((uint32_t)(x) << 0U)

#define	DBSCHSZ0_SZTH(x)		((uint32_t)(x) << 0U)

#define	DBSCHRW1_SCBADEC(x)		((uint32_t)(x) << 16U)
#define	DBSCHRW1_RWWTPERI(x)		((uint32_t)(x) << 0U)

#define	DBSYSCONF1_FREQRATIO_1_4	(0x02U)

#define	DBPDLK_KEYWORD			(0xA55AU)

#define	DBDFICNT_DFIBYTEDIS(x)		((uint32_t)(x) << 16U)
#define	DBDFICNT_DFICLKDIS(x)		((uint32_t)(x) << 8U)
#define	DBDFICNT_DFIFREQRATIO_1_1	(0x00U)
#define	DBDFICNT_DFIFREQRATIO_1_2	(0x10U)
#define	DBDFICNT_DFIFREQRATIO_1_4	(0x20U)
#define	DBDFICNT_DFIINITSTART		(0x01U)

#define	DBCMD_OPC_WAIT			((uint32_t)0x00U << 24U)
#define	DBCMD_OPC_RESET			((uint32_t)0x01U << 24U)
#define	DBCMD_OPC_LOCK			((uint32_t)0x02U << 24U)
#define	DBCMD_OPC_PREA			((uint32_t)0x04U << 24U)
#define	DBCMD_OPC_REF			((uint32_t)0x05U << 24U)
#define	DBCMD_OPC_ZQC			((uint32_t)0x06U << 24U)
#define	DBCMD_OPC_PD			((uint32_t)0x08U << 24U)
#define	DBCMD_OPC_DPD			((uint32_t)0x09U << 24U)
#define	DBCMD_OPC_SR			((uint32_t)0x0AU << 24U)
#define	DBCMD_OPC_MPC			((uint32_t)0x0DU << 24U)
#define	DBCMD_OPC_MRW			((uint32_t)0x0EU << 24U)
#define	DBCMD_OPC_MRR			((uint32_t)0x0FU << 24U)
#define	DBCMD_CH(x)			((uint32_t)(x) << 20U)
#define	DBCMD_CH_ALL			((uint32_t)8U << 20U)
#define	DBCMD_RANK(x)			((uint32_t)(x) << 16U)
#define	DBCMD_RANK_ALL			((uint32_t)4U << 16U)
#define	DBCMD_ARG_RESET_NEGATE		(0x0000U)
#define	DBCMD_ARG_RESET_ASSERT		(0x0001U)
#define	DBCMD_ARG_LOCK_DISABLED		(0x0000U)
#define	DBCMD_ARG_LOCK_DISABLING	(0x0001U)
#define	DBCMD_ARG_PREA			(0x0010U)
#define	DBCMD_ARG_REF			(0x0010U)
#define	DBCMD_ARG_ZQCS			(0x0000U)
#define	DBCMD_ARG_ZQCL			(0x0001U)
#define	DBCMD_ARG_ENTER			(0x0000U)
#define	DBCMD_ARG_EXIT			(0x0001U)
#define	DBCMD_ARG_MPC(op)		((uint32_t)(op) << 0U)
#define	DBCMD_ARG_MRW(ma, op)		(((uint32_t)(ma) << 8U) | \
					 ((uint32_t)(op) << 0U))
#define	DBCMD_ARG_MRR(ma)		((uint32_t)(ma) << 8U)

#define	DBDFIPMSTRCNF_EN		(0x01U)
#define	DBDFIPMSTRCNF_DIS		(0x00U)

#define	DBBUS0CNF0_EXSTA(x)		((uint32_t)(x) << 24U)
#define	DBBUS0CNF0_EN			(0x00010000U)
#define	DBBUS0CNF0_DIS			(0x00000000U)
#define	DBBUS0CNF0_ROUND_ROBIN		(0x00U)
#define	DBBUS0CNF0_READ_HIGH_PRIORITY	(0x01U)
#define	DBBUS0CNF0_WRITE_HIGH_PRIORITY	(0x02U)

#define	DBRFCNF1_REFPMAX(x)		((uint32_t)(x) << 16U)
#define	DBRFCNF1_REFINT(x)		((uint32_t)(x) << 0U)

#define	DBRFCNF2_REFPMIN		((uint32_t)0x0001U << 16U)
#define	DBRFCNF2_REFINT_1_1		(0x00U)
#define	DBRFCNF2_REFINT_1_2		(0x01U)

#define	DBRFEN_ARFEN_START		(0x01U)
#define	DBRFEN_ARFEN_STOP		(0x00U)

#define DBACEN_ACCEN_EN			(0x01U)
#define DBACEN_ACCEN_DIS		(0x00U)


static void SoftDelay(uint32_t loop);
static void pvt_dbsc_regset(void);
static void pvt_manual_update(uint32_t flag);
static void pvt_freq_change_ack(uintptr_t phychno, uint32_t freq_flag);
static uint32_t pvt_pi_training_go_all(void);
static uint32_t pvt_pi_training(void);
static void WaitDBCMD(void);
static void DDRPHY_PVT_OVERWRITE(void);
static void change_lpddr4_en(uintptr_t phychno, uint32_t mode);
static void copy_dqdelay(uintptr_t phychno, uint32_t s_byte, uint32_t d_byte);

static void InitDDR1600_4GByte_mod_0804(void);

#ifdef DEBUG_SDRA_REG_PRINT
static void print_dbsc4_reg(uint32_t address);
static void print_phy_reg(uint32_t address);
static void print_regs(void);
#endif /* DEBUG_SDRA_REG_PRINT */


static const uint16_t	dqdelay_tbl[4][38] = {
	       {(uint16_t)PHY_CLK_WRDQ1_SLAVE_DELAY_0,
		(uint16_t)PHY_CLK_WRDQ0_SLAVE_DELAY_0,
		(uint16_t)PHY_CLK_WRDQ3_SLAVE_DELAY_0,
		(uint16_t)PHY_CLK_WRDQ2_SLAVE_DELAY_0,
		(uint16_t)PHY_CLK_WRDQ5_SLAVE_DELAY_0,
		(uint16_t)PHY_CLK_WRDQ4_SLAVE_DELAY_0,
		(uint16_t)PHY_CLK_WRDQ7_SLAVE_DELAY_0,
		(uint16_t)PHY_CLK_WRDQ6_SLAVE_DELAY_0,
		(uint16_t)PHY_CLK_WRDM_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQ1_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQ0_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQ3_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQ2_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQ5_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQ4_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQ7_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQ6_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQS_DQ0_FALL_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQS_DQ0_RISE_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQS_DQ1_FALL_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQS_DQ1_RISE_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQS_DQ2_FALL_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQS_DQ2_RISE_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQS_DQ3_FALL_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQS_DQ3_RISE_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQS_DQ4_FALL_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQS_DQ4_RISE_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQS_DQ5_FALL_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQS_DQ5_RISE_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQS_DQ6_FALL_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQS_DQ6_RISE_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQS_DQ7_FALL_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQS_DQ7_RISE_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQS_DM_FALL_SLAVE_DELAY_0,
		(uint16_t)PHY_RDDQS_DM_RISE_SLAVE_DELAY_0,
		(uint16_t)PHY_WRITE_PATH_LAT_ADD_0,
		(uint16_t)PHY_RDDQS_LATENCY_ADJUST_0,
		(uint16_t)PHY_RDDQS_GATE_SLAVE_DELAY_0},
	       {(uint16_t)PHY_CLK_WRDQ1_SLAVE_DELAY_1,
		(uint16_t)PHY_CLK_WRDQ0_SLAVE_DELAY_1,
		(uint16_t)PHY_CLK_WRDQ3_SLAVE_DELAY_1,
		(uint16_t)PHY_CLK_WRDQ2_SLAVE_DELAY_1,
		(uint16_t)PHY_CLK_WRDQ5_SLAVE_DELAY_1,
		(uint16_t)PHY_CLK_WRDQ4_SLAVE_DELAY_1,
		(uint16_t)PHY_CLK_WRDQ7_SLAVE_DELAY_1,
		(uint16_t)PHY_CLK_WRDQ6_SLAVE_DELAY_1,
		(uint16_t)PHY_CLK_WRDM_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQ1_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQ0_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQ3_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQ2_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQ5_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQ4_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQ7_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQ6_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQS_DQ0_FALL_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQS_DQ0_RISE_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQS_DQ1_FALL_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQS_DQ1_RISE_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQS_DQ2_FALL_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQS_DQ2_RISE_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQS_DQ3_FALL_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQS_DQ3_RISE_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQS_DQ4_FALL_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQS_DQ4_RISE_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQS_DQ5_FALL_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQS_DQ5_RISE_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQS_DQ6_FALL_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQS_DQ6_RISE_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQS_DQ7_FALL_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQS_DQ7_RISE_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQS_DM_FALL_SLAVE_DELAY_1,
		(uint16_t)PHY_RDDQS_DM_RISE_SLAVE_DELAY_1,
		(uint16_t)PHY_WRITE_PATH_LAT_ADD_1,
		(uint16_t)PHY_RDDQS_LATENCY_ADJUST_1,
		(uint16_t)PHY_RDDQS_GATE_SLAVE_DELAY_1},
	       {(uint16_t)PHY_CLK_WRDQ1_SLAVE_DELAY_2,
		(uint16_t)PHY_CLK_WRDQ0_SLAVE_DELAY_2,
		(uint16_t)PHY_CLK_WRDQ3_SLAVE_DELAY_2,
		(uint16_t)PHY_CLK_WRDQ2_SLAVE_DELAY_2,
		(uint16_t)PHY_CLK_WRDQ5_SLAVE_DELAY_2,
		(uint16_t)PHY_CLK_WRDQ4_SLAVE_DELAY_2,
		(uint16_t)PHY_CLK_WRDQ7_SLAVE_DELAY_2,
		(uint16_t)PHY_CLK_WRDQ6_SLAVE_DELAY_2,
		(uint16_t)PHY_CLK_WRDM_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQ1_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQ0_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQ3_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQ2_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQ5_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQ4_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQ7_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQ6_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQS_DQ0_FALL_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQS_DQ0_RISE_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQS_DQ1_FALL_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQS_DQ1_RISE_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQS_DQ2_FALL_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQS_DQ2_RISE_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQS_DQ3_FALL_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQS_DQ3_RISE_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQS_DQ4_FALL_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQS_DQ4_RISE_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQS_DQ5_FALL_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQS_DQ5_RISE_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQS_DQ6_FALL_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQS_DQ6_RISE_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQS_DQ7_FALL_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQS_DQ7_RISE_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQS_DM_FALL_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQS_DM_RISE_SLAVE_DELAY_2,
		(uint16_t)PHY_WRITE_PATH_LAT_ADD_2,
		(uint16_t)PHY_RDDQS_LATENCY_ADJUST_2,
		(uint16_t)PHY_RDDQS_GATE_SLAVE_DELAY_2},
	       {(uint16_t)PHY_CLK_WRDQ1_SLAVE_DELAY_2,
		(uint16_t)PHY_CLK_WRDQ0_SLAVE_DELAY_2,
		(uint16_t)PHY_CLK_WRDQ3_SLAVE_DELAY_2,
		(uint16_t)PHY_CLK_WRDQ2_SLAVE_DELAY_2,
		(uint16_t)PHY_CLK_WRDQ5_SLAVE_DELAY_2,
		(uint16_t)PHY_CLK_WRDQ4_SLAVE_DELAY_2,
		(uint16_t)PHY_CLK_WRDQ7_SLAVE_DELAY_2,
		(uint16_t)PHY_CLK_WRDQ6_SLAVE_DELAY_2,
		(uint16_t)PHY_CLK_WRDM_SLAVE_DELAY_2,
		(uint16_t)PHY_RDDQ1_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQ0_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQ3_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQ2_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQ5_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQ4_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQ7_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQ6_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQS_DQ0_FALL_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQS_DQ0_RISE_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQS_DQ1_FALL_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQS_DQ1_RISE_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQS_DQ2_FALL_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQS_DQ2_RISE_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQS_DQ3_FALL_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQS_DQ3_RISE_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQS_DQ4_FALL_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQS_DQ4_RISE_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQS_DQ5_FALL_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQS_DQ5_RISE_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQS_DQ6_FALL_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQS_DQ6_RISE_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQS_DQ7_FALL_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQS_DQ7_RISE_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQS_DM_FALL_SLAVE_DELAY_3,
		(uint16_t)PHY_RDDQS_DM_RISE_SLAVE_DELAY_3,
		(uint16_t)PHY_WRITE_PATH_LAT_ADD_3,
		(uint16_t)PHY_RDDQS_LATENCY_ADJUST_3,
		(uint16_t)PHY_RDDQS_GATE_SLAVE_DELAY_3}
};


static void SoftDelay(uint32_t loop)
{
	volatile uint32_t i;
	for (i = 0U; i < loop; i++) {
	}
}


void bl2_sdram_init(void)
{
	uint32_t md;

	md = (mmio_read_32(RST_MODEMR));
	if ((uint32_t)(md & MODEMR_DDR_CLOCK_MASK) == MODEMR_DDR_CLOCK_3200) {
		InitDDR1600_4GByte_mod_0804(); /* MD19=0,MD17=0 : DDR3200 */
	} else {
		InitDDR1600_4GByte_mod_0804(); /* MD19=1,MD17=1 : DDR1600 */
	}

#ifdef DEBUG_SDRA_REG_PRINT
	print_regs(); /* print DBSC4 regsiters */
#endif /* DEBUG_SDRA_REG_PRINT */
}


static void InitDDR1600_4GByte_mod_0804(void)
{
	uint32_t mode = 0U;
	uint32_t i;
	uint32_t j;
	uint32_t dataL;
	uint32_t reset_flag;
	uintptr_t phychno;
	uint32_t phytrainingok;

	uint32_t freq = 0x2fU;
	uint32_t fdiv = 0x0U;
	uint32_t lvlen = 0x17U;
	uint32_t pvtover = 0x1U;
	uint32_t drive = 0x1U;

	uint32_t cad[4] = {0x2a0U, 0x2a0U, 0x2a0U, 0x2a0U};
	uint32_t grp[4] = {0x2a0U, 0x2a0U, 0x2a0U, 0x2a0U};

	uint32_t skip_pll_reset = 0x0U;
	uint32_t phymrw = 0x0fU;
	uint32_t pad_cal_wa = 0x0U;
	uint32_t updt_wa = 0x3U;

	uint32_t pvtcode_cal_lpddr4[4][8];
	uint32_t pvtcode_cal_lpddr3[4][8];
	
	/* DDR RESET ASSERT */
	mmio_write_32(DBSC_DBPDCNT00, DBPDCNT0_DFIRESETN);
	mmio_write_32(DBSC_DBPDCNT10, DBPDCNT0_DFIRESETN);
	mmio_write_32(DBSC_DBPDCNT20, DBPDCNT0_DFIRESETN);
	mmio_write_32(DBSC_DBPDCNT30, DBPDCNT0_DFIRESETN);

	if (!skip_pll_reset) {
		/* reset pulse */
		do {
			mmio_write_32(CPG_CPGWPR, 0xBFFFFFFFU);
			mmio_write_32(CPG_SRCR4, 0x40000000U);
			mmio_write_32(CPG_CPGWPR, 0xBFFFFFFFU);
			mmio_write_32(CPG_SRSTCLR4, 0x40000000U);
			reset_flag = mmio_read_32(DBSC_DBKIND);
		} while (reset_flag!=0U);

		/* Unlock DDRPHY register */
		mmio_write_32(DBSC_DBPDLK_0, DBPDLK_KEYWORD);
		mmio_write_32(DBSC_DBPDLK_1, DBPDLK_KEYWORD);
		mmio_write_32(DBSC_DBPDLK_2, DBPDLK_KEYWORD);
		mmio_write_32(DBSC_DBPDLK_3, DBPDLK_KEYWORD);

		for (i = 0U; i < PHY_MAX_NUM; i++) {
			ddr_phy_reg_rawwrite(i, PI_CS_MAP, 0U);
			ddr_phy_reg_rawwrite(i, PI_SW_RST_N, 0U);
			ddr_phy_reg_rawwrite(i, PI_WDT_DISABLE, 0U);
		}

		j = mmio_read_32(CPG_PLL3CR);
		j = (j & 0x00ffff7fU) | ((freq << 24U) & 0xff000000U)
				      | ((fdiv <<  7U) & 0x00000080U);
		mmio_write_32(CPG_CPGWPR, ~j);
		mmio_write_32(CPG_PLL3CR, j);

		SoftDelay(20000U);
		
		do {
			dataL = mmio_read_32(CPG_PLLECR);
		} while ((dataL & PLLECR_PLL3ST) == 0U);
		
		for (i = 0U; i < PHY_MAX_NUM; i++) {
			ddr_phy_reg_rawwrite(i, PI_SW_RST_N, 1U);
		}
	}

	pvt_dbsc_regset();

	/* DDR RESET NEGATE */
	mmio_write_32(DBSC_DBPDCNT00, 0x0U);
	mmio_write_32(DBSC_DBPDCNT10, 0x0U);
	mmio_write_32(DBSC_DBPDCNT20, 0x0U);
	mmio_write_32(DBSC_DBPDCNT30, 0x0U);
	
	/* (DDRPHY Kick Initialize Sequence) */
	/* Unlock DDRPHY register(AGAIN) */
	mmio_write_32(DBSC_DBPDLK_0, DBPDLK_KEYWORD);
	mmio_write_32(DBSC_DBPDLK_1, DBPDLK_KEYWORD);
	mmio_write_32(DBSC_DBPDLK_2, DBPDLK_KEYWORD);
	mmio_write_32(DBSC_DBPDLK_3, DBPDLK_KEYWORD);

	/* (PHY parameter and PI parameter Initialize)
	 * Initialize Leveling parameter including WDQLVL */
	bl2_phy_init();

	if(drive)
	{
		for (i = 0U; i < PHY_MAX_NUM; i++) {
		/* TSEL_SELECT 0x0e6e0e -> 0x006e0e */
			ddr_phy_reg_write(i, PHY_DQ_TSEL_SELECT_0,
					  0x00006E0EU);
			ddr_phy_reg_write(i, PHY_DQS_TSEL_SELECT_0,
					  0x00006E0EU);
			ddr_phy_reg_write(i, PHY_DQ_TSEL_SELECT_1,
					  0x00006E0EU);
			ddr_phy_reg_write(i, PHY_DQS_TSEL_SELECT_1,
					  0x00006E0EU);
			ddr_phy_reg_write(i, PHY_DQ_TSEL_SELECT_2,
					  0x00006E0EU);
			ddr_phy_reg_write(i, PHY_DQS_TSEL_SELECT_2,
					  0x00006E0EU);
			ddr_phy_reg_write(i, PHY_DQ_TSEL_SELECT_3,
					  0x00006E0EU);
			ddr_phy_reg_write(i, PHY_DQS_TSEL_SELECT_3,
					  0x00006E0EU);

			/* CKE 27F6E(LVSTL) -> 03fee(CMOS40orm) */
			ddr_phy_reg_write(i, PHY_PAD_CKE_DRIVE, 0x00003FEEU);

			/* RST 27F6E(LVSTL) -> 03fee(CMOS40orm) */
			ddr_phy_reg_write(i, PHY_PAD_RST_DRIVE, 0x00003FEEU);
			ddr_phy_reg_write(i, PHY_PAD_CS_TERM,   0x00010F68U);
	/* TERM 10F68(PVTR=10,PVTN=3D,PVTP=28)
	 *  -> 1154f(PVTR=11,PVTN=15,PVTP=0f */
			ddr_phy_reg_write(i, PHY_PAD_ADDR_TERM, 0x0001154FU);
			ddr_phy_reg_write(i, PHY_PAD_CKE_TERM,  0x0001154FU);
			ddr_phy_reg_write(i, PHY_PAD_CLK_TERM,  0x0001154FU);
			ddr_phy_reg_write(i, PHY_PAD_CS_TERM,   0x0001154FU);
			ddr_phy_reg_write(i, PHY_PAD_DATA_TERM, 0x0001154FU);
			ddr_phy_reg_write(i, PHY_PAD_FDBK_TERM, 0x0003154FU);
			ddr_phy_reg_write(i, PHY_PAD_CS_TERM,   0x0001154FU);
		}
	}
	

	/* Set PCB bit swap connection for Salvator-X */
	ddr_phy_reg_write(0U, PHY_DQ_SWIZZLING_0, 0x20741365U); /* DQ7-0   */
	ddr_phy_reg_write(0U, PHY_DQ_SWIZZLING_1, 0x34256107U); /* DQ15-8  */
	ddr_phy_reg_write(0U, PHY_DQ_SWIZZLING_2, 0x57460321U); /* DQ23-16 */
	ddr_phy_reg_write(0U, PHY_DQ_SWIZZLING_3, 0x70614532U); /* DQ31-24 */
	ddr_phy_reg_write(0U, PHY_ADR_ADDR_SEL_0, 0x00543210U); /* AC5-0   */
	ddr_phy_reg_write(0U, PHY_ADR_CALVL_SWIZZLE0_0_0, 0x00DCBA98U); /* DRAM Byte 0 */
	ddr_phy_reg_write(0U, PHY_ADR_CALVL_SWIZZLE1_0_0, 0x00000000U); /* DRAM Byte 1 */
	ddr_phy_reg_write(0U, PHY_ADR_CALVL_SWIZZLE0_1_0, 0x00DCBA98U); /* DRAM Byte 2 */
	ddr_phy_reg_write(0U, PHY_ADR_CALVL_SWIZZLE1_1_0, 0x00000000U); /* DRAM Byte 3 */
	ddr_phy_reg_write(0U, PHY_ADR_CALVL_DEVICE_MAP_0, 0x1U);

	ddr_phy_reg_write(1U, PHY_DQ_SWIZZLING_0, 0x23547610U); /* DQ7-0   */
	ddr_phy_reg_write(1U, PHY_DQ_SWIZZLING_1, 0x34526107U); /* DQ15-8  */
	ddr_phy_reg_write(1U, PHY_DQ_SWIZZLING_2, 0x67452310U); /* DQ23-16 */
	ddr_phy_reg_write(1U, PHY_DQ_SWIZZLING_3, 0x32106754U); /* DQ31-24 */
	ddr_phy_reg_write(1U, PHY_ADR_ADDR_SEL_0, 0x00543210U); /* AC5-0   */
	ddr_phy_reg_write(1U, PHY_ADR_CALVL_SWIZZLE0_0_0, 0x00DCBA98U); /* DRAM Byte 2 */
	ddr_phy_reg_write(1U, PHY_ADR_CALVL_SWIZZLE1_0_0, 0x00DCBA98U); /* DRAM Byte 0 */
	ddr_phy_reg_write(1U, PHY_ADR_CALVL_SWIZZLE0_1_0, 0x00DCBA98U); /* DRAM Byte 1 */
	ddr_phy_reg_write(1U, PHY_ADR_CALVL_SWIZZLE1_1_0, 0x00DCBA98U); /* DRAM Byte 3 */
	ddr_phy_reg_write(1U, PHY_ADR_CALVL_DEVICE_MAP_0, 0x2U);

	ddr_phy_reg_write(2U, PHY_DQ_SWIZZLING_0, 0x30216754U); /* DQ7-0   */
	ddr_phy_reg_write(2U, PHY_DQ_SWIZZLING_1, 0x67453210U); /* DQ15-8  */
	ddr_phy_reg_write(2U, PHY_DQ_SWIZZLING_2, 0x70165243U); /* DQ23-16 */
	ddr_phy_reg_write(2U, PHY_DQ_SWIZZLING_3, 0x07162345U); /* DQ31-24 */
	ddr_phy_reg_write(2U, PHY_ADR_ADDR_SEL_0, 0x00543210U); /* AC5-0   */
	ddr_phy_reg_write(2U, PHY_ADR_CALVL_SWIZZLE0_0_0, 0x00DCBA98U); /* DRAM Byte 3 */
	ddr_phy_reg_write(2U, PHY_ADR_CALVL_SWIZZLE1_0_0, 0x00DCBA98U); /* DRAM Byte 1 */
	ddr_phy_reg_write(2U, PHY_ADR_CALVL_SWIZZLE0_1_0, 0x00DCBA98U); /* DRAM Byte 2 */
	ddr_phy_reg_write(2U, PHY_ADR_CALVL_SWIZZLE1_1_0, 0x00DCBA98U); /* DRAM Byte 0 */
	ddr_phy_reg_write(2U, PHY_ADR_CALVL_DEVICE_MAP_0, 0x1U);

	ddr_phy_reg_write(3U, PHY_DQ_SWIZZLING_0, 0x01327654U); /* DQ7-0   */
	ddr_phy_reg_write(3U, PHY_DQ_SWIZZLING_1, 0x70615432U); /* DQ15-8  */
	ddr_phy_reg_write(3U, PHY_DQ_SWIZZLING_2, 0x54760123U); /* DQ23-16 */
	ddr_phy_reg_write(3U, PHY_DQ_SWIZZLING_3, 0x07162345U); /* DQ31-24 */
	ddr_phy_reg_write(3U, PHY_ADR_ADDR_SEL_0, 0x00543210U); /* AC5-0   */
	ddr_phy_reg_write(3U, PHY_ADR_CALVL_SWIZZLE0_0_0, 0X00DCBA98U); /* DRAM Byte 3 */
	ddr_phy_reg_write(3U, PHY_ADR_CALVL_SWIZZLE1_0_0, 0X00DCBA98U); /* DRAM Byte 1 */
	ddr_phy_reg_write(3U, PHY_ADR_CALVL_SWIZZLE0_1_0, 0X00DCBA98U); /* DRAM Byte 2 */
	ddr_phy_reg_write(3U, PHY_ADR_CALVL_SWIZZLE1_1_0, 0X00DCBA98U); /* DRAM Byte 0 */
	ddr_phy_reg_write(3U, PHY_ADR_CALVL_DEVICE_MAP_0, 0x1U);

	/* CH1 CALVL for swap */
	/* CH1 CALVL can be done only for CS[2] ( M2 dq_h -> soc M2 dq_h ) */
	/* PI_CALVL_CS_MAP:RW:0:4:= 0x4 */
	ddr_phy_reg_write(1U, PI_CALVL_CS_MAP, 0x4U);

	mmio_write_32(DBSC_DBDFIPMSTRCNF, DBDFIPMSTRCNF_EN);

	for (i = 0U; i < PHY_MAX_NUM; i++) {
		/* CALVL_EN */
		if (lvlen & 0x00000001U) {
			ddr_phy_reg_write(i, PI_CALVL_EN, 1U);
		} else {
			ddr_phy_reg_write(i, PI_CALVL_EN, 0U);
		}

		/* WRLVL_EN */
		if (lvlen & 0x00000002U) {
			ddr_phy_reg_write(i, PI_WRLVL_EN, 1U);
		}
		else
		{
			ddr_phy_reg_write(i, PI_WRLVL_EN, 0U);
		}

		/* RDLVL_GATE_EN */
		if (lvlen & 0x00000004U) {
			ddr_phy_reg_write(i, PI_RDLVL_GATE_EN, 1U);
		} else {
			ddr_phy_reg_write(i, PI_RDLVL_GATE_EN, 0U);
		}

		/* RDLVL_EN */
		if (lvlen & 0x00000008U) {
			ddr_phy_reg_write(i, PI_RDLVL_EN, 1U);
		}
		else
		{
			ddr_phy_reg_write(i, PI_RDLVL_EN, 0U);
		}

		/* WRDQLVL_EN */
		if (lvlen & 0x00000010U) {
			ddr_phy_reg_write(i, PI_WDQLVL_EN, 1U);
		}
		else
		{
			ddr_phy_reg_write(i, PI_WDQLVL_EN, 0U);
		}
	}

	if (pad_cal_wa) {
		for (i = 0U; i < PHY_MAX_NUM; i++) {
			ddr_phy_reg_write(i, PHY_CAL_MODE_0, 0U);
			ddr_phy_reg_write(i, PHY_TST_CLK_PAD_CTRL2, 0x000700U);
		}
	}

	/* dbdficnt0 freq_ratio = 01 (2:1)init_start =1 */
	mmio_write_32(DBSC_DBDFICNT_0, DBDFICNT_DFIBYTEDIS(0U)
				     | DBDFICNT_DFICLKDIS(0U)
				     | DBDFICNT_DFIFREQRATIO_1_2
				     | DBDFICNT_DFIINITSTART);
	mmio_write_32(DBSC_DBDFICNT_1, DBDFICNT_DFIBYTEDIS(0U)
				     | DBDFICNT_DFICLKDIS(0U)
				     | DBDFICNT_DFIFREQRATIO_1_2
				     | DBDFICNT_DFIINITSTART);
	mmio_write_32(DBSC_DBDFICNT_2, DBDFICNT_DFIBYTEDIS(0U)
				     | DBDFICNT_DFICLKDIS(0U)
				     | DBDFICNT_DFIFREQRATIO_1_2
				     | DBDFICNT_DFIINITSTART);
	mmio_write_32(DBSC_DBDFICNT_3, DBDFICNT_DFIBYTEDIS(0U)
				     | DBDFICNT_DFICLKDIS(0U)
				     | DBDFICNT_DFIFREQRATIO_1_2
				     | DBDFICNT_DFIINITSTART);

	mmio_write_32(DBSC_DBPDCNT03, DBPDCNT3_DLLRST); /* dll_rst_n0 -> 1 */
	mmio_write_32(DBSC_DBPDCNT13, DBPDCNT3_DLLRST);
	mmio_write_32(DBSC_DBPDCNT23, DBPDCNT3_DLLRST);
	mmio_write_32(DBSC_DBPDCNT33, DBPDCNT3_DLLRST);

	/* Need Handshake sequence betweed DBSC and PHY */
	do {
		dataL = 0x00000001U & mmio_read_32(DBSC_DBDFISTAT0);
	} while (dataL != 0x00000001U); /* Wait for PI status[0] is 0 */
	do {
		dataL = 0x00000001U & mmio_read_32(DBSC_DBDFISTAT1);
	} while (dataL != 0x00000001U); /* Wait for PI status[0] is 0 */
	do {
		dataL = 0x00000001U & mmio_read_32(DBSC_DBDFISTAT2);
	} while (dataL != 0x00000001U); /* Wait for PI status[0] is 0 */
	do {
		dataL = 0x00000001U & mmio_read_32(DBSC_DBDFISTAT3);
	} while (dataL != 0x00000001U); /* Wait for PI status[0] is 0 */

	/* Need Handshake sequence betweed DBSC and PHY */
	if (pad_cal_wa) {
		for (j = 0U; j < PHY_MAX_NUM; j++) {
			pvtcode_cal_lpddr3[j][0] =
					ddr_phy_reg_read(j, PHY_PAD_FDBK_TERM);
			pvtcode_cal_lpddr3[j][1] =
					ddr_phy_reg_read(j, PHY_PAD_DATA_TERM);
			pvtcode_cal_lpddr3[j][2] =
					ddr_phy_reg_read(j, PHY_PAD_DQS_TERM);
			pvtcode_cal_lpddr3[j][3] =
					ddr_phy_reg_read(j, PHY_PAD_ADDR_TERM);
/*			pvtcode_cal_lpddr3[j][4] =
					ddr_phy_reg_read(j, PHY_PAD_CLK_TERM);
*/
			pvtcode_cal_lpddr3[j][5] =
					ddr_phy_reg_read(j, PHY_PAD_CKE_TERM);
			pvtcode_cal_lpddr3[j][6] =
					ddr_phy_reg_read(j, PHY_PAD_RST_TERM);
			pvtcode_cal_lpddr3[j][7] =
					ddr_phy_reg_read(j, PHY_PAD_CS_TERM);
			pvtcode_cal_lpddr3[j][4] = pvtcode_cal_lpddr3[j][3];
		}
	
		for (j = 0U; j < PHY_MAX_NUM; j++) {
			ddr_phy_reg_write(i, PHY_TST_CLK_PAD_CTRL2, 0x000B00U); /* change to LPDDR4 class */
			ddr_phy_reg_write(i, PHY_CAL_CLEAR_0, 1U); /* [0]cal_clear */
			ddr_phy_reg_write(i, PHY_CAL_CLK_SELECT_0, 0U);
			ddr_phy_reg_write(i, PHY_CAL_MODE_0, 0x65U);
			ddr_phy_reg_write(i, PHY_CAL_START_0, 1U); /* [8]cal_start */
		}
		SoftDelay(2000U); /* wait for 2msec */

		for (j = 0U; j < PHY_MAX_NUM; j++) {
			pvtcode_cal_lpddr4[j][0] =
					ddr_phy_reg_read(j, PHY_PAD_FDBK_TERM);
			pvtcode_cal_lpddr4[j][1] =
					ddr_phy_reg_read(j, PHY_PAD_DATA_TERM);
			pvtcode_cal_lpddr4[j][2] =
					ddr_phy_reg_read(j, PHY_PAD_DQS_TERM);
			pvtcode_cal_lpddr4[j][3] =
					ddr_phy_reg_read(j, PHY_PAD_ADDR_TERM);
/*			pvtcode_cal_lpddr4[j][4] =
					ddr_phy_reg_read(j, PHY_PAD_CLK_TERM);
*/
			pvtcode_cal_lpddr4[j][5] =
					ddr_phy_reg_read(j, PHY_PAD_CKE_TERM);
			pvtcode_cal_lpddr4[j][6] =
					ddr_phy_reg_read(j, PHY_PAD_RST_TERM);
			pvtcode_cal_lpddr4[j][7] =
					ddr_phy_reg_read(j, PHY_PAD_CS_TERM);
			pvtcode_cal_lpddr4[j][4] =
					pvtcode_cal_lpddr3[j][3];
		}

		for (j = 0U; j < PHY_MAX_NUM; j++) {
			dataL = (pvtcode_cal_lpddr4[j][0] & 0xFFFFF000U)
			      | (pvtcode_cal_lpddr3[j][0] & 0x00000FFFU);
			ddr_phy_reg_write(i, PHY_PAD_FDBK_TERM, dataL);
			dataL = (pvtcode_cal_lpddr4[j][1] & 0xFFFFF000U)
			      | (pvtcode_cal_lpddr3[j][1] & 0x00000FFFU);
			ddr_phy_reg_write(i, PHY_PAD_DATA_TERM, dataL);
			dataL = (pvtcode_cal_lpddr4[j][2] & 0xFFFFF000U)
			      | (pvtcode_cal_lpddr3[j][2] & 0x00000FFFU);
			ddr_phy_reg_write(i, PHY_PAD_DQS_TERM, dataL);
			dataL = (pvtcode_cal_lpddr4[j][3] & 0xFFFFF000U)
			      | (pvtcode_cal_lpddr3[j][3] & 0x00000FFFU);
			ddr_phy_reg_write(i, PHY_PAD_ADDR_TERM, dataL);
			dataL = (pvtcode_cal_lpddr4[j][4] & 0xFFFFF000U)
			      | (pvtcode_cal_lpddr3[j][4] & 0x00000FFFU);
			ddr_phy_reg_write(i, PHY_PAD_CLK_TERM, dataL);
			dataL = (pvtcode_cal_lpddr4[j][5] & 0xFFFFF000U)
			      | (pvtcode_cal_lpddr3[j][5] & 0x00000FFFU);
			ddr_phy_reg_write(i, PHY_PAD_CKE_TERM, dataL);
			dataL = (pvtcode_cal_lpddr4[j][6] & 0xFFFFF000U)
			      | (pvtcode_cal_lpddr3[j][6] & 0x00000FFFU);
			ddr_phy_reg_write(i, PHY_PAD_RST_TERM, dataL);
			dataL = (pvtcode_cal_lpddr4[j][7] & 0xFFFFF000U)
			      | (pvtcode_cal_lpddr3[j][7] & 0x00000FFFU);
			ddr_phy_reg_write(i, PHY_PAD_CS_TERM, dataL);
		}
	}

/* ====== PVT_Overwrite ====== */
	if (pvtover == 1U) {
		DDRPHY_PVT_OVERWRITE();
	}

	for (i = 0U; i < PHY_MAX_NUM; i++) {
		ddr_phy_reg_write(i, PHY_ADR0_CLK_WR_SLAVE_DELAY_0, cad[i]);
		ddr_phy_reg_write(i, PHY_ADR1_CLK_WR_SLAVE_DELAY_0, cad[i]);
		ddr_phy_reg_write(i, PHY_ADR2_CLK_WR_SLAVE_DELAY_0, cad[i]);
		ddr_phy_reg_write(i, PHY_ADR3_CLK_WR_SLAVE_DELAY_0, cad[i]);
		ddr_phy_reg_write(i, PHY_ADR4_CLK_WR_SLAVE_DELAY_0, cad[i]);
		ddr_phy_reg_write(i, PHY_ADR5_CLK_WR_SLAVE_DELAY_0, cad[i]);
		ddr_phy_reg_write(i, PHY_GRP_SLAVE_DELAY_0, grp[i]);
		ddr_phy_reg_write(i, PHY_GRP_SLAVE_DELAY_1, grp[i]);
		ddr_phy_reg_write(i, PHY_GRP_SLAVE_DELAY_2, grp[i]);
		ddr_phy_reg_write(i, PHY_GRP_SLAVE_DELAY_3, grp[i]);
	}

	/* Need Handshake sequence betweed DBSC and PHY */

	if (updt_wa & 0x2U) {
		pvt_manual_update(0x0fU);
	}
	if (updt_wa & 0x4U) {
		pvt_manual_update(0xf0U);
	}


	/* CHANGE IMPEDANCE CODE to CMOS MODE */
	change_lpddr4_en(0xfU, 0U);

	phychno = 0x0fU;


	/* (SDRAM Initalize) */
	mmio_write_32(DBSC_DBCMD, DBCMD_OPC_RESET /* RSX chA rkA */
				| DBCMD_CH(phychno)
				| DBCMD_RANK_ALL
				| DBCMD_ARG_RESET_ASSERT);
	WaitDBCMD();

	SoftDelay(2000U); /* wait for 2msec */

	mmio_write_32(DBSC_DBCMD, DBCMD_OPC_PD /* PDE chA rkA */
				| DBCMD_CH(phychno)
				| DBCMD_RANK_ALL
				| DBCMD_ARG_ENTER);
	WaitDBCMD();

	mmio_write_32(DBSC_DBCMD, DBCMD_OPC_PD
				| DBCMD_CH(phychno)
				| DBCMD_RANK_ALL
				| DBCMD_ARG_EXIT);
	WaitDBCMD();

	mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW /* MRW chA rkA 02:00 */
				| DBCMD_CH(phychno)
				| DBCMD_RANK_ALL
				| DBCMD_ARG_MRW(0x02U, 0x00U));
	WaitDBCMD();

	mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW /* MRW chA rkA 03:31 */
				| DBCMD_CH(phychno)
				| DBCMD_RANK_ALL
				| DBCMD_ARG_MRW(0x03U, 0x31U));
	WaitDBCMD();

	mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW /* MRW chA rkA 01:00 */
				| DBCMD_CH(phychno)
				| DBCMD_RANK_ALL
				| DBCMD_ARG_MRW(0x01U, 0x00U));
	WaitDBCMD();

	if(phymrw & (1U<<phychno))
	{
		/* MRW chA rkA 0d:00 //FSPOP&FSPWP is fixed to 0, VRO off */
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x0DU, 0x40U));
		WaitDBCMD();
		
		/* MRW DeviceFeature1
		 * (Post=1.5tck nWR=30 RDpre=toggle WRPre=2tCK BL=16 */
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x01U, 0xD4U));
		WaitDBCMD();
		
		/* MRW DeviceFeature2(0,0SetA,101=WL14,110=RL32) */
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x02U, 0x2EU));
		WaitDBCMD();
		
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW /* MRW chA rkA 03:31 */
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x03U, 0x31U));
		WaitDBCMD();
		
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x0BU, 0x36U));
		WaitDBCMD();
		
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x0CU, 0x11U));
		WaitDBCMD();
		
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x0EU, 0x11U));
		WaitDBCMD();
		
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x16U, 0x06U));
		WaitDBCMD();
		
		/* MRW chA rkA 0d:00 //FSPOP&FSPWP is fixed to 0, VRO off */
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x0DU, 0x00U));
		WaitDBCMD();
		
		/* MRW DeviceFeature1(Post=1.5tck nWR=30 RDpre=toggle WRPre=2tCK BL=16 */
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x01U, 0xD4U));
		WaitDBCMD();
		
		/* MRW DeviceFeature2(0,0SetA,101=WL14,110=RL32) */
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x02U, 0x2EU));
		WaitDBCMD();
		
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW /* MRW chA rkA 03:31 */
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x03U, 0x31U));
		WaitDBCMD();
		
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x0DU, 0x36U));
		WaitDBCMD();
		
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x0CU, 0x11U));
		WaitDBCMD();
		
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x0EU, 0x11U));
		WaitDBCMD();
		
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x16U, 0x06U));
		WaitDBCMD();
	}
		
	SoftDelay(20U); /* wait for 2usec */
	
	/* MPC chA rkA 4FH (ZQCAL start) */
	mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MPC
				| DBCMD_CH(phychno)
				| DBCMD_RANK_ALL
				| DBCMD_ARG_MPC(0x4FU));
	WaitDBCMD();
	
	/* MPC chA rkA 51H (ZQCAL latch) */
	mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MPC
				| DBCMD_CH(phychno)
				| DBCMD_RANK_ALL
				| DBCMD_ARG_MPC(0x51U));
	WaitDBCMD();

	change_lpddr4_en(0xfU, 1U);

	/* WHEN mode==3, skip training */
	if (mode != 3U) {
		phytrainingok = pvt_pi_training();

		if (updt_wa & 0x2U) {
			pvt_manual_update(0x0fU);
		}
		if (updt_wa & 0x4U) {
			pvt_manual_update(0xf0U);
		}
	} else {
		phytrainingok = 0x0fU;
	}

	for (phychno = 0U; phychno < 4U; phychno += 1U) {
		if ((phytrainingok & 0x0fU) == 0x0fU) {
			phychno = 0x0fU;
		} else {
			if ((phytrainingok & (1U << phychno)) == 0U) {
				continue;
			}
		}
		/* MRW chA rkA 0d:00 //FSPOP&FSPWP is fixed to 0, VRO off */
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x0DU, 0x40U));
		WaitDBCMD();

		/* MRW DeviceFeature1(Post=1.5tck nWR=30 RDpre=toggle WRPre=2tCK BL=16 */
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x01U, 0xD4U));
		WaitDBCMD();
		
		/* MRW DeviceFeature2(0,0SetA,101=WL14,110=RL32) */
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x02U, 0x2EU));
		WaitDBCMD();

		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW /* MRW chA rkA 03:31 */
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x03U, 0x31U));
		WaitDBCMD();

		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x0BU, 0x36U));
		WaitDBCMD();
		
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x0CU, 0x11U));
		WaitDBCMD();
		
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x0EU, 0x11U));
		WaitDBCMD();
		
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x16U, 0x06U));
		WaitDBCMD();

		/* MRW chA rkA 0d:00 //FSPOP&FSPWP is fixed to 0, VRO off */
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x0DU, 0x00U));
		WaitDBCMD();
		
		/* MRW DeviceFeature1(Post=1.5tck nWR=30 RDpre=toggle WRPre=2tCK BL=16 */
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x01U, 0xD4U));
		WaitDBCMD();
		
		/* MRW DeviceFeature2(0,0SetA,101=WL14,110=RL32) */
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x02U, 0x2EU));
		WaitDBCMD();
		
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW /* MRW chA rkA 03:31 */
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x03U, 0x31U));
		WaitDBCMD();
		
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x0BU, 0x36U));
		WaitDBCMD();
		
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x0CU, 0x11U));
		WaitDBCMD();
		
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x0EU, 0x11U));
		WaitDBCMD();
		
		mmio_write_32(DBSC_DBCMD, DBCMD_OPC_MRW
					| DBCMD_CH(phychno)
					| DBCMD_RANK_ALL
					| DBCMD_ARG_MRW(0x16U, 0x06U));
		WaitDBCMD();
	}

	mmio_write_32(DBSC_DBBUS0CNF1, 0x70000100U);
	mmio_write_32(DBSC_DBBUS0CNF0, DBBUS0CNF0_EXSTA(24U)
				     | DBBUS0CNF0_EN
				     | DBBUS0CNF0_READ_HIGH_PRIORITY);
	
	/* Auto Refresh setting */
	mmio_write_32(DBSC_DBRFCNF1, DBRFCNF1_REFPMAX(8U) /* refpmax=8 */
				   | DBRFCNF1_REFINT(6240U)); /* refint=6240 */
	mmio_write_32(DBSC_DBRFCNF2, DBRFCNF2_REFPMIN /* refpmin=1 */
				   | DBRFCNF2_REFINT_1_1); /* refints=0 */
	mmio_write_32(DBSC_DBRFEN, DBRFEN_ARFEN_START);
	
	/* DRAM ACCESS enable */
	mmio_write_32(DBSC_DBACEN, DBACEN_ACCEN_EN);


}


static void pvt_dbsc_regset(void)
{
	mmio_write_32(DBSC_DBPDCNT00, 0x0U);
	mmio_write_32(DBSC_DBPDCNT10, 0x0U);
	mmio_write_32(DBSC_DBPDCNT20, 0x0U);
	mmio_write_32(DBSC_DBPDCNT30, 0x0U);
	
	/* LPDDR4-1600(CL14NoDBI),4GByte(8GBit x 4) */
	mmio_write_32(DBSC_DBSTATE0, 0x00000010U);
	mmio_write_32(DBSC_DBKIND, DDCG_LPDDR4); /* DRAM type LPDDR4 */

	mmio_write_32(DBSC_DBBL, DBBL_BL16); /* dbbl=2(BL16) */

	/* DRAM Size Registers */
	/* Please set for 4ranks */
	mmio_write_32(DBSC_DBMEMCONF_0_0, DBMEMCONF_DENS_2N
					 | DBMEMCONF_AWRW_15
					 | DBMEMCONF_AWBK
					 | DBMEMCONF_AWCL_10
					 | DBMEMCONF_DW_10);
	mmio_write_32(DBSC_DBMEMCONF_0_1, DBMEMCONF_DENS_2N
					 | DBMEMCONF_AWRW_15
					 | DBMEMCONF_AWBK
					 | DBMEMCONF_AWCL_10
					 | DBMEMCONF_DW_10);
	mmio_write_32(DBSC_DBMEMCONF_1_0, DBMEMCONF_DENS_2N
					 | DBMEMCONF_AWRW_15
					 | DBMEMCONF_AWBK
					 | DBMEMCONF_AWCL_10
					 | DBMEMCONF_DW_10);
	mmio_write_32(DBSC_DBMEMCONF_1_1, DBMEMCONF_DENS_2N
					 | DBMEMCONF_AWRW_15
					 | DBMEMCONF_AWBK
					 | DBMEMCONF_AWCL_10
					 | DBMEMCONF_DW_10);
	mmio_write_32(DBSC_DBMEMCONF_2_0, DBMEMCONF_DENS_2N
					 | DBMEMCONF_AWRW_15
					 | DBMEMCONF_AWBK
					 | DBMEMCONF_AWCL_10
					 | DBMEMCONF_DW_10);
	mmio_write_32(DBSC_DBMEMCONF_2_1, DBMEMCONF_DENS_2N
					 | DBMEMCONF_AWRW_15
					 | DBMEMCONF_AWBK
					 | DBMEMCONF_AWCL_10
					 | DBMEMCONF_DW_10);
	mmio_write_32(DBSC_DBMEMCONF_3_0, DBMEMCONF_DENS_2N
					 | DBMEMCONF_AWRW_15
					 | DBMEMCONF_AWBK
					 | DBMEMCONF_AWCL_10
					 | DBMEMCONF_DW_10);
	mmio_write_32(DBSC_DBMEMCONF_3_1, DBMEMCONF_DENS_2N
					 | DBMEMCONF_AWRW_15
					 | DBMEMCONF_AWBK
					 | DBMEMCONF_AWCL_10
					 | DBMEMCONF_DW_10);

	mmio_write_32(DBSC_DBMEMCONF_0_2, DBMEMCONF_DENS_2N
					 | DBMEMCONF_AWRW_15
					 | DBMEMCONF_AWBK
					 | DBMEMCONF_AWCL_10
					 | DBMEMCONF_DW_10);
	mmio_write_32(DBSC_DBMEMCONF_0_3, DBMEMCONF_DENS_2N
					 | DBMEMCONF_AWRW_15
					 | DBMEMCONF_AWBK
					 | DBMEMCONF_AWCL_10
					 | DBMEMCONF_DW_10);
	mmio_write_32(DBSC_DBMEMCONF_1_2, DBMEMCONF_DENS_2N
					 | DBMEMCONF_AWRW_15
					 | DBMEMCONF_AWBK
					 | DBMEMCONF_AWCL_10
					 | DBMEMCONF_DW_10);
	mmio_write_32(DBSC_DBMEMCONF_1_3, DBMEMCONF_DENS_2N
					 | DBMEMCONF_AWRW_15
					 | DBMEMCONF_AWBK
					 | DBMEMCONF_AWCL_10
					 | DBMEMCONF_DW_10);
	mmio_write_32(DBSC_DBMEMCONF_2_2, DBMEMCONF_DENS_2N
					 | DBMEMCONF_AWRW_15
					 | DBMEMCONF_AWBK
					 | DBMEMCONF_AWCL_10
					 | DBMEMCONF_DW_10);
	mmio_write_32(DBSC_DBMEMCONF_2_3, DBMEMCONF_DENS_2N
					 | DBMEMCONF_AWRW_15
					 | DBMEMCONF_AWBK
					 | DBMEMCONF_AWCL_10
					 | DBMEMCONF_DW_10);
	mmio_write_32(DBSC_DBMEMCONF_3_2, DBMEMCONF_DENS_2N
					 | DBMEMCONF_AWRW_15
					 | DBMEMCONF_AWBK
					 | DBMEMCONF_AWCL_10
					 | DBMEMCONF_DW_10);
	mmio_write_32(DBSC_DBMEMCONF_3_3, DBMEMCONF_DENS_2N
					 | DBMEMCONF_AWRW_15
					 | DBMEMCONF_AWBK
					 | DBMEMCONF_AWCL_10
					 | DBMEMCONF_DW_10);

	mmio_write_32(DBSC_DBPHYCONF0, DBPHYCONF0_PHY_DFI);

	/* DRAM Timing Registers */
	mmio_write_32(DBSC_DBTR0, DBTR0_CL(32U)); /* cl=32 */
	mmio_write_32(DBSC_DBTR1, DBTR1_CWL(14U)); /* cwl=14 */
	mmio_write_32(DBSC_DBTR3, DBTR3_TRCD(29U)); /* trcd=29 */
	mmio_write_32(DBSC_DBTR4, DBTR4_TRPA(5U) /* trpa=34 */
				| DBTR4_TRP(80U)); /* trp=29 */
	mmio_write_32(DBSC_DBTR5, DBTR5_TRC(102U)); /* trc=102 */
	mmio_write_32(DBSC_DBTR6, DBTR6_TRAS(68U)); /* tras=68 */
	mmio_write_32(DBSC_DBTR7, DBTR7_TRRD_S(16U) /* trrd_s=16 */
				 | DBTR7_TRRD_S(16U)); /* trrd=16 */
	mmio_write_32(DBSC_DBTR8, DBTR8_TFAW(64U)); /* tfaw=64 */
	mmio_write_32(DBSC_DBTR9, DBTR9_TRDPR(12U)); /* trdpr=12 */
	mmio_write_32(DBSC_DBTR10, DBTR10_TWR(30U)); /* twr=30 */
	mmio_write_32(DBSC_DBTR11, DBTR11_TWRTWR(80U)); /* trdwr=18 */
	mmio_write_32(DBSC_DBTR12, DBTR12_TWRRD_S(80U) /* twrrd_s=20 */
				 | DBTR12_TWRRD(80U)); /* twrrd=20 */
	mmio_write_32(DBSC_DBTR13, DBTR13_TRFCPB(144U)
				 | DBTR13_TRFC_TRFCAB(288U));
	mmio_write_32(DBSC_DBTR14, DBTR14_TCKEHDLL(12U) /* tckehdll=12 */
				 | DBTR14_TCKEH(12U)); /* tckeh=12 */
	mmio_write_32(DBSC_DBTR15, DBTR15_TCKESR(30U) /* tckesr=30 */
				 | DBTR15_TCKEL(12U)); /* tckel=12 */
	
	mmio_write_32(DBSC_DBTR16, DBTR16_DQIENLTNCY(16U)
				 | DBTR16_DQL(64U)
				 | DBTR16_DQENLTNCY(12U)
				 | DBTR16_WDQL(14U));
	mmio_write_32(DBSC_DBTR17, DBTR17_TMODRD(8U) /* tmodrd=8 */
				 | DBTR17_TMOD(10U) /* tmod=10 */
				 | DBTR17_TRDMR(4U)); /* trdmr=4 */
	mmio_write_32(DBSC_DBTR18, DBTR18_RODTL(0U)
				 | DBTR18_RODTA(0U)
				 | DBTR18_WODTL(0U)
				 | DBTR18_WODTA(0U));
	mmio_write_32(DBSC_DBTR19, DBTR19_TZQCL(0U) /* tzqcl=0 */
				 | DBTR19_TZQCS(0U)); /* tzqcs=0 */
	mmio_write_32(DBSC_DBTR20, DBTR20_TXSDLL(220U) /* txsdll=trfc+12=220 */
				 | DBTR20_TXS(220U)); /* txs=220 */
	mmio_write_32(DBSC_DBTR21, DBTR21_TCCD_S(16U) /* tccd_s=8 */
				 | DBTR21_TCCD(16U)); /* tccd=8 */
	mmio_write_32(DBSC_DBTR22, DBTR22_TZQCAL(1600U) /* tzqcal=1600 */
				 | DBTR22_TZQLAT(48U)); /* tzqlat=48 */

	mmio_write_32(DBSC_DBRNK0, 0x00000000U);
	mmio_write_32(DBSC_DBRNK1, DBRNK1_RKRP(0U)
				 | DBRNK1_RKRF(0U));
	mmio_write_32(DBSC_DBRNK2, DBRNK2_RKRR7(0U) /* rkrr=8 */
				 | DBRNK2_RKRR6(0U)
				 | DBRNK2_RKRR5(0U)
				 | DBRNK2_RKRR4(0U)
				 | DBRNK2_RKRR3(8U)
				 | DBRNK2_RKRR2(8U)
				 | DBRNK2_RKRR1(8U)
				 | DBRNK2_RKRR0(8U));
	mmio_write_32(DBSC_DBRNK3, DBRNK3_RKRW7(0U) /* rkrw=8 */
				 | DBRNK3_RKRW6(0U)
				 | DBRNK3_RKRW5(0U)
				 | DBRNK3_RKRW4(0U)
				 | DBRNK3_RKRW3(8U)
				 | DBRNK3_RKRW2(8U)
				 | DBRNK3_RKRW1(8U)
				 | DBRNK3_RKRW0(8U));
	mmio_write_32(DBSC_DBRNK4, DBRNK4_RKWR7(0U) /* rkwr=8 */
				 | DBRNK4_RKWR6(0U)
				 | DBRNK4_RKWR5(0U)
				 | DBRNK4_RKWR4(0U)
				 | DBRNK4_RKWR3(8U)
				 | DBRNK4_RKWR2(8U)
				 | DBRNK4_RKWR1(8U)
				 | DBRNK4_RKWR0(8U));
	mmio_write_32(DBSC_DBRNK5, DBRNK5_RKWW7(0U) /* rkwww=8 */
				 | DBRNK5_RKWW6(0U)
				 | DBRNK5_RKWW5(0U)
				 | DBRNK5_RKWW4(0U)
				 | DBRNK5_RKWW3(8U)
				 | DBRNK5_RKWW2(8U)
				 | DBRNK5_RKWW1(8U)
				 | DBRNK5_RKWW0(8U));
	mmio_write_32(DBSC_DBRNK6, DBRNK6_REFMODE_ALL); /* Use ALL Rank */
	mmio_write_32(DBSC_DBADJ0, DBADJ0_CAMODE_1_CLK);
	mmio_write_32(DBSC_DBADJ2, 0x00000000U);

	/* BUFCAM settings */
	mmio_write_32(DBSC_DBCAM0CNF0, 0x00000000U);
	mmio_write_32(DBSC_DBCAM0CNF1, DBCAM0CNF1_WBKWAIT(8U) /* wbkwait=8 */
				     | DBCAM0CNF1_SWPINPRI3(2U)
				     | DBCAM0CNF1_SWPINPRI2(0U)
				     | DBCAM0CNF1_SWPINPRI1(1U)
				     | DBCAM0CNF1_SWPINPRI1F(0U));
	mmio_write_32(DBSC_DBCAM0CNF2, 0x00002000U
				     | DBCAM0CNF2_FILLUNIT_64
				     | DBCAM0CNF2_FCDIRTYMAX(0U)
				     | DBCAM0CNF2_FCDIRTYMIN(0U));
	mmio_write_32(DBSC_DBCAM0CNF3, DBCAM0CNF3_RDFULL(24U));
	mmio_write_32(DBSC_DBSCHSZ0, DBSCHCNT0_SCWBPRI_3
				   | DBSCHCNT0_SCQTZEN_3
				   | DBSCHCNT0_SCQTZEN_2
				   | DBSCHCNT0_SCQTZEN_1
				   | DBSCHCNT0_SCQTZEN_0
				   | DBSCHCNT0_SCSZEN_EN
				   | DBSCHCNT0_SCBAEN_EN
				   | (uint32_t)0x08U
				   | DBSCHCNT0_SCPGEN_EN
				   | DBSCHCNT0_SCRWEN_EN
				   | DBSCHCNT0_SCQOSEN_EN);
	mmio_write_32(DBSC_DBSCHCNT1, DBSCHCNT1_SCHCH3(1U)
				    | DBSCHCNT1_SCHCH2(0U)
				    | DBSCHCNT1_SCHCH1(1U)
				    | DBSCHCNT1_SCHCH0(0U));
	mmio_write_32(DBSC_DBSCHSZ0, DBSCHSZ0_SZTH(1U));
	mmio_write_32(DBSC_DBSCHRW0, 0x00000100U);
	mmio_write_32(DBSC_DBSCHRW1, DBSCHRW1_SCBADEC(0U)
				   | DBSCHRW1_RWWTPERI(0x40U));
	
	/* QoS Settings */
	mmio_write_32(DBSC_DBSCHQOS_4_0,  0x00000600U);
	mmio_write_32(DBSC_DBSCHQOS_4_1,  0x00000480U);
	mmio_write_32(DBSC_DBSCHQOS_4_2,  0x00000300U);
	mmio_write_32(DBSC_DBSCHQOS_4_3,  0x00000180U);
	mmio_write_32(DBSC_DBSCHQOS_9_0,  0x00000400U);
	mmio_write_32(DBSC_DBSCHQOS_9_1,  0x00000300U);
	mmio_write_32(DBSC_DBSCHQOS_9_2,  0x00000200U);
	mmio_write_32(DBSC_DBSCHQOS_9_3,  0x00000100U);
	mmio_write_32(DBSC_DBSCHQOS_13_0, 0x00000300U);
	mmio_write_32(DBSC_DBSCHQOS_13_1, 0x00000240U);
	mmio_write_32(DBSC_DBSCHQOS_13_2, 0x00000180U);
	mmio_write_32(DBSC_DBSCHQOS_13_3, 0x000000C0U);
	mmio_write_32(DBSC_DBSCHQOS_14_0, 0x00000200U);
	mmio_write_32(DBSC_DBSCHQOS_14_1, 0x00000180U);
	mmio_write_32(DBSC_DBSCHQOS_14_2, 0x00000100U);
	mmio_write_32(DBSC_DBSCHQOS_14_3, 0x00000080U);
	mmio_write_32(DBSC_DBSCHQOS_15_0, 0x00000100U);
	mmio_write_32(DBSC_DBSCHQOS_15_1, 0x000000C0U);
	mmio_write_32(DBSC_DBSCHQOS_15_2, 0x00000080U);
	mmio_write_32(DBSC_DBSCHQOS_15_3, 0x00000040U);
	
	/* dbsysconf1 freqratio=2. */
	mmio_write_32(DBSC_DBSYSCONF1, DBSYSCONF1_FREQRATIO_1_4);
	
	/* Under Verification */
	/* wbkwait(0004), wbkmdhi(4,2),wbkmdlo(1,8) */
	mmio_write_32(DBSC_DBCAM0CNF1, DBCAM0CNF1_WBKWAIT(4U)
				     | DBCAM0CNF1_SWPINPRI3(4U)
				     | DBCAM0CNF1_SWPINPRI2(2U)
				     | DBCAM0CNF1_SWPINPRI1(1U)
				     | DBCAM0CNF1_SWPINPRI1F(8U));
	/* rg_scexp[15:0] 0,0(fillunit),8(dirtymax),4(dirtymin) */
	mmio_write_32(DBSC_DBCAM0CNF2, DBCAM0CNF2_FILLUNIT_256
				     | DBCAM0CNF2_FCDIRTYMAX(8U)
				     | DBCAM0CNF2_FCDIRTYMIN(4U));
	mmio_write_32(DBSC_DBSCHRW0, 0xF7311111U);
	mmio_write_32(DBSC_DBSCHRW1, DBSCHRW1_SCBADEC(0U)
				   | DBSCHRW1_RWWTPERI(52U));
	mmio_write_32(DBSC_DBSCTR0, 0x1A0C1608U);
	mmio_write_32(DBSC_DBSCTR1, 0x0809070CU);
	mmio_write_32(DBSC_DBSCHRW2, 0x111F1FFFU);
}

static void pvt_manual_update(uint32_t flag)
{
	if (flag & 0x1U) {
		ddr_phy_reg_write(0U, SC_PHY_MANUAL_UPDATE, 1U);
	}
	if (flag & 0x2U) {
		ddr_phy_reg_write(1U, SC_PHY_MANUAL_UPDATE, 1U);
	}
	if (flag & 0x4U) {
		ddr_phy_reg_write(2U, SC_PHY_MANUAL_UPDATE, 1U);
	}
	if (flag & 0x8U) {
		ddr_phy_reg_write(3U, SC_PHY_MANUAL_UPDATE, 1U);
	}
	
	if (flag & 0x10U) {
		ddr_phy_reg_write(0U, PHY_ADRCTL_MANUAL_UPDATE, 1U);
	}
	if (flag & 0x20U) {
		ddr_phy_reg_write(1U, PHY_ADRCTL_MANUAL_UPDATE, 1U);
	}
	if (flag & 0x40U) {
		ddr_phy_reg_write(2U, PHY_ADRCTL_MANUAL_UPDATE, 1U);
	}
	if (flag & 0x80U) {
		ddr_phy_reg_write(3U, PHY_ADRCTL_MANUAL_UPDATE, 1U);
	}
}


static void pvt_freq_change_ack(uintptr_t phychno, uint32_t freq_flag)
{
	/* DFI FREQ */
	mmio_write_32(DBSC_DBPDCNT01 + (0x40U * phychno), freq_flag);

	/* FREQ CHANGE ACK */
	mmio_write_32(DBSC_DBPDCNT02 + (0x40U * phychno), 0x0CF20000U);
	mmio_write_32(DBSC_DBPDCNT02 + (0x40U * phychno), 0x00000000U);
}

static uint32_t pvt_pi_training_go_all(void)
{
	uint32_t dataL;
	uint32_t phytrainingok;
	uint32_t retry;
	uint32_t pll_status[4] = {0U, 0U, 0U, 0U};
	uint32_t wait_count[4];
	uintptr_t phychno;

	uint32_t init_start_deassert = 0U;

	if (init_start_deassert == 0U) {
		for (phychno = 0U; phychno < 4U; phychno++) {
			/* dfi_init_start negate */
			mmio_write_32(DBSC_DBDFICNT(phychno),
					   DBDFICNT_DFIBYTEDIS(0U)
					 | DBDFICNT_DFICLKDIS(0U)
					 | DBDFICNT_DFIFREQRATIO_1_2);
		}
	}

	for (phychno = 0U; phychno < 4U; phychno++) {
/*		ddr_phy_reg_rawwrite(phychno, 0x200, 0x00000200U); */
		ddr_phy_reg_write(phychno, PI_START, 1U);
		if (init_start_deassert == 1U) {
			/* dfi_init_start negate */
			mmio_write_32(DBSC_DBDFICNT(phychno),
					   DBDFICNT_DFIBYTEDIS(0U)
					 | DBDFICNT_DFICLKDIS(0U)
					 | DBDFICNT_DFIFREQRATIO_1_2);
		}
	}
	/* Report_DDRPHY_Check_BIST_PLL_LOCK (); */
	SoftDelay(20U); /* wait for 2usec */

	if (init_start_deassert == 2U) {
		for (phychno = 0U; phychno < 4U; phychno++) {
			/* dfi_init_start negate */
			mmio_write_32(DBSC_DBDFICNT(phychno),
					   DBDFICNT_DFIBYTEDIS(0U)
					 | DBDFICNT_DFICLKDIS(0U)
					 | DBDFICNT_DFIFREQRATIO_1_2);
		}
	}

	phytrainingok = 0U;
	retry = 0U;

	/* 0 : wait first unlock */
	/* 1 : pll is off */
	/* 2 : pll is on */

	pll_status[0] = 0U; /* wait unlock */
	pll_status[1] = 0U; /* wait unlock */
	pll_status[2] = 0U; /* wait unlock */
	pll_status[3] = 0U; /* wait unlock */

	while (1) {
		for (phychno = 0U; phychno < 4U; phychno++) {
			if ((phytrainingok & (1U << phychno)) == 1U) {
				continue;
			}

			if (pll_status[phychno] == 0U) {
				dataL = mmio_read_32(DBSC_PLL_LOCK(phychno)); /* Wait for PLL is unlock */
				if ((dataL & 0x0000001FU) == 0x0U) {
					pll_status[phychno] = 1U;
					wait_count[phychno] = 1000U;

					pvt_freq_change_ack(phychno, 0U);
					retry = 0U;
				}
			} else if (pll_status[phychno] == 1U) {
				dataL = mmio_read_32(DBSC_PLL_LOCK(phychno)); /* Wait for PLL is unlock */
				if ((dataL & 0x0000001FU) == 0x1FU) {
					pll_status[phychno] = 2U;
					wait_count[phychno] = 1000U;
					retry = 0U;
				} else {
					wait_count[phychno]--;
					if (wait_count[phychno] == 0U) {
						pvt_freq_change_ack(phychno,
								2U);
						wait_count[phychno] = 1000U;
						retry = 0U;
					}
				}
			} else {
				if (wait_count[phychno] > 0U) {
					wait_count[phychno]--;
					dataL = ddr_phy_reg_read(phychno,
								 PI_INT_STATUS);
					if (dataL & 0x1U) {
						phytrainingok |= (0x1U
								<< phychno);
						retry = 0U; /* TRAING OK */
					}
				} else {
					dataL = mmio_read_32( /* Check PLL is unlock again */
							DBSC_PLL_LOCK(phychno));
					if ((dataL & 0x0000001FU) == 0x00U) {
						pll_status[phychno] = 1U;
						pvt_freq_change_ack(phychno,
								0U);
						wait_count[phychno] = 1000U;
						retry = 0U;
					} else {
						wait_count[phychno] = 1000U;
					}
				}
			}
		}
		
		if ((phytrainingok & 0xfU) == 0xfU) {
			break;
		}
		retry++;
		if (retry == 10000U) {
			break;
		}
	}

	for (phychno = 0U; phychno < 4U; phychno++) {
		(void)ddr_phy_reg_read(0U, PI_INT_STATUS);
	}

	return phytrainingok;
}

static uint32_t pvt_pi_training(void)
{
	uintptr_t phychno;
	uint32_t phytrainingok = 0U;
	uint32_t i, j;
	uint32_t dataL;

	if (0) {
		for (phychno = 0U; phychno < 4U; phychno++) {
		}

		dataL = ddr_phy_reg_read(0U, PI_INT_STATUS);
		if (dataL & 0x1U) {
			phytrainingok |= 0x1U;
		}
		dataL = ddr_phy_reg_read(1U, PI_INT_STATUS);
		if (dataL & 0x1U) {
			phytrainingok |= 0x2U;
		}
		
		dataL = ddr_phy_reg_read(2U, PI_INT_STATUS);
		if (dataL & 0x1U) {
			phytrainingok |= 0x4U;
		}
		dataL = ddr_phy_reg_read(3U, PI_INT_STATUS);
		if (dataL & 0x1U) {
			phytrainingok |= 0x8U;
		}

		
		/* FREQ_SEL_INDEX=3 (not needed? because MULTICAST_EN=1)
		 * ==> NEED!! otherwise, copy will not copy */
		ddr_phy_reg_write(0U, PHY_FREQ_SEL_INDEX, 3U);
		ddr_phy_reg_write(1U, PHY_FREQ_SEL_INDEX, 3U);
		ddr_phy_reg_write(2U, PHY_FREQ_SEL_INDEX, 3U);
		ddr_phy_reg_write(3U, PHY_FREQ_SEL_INDEX, 3U);
		
		copy_dqdelay(1U, 1U, 2U); /* ch1 2:0:1:3 */
		copy_dqdelay(1U, 3U, 0U);
		copy_dqdelay(2U, 1U, 3U); /* ch2 3:1:2:0 */
		copy_dqdelay(2U, 2U, 0U);
		copy_dqdelay(3U, 1U, 3U); /* ch3 3:1:2:0 */
		copy_dqdelay(3U, 2U, 0U);
	
		for (phychno = 0U; phychno < 4U; phychno++) {
			for (i = 0U; i < ARRAY_SIZE(dqdelay_tbl[0]); i++) {
				for (j = 0U; j < 4U; j++) {
					dataL = ddr_phy_reg_read(phychno,
							dqdelay_tbl[j][i]);
				}
			}
		}

	} else {
		phytrainingok = pvt_pi_training_go_all();
		for (phychno = 0U; phychno < 4U; phychno++) {
		}

		/* FREQ_SEL_INDEX=3 (not needed? because MULTICAST_EN=1)
		 * ==> NEED!! otherwise, copy will not copy */
		ddr_phy_reg_write(0U, PHY_FREQ_SEL_INDEX, 3U);
		ddr_phy_reg_write(1U, PHY_FREQ_SEL_INDEX, 3U);
		ddr_phy_reg_write(2U, PHY_FREQ_SEL_INDEX, 3U);
		ddr_phy_reg_write(3U, PHY_FREQ_SEL_INDEX, 3U);
		
		copy_dqdelay(1U, 1U, 2U); /* ch1 2:0:1:3 */
		copy_dqdelay(1U, 3U, 0U);
		copy_dqdelay(2U, 1U, 3U); /* ch2 3:1:2:0 */
		copy_dqdelay(2U, 2U, 0U);
		copy_dqdelay(3U, 1U, 3U); /* ch3 3:1:2:0 */
		copy_dqdelay(3U, 2U, 0U);
	}

	return phytrainingok;
}

static void WaitDBCMD(void)
{
	uint32_t dataL;

	while (1) {
		dataL = mmio_read_32(DBSC_DBWAIT);
		if ((dataL & 0x00000001U) == 0x0U) {
			break;
		}
	}
}

static void DDRPHY_PVT_OVERWRITE(void)
{
	uint32_t j;
	uint32_t dataL;
	uint32_t pvtcode[4][3] = {{0x10U, 0x20U, 0x20U},
				  {0x10U, 0x20U, 0x20U},
				  {0x10U, 0x20U, 0x20U},
				  {0x10U, 0x20U, 0x20U}};
	
	for (j = 0U; j < PHY_MAX_NUM ; j++) {
		dataL = ddr_phy_reg_read(j, PHY_PAD_FDBK_TERM);
		dataL = (dataL & 0xfffe0000U) | ((pvtcode[j][0] << 12U)
					      |  (pvtcode[j][1] <<  6U)
					      |  (pvtcode[j][2] <<  0U));
		ddr_phy_reg_write(j, PHY_PAD_FDBK_TERM, dataL);
		dataL = ddr_phy_reg_read(j, PHY_PAD_DATA_TERM);
		dataL = (dataL & 0xfffe0000U) | ((pvtcode[j][0] << 12U)
					      |  (pvtcode[j][1] <<  6U)
					      |  (pvtcode[j][2] <<  0U));
		ddr_phy_reg_write(j, PHY_PAD_DATA_TERM, dataL);
		dataL = ddr_phy_reg_read(j, PHY_PAD_DQS_TERM);
		dataL = (dataL & 0xfffe0000U) | ((pvtcode[j][0] << 12U)
					      |  (pvtcode[j][1] <<  6U)
					      |  (pvtcode[j][2] <<  0U));
		ddr_phy_reg_write(j, PHY_PAD_DQS_TERM,  dataL);
		dataL = ddr_phy_reg_read(j, PHY_PAD_ADDR_TERM);
		dataL = (dataL & 0xfffe0000U) | ((pvtcode[j][0] << 12U)
					      |  (pvtcode[j][1] <<  6U)
					      |  (pvtcode[j][2] <<  0U));
		ddr_phy_reg_write(j, PHY_PAD_ADDR_TERM, dataL);
		dataL = ddr_phy_reg_read(j, PHY_PAD_CLK_TERM);
		dataL = (dataL & 0xfffe0000U) | ((pvtcode[j][0] << 12U)
					      |  (pvtcode[j][1] <<  6U)
					      |  (pvtcode[j][2] <<  0U));
		ddr_phy_reg_write(j, PHY_PAD_CLK_TERM,  dataL);
		dataL = ddr_phy_reg_read(j, PHY_PAD_CKE_TERM);
		dataL = (dataL & 0xfffe0000U) | ((pvtcode[j][0] << 12U)
					      |  (pvtcode[j][1] <<  6U)
					      |  (pvtcode[j][2] <<  0U));
		ddr_phy_reg_write(j, PHY_PAD_CKE_TERM,  dataL);
		dataL = ddr_phy_reg_read(j, PHY_PAD_RST_TERM);
		dataL = (dataL & 0xfffe0000U) | ((pvtcode[j][0] << 12U)
					      |  (pvtcode[j][1] <<  6U)
					      |  (pvtcode[j][2] <<  0));
		ddr_phy_reg_write(j, PHY_PAD_RST_TERM,  dataL);
		dataL = ddr_phy_reg_read(j, PHY_PAD_CS_TERM);
		dataL = (dataL & 0xfffe0000U) | ((pvtcode[j][0] << 12U)
					      |  (pvtcode[j][1] <<  6U)
					      |  (pvtcode[j][2] <<  0U));
		ddr_phy_reg_write(j, PHY_PAD_CS_TERM, dataL);
	}
}


static void change_lpddr4_en(uintptr_t phychno, uint32_t mode)
{
	uint32_t i, from, to;
	uint32_t set, clr;
	uint32_t dataL;
	uint32_t lpddr4_en_off_first = 0x2U;

	if (lpddr4_en_off_first == 0U) {
		return;
	}

	if (phychno >= 4U) {
		from = 0U;
		to = 3U;
	} else {
		from = phychno;
		to = phychno;
	}
	

	for (i = from; i <= to; i++) {
		if (lpddr4_en_off_first == 1U) {
			set = 1U << 14U;
			clr = ~set;
			if (!mode) {
				set = 0U;
			}
			dataL = ddr_phy_reg_read(i, PHY_PAD_ADDR_DRIVE);
			dataL = (dataL & clr) | set;
			ddr_phy_reg_write(i, PHY_PAD_ADDR_DRIVE, dataL);
			dataL = ddr_phy_reg_read(i, PHY_PAD_CLK_DRIVE);
			dataL = (dataL & clr) | set;
			ddr_phy_reg_write(i, PHY_PAD_CLK_DRIVE, dataL);
			dataL = ddr_phy_reg_read(i, PHY_PAD_CKE_DRIVE);
			dataL = (dataL & clr) | set;
			ddr_phy_reg_write(i, PHY_PAD_CKE_DRIVE, dataL);
			dataL = ddr_phy_reg_read(i, PHY_PAD_CS_DRIVE);
			dataL = (dataL & clr) | set;
			ddr_phy_reg_write(i, PHY_PAD_CS_DRIVE, dataL);
		} else {
			if (mode) {
				ddr_phy_reg_write(i,
						PHY_PAD_ADDR_DRIVE, 0x27F6EU);
				ddr_phy_reg_write(i,
						PHY_PAD_CLK_DRIVE,  0x47F6EU);
				ddr_phy_reg_write(i,
						PHY_PAD_CKE_DRIVE,  0x27F6EU);
				ddr_phy_reg_write(i,
						PHY_PAD_CS_DRIVE,   0x27F6EU);
			} else {
				ddr_phy_reg_write(i,
						PHY_PAD_ADDR_DRIVE, 0x27B22U);
				ddr_phy_reg_write(i,
						PHY_PAD_CLK_DRIVE,  0x47B22U);
				ddr_phy_reg_write(i,
						PHY_PAD_CKE_DRIVE,  0x27B22U);
				ddr_phy_reg_write(i,
						PHY_PAD_CS_DRIVE,   0x27B22U);
			}
		}
	}

}


static void copy_dqdelay(uintptr_t phychno, uint32_t s_byte, uint32_t d_byte)
{
	uint32_t i;
	uint32_t s_dataL;

	/* 43A-43E:PHY_CLK_WRDQx_SLAVE_DELAY_0 */
	/* 43F-442:PHY_RDDQx_SLAVE_DELAY_0 */
	/* 443-44B:PHY_RDDQS_DQx_FALL_SLAVE_DELAY_0 */

	for (i = 0U; i < ARRAY_SIZE(dqdelay_tbl[0]); i++) {
		s_dataL = ddr_phy_reg_read(phychno, dqdelay_tbl[s_byte][i]);
		ddr_phy_reg_write(phychno, dqdelay_tbl[d_byte][i], s_dataL);
	}
}


#ifdef DEBUG_SDRA_REG_PRINT
static void print_dbsc4_reg(uint32_t address)
{
	uint32_t data;

	data = mmio_read_32((uintptr_t)address);
	tf_printf("addr = 0x%x, data = 0x%x\n", address, data);
}
#endif /* DEBUG_SDRA_REG_PRINT */


#ifdef DEBUG_SDRA_REG_PRINT
static void print_phy_reg(uint32_t address)
{
	uint32_t data0;
	uint32_t data1;
	uint32_t data2;
	uint32_t data3;

	mmio_write_32(DBSC_DBPDRGA_0, address);
	mmio_write_32(DBSC_DBPDRGA_1, address);
	mmio_write_32(DBSC_DBPDRGA_2, address);
	mmio_write_32(DBSC_DBPDRGA_3, address);

	data0 = mmio_read_32(DBSC_DBPDRGA_0);
	data1 = mmio_read_32(DBSC_DBPDRGA_1);
	data2 = mmio_read_32(DBSC_DBPDRGA_2);
	data3 = mmio_read_32(DBSC_DBPDRGA_3);

	tf_printf("addr = 0x%x, data = 0x%x 0x%x 0x%x 0x%x\n",
			address, data0, data1, data2, data3);
}
#endif /* DEBUG_SDRA_REG_PRINT */


#ifdef DEBUG_SDRA_REG_PRINT
static void print_regs(void)
{
	uint32_t i;
	uint32_t m;
	uint32_t n;
	uint32_t p;
	uint32_t q;
	uint32_t md;
	uint32_t init_phy_num;
	uint32_t init_pi_num;

	print_dbsc4_reg(DBSC_DBSYSCONF0);
	print_dbsc4_reg(DBSC_DBSYSCONF1);
	print_dbsc4_reg(DBSC_DBPHYCONF0);
	print_dbsc4_reg(DBSC_DBKIND);

	for (m = 0U; m < 3U; m++) {
		for (n = 0U; n < 3U; n++) {
			print_dbsc4_reg(DBSC_DBMEMCONF_0_0
					+ (m * 0x10U)
					+ (n * 0x04U)
					+ 0x30U);
		}
	}

	print_dbsc4_reg(DBSC_DBINTEN);
	print_dbsc4_reg(DBSC_DBINTSTAT0);
	print_dbsc4_reg(DBSC_DBACEN);
	print_dbsc4_reg(DBSC_DBRFEN);
	print_dbsc4_reg(DBSC_DBWAIT);
	print_dbsc4_reg(DBSC_DBSYSCTRL0);

	for (i = 0U; i < 23U; i++) {
		print_dbsc4_reg(DBSC_DBTR0 + (i * 4U));
	}

	print_dbsc4_reg(DBSC_DBBL);
	print_dbsc4_reg(DBSC_DBRFCNF1);
	print_dbsc4_reg(DBSC_DBRFCNF2);
	print_dbsc4_reg(DBSC_DBRFCNF3);
	print_dbsc4_reg(DBSC_DBTSPCNF);
	print_dbsc4_reg(DBSC_DBCALCNF);
	print_dbsc4_reg(DBSC_DBCALTR);

	for (i = 0U; i < 6U; i++) {
		print_dbsc4_reg(DBSC_DBRNK0 + (i * 4U));
	}

	print_dbsc4_reg(DBSC_DBPDNCNF);

	for (p = 0U; p < 7U; p++) {
		print_dbsc4_reg(DBSC_DBODT0 + (p * 4U));
	}

	print_dbsc4_reg(DBSC_DBMRRDR);
	print_dbsc4_reg(DBSC_DBTSPDR);
	print_dbsc4_reg(DBSC_DBADJ0);
	print_dbsc4_reg(DBSC_DBADJ2);
	print_dbsc4_reg(DBSC_DBDBICNT);
	print_dbsc4_reg(DBSC_DBDFIPMSTRCNF);
	print_dbsc4_reg(DBSC_DBDFIPMSTRSTAT);
	print_dbsc4_reg(DBSC_DBDFILPCNF);

	for (m = 0U; m < 3U; m++) {
		print_dbsc4_reg(DBSC_DBDFISTAT0 + (m * 0x40U));
		print_dbsc4_reg(DBSC_DBDFICNT_0 + (m * 0x40U));
		print_dbsc4_reg(DBSC_DBPDCNT00 + (m * 0x40U));
		print_dbsc4_reg(DBSC_DBPDCNT01 + (m * 0x40U));
		print_dbsc4_reg(DBSC_DBPDCNT02 + (m * 0x40U));
		print_dbsc4_reg(DBSC_DBPDCNT03 + (m * 0x40U));
		print_dbsc4_reg(DBSC_DBPDLK_0 + (m * 0x40U));
	}

	print_dbsc4_reg(DBSC_DBBUS0CNF0);
	print_dbsc4_reg(DBSC_DBBUS0CNF1);
	print_dbsc4_reg(DBSC_DBCAM0CNF0);
	print_dbsc4_reg(DBSC_DBCAM0CNF1);
	print_dbsc4_reg(DBSC_DBCAM0CNF2);
	print_dbsc4_reg(DBSC_DBBCAMSWAP);
	print_dbsc4_reg(DBSC_DBBCAMDIS);
	print_dbsc4_reg(DBSC_DBSCHCNT0);
	print_dbsc4_reg(DBSC_DBSCHCNT1);
	print_dbsc4_reg(DBSC_DBSCHSZ0);
	print_dbsc4_reg(DBSC_DBSCHRW0);
	print_dbsc4_reg(DBSC_DBSCHRW1);

	for (q = 0U; q < 15U; q++) {
		print_dbsc4_reg(DBSC_DBSCHQOS_0_0 + (q * 0x10U));
		print_dbsc4_reg(DBSC_DBSCHQOS_0_1 + (q * 0x10U));
		print_dbsc4_reg(DBSC_DBSCHQOS_0_2 + (q * 0x10U));
		print_dbsc4_reg(DBSC_DBSCHQOS_0_3 + (q * 0x10U));
	}

	for (m = 0U; m < 3U; m++) {
		print_dbsc4_reg(DBSC_DBMRRDR0 + (m * 0x04U));
		print_dbsc4_reg(DBSC_DBDTMP0 + (m * 0x04U));
	}

	for (m = 0U; m < 3U; m++) {
		print_dbsc4_reg(DBSC_DBDQSOSC00 + (m * 0x08U));
		print_dbsc4_reg(DBSC_DBDQSOSC01 + (m * 0x08U));
	}
	for (m = 0U; m < 3U; m++) {
		print_dbsc4_reg(DBSC_DBOSCTHH00 + (m * 0x08U));
		print_dbsc4_reg(DBSC_DBOSCTHH01 + (m * 0x08U));
	}
	for (m = 0U; m < 3U; m++) {
		print_dbsc4_reg(DBSC_DBOSCTHL00 + (m * 0x08U));
		print_dbsc4_reg(DBSC_DBOSCTHL01 + (m * 0x08U));
	}

	print_dbsc4_reg(DBSC_DBFSFCONF0);


	md = (mmio_read_32(RST_MODEMR));
	if ((uint32_t)(md & MODEMR_DDR_CLOCK_MASK) == MODEMR_DDR_CLOCK_3200) {
		init_phy_num = INIT_DRAM_TBL_PHY_3200_NUM; /* MD19=0,MD17=0 */
		init_pi_num = INIT_DRAM_TBL_PI_3200_NUM;
	} else {
		init_phy_num = INIT_DRAM_TBL_PHY_1600_NUM; /* MD19=0,MD17=0 */
		init_pi_num = INIT_DRAM_TBL_PI_1600_NUM;
	}

	for (i = 0U; i < init_phy_num; i++) {
		print_phy_reg(INIT_DRAM_TBL_PHY_START + i);
	}

	for (i = 0U; i < init_pi_num; i++) {
		print_phy_reg(INIT_DRAM_TBL_PI_3200_NUM + i);
	}
}
#endif /* DEBUG_SDRA_REG_PRINT */


