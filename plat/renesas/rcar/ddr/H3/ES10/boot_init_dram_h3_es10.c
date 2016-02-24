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

#include "boot_init_dram_h3_es10.h"

#include <stdint.h>		//for uint32_t
#include <string.h>		//for uint32_t
#include <stdio.h>		//for uint32_t
#include "init_dram_tbl_h3_es10.h"
//#include "bit.h"
//#include "reg_rcarh3.h"


#define	RCAR_DDR_VERSION	"rev.0.10"


///////////////////////////////////////////////////////////
/* add start */
#include <debug.h>
#define	RST_BASE		(0xE6160000U)
#define	RST_MODEMR		(RST_BASE + 0x0060U)
#define	CPG_BASE		(0xE6150000U)
#define	CPG_PLLECR		(CPG_BASE + 0x00D0U)
#define	CPG_CPGWPR		(CPG_BASE + 0x900U)
#define	CPG_PLL3CR		(CPG_BASE + 0x0DCU)
#define	CPG_SRCR4		(CPG_BASE + 0x0BCU)
#define	CPG_SRSTCLR4		(CPG_BASE + 0x950U)
///////////////////////////////////////////////////////////


#define	DDR_LVLEN				0x1e
#define	DDR_FDIV				0x0
#define	DDR_FDIVA				0x0
#define	DDR_PLL3ONLY

#define	DDR_DRIVE				0x1
#define	DDR_LPDDR4_EN_OFF_FIRST	0x1
#define	DDR_LP4_BOOT_DISABLE	0x1

#define	DDR_PAD_BOOST			0x0
#define	DDR_PAD_ADDR_CS_DRIVE	0x7f2c

#define	DDR_PVTR_ADJ			0x8

#define	DDR_RX_CAL_MAN			0x033f

#define	DDR_MR3					0x31
#define	DDR_MR11				0x32	// 120 ohm
#define	DDR_MR22				0x06

#define	DDR_TSEL_SELECT			0x00002C0C

#define	DDR_VREF				0x0F1F

#define	DDR_PAD_CAL_WA

#define	DDR_UPDT_WA				0x1c48
#define	DDR_FREQCHG

#define	DDR_RDDQSDQ				0x0
#define	DDR_CALVLSIDE			0x2
#define	DDR_LOWDIV				0x4
#define	DDR_LOWFIX				0x17

#define	DDR_BPGRP				0x000
#define	DDR_BPCAD				0x000

#define	DDR_PHYVALID			0xf
#define	DDR_PHYMRW				0xf

#define	DDR_TVAL0				0
#define	DDR_TVAL1				0
#define	DDR_TVAL2				0
#define	DDR_DBS_DFI_WA

#define	DDR_CAD					0x200
#define	DDR_GRP_A				0x200
#define	DDR_GRP_B				0x200

#define	DDR_PVTCODE_0			0x10
#define	DDR_PVTCODE_1			0x20
#define	DDR_PVTCODE_2			0x20

#define DDR1600_CLK				0x2F		// 1600
#define DDR2400_CLK				0x47		// 2400
#define DDR2800_CLK				0x53		// 2800
#define DDR3200_CLK				0x5f		// 3100

// 9/24
#define DDR_CACS_CAPTURE_CNT	0x02
#define DDR_CACS_RESP_WAIT_CNT	0x00
#define DDR_CACS_CCCP			0x2816
#define DDR_CACS_START			0x280		// 0x320
#define DDR_CACS_QTR			0x80		// 0x40
#define DDR_CACS_STEP			0x03

#define DBSC_DBSYSCONF1			0xE6790004
#define DBSC_DBPHYCONF0			0xE6790010 //
#define DBSC_DBKIND				0xE6790020 //

#define DBSC_DBMEMCONF_0_0		0xE6790030
#define DBSC_DBMEMCONF_0_1		0xE6790034
#define DBSC_DBMEMCONF_1_0		0xE6790040
#define DBSC_DBMEMCONF_1_1		0xE6790044
#define DBSC_DBMEMCONF_2_0		0xE6790050
#define DBSC_DBMEMCONF_2_1		0xE6790054
#define DBSC_DBMEMCONF_3_0		0xE6790060
#define DBSC_DBMEMCONF_3_1		0xE6790064

#define DBSC_DBMEMCONF_0_2		0xE6790038
#define DBSC_DBMEMCONF_0_3		0xE679003C
#define DBSC_DBMEMCONF_1_2		0xE6790048
#define DBSC_DBMEMCONF_1_3		0xE679004C
#define DBSC_DBMEMCONF_2_2		0xE6790058
#define DBSC_DBMEMCONF_2_3		0xE679005C
#define DBSC_DBMEMCONF_3_2		0xE6790068
#define DBSC_DBMEMCONF_3_3		0xE679006C

#define DBSC_DBSTATE0			0xE6790108

#define DBSC_DBACEN				0xE6790200
#define DBSC_DBRFEN				0xE6790204
#define DBSC_DBCMD				0xE6790208
#define DBSC_DBWAIT				0xE6790210	//wait DBCMD 1=busy, 0=ready

#define DBSC_DBTR0				0xE6790300
#define DBSC_DBTR1				0xE6790304
#define DBSC_DBTR3				0xE679030C
#define DBSC_DBTR4				0xE6790310
#define DBSC_DBTR5				0xE6790314
#define DBSC_DBTR6				0xE6790318
#define DBSC_DBTR7				0xE679031C
#define DBSC_DBTR8				0xE6790320
#define DBSC_DBTR9				0xE6790324
#define DBSC_DBTR10				0xE6790328
#define DBSC_DBTR11				0xE679032C
#define DBSC_DBTR12				0xE6790330
#define DBSC_DBTR13				0xE6790334
#define DBSC_DBTR14				0xE6790338
#define DBSC_DBTR15				0xE679033C
#define DBSC_DBTR16				0xE6790340
#define DBSC_DBTR17				0xE6790344
#define DBSC_DBTR18				0xE6790348
#define DBSC_DBTR19				0xE679034C
#define DBSC_DBTR20				0xE6790350
#define DBSC_DBTR21				0xE6790354
#define DBSC_DBTR22				0xE6790358

#define DBSC_DBBL				0xE6790400

#define DBSC_DBRFCNF1			0xE6790414
#define DBSC_DBRFCNF2			0xE6790418

#define DBSC_DBRNK0				0xE6790430
#define DBSC_DBRNK1				0xE6790434
#define DBSC_DBRNK2				0xE6790438
#define DBSC_DBRNK3				0xE679043C
#define DBSC_DBRNK4				0xE6790440
#define DBSC_DBRNK5				0xE6790444
#define DBSC_DBRNK6				0xE6790448

#define DBSC_DBADJ0				0xE6790500
#define DBSC_DBADJ2				0xE6790508

#define DBSC_DBDFIPMSTRCNF		0xE6790520

#define DBSC_DBPDLK_0			0xE6790620
#define DBSC_DBPDLK_1			0xE6790660
#define DBSC_DBPDLK_2			0xE67906a0
#define DBSC_DBPDLK_3			0xE67906e0

#define DBSC_INITCOMP_0			0xE6790600
#define DBSC_INITCOMP_1			0xE6790640
#define DBSC_INITCOMP_2			0xE6790680
#define DBSC_INITCOMP_3			0xE67906C0

#define DBSC_DBDFICNT_0			0xE6790604
#define DBSC_DBDFICNT_1			0xE6790644
#define DBSC_DBDFICNT_2			0xE6790684
#define DBSC_DBDFICNT_3			0xE67906C4

#define DBSC_DBPDCNT0_0			0xE6790610
#define DBSC_DBPDCNT0_1			0xE6790650
#define DBSC_DBPDCNT0_2			0xE6790690
#define DBSC_DBPDCNT0_3			0xE67906D0

#define DBSC_DBPDCNT_0			0xE679061C
#define DBSC_DBPDCNT_1			0xE679065C
#define DBSC_DBPDCNT_2			0xE679069C
#define DBSC_DBPDCNT_3			0xE67906DC

#define DBSC_DBPDRGA_0			0xE6790624  //
#define DBSC_DBPDRGD_0			0xE6790628  //
#define DBSC_DBPDRGA_1			0xE6790664  //
#define DBSC_DBPDRGD_1			0xE6790668  //
#define DBSC_DBPDRGA_2			0xE67906A4  //
#define DBSC_DBPDRGD_2			0xE67906A8  //
#define DBSC_DBPDRGA_3			0xE67906E4  //
#define DBSC_DBPDRGD_3			0xE67906E8  //

#define DBSC_DBBUS0CNF0			0xE6790800
#define DBSC_DBBUS0CNF1			0xE6790804

#define DBSC_DBCAM0CNF0			0xE6790900
#define DBSC_DBCAM0CNF1			0xE6790904
#define DBSC_DBCAM0CNF2			0xE6790908
#define DBSC_DBCAM0CNF3			0xE679090C
#define DBSC_DBBCAMDIS			0xE67909FC
#define DBSC_DBSCHCNT0			0xE6791000
#define DBSC_DBSCHCNT1			0xE6791004
#define DBSC_DBSCHSZ0			0xE6791010
#define DBSC_DBSCHRW0			0xE6791020
#define DBSC_DBSCHRW1			0xE6791024

#define DBSC_DBSCHQOS_0_0		0xE6791030
#define DBSC_DBSCHQOS_0_1		0xE6791034
#define DBSC_DBSCHQOS_0_2		0xE6791038
#define DBSC_DBSCHQOS_0_3		0xE679103C
#define DBSC_DBSCHQOS_4_0		0xE6791070
#define DBSC_DBSCHQOS_4_1		0xE6791074
#define DBSC_DBSCHQOS_4_2		0xE6791078
#define DBSC_DBSCHQOS_4_3		0xE679107C
#define DBSC_DBSCHQOS_9_0		0xE67910C0
#define DBSC_DBSCHQOS_9_1		0xE67910C4
#define DBSC_DBSCHQOS_9_2		0xE67910C8
#define DBSC_DBSCHQOS_9_3		0xE67910CC
#define DBSC_DBSCHQOS_13_0		0xE6791100
#define DBSC_DBSCHQOS_13_1		0xE6791104
#define DBSC_DBSCHQOS_13_2		0xE6791108
#define DBSC_DBSCHQOS_13_3		0xE679110C
#define DBSC_DBSCHQOS_14_0		0xE6791110
#define DBSC_DBSCHQOS_14_1		0xE6791114
#define DBSC_DBSCHQOS_14_2		0xE6791118
#define DBSC_DBSCHQOS_14_3		0xE679111C
#define DBSC_DBSCHQOS_15_0		0xE6791120
#define DBSC_DBSCHQOS_15_1		0xE6791124
#define DBSC_DBSCHQOS_15_2		0xE6791128
#define DBSC_DBSCHQOS_15_3		0xE679112C

#define DBSC_DBSCTR0			0xE6791700
#define DBSC_DBSCTR1			0xE6791708
#define DBSC_DBSCHRW2			0xE679170C

#define DBSC_SCFCTST0 0xE6791700	//Schedule timing setting register 0
#define DBSC_SCFCTST1 0xE6791708	//Schedule timing setting register 1
#define DBSC_SCFCTST2 0xE679170C	//Schedule timing setting register 2

#define TSREG1					0xE60603E8	// Fuse Monitor Register 1

#define DBSC_PLL_LOCK_0 		0xE6794054
#define DBSC_PLL_LOCK_1 		0xE6794154
#define DBSC_PLL_LOCK_2 		0xE6794254
#define DBSC_PLL_LOCK_3 		0xE6794354

#define DBSC_FREQ_CHG_ACK_0 	0xE6790618
#define DBSC_FREQ_CHG_ACK_1 	0xE6790658
#define DBSC_FREQ_CHG_ACK_2 	0xE6790698
#define DBSC_FREQ_CHG_ACK_3 	0xE67906D8

#define DBSC_DFI_FREQ_0 		0xE6790614
#define DBSC_DFI_FREQ_1 		0xE6790654
#define DBSC_DFI_FREQ_2 		0xE6790694
#define DBSC_DFI_FREQ_3 		0xE67906D4



////////////////////////////////////////////////////////////
// 2800: RL=28,WL=14,nWR=30,nRTP=12
#define DDR2800_PI_MR1				0xd4	//DeviceFeature1(Post=1.5tck nWR=30 RDpre=static WRPre=2tCK BL=16		// 1 101 0100
#define DDR2800_PI_MR2				0x2d	//DeviceFeature2(0,0SetA,101=WL14,101=RL28)					// 0 0 101 101
#define DDR2800_PI_TFC				0x00c8	//	xxxx
#define DDR2800_PI_TRTP				0x0B
#define DDR2800_PI_TCCD				0x08
#define DDR2800_PI_TWR				0x1A
//#define DDR2800_PI_TWTR				0x25
#define DDR2800_PI_TWTR				0x0E
#define DDR2800_PI_TRCD				0x1A
#define DDR2800_PI_TRP				0x1E
#define DDR2800_PI_TRAS_MIN			0x3B
#define DDR2800_PI_TRAS_MAX			0x0db60	//	xxxx
#define DDR2800_PI_TMRW				0x08	//	xxxx
#define DDR2800_PI_TMRD				0x0c	//	xxxx
#define DDR2800_PI_TCCDMW			0x20
#define DDR2800_PI_TDQSCK_MAX		0x03
#define DDR2800_PI_RDLAT_ADJ		0x10	//???
#define DDR2800_PI_CASLAT_LIN		0x50
#define DDR2800_PI_WRLAT			0x0e
#define DDR2800_PI_WRLAT_ADJ		0x0c


///////////////////////////////////////////////////////////
// 2400: RL=24,WL=12,nWR=24,nRTP=10
#define DDR2400_PI_MR1				0xc4	//DeviceFeature1(Post=1.5tck nWR=24 RDpre=static WRPre=2tCK BL=16		// 1 100 0100
#define DDR2400_PI_MR2				0x24	//DeviceFeature2(0,0SetA,100=WL12,100=RL24)					// 0 0 100 100
#define DDR2400_PI_TFC				0x00c8	//	xxxx
#define DDR2400_PI_TRTP				0x09
#define DDR2400_PI_TCCD				0x08
#define DDR2400_PI_TWR				0x16
#define DDR2400_PI_TWTR				0x0c
#define DDR2400_PI_TRCD				0x16
#define DDR2400_PI_TRP				0x1a
#define DDR2400_PI_TRAS_MIN			0x33
#define DDR2400_PI_TRAS_MAX			0x0db60	//	xxxx
#define DDR2400_PI_TMRW				0x08	//	xxxx
#define DDR2400_PI_TMRD				0x0c	//	xxxx
#define DDR2400_PI_TCCDMW			0x20
#define DDR2400_PI_TDQSCK_MAX		0x03
#define DDR2400_PI_RDLAT_ADJ		0x0e
#define DDR2400_PI_CASLAT_LIN		0x50	//PI_CASLAT_LIN_F2:RW:0:7:=0x1c
#define DDR2400_PI_WRLAT			0x0c	//PI_WRLAT_F2:RW:16:5:=0x08
#define DDR2400_PI_WRLAT_ADJ		0x0a
/////////////////
#define DDR1600_PI_MR1				0xa4	//MRW DeviceFeature1(Post=1.5tck nWR=16 RDpre=static WRPre=2tCK BL=16//OK		// 1 010 0100
#define DDR1600_PI_MR2				0x52	//MRW DeviceFeature2(0,1SetB,010=WL12,010=RL14(nRTP14))//						// 01 010 010
#define DDR1600_PI_TFC				0x00c8
#define DDR1600_PI_TRTP				0x08
#define DDR1600_PI_TCCD				0x08
#define DDR1600_PI_TWR				0x11
#define DDR1600_PI_TWTR				0x0a
#define DDR1600_PI_TRCD				0x0f
#define DDR1600_PI_TRP				0x11
#define DDR1600_PI_TRAS_MIN			0x22
#define DDR1600_PI_TRAS_MAX			0x0db60
#define DDR1600_PI_TMRW				0x08	//10
#define DDR1600_PI_TMRD				0x0c
#define DDR1600_PI_TCCDMW			0x20
#define DDR1600_PI_TDQSCK_MAX		0x3
#define DDR1600_PI_RDLAT_ADJ		0x08	//PI_RDLAT_ADJ_F2:RW:16:8:=0x07       //try 07+2 1600
#define DDR1600_PI_CASLAT_LIN		0x50	//PI_CASLAT_LIN_F2:RW:0:7:=0x1c
#define DDR1600_PI_WRLAT			0x0c	//PI_WRLAT_F2:RW:16:5:=0x08
#define DDR1600_PI_WRLAT_ADJ		0x0a

//////////////////////////////////////////////////////////////////

void InitDram_h3_es10(void);

static void pvt_dbsc_regset(uint32_t freq);
static void pvt_manual_update(uint32_t flag);
static void set_cacs_delay();
static void _rx_cal_code_set(uint32_t ch, uint32_t slice, uint8_t _cal_code_up[][4][16],uint8_t _cal_code_dn[][4][16]);
static void rx_cal_manual1(uint32_t chmask);
static void mode_register_set(uint32_t freq,uintptr_t phychno,uint32_t mr13_val);
static void _set_reg(uint32_t ch_mask, uint32_t regadd, uint32_t pos, uint32_t width, uint32_t val);
static void pvt_lvl_enable_set(uint32_t go);
static void pvt_freq_change_ack(uint32_t freq, uintptr_t phychno, uint32_t freq_flag);
static uint32_t pvt_pi_training_go_all_o(uint32_t freq);

inline void PLL3_FREQ(uint32_t freq,uint32_t freq_flag);
static uint32_t pvt_pi_training_go_all(uint32_t freq);
static uint32_t pvt_pi_training(uint32_t freq);
static void WaitDBCMD(void);

static void REG_DDRPHY_WRITE ( uintptr_t phyno, uint32_t regadd, uint32_t regdata);
static void change_lpddr4_en(uintptr_t phychno, uint32_t mode);
static void copy_csdelay(uint32_t	phychno, uint32_t s_byte);
static void adjust_rddqsgatedelay(uintptr_t phychno);
static void copy_dqdelay(uintptr_t phychno, uint32_t s_byte, uint32_t d_byte);
static uint32_t pvt_pi_training_go_all_soft(uint32_t freq);

inline void dsb_sev(void);

static uint32_t REG_DDRPHY_READ ( uintptr_t phyno, uint32_t regadd);
static uint32_t InitDDR_0917(uint32_t freq);
//void freq2800_reg_set();
//void freq2400_reg_set();
static void freq1600_reg_set();


uint8_t cal_code_up[4][4][16];//[11-15]are dummy
uint8_t cal_code_dn[4][4][16];//[11-15]are dummy
uint8_t fst1[4][4][16];//[11-15]are dummy
uint8_t lst0[4][4][16];//[11-15]are dummy
uint32_t val[4][4][16][4];

void SoftDelay(uint32_t loop)
{
	volatile uint32_t i;
	for(i=0;i<loop;i++);
}

///////////////////////////////////////////////////////////////////

void InitDram_h3_es10(void)
{
	uint32_t md=0;
	uint32_t freq=DDR3200_CLK;

	md = (*((volatile uint32_t*)RST_MODEMR) & 0x000A0000) >> 17;

	if (md == 0x0) {
		NOTICE("BL2: DDR3200(%s)\n", RCAR_DDR_VERSION);
	} else if (md == 0x1) {
		freq = DDR2800_CLK;	//MD19=0,MD17=1 : LPDDR4-3000, 4GByte(1GByte x4)
		NOTICE("BL2: DDR2800(%s)\n", RCAR_DDR_VERSION);
	} else if (md == 0x4) {
		freq = DDR2400_CLK;	//MD19=1,MD17=0 : LPDDR4-2400, 4GByte(1GByte x4)
		NOTICE("BL2: DDR2400(%s)\n", RCAR_DDR_VERSION);
	} else {
		freq = DDR1600_CLK;	//MD19=1,MD17=1 : LPDDR4-1600, 4GByte(1GByte x4)
		NOTICE("BL2: DDR1600(%s)\n", RCAR_DDR_VERSION);
	}
	InitDDR_0917(freq);
}


static void pvt_dbsc_regset(uint32_t freq)
{

	*((volatile uint32_t*)DBSC_DBPDCNT0_0)	= 0x00;
	*((volatile uint32_t*)DBSC_DBPDCNT0_1)	= 0x00;
	*((volatile uint32_t*)DBSC_DBPDCNT0_2)	= 0x00;
	*((volatile uint32_t*)DBSC_DBPDCNT0_3)	= 0x00;

	//LPDDR4-1600(CL14NoDBI),4GByte(8GBit x 4)

	*((volatile uint32_t*)DBSC_DBSTATE0)	= 0x00000010;
	*((volatile uint32_t*)DBSC_DBKIND)		= 0x0000000a;	//ddcg=a(lpddr4)

	*((volatile uint32_t*)DBSC_DBBL)		= 0x00000002;	//BL=16(lpddr4)

//DRAM Size Registers
//Please set for 4ranks
	*((volatile uint32_t*)DBSC_DBMEMCONF_0_0)	= 0x0f030a02;	//memconf00(ch0/rank0) 4GB
	*((volatile uint32_t*)DBSC_DBMEMCONF_0_1)	= 0x0f030a02;	//memconf01(ch0/rank1) 4GB
	*((volatile uint32_t*)DBSC_DBMEMCONF_1_0)	= 0x0f030a02;	//memconf10(ch1/rank0) 4GB
	*((volatile uint32_t*)DBSC_DBMEMCONF_1_1)	= 0x0f030a02;	//memconf11(ch1/rank1) 4GB
	*((volatile uint32_t*)DBSC_DBMEMCONF_2_0)	= 0x0f030a02;	//memconf20(ch2/rank0) 4GB
	*((volatile uint32_t*)DBSC_DBMEMCONF_2_1)	= 0x0f030a02;	//memconf21(ch2/rank1) 4GB
	*((volatile uint32_t*)DBSC_DBMEMCONF_3_0)	= 0x0f030a02;	//memconf30(ch3/rank0) 4GB
	*((volatile uint32_t*)DBSC_DBMEMCONF_3_1)	= 0x0f030a02;	//memconf31(ch3/rank1) 4GB

	*((volatile uint32_t*)DBSC_DBMEMCONF_0_2)	= 0x0f030a02;	//memconf01(ch0/rank2) 4GB
	*((volatile uint32_t*)DBSC_DBMEMCONF_0_3)	= 0x0f030a02;	//memconf01(ch0/rank3) 4GB
	*((volatile uint32_t*)DBSC_DBMEMCONF_1_2)	= 0x0f030a02;	//memconf01(ch1/rank2) 4GB
	*((volatile uint32_t*)DBSC_DBMEMCONF_1_3)	= 0x0f030a02;	//memconf01(ch1/rank3) 4GB
	*((volatile uint32_t*)DBSC_DBMEMCONF_2_2)	= 0x0f030a02;	//memconf01(ch2/rank2) 4GB
	*((volatile uint32_t*)DBSC_DBMEMCONF_2_3)	= 0x0f030a02;	//memconf01(ch2/rank3) 4GB
	*((volatile uint32_t*)DBSC_DBMEMCONF_3_2)	= 0x0f030a02;	//memconf01(ch3/rank2) 4GB
	*((volatile uint32_t*)DBSC_DBMEMCONF_3_3)	= 0x0f030a02;	//memconf01(ch3/rank3) 4GB

	*((volatile uint32_t*)DBSC_DBPHYCONF0)	= 0x00000001;	//phyconf0

	if(freq>0x53)
	{
		// DDR3200
		*((volatile uint32_t*)DBSC_DBTR0)	= 0x00000020;	//dbtr0  cl=32
		*((volatile uint32_t*)DBSC_DBTR1)	= 0x0000000e;	//dbtr1  cwl=14
		*((volatile uint32_t*)DBSC_DBTR3)	= 0x0000001d;	//dbtr3  trcd=29
		*((volatile uint32_t*)DBSC_DBTR4)	= 0x0022001d;	//dbtr4  trpa=34 trp=29
		*((volatile uint32_t*)DBSC_DBTR5)	= 0x00000060;	//dbtr5  trc=102
		*((volatile uint32_t*)DBSC_DBTR6)	= 0x00000044;	//dbtr6  tras=68
		*((volatile uint32_t*)DBSC_DBTR7)	= 0x00100010;	//dbtr7  trrd_s=16 trrd=16
		*((volatile uint32_t*)DBSC_DBTR8)	= 0x00000040;	//dbtr8  tfaw=64
		*((volatile uint32_t*)DBSC_DBTR9)	= 0x0000000c;	//dbtr9  trdpr=12
		*((volatile uint32_t*)DBSC_DBTR10)	= 0x0000001d;	//dbtr10 twr=30
//NG:	*((volatile uint32_t*)DBSC_DBTR11)	= 0x00000021;	//dbtr11 trdwr=33
		*((volatile uint32_t*)DBSC_DBTR11)	= 0x00000025;	//dbtr11 trdwr=37
		*((volatile uint32_t*)DBSC_DBTR12)	= 0x00270027;	//dbtr12 twrrd_s=39 twrrd=39
		*((volatile uint32_t*)DBSC_DBTR13)	= 0x00900120;	//dbtr13 trfcpb=96 trfc=208
		*((volatile uint32_t*)DBSC_DBTR14)	= 0x000c000c;	//dbtr14 tckehdll=12 tckeh=12
		*((volatile uint32_t*)DBSC_DBTR15)	= 0x001e000c;	//dbtr15 tckesr=30 tckel=12   currently tckesr is TBD.

		*((volatile uint32_t*)DBSC_DBTR16)	= 0x10400c0e;	//dbtr16 latency?  CL=32 & PHY_RDDATA_EN_DLY_X = 0e (32+2-14=20), CWL=14 & TDFI_PHY_WRDATA=2 
		*((volatile uint32_t*)DBSC_DBTR17)	= 0x080a0004;	//dbtr17 tmodrd=8  tmod=10 trdmr=4
		*((volatile uint32_t*)DBSC_DBTR18)	= 0x00000000;	//dbtr18 No odt for DQ exists in LPDDR4.
		*((volatile uint32_t*)DBSC_DBTR19)	= 0x00000000;	//dbtr19 tzqcl=0 tzqcs=0 because LPDDR4 doesn't support ZQC[SL].
		*((volatile uint32_t*)DBSC_DBTR20)	= 0x012c012c;	//dbtr20 txsdll=trfc+12=220 txs=220
//		*((volatile uint32_t*)DBSC_DBTR21)	= 0x00080008;	//dbtr21 tccd_s= 8 tccd= 8
//		*((volatile uint32_t*)DBSC_DBTR21)	= 0x000C000C;	//dbtr21 tccd_s=12 tccd=12	u-boot ng			// for lowfreq
//		*((volatile uint32_t*)DBSC_DBTR21)	= 0x000d000d;	//dbtr21 tccd_s=13 tccd=13	u-boot ng			// for lowfreq
//		*((volatile uint32_t*)DBSC_DBTR21)	= 0x000e000e;	//dbtr21 tccd_s=14 tccd=14	u-boot ok			// for lowfreq
		*((volatile uint32_t*)DBSC_DBTR21)	= 0x00100010;	//dbtr21 tccd_s=16 tccd=16	u-boot ok			// for lowfreq
		*((volatile uint32_t*)DBSC_DBTR22)	= 0x06400030;	//dbtr22 tzqcal=1600 tzqlat=48

	} 
	else if(freq>0x47)
	{
		// DDR2800
		*((volatile uint32_t*)DBSC_DBTR0)	= 0x0000001c;	//dbtr0  cl=28
		*((volatile uint32_t*)DBSC_DBTR1)	= 0x0000000e;	//dbtr1  cwl=14
		*((volatile uint32_t*)DBSC_DBTR3)	= 0x0000001a;	//dbtr3  trcd=26
		*((volatile uint32_t*)DBSC_DBTR4)	= 0x001e001a;	//dbtr4  trpa=30 trp=26
//		*((volatile uint32_t*)DBSC_DBTR5)	= 0x00000059;	//dbtr5  trc=89
		*((volatile uint32_t*)DBSC_DBTR5)	= 0x00000055;	//dbtr5  trc=85
		*((volatile uint32_t*)DBSC_DBTR6)	= 0x0000003b;	//dbtr6  tras=59
//		*((volatile uint32_t*)DBSC_DBTR7)	= 0x000e000e;	//dbtr7  trrd_s=14 trrd=14
		*((volatile uint32_t*)DBSC_DBTR7)	= 0x000f000f;	//dbtr7  trrd_s=15 trrd=15
//		*((volatile uint32_t*)DBSC_DBTR8)	= 0x00000038;	//dbtr8  tfaw=56
		*((volatile uint32_t*)DBSC_DBTR8)	= 0x00000039;	//dbtr8  tfaw=57
//		*((volatile uint32_t*)DBSC_DBTR9)	= 0x0000000b;	//dbtr9  trdpr=11
		*((volatile uint32_t*)DBSC_DBTR9)	= 0x0000000c;	//dbtr9  trdpr=12
//		*((volatile uint32_t*)DBSC_DBTR10)	= 0x0000001b;	//dbtr10 twr=27
		*((volatile uint32_t*)DBSC_DBTR10)	= 0x0000001E;	//dbtr10 twr=30, nWR is 26, but its value is not defined in MR1.nWR
//		*((volatile uint32_t*)DBSC_DBTR11)	= 0x0000001e;	//dbtr11 trdwr=30
//NG:	*((volatile uint32_t*)DBSC_DBTR11)	= 0x00000020;	//dbtr11 trdwr=32
		*((volatile uint32_t*)DBSC_DBTR11)	= 0x00000022;	//dbtr11 trdwr=33
//		*((volatile uint32_t*)DBSC_DBTR12)	= 0x00250025;	//dbtr12 twrrd_s=37 twrrd=37
		*((volatile uint32_t*)DBSC_DBTR12)	= 0x00260026;	//dbtr12 twrrd_s=38 twrrd=38
		*((volatile uint32_t*)DBSC_DBTR13)	= 0x007f00fd;	//dbtr13 trfcpb=127 trfc=253
		*((volatile uint32_t*)DBSC_DBTR14)	= 0x000c000c;	//dbtr14 tckehdll=12 tckeh=12
		*((volatile uint32_t*)DBSC_DBTR15)	= 0x0016000b;	//dbtr15 tckesr=22 tckel=11 

		*((volatile uint32_t*)DBSC_DBTR16)	= 0x101f0c0e;	//dbtr16 latency?  CL=32 & PHY_RDDATA_EN_DLY_X = 0e (32+2-14=20), CWL=14 & TDFI_PHY_WRDATA=2 
		*((volatile uint32_t*)DBSC_DBTR17)	= 0x0c0f0020;	//dbtr17 tmodrd=12  tmod=15 trdmr=32
		*((volatile uint32_t*)DBSC_DBTR18)	= 0x00000000;	//dbtr18 No odt for DQ exists in LPDDR4.
		*((volatile uint32_t*)DBSC_DBTR19)	= 0x00000000;	//dbtr19 tzqcl=0 tzqcs=0 because LPDDR4 doesn't support ZQC[SL].
		*((volatile uint32_t*)DBSC_DBTR20)	= 0x01070107;	//dbtr20 txsdll=txs=tRFCab+7.5ns=263
//		*((volatile uint32_t*)DBSC_DBTR21)	= 0x00080008;	//dbtr21 tccd_s= 8 tccd= 8
//		*((volatile uint32_t*)DBSC_DBTR21)	= 0x000b000b;	//dbtr21 tccd_s=11 tccd=11 But PHY may restrict tccd+1/+2, so set it big enough.
//		*((volatile uint32_t*)DBSC_DBTR21)	= 0x000C000C;	//dbtr21 tccd_s=12 tccd=12	u-boot ng			// for lowfreq
//		*((volatile uint32_t*)DBSC_DBTR21)	= 0x000d000d;	//dbtr21 tccd_s=16 tccd=13	u-boot ok			// for lowfreq
		*((volatile uint32_t*)DBSC_DBTR21)	= 0x000e000e;	//dbtr21 tccd_s=16 tccd=14	u-boot ok			// for lowfreq
//		*((volatile uint32_t*)DBSC_DBTR21)	= 0x00100010;	//dbtr21 tccd_s=16 tccd=16	u-boot ok			// for lowfreq
		*((volatile uint32_t*)DBSC_DBTR22)	= 0x06400030;	//dbtr22 tzqcal=1600 tzqlat=48

	} 
	else if(freq>0x2f)
	{
		// DDR2400
		*((volatile uint32_t*)DBSC_DBTR0)	= 0x00000018;	//dbtr0  cl=24
		*((volatile uint32_t*)DBSC_DBTR1)	= 0x0000000c;	//dbtr1  cwl=12
		*((volatile uint32_t*)DBSC_DBTR3)	= 0x00000016;	//dbtr3  trcd=22
		*((volatile uint32_t*)DBSC_DBTR4)	= 0x001a0016;	//dbtr4  trpa=26 trp=22
		*((volatile uint32_t*)DBSC_DBTR5)	= 0x00000049;	//dbtr5  trc=73
		*((volatile uint32_t*)DBSC_DBTR6)	= 0x00000033;	//dbtr6  tras=51
		*((volatile uint32_t*)DBSC_DBTR7)	= 0x000d000d;	//dbtr7  trrd_s=13 trrd=13
		*((volatile uint32_t*)DBSC_DBTR8)	= 0x00000031;	//dbtr8  tfaw=49
		*((volatile uint32_t*)DBSC_DBTR9)	= 0x0000000a;	//dbtr9  trdpr=10
		*((volatile uint32_t*)DBSC_DBTR10)	= 0x00000016;	//dbtr10 twr=22
		*((volatile uint32_t*)DBSC_DBTR11)	= 0x0000001c;	//dbtr11 trdwr=28   PHY may have restriction of this value.
		*((volatile uint32_t*)DBSC_DBTR12)	= 0x00220022;	//dbtr12 twrrd_s=34 twrrd=34   PHY may restriction of this value.
		*((volatile uint32_t*)DBSC_DBTR13)	= 0x006d00d9;	//dbtr13 trfcpb=109 trfc=217
		*((volatile uint32_t*)DBSC_DBTR14)	= 0x000a000a;	//dbtr14 tckehdll=10 tckeh=10
		*((volatile uint32_t*)DBSC_DBTR15)	= 0x0013000c;	//dbtr15 tckesr=19 tckel=12 

		*((volatile uint32_t*)DBSC_DBTR16)	= 0x08400a0c;	//dbtr16 latency?  CL=24 & PHY_RDDATA_EN_DLY_X = 0e (???), CWL=12 & TDFI_PHY_WRDATA=2 
		*((volatile uint32_t*)DBSC_DBTR17)	= 0x080a0004;	//dbtr17 tmodrd=8  tmod=10 trdmr=4
		*((volatile uint32_t*)DBSC_DBTR18)	= 0x00000000;	//dbtr18 No odt for DQ exists in LPDDR4.
		*((volatile uint32_t*)DBSC_DBTR19)	= 0x00000000;	//dbtr19 tzqcl=0 tzqcs=0 because LPDDR4 doesn't support ZQC[SL].
		*((volatile uint32_t*)DBSC_DBTR20)	= 0x00e200e2;	//dbtr20 txsdll=txs=tRFCab+7.5ns=226
//		*((volatile uint32_t*)DBSC_DBTR21)	= 0x00080008;	//dbtr21 tccd_s=8 tccd=8
//		*((volatile uint32_t*)DBSC_DBTR21)	= 0x000b000b;	//dbtr21 tccd_s=11 tccd=11 But PHY may restrict tccd+1/+2, so set it big enough.
		*((volatile uint32_t*)DBSC_DBTR21)	= 0x000C000C;	//dbtr21 tccd_s=12 tccd=12	u-boot ok			// for lowfreq
//		*((volatile uint32_t*)DBSC_DBTR21)	= 0x00100010;	//dbtr21 tccd_s=16 tccd=16	u-boot ok			// for lowfreq
		*((volatile uint32_t*)DBSC_DBTR22)	= 0x04b10025;	//dbtr22 tzqcal=1201 tzqlat=37

	}
	else
	{
		// DDR1600
		*((volatile uint32_t*)DBSC_DBTR0)	= 0x0000000e;	//dbtr0  cl=14
		*((volatile uint32_t*)DBSC_DBTR1)	= 0x0000000c;	//dbtr1  cwl=c
		*((volatile uint32_t*)DBSC_DBTR3)	= 0x0000000f;	//dbtr3  trcd=15
		*((volatile uint32_t*)DBSC_DBTR4)	= 0x0011000f;	//dbtr4  trpa=17 trp=15
		*((volatile uint32_t*)DBSC_DBTR5)	= 0x00000030;	//dbtr5  trc=48
		*((volatile uint32_t*)DBSC_DBTR6)	= 0x00000022;	//dbtr6  tras=34
		*((volatile uint32_t*)DBSC_DBTR7)	= 0x00080008;	//dbtr7  trrd_s=8 trrd=8
		*((volatile uint32_t*)DBSC_DBTR8)	= 0x00000020;	//dbtr8  tfaw=32
		*((volatile uint32_t*)DBSC_DBTR9)	= 0x00000006;	//dbtr9  trdpr=6
		*((volatile uint32_t*)DBSC_DBTR10)	= 0x0000000f;	//dbtr10 twr=15
		*((volatile uint32_t*)DBSC_DBTR11)	= 0x00000011;	//dbtr11 trdwr=22
		*((volatile uint32_t*)DBSC_DBTR12)	= 0x001D001D;	//dbtr12 twrrd_s=20
		*((volatile uint32_t*)DBSC_DBTR13)	= 0x00480090;	//dbtr13 trfcpb=72  trfcab=144
		*((volatile uint32_t*)DBSC_DBTR14)	= 0x00060006;	//dbtr14 tckehdll=6
		*((volatile uint32_t*)DBSC_DBTR15)	= 0x000c0006;	//dbtr15 tckesr=12

		*((volatile uint32_t*)DBSC_DBTR16)	= 0x06200a0C;	//dbtr16 latency?
		*((volatile uint32_t*)DBSC_DBTR17)	= 0x080a0004;	//dbtr17 tmodrd=8  tmod=10 trdmr=4
		*((volatile uint32_t*)DBSC_DBTR18)	= 0x00000000;	//dbtr18 No odt for DQ exists in LPDDR4.
		*((volatile uint32_t*)DBSC_DBTR19)	= 0x00000000;	//dbtr19 tzqcl=0 tzqcs=0 because LPDDR4 doesn't support ZQC[SL].
		*((volatile uint32_t*)DBSC_DBTR20)	= 0x00b00096;	//dbtr20 txsdll=150
//		*((volatile uint32_t*)DBSC_DBTR21)	= 0x00080008;	//dbtr21 tccd_s=8 tccd=8
		*((volatile uint32_t*)DBSC_DBTR21)	= 0x000b000b;	//dbtr21 tccd_s=11 tccd=11	u-boot ok But PHY may restrict tccd+1/+2, so set it big enough.
//		*((volatile uint32_t*)DBSC_DBTR21)	= 0x000C000C;	//dbtr21 tccd_s=12 tccd=12	u-boot ok			// for lowfreq
//		*((volatile uint32_t*)DBSC_DBTR21)	= 0x00100010;	//dbtr21 tccd_s=16 tccd=16	u-boot ok			// for lowfreq
		*((volatile uint32_t*)DBSC_DBTR22)	= 0x03200018;	//dbtr22 tzqcal=800 tzqlat=24
	}

	*((volatile uint32_t*)DBSC_DBRNK0)	= 0x00000000;	//dbrnk0 No odt for DQ exists in LPDDR4(N.Honda).
	*((volatile uint32_t*)DBSC_DBRNK1)	= 0x00000000;	//dbrnk1 Don't need to set it cause we use all-rank refresh.
	*((volatile uint32_t*)DBSC_DBRNK2)	= 0x00008888;	//dbrnk2 rkrr=8
	*((volatile uint32_t*)DBSC_DBRNK3)	= 0x00008888;	//dbrnk3 rkrw=8
	*((volatile uint32_t*)DBSC_DBRNK4)	= 0x00008888;	//dbrnk4 rkwr=8
	*((volatile uint32_t*)DBSC_DBRNK5)	= 0x00008888;	//dbrnk5 rkww=8
	*((volatile uint32_t*)DBSC_DBRNK6)	= 0x00000000;	//dbrnk6 Use ALL Rank
	*((volatile uint32_t*)DBSC_DBADJ0)	= 0x00000000;	//dbadj0 No Optional setting
	*((volatile uint32_t*)DBSC_DBADJ2)	= 0x00000000;	//dbadj2 No Optional setting

//BUFCAM settings
//	*((volatile uint32_t*)DBSC_DBCAM0CNF0)	= 0x00000000;	//dbcam0cnf0 inoen=0, actually it is not used.
	*((volatile uint32_t*)DBSC_DBCAM0CNF1)	= 0x00044218;	//dbcam0cnf1 
	*((volatile uint32_t*)DBSC_DBCAM0CNF2)	= 0x000000F4;	//dbcam0cnf2 
//	*((volatile uint32_t*)DBSC_DBCAM0CNF3)	= 0x00000003;	//
	*((volatile uint32_t*)DBSC_DBBCAMDIS)	= 0x00000002;	//Subcam Disable
	*((volatile uint32_t*)DBSC_DBSCHCNT0)	= 0x080F003F;	//dbschcnt0
	*((volatile uint32_t*)DBSC_DBSCHCNT1)	= 0x00001010;	//dbschcnt1
	*((volatile uint32_t*)DBSC_DBSCHSZ0)	= 0x00000001;	//dbschsz0
	*((volatile uint32_t*)DBSC_DBSCHRW0)	= 0xF7311111;	//dbschrw0
	*((volatile uint32_t*)DBSC_DBSCHRW1)	= 0x00000037;	//dbschrw1

//QoS Settings
	*((volatile uint32_t*)DBSC_DBSCHQOS_0_0)	= 0x0000F000;	//dbschqos00
	*((volatile uint32_t*)DBSC_DBSCHQOS_0_1)	= 0x0000E000;	//dbschqos01
	*((volatile uint32_t*)DBSC_DBSCHQOS_0_2)	= 0x00007000;	//dbschqos02
	*((volatile uint32_t*)DBSC_DBSCHQOS_0_3)	= 0x00000000;	//dbschqos03
	*((volatile uint32_t*)DBSC_DBSCHQOS_4_0)	= 0x0000F000;	//dbschqos40
	*((volatile uint32_t*)DBSC_DBSCHQOS_4_1)	= 0x0000EFFF;	//dbschqos41
	*((volatile uint32_t*)DBSC_DBSCHQOS_4_2)	= 0x0000B000;	//dbschqos42
	*((volatile uint32_t*)DBSC_DBSCHQOS_4_3)	= 0x00000000;	//dbschqos43
	*((volatile uint32_t*)DBSC_DBSCHQOS_9_0)	= 0x0000F000;	//dbschqos90
	*((volatile uint32_t*)DBSC_DBSCHQOS_9_1)	= 0x0000EFFF;	//dbschqos91
	*((volatile uint32_t*)DBSC_DBSCHQOS_9_2)	= 0x0000D000;	//dbschqos92
	*((volatile uint32_t*)DBSC_DBSCHQOS_9_3)	= 0x00000000;	//dbschqos93
	*((volatile uint32_t*)DBSC_DBSCHQOS_13_0)	= 0x0000F000;	//dbschqos130
	*((volatile uint32_t*)DBSC_DBSCHQOS_13_1)	= 0x0000EFFF;	//dbschqos131
	*((volatile uint32_t*)DBSC_DBSCHQOS_13_2)	= 0x0000E800;	//dbschqos132
	*((volatile uint32_t*)DBSC_DBSCHQOS_13_3)	= 0x00007000;	//dbschqos133
	*((volatile uint32_t*)DBSC_DBSCHQOS_14_0)	= 0x0000F000;	//dbschqos140
	*((volatile uint32_t*)DBSC_DBSCHQOS_14_1)	= 0x0000EFFF;	//dbschqos141
	*((volatile uint32_t*)DBSC_DBSCHQOS_14_2)	= 0x0000E800;	//dbschqos142
	*((volatile uint32_t*)DBSC_DBSCHQOS_14_3)	= 0x00007000;	//dbschqos143
	*((volatile uint32_t*)DBSC_DBSCHQOS_15_0)	= 0x000007D0;	//dbschqos150
	*((volatile uint32_t*)DBSC_DBSCHQOS_15_1)	= 0x000007CF;	//dbschqos151
	*((volatile uint32_t*)DBSC_DBSCHQOS_15_2)	= 0x000005D0;	//dbschqos152
	*((volatile uint32_t*)DBSC_DBSCHQOS_15_3)	= 0x000003D0;	//dbschqos153

	*((volatile uint32_t*)DBSC_DBSYSCONF1)		= 0x00000002;	//dbsysconf1 freqratio=2.

//Under Verification
	*((volatile uint32_t*)DBSC_DBCAM0CNF1)	= 0x00044218;	//wbkwait(0004), wbkmdhi(4,2),wbkmdlo(1,8)             <- independent of dram freq
	*((volatile uint32_t*)DBSC_DBCAM0CNF2)	= 0x00000284;	//rg_scexp[15:0] 0,0(fillunit),8(dirtymax),4(dirtymin) <- independent of dram freq

	if(freq>0x53)
	{
		// DDR3200
		*((volatile uint32_t*)DBSC_DBSCHRW0)	= 0xF7311111;	//stop_tolerance, independent of dram freq
		*((volatile uint32_t*)DBSC_DBSCHRW1)	= 0x00000048;	//[15: 8]=trfcpb/4; [ 7: 0]=trfcab/4   ;
		*((volatile uint32_t*)DBSC_SCFCTST0)	= 0x18030d09;	//[31:24]=trc/4   ; [23:16]=trda->act/4; [15:8]=twra->act/4; [7:0]=trp/4
		*((volatile uint32_t*)DBSC_SCFCTST1)	= 0x090a080c;	//[31:24]=trdwr/4 ; [23:16]=twrrd/4    ; [15:8]=trcd/4     ; [7:0]=asyncofs
		*((volatile uint32_t*)DBSC_SCFCTST2)	= 0x111f1fff;	//rd-wr/wr-rd toggle count, independent of dram freq
	}
	else if(freq>0x47)
	{
		// DDR2800
		*((volatile uint32_t*)DBSC_DBSCHRW0)	= 0xF7311111;	//stop_tolerance, independent of dram freq
		*((volatile uint32_t*)DBSC_DBSCHRW1)	= 0x00018248;	//[15: 8]=trfcpb/4; [ 7: 0]=trfcab/4   ;
//		*((volatile uint32_t*)DBSC_SCFCTST0)	= 0x15030d09;	//[31:24]=trc/4   ; [23:16]=trda->act/4; [15:8]=twra->act/4; [7:0]=trp/4
		*((volatile uint32_t*)DBSC_SCFCTST0)	= 0x180b1508;	//[31:24]=trc/2.5=18   ; [23:16]=trda->act/2.5=11; [15:8]=twra->act/2.5=21; [7:0]=trp/2.5=8
//		*((volatile uint32_t*)DBSC_SCFCTST1)	= 0x080a070c;	//[31:24]=trdwr/4 ; [23:16]=twrrd/4    ; [15:8]=trcd/4     ; [7:0]=asyncofs
		*((volatile uint32_t*)DBSC_SCFCTST1)	= 0x0a0b080C;	//[31:24]=trdwr/2.5=9 ; [23:16]=twrrd/2.5=16   ; [15:8]=trcd/2.5=8     ; [7:0]=asyncofs
		*((volatile uint32_t*)DBSC_SCFCTST2)	= 0x000F0FFF;	//rd-wr/wr-rd toggle count, independent of dram freq
	}
	else if(freq>0x2f)
	{
		// DDR2400
		*((volatile uint32_t*)DBSC_DBSCHRW0)	= 0x22421111;	//stop_tolerance, independent of dram freq
		*((volatile uint32_t*)DBSC_DBSCHRW1)	= 0x00180034;	//[15: 8]=trfcpb/4; [ 7: 0]=trfcab/4   ;
		*((volatile uint32_t*)DBSC_SCFCTST0)	= 0x180B1708;	//[31:24]=trc/4   ; [23:16]=trda->act/4; [15:8]=twra->act/4; [7:0]=trp/4
		*((volatile uint32_t*)DBSC_SCFCTST1)	= 0x0808070C;	//[31:24]=trdwr/4 ; [23:16]=twrrd/4    ; [15:8]=trcd/4     ; [7:0]=asyncofs
		*((volatile uint32_t*)DBSC_SCFCTST2)	= 0x012F1123;	//rd-wr/wr-rd toggle count, independent of dram freq
	}
	else
	{
		*((volatile uint32_t*)DBSC_DBSCHRW0)	= 0xF7311111;	//stop_tolerance, independent of dram freq
		*((volatile uint32_t*)DBSC_DBSCHRW1)	= 0x00000024;	//[15: 8]=trfcpb/4; [ 7: 0]=trfcab/4   ;
		*((volatile uint32_t*)DBSC_SCFCTST0)	= 0x0c020905;	//[31:24]=trc/4   ; [23:16]=trda->act/4; [15:8]=twra->act/4; [7:0]=trp/4
		*((volatile uint32_t*)DBSC_SCFCTST1)	= 0x0508040C;	//[31:24]=trdwr/4 ; [23:16]=twrrd/4    ; [15:8]=trcd/4     ; [7:0]=asyncofs
		*((volatile uint32_t*)DBSC_SCFCTST2)	= 0x000F0FFF;	//rd-wr/wr-rd toggle count, independent of dram freq
	}
}


static void pvt_manual_update(uint32_t flag)
{
	if(flag&0x1) {REG_DDRPHY_WRITE(0,0x702,REG_DDRPHY_READ(0,0x702) | (1<<16));};
	if(flag&0x2) {REG_DDRPHY_WRITE(1,0x702,REG_DDRPHY_READ(1,0x702) | (1<<16));};
	if(flag&0x4) {REG_DDRPHY_WRITE(2,0x702,REG_DDRPHY_READ(2,0x702) | (1<<16));};
	if(flag&0x8) {REG_DDRPHY_WRITE(3,0x702,REG_DDRPHY_READ(3,0x702) | (1<<16));};

	if(flag&0x10) {REG_DDRPHY_WRITE(0,0x732,REG_DDRPHY_READ(0,0x732) | (1<<16));};
	if(flag&0x20) {REG_DDRPHY_WRITE(1,0x732,REG_DDRPHY_READ(1,0x732) | (1<<16));};
	if(flag&0x40) {REG_DDRPHY_WRITE(2,0x732,REG_DDRPHY_READ(2,0x732) | (1<<16));};
	if(flag&0x80) {REG_DDRPHY_WRITE(3,0x732,REG_DDRPHY_READ(3,0x732) | (1<<16));};
}


static void set_cacs_delay()
{
	uint32_t i,f,dataL;

	//FREQ_SEL_INDEX=3 (not needed? because MULTICAST_EN=1) ==> NEED!! otherwise, copy will not copy
	REG_DDRPHY_WRITE (1,0x00000700,0x0301);
	REG_DDRPHY_WRITE (2,0x00000700,0x0301);
	REG_DDRPHY_WRITE (3,0x00000700,0x0301);

	for (i=0;i<4;i++)
	{
		for(f=0;f<2;f++)
		{
			REG_DDRPHY_WRITE(i,0x700,dataL=((REG_DDRPHY_READ(i,0x700) & 0xfffffcfe) | (f<<8)) );

			REG_DDRPHY_WRITE(i,DDR_CA_DELAY[0][0],dataL=((DDR_CA_DELAY[0][1] & 0x000000ff) | (DDR_CAD<<8)) );
			REG_DDRPHY_WRITE(i,DDR_CA_DELAY[1][0],dataL=((DDR_CA_DELAY[1][1] & 0x00000000) | (DDR_CAD<<16) | (DDR_CAD<<0)) );
			REG_DDRPHY_WRITE(i,DDR_CA_DELAY[2][0],dataL=((DDR_CA_DELAY[2][1] & 0x00000000) | (DDR_CAD<<16) | (DDR_CAD<<0)) );
			REG_DDRPHY_WRITE(i,DDR_CA_DELAY[3][0],dataL=((DDR_CA_DELAY[3][1] & 0x000f0000) | (DDR_CAD<<0)) );

			REG_DDRPHY_WRITE(i,DDR_CA_DELAY[0][0]+0x80,dataL=((DDR_CA_DELAY[0][1] & 0x000000ff) | (DDR_CAD<<8)) );
			REG_DDRPHY_WRITE(i,DDR_CA_DELAY[1][0]+0x80,dataL=((DDR_CA_DELAY[1][1] & 0x00000000) | (DDR_CAD<<16) | (DDR_CAD<<0)) );
			REG_DDRPHY_WRITE(i,DDR_CA_DELAY[2][0]+0x80,dataL=((DDR_CA_DELAY[2][1] & 0x00000000) | (DDR_CAD<<16) | (DDR_CAD<<0)) );
			REG_DDRPHY_WRITE(i,DDR_CA_DELAY[3][0]+0x80,dataL=((DDR_CA_DELAY[3][1] & 0x000f0000) | (DDR_CAD<<0)) );

			REG_DDRPHY_WRITE(i,DDR_CA_DELAY[4][0],dataL=((DDR_CA_DELAY[4][1] & 0x000000ff) | (DDR_GRP_A<<8)) );
			REG_DDRPHY_WRITE(i,DDR_CA_DELAY[5][0],dataL=((DDR_CA_DELAY[5][1] & 0x00000000) | (DDR_GRP_B<<16) | (DDR_GRP_A<<0)) );
			REG_DDRPHY_WRITE(i,DDR_CA_DELAY[6][0],dataL=((DDR_CA_DELAY[6][1] & 0x00000000) | (DDR_GRP_B<<0)) );
		}
		REG_DDRPHY_WRITE(i,0x700,dataL=((REG_DDRPHY_READ(i,0x700) & 0xfffffffe) | 1) );
	}

	for (i=0;i<4;i++)
	{
		REG_DDRPHY_WRITE(i,0x702,dataL=((REG_DDRPHY_READ(i,0x702) & 0xfeffffff) | (DDR_LP4_BOOT_DISABLE<<24) ) );
//		0x00000601, 0x00000000, // PHY_ADR_CLK_WR_BYPASS_SLAVE_DELAY_0:RW:16:11:=0x0000 PHY_ADR5_SW_WRADDR_SHIFT_0:RW+:8:5:=0x00 PHY_ADR4_SW_WRADDR_SHIFT_0:RW+:0:5:=0x00
//		0x00000681, 0x00000404, // PHY_ADR_CLK_WR_BYPASS_SLAVE_DELAY_1:RW:16:11:=0x0000 PHY_ADR5_SW_WRADDR_SHIFT_1:RW+:8:5:=0x04 PHY_ADR4_SW_WRADDR_SHIFT_1:RW+:0:5:=0x04
//		0x00000701, 0x00000000, // PHY_GRP_BYPASS_SLAVE_DELAY:RW:16:11:=0x0000 PHY_SW_GRP_SHIFT_3:RW+:8:5:=0x00 PHY_SW_GRP_SHIFT_2:RW+:0:5:=0x00
		REG_DDRPHY_WRITE(i,0x601,dataL=((REG_DDRPHY_READ(i,0x601) & 0xf800ffff) | (DDR_BPCAD<<16)) );
		REG_DDRPHY_WRITE(i,0x681,dataL=((REG_DDRPHY_READ(i,0x681) & 0xf800ffff) | (DDR_BPCAD<<16)) );
		REG_DDRPHY_WRITE(i,0x701,dataL=((REG_DDRPHY_READ(i,0x701) & 0xf800ffff) | (DDR_BPGRP<<16)) );

//		0x00000680, 0x04040404, // PHY_ADR3_SW_WRADDR_SHIFT_1:RW+:24:5:=0x04 PHY_ADR2_SW_WRADDR_SHIFT_1:RW+:16:5:=0x04 PHY_ADR1_SW_WRADDR_SHIFT_1:RW+:8:5:=0x04 PHY_ADR0_SW_WRADDR_SHIFT_1:RW+:0:5:=0x04
//		0x00000681, 0x00000404, // PHY_ADR_CLK_WR_BYPASS_SLAVE_DELAY_1:RW:16:11:=0x0000 PHY_ADR5_SW_WRADDR_SHIFT_1:RW+:8:5:=0x04 PHY_ADR4_SW_WRADDR_SHIFT_1:RW+:0:5:=0x04
		REG_DDRPHY_WRITE(i,0x680,dataL=((REG_DDRPHY_READ(i,0x680) & 0xe0e0e0e0) | 0x05050505) );
		REG_DDRPHY_WRITE(i,0x681,dataL=((REG_DDRPHY_READ(i,0x681) & 0xffffe0e0) | 0x00000505) );
	}
}



static void _rx_cal_code_set(uint32_t ch, uint32_t slice, uint8_t _cal_code_up[][4][16],uint8_t _cal_code_dn[][4][16])
{
	uint32_t i;
	uint32_t code;
	for(i=0;i<6;i++)
	{
		code=((      cal_code_up[ch][slice][i*2+1] &0x3f) <<22)
		    |((      cal_code_dn[ch][slice][i*2+1] &0x3f) <<16)
			|((      cal_code_up[ch][slice][i*2]   &0x3f) << 6)
		    |((      cal_code_dn[ch][slice][i*2]   &0x3f) << 0);
		REG_DDRPHY_WRITE(ch,0x434+i+slice*0x80,code);
	}
}

static void rx_cal_manual1(uint32_t chmask)
{ 

	uint32_t ch,slice;
	uint32_t index;
	uint32_t trial;
	
	uint16_t cal_obs;
	uint32_t rx_cal_val;
	uint32_t NS_WA; //for backward compat
	
	uint32_t regadd, o_dataL,dataL;
	
/*
Reports the calibration results. This parameter is only applicable if the phy_rx_cal_override_X parameter is set to fb1. 
There is a phy_rx_cal_obs_X parameter for each of the slices of data sent on the DFI data bus.
? Bit [10] = Calibration result from FDBK
? Bit [9] = rx_cal_rd_dqs; Calibration result from DQS
? Bit [8] = rx_cal_rd_dm; Calibration result from DM
? Bits [7:0] = wrlvl_rd_dq;	
*/
	rx_cal_val = DDR_RX_CAL_MAN&0x3f;
	if(DDR_RX_CAL_MAN&0x200)NS_WA=1;
	else NS_WA=0;

	for(ch=0;ch<4;ch++)
	{
		for(slice=0;slice<4;slice++)
		{
			REG_DDRPHY_WRITE(ch,0x452+0x80*slice,REG_DDRPHY_READ(ch,0x452+0x80*slice) | (1<<16));	//IE_on
			REG_DDRPHY_WRITE(ch,0x433+0x80*slice,REG_DDRPHY_READ(ch,0x433+0x80*slice)|0x100); //CAL_OVERRIDE on
			for(index=0;index<16;index++)
			{
				cal_code_up[ch][slice][index]=rx_cal_val;
				cal_code_dn[ch][slice][index]=rx_cal_val;
				fst1[ch][slice][index]=0xff;
				lst0[ch][slice][index]=0xff;
				for(trial=0;trial<128;trial+=32)
					val[ch][slice][index][trial/32]=0;
			}
			_rx_cal_code_set(ch,slice,cal_code_up,cal_code_dn);
		}
	}

	for(trial=0;trial<2*rx_cal_val;trial++)
	{
		for(ch=0;ch<4;ch++)
		{
			if(!(chmask & (1<<ch)))continue;
			for(slice=0;slice<4;slice++)
			{
				for(index=0;index<11;index++)
				{
					if(trial>=rx_cal_val)
					{
						cal_code_up[ch][slice][index]=rx_cal_val;
						cal_code_dn[ch][slice][index]=2*rx_cal_val-trial;
					}
					else
					{
						cal_code_up[ch][slice][index]=trial;
						cal_code_dn[ch][slice][index]=rx_cal_val;
					}
				}
				_rx_cal_code_set(ch,slice,cal_code_up,cal_code_dn);
			}
		}
		for(ch=0;ch<4;ch++)
		{
			for(slice=0;slice<4;slice++)
			{
				cal_obs = (uint16_t)(REG_DDRPHY_READ(ch,0x439+0x80*slice)>>16);
				for(index=0;index<11;index++)
				{
					if(cal_obs&(1<<index) )
					{
						val[ch][slice][index][trial/32]|=(1ULL<<trial%32);
					}
					else
					{
						val[ch][slice][index][trial/32]&=~(1ULL<<trial%32);
					}

					if(((ch==0 || ch==3)&&(slice==2||slice==3)) || !NS_WA)
					{
						if(cal_obs&(1<<index) )
						{
							if(fst1[ch][slice][index]==0xff)fst1[ch][slice][index]=trial;
						}
						else
						{
							lst0[ch][slice][index]=trial;
						}
					}
					else
					{
						if(!(cal_obs&(1<<index) ))
						{
							if(fst1[ch][slice][index]==0xff)fst1[ch][slice][index]=trial;
						}
						else
						{
							lst0[ch][slice][index]=trial;
						}
					}
				}
			}
		}
	}

	for(ch=0;ch<4;ch++)
	{
		for(slice=0;slice<4;slice++)
		{
			for(index=0;index<16;index++)
			{
				if(fst1[ch][slice][index]==0xff || lst0[ch][slice][index]==0xff)
				{
					cal_code_up[ch][slice][index]=rx_cal_val;
					cal_code_dn[ch][slice][index]=rx_cal_val;
				}
				else
				{
					trial=(fst1[ch][slice][index]+lst0[ch][slice][index])>>1;
					if(trial>=rx_cal_val)
					{
						cal_code_up[ch][slice][index]=rx_cal_val;
						cal_code_dn[ch][slice][index]=2*rx_cal_val-trial;
					}
					else
					{
						cal_code_up[ch][slice][index]=trial;
						cal_code_dn[ch][slice][index]=rx_cal_val;
					}
				}
			}
			_rx_cal_code_set(ch,slice,cal_code_up,cal_code_dn);
		}
	}

	for(ch=0;ch<4;ch++)
	{
		for(slice=0;slice<4;slice++)
		{
			REG_DDRPHY_WRITE(ch,0x452+0x80*slice,REG_DDRPHY_READ(ch,0x452+0x80*slice)&~(1<<16));	//IE_off
			REG_DDRPHY_WRITE(ch,0x433+0x80*slice,REG_DDRPHY_READ(ch,0x433+0x80*slice)&~0x100); //CAL_OVERRIDE off
		}
	}
	if(1)
	{
		for(ch=0;ch<4;ch++)
		{
			for(slice=0;slice<4;slice++)
			{
				for(index=0;index<11;index++)
				{
					regadd=0x434+0x80*slice+index/2;
					o_dataL=REG_DDRPHY_READ(ch,regadd);
					if(index%2)dataL=(o_dataL>>16);
					else dataL=o_dataL;
					dataL&=0xffff;
				}
			}
		}
	}
}


static void mode_register_set(uint32_t freq,uintptr_t phychno,uint32_t mr13_val)
{

	*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040d00|(0x00100000 * phychno)|mr13_val;	//MRW 13 chA rkA 0d:00 //FSPOP&FSPWP is fixed to 0, VRO off
	WaitDBCMD();

	if(freq>0x47)
	{
		*((volatile uint32_t*)DBSC_DBCMD) = 0x0e0401d4|(0x00100000 * phychno);	//MRW 1 DeviceFeature1(Post=1.5tck nWR=30 RDpre=static WRPre=2tCK BL=16
	}
	else if (freq > 0x2f)
	{
		*((volatile uint32_t*)DBSC_DBCMD) = 0x0e0401c4|(0x00100000 * phychno);	//MRW 1 DeviceFeature1(Post=1.5tck nWR=24 RDpre=static WRPre=2tCK BL=16
	}
	else
	{
		*((volatile uint32_t*)DBSC_DBCMD) = 0x0e0401a4|(0x00100000 * phychno);	//MRW 1 DeviceFeature1(Post=1.5tck nWR=16 RDpre=static WRPre=2tCK BL=16
	}

	WaitDBCMD();

	if(freq>0x47)
	{
		*((volatile uint32_t*)DBSC_DBCMD) = 0x0e04022e|(0x00100000 * phychno);	//MRW 2 DeviceFeature2(0,0SetA,101=WL14,110=RL32)
	}
	else if (freq > 0x2f)
	{
		*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040224|(0x00100000 * phychno);	//MRW 2 DeviceFeature2(0,0SetA,100=WL12,100=RL24)
	}
	else
	{
		*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040252|(0x00100000 * phychno);	//MRW 2 DeviceFeature2(0,1SetB,010=WL12,010=RL14)
	}

	
	WaitDBCMD();

	*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040300|(0x00100000 * phychno)|DDR_MR3;	//MRW 3 chA rkA 03:31
	WaitDBCMD();

	*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040b00|(0x00100000 * phychno)|DDR_MR11;	//MRW 11
	WaitDBCMD();

	*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040c11|(0x00100000 * phychno);	//MRW 12
	WaitDBCMD();

	*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040e11|(0x00100000 * phychno);	//MRW 14
	WaitDBCMD();

	*((volatile uint32_t*)DBSC_DBCMD) = 0x0e041600|(0x00100000 * phychno)|DDR_MR22;	//MRW 22 
	WaitDBCMD();
}

static void _set_reg(uint32_t ch_mask, uint32_t regadd, uint32_t pos, uint32_t width, uint32_t val)
{
	int ch;
	uint32_t dataL;
	uint64_t valids;
	uint64_t valids_shift;

	valids=(1ULL<<width)-1;
	valids_shift = valids<<pos;

	for(ch=0;ch<4;ch++)
	{
		if(!((1<<ch)&ch_mask))continue;
		dataL=REG_DDRPHY_READ(ch,regadd);
		dataL=(dataL&~valids_shift) | ((val&valids)<<pos);
		REG_DDRPHY_WRITE(ch,regadd,dataL);
	}
}


static void pvt_lvl_enable_set(uint32_t go)
{
	uint32_t i;

	for(i=0;i<4;i++)
	{
	//CALVL_EN
		if((DDR_LVLEN&go)&0x00000001)
		{
			REG_DDRPHY_WRITE(i,0x25d,REG_DDRPHY_READ(i,0x25d) | (1ULL<<8));
		}
		else
		{
			REG_DDRPHY_WRITE(i,0x25d,REG_DDRPHY_READ(i,0x25d) &~(1ULL<<8));
		}

	//WRLVL_EN
		if((DDR_LVLEN&go)&0x00000002)
		{
			REG_DDRPHY_WRITE(i,0x23a,REG_DDRPHY_READ(i,0x23a) | (1ULL<<24));
		}
		else
		{
			REG_DDRPHY_WRITE(i,0x23a,REG_DDRPHY_READ(i,0x23a) &~(1ULL<<24));
		}

	//RDLVL_GATE_EN
		if((DDR_LVLEN&go)&0x00000004)
		{
			REG_DDRPHY_WRITE(i,0x24d,REG_DDRPHY_READ(i,0x24d) | (1ULL<<24));
		}
		else
		{
			REG_DDRPHY_WRITE(i,0x24d,REG_DDRPHY_READ(i,0x24d) &~(1ULL<<24));
		}

	//RDLVL_EN
		if((DDR_LVLEN&go)&0x00000008)
		{
			REG_DDRPHY_WRITE(i,0x24d,REG_DDRPHY_READ(i,0x24d) | (1ULL<<16));
		}
		else
		{
			REG_DDRPHY_WRITE(i,0x24d,REG_DDRPHY_READ(i,0x24d) &~(1ULL<<16));
		}

	//WRDQLVL_EN
		if((DDR_LVLEN&go)&0x00000010)
		{
			REG_DDRPHY_WRITE(i,0x275,REG_DDRPHY_READ(i,0x275) | (1ULL<<16));
		}
		else
		{
			REG_DDRPHY_WRITE(i,0x275,REG_DDRPHY_READ(i,0x275) &~(1ULL<<16));
		}

	//CSLVL_EN
		if((DDR_LVLEN&go)&0x00000020)
		{
			REG_DDRPHY_WRITE(i,0x703,REG_DDRPHY_READ(i,0x703) | (1));
		}
		else
		{
			REG_DDRPHY_WRITE(i,0x703,REG_DDRPHY_READ(i,0x703) &~(1));
		}
	}
}

static void pvt_freq_change_ack(uint32_t freq, uintptr_t phychno, uint32_t freq_flag)
{
	uint32_t dataL;
	uint32_t j;

	dsb_sev();
	//DFI FREQ
	*((volatile uint32_t*)(DBSC_DFI_FREQ_0 +0x040 * phychno)) = freq_flag;
	dataL=*((volatile uint32_t*)(DBSC_DFI_FREQ_0 +0x040 * phychno));
	dsb_sev();

#ifdef DDR_FREQCHG
	j = *((volatile uint32_t*)CPG_PLL3CR);
	dsb_sev();

//	j = (j&0x00ffff7f) | ((((freq/(3-freq_flag))  << 24))&0xff000000)| ((fdiv << 7)&0x00000080);
	j = (j&0x00ffff7f) | ((freq & 0xff) << 24)| ((freq_flag == 0) << 7);
	*((volatile uint32_t*)CPG_PLL3CR) = j;
	dsb_sev();
	j = *((volatile uint32_t*)CPG_PLL3CR);

	dsb_sev();
	do {
		dataL=*((volatile uint32_t*)CPG_PLLECR);
	} while( (dataL&0x1f00)!=0x1f00 );
#endif

	//FREQ CHANGE ACK
	*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0 + 0x40 * phychno)) = 0x0CF20000;
	dataL=*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0 + 0x40 * phychno));

	*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0 + 0x40 * phychno)) = 0x00000000;
	dsb_sev();
}


static uint32_t pvt_pi_training_go_all_o(uint32_t freq)
{
	uint32_t dataL;
	uint32_t phytrainingok;
	uint32_t retry;
	uint32_t pll_status[4];
	uint32_t wait_count[4];
	uintptr_t phychno;

	if(DDR_TVAL1==0)
	{
		for(phychno=0;phychno<4;phychno++)
		{
			//dfi_init_start negate
			*((volatile uint32_t*)(DBSC_DBDFICNT_0+0x40*phychno)) = 0x00000010;
			dsb_sev();
		}
	}

	for(phychno=0;phychno<4;phychno++)
	{
		if( (DDR_PHYVALID&(1<<phychno))==0 )
		{
			continue;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////// PI_START 
		REG_DDRPHY_WRITE(phychno, 0x200, 0x20410B01);
//		*((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x040 * phychno)) = 0x00000200;
//		*((volatile uint32_t*)(DBSC_DBPDRGD_0 + 0x040 * phychno)) = 0x20410B01;
		if(DDR_TVAL1==1)
		{
			//dfi_init_start negate 
			*((volatile uint32_t*)(DBSC_DBDFICNT_0+0x40*phychno)) = 0x00000010;
			dsb_sev();
		}
	}
	//////////////////////

	dsb_sev();

	if(DDR_TVAL1==2)
	{
		for(phychno=0;phychno<4;phychno++)
		{
			//dfi_init_start negate 
			*((volatile uint32_t*)(DBSC_DBDFICNT_0+0x40*phychno)) = 0x00000010;
			dsb_sev();
		}
	}

	phytrainingok=0;
	retry=0;

	// 0 : wait first unlock
	// 1 : pll is off
	// 2 : pll is on

	pll_status[0]=0;//wait unlock
	pll_status[1]=0;//wait unlock
	pll_status[2]=0;//wait unlock
	pll_status[3]=0;//wait unlock

	while(1)
	{
		for(phychno=0;phychno<4;phychno++)
		{
			if(DDR_PHYVALID&phytrainingok&(1<<phychno))
			{
				continue;
			}

			if(pll_status[phychno]==0)
			{
				dataL = *((volatile uint32_t*)(DBSC_PLL_LOCK_0+phychno*0x100));	//Wait for PLL is unlock
				if((dataL & 0x0000001F) == 0x0)
				{
					pll_status[phychno]=1;
					wait_count[phychno]=1000;

					pvt_freq_change_ack(freq,phychno,0);
					retry=0;
				}
			}
			else if(pll_status[phychno]==1)
			{
				dataL = *((volatile uint32_t*)(DBSC_PLL_LOCK_0+phychno*0x100));	//Wait for PLL Lock
				if((dataL & 0x0000001F) == 0x1F)
				{
					pll_status[phychno]=2;
					wait_count[phychno]=1000;
					retry=0;
				}
				else
				{
					wait_count[phychno]--;
					if(wait_count[phychno]==0)
					{
						pvt_freq_change_ack(freq,phychno,2);
						wait_count[phychno]=1000;
					}
				}
			}
			else
			{
				if(wait_count[phychno]>0)
				{
					wait_count[phychno]--;
					dataL = REG_DDRPHY_READ ( phychno, 0x0000029B);
					if(dataL&0x1)
					{
						phytrainingok |= (0x1<<phychno);
						retry=0;
						// TRAING OK
					}
				}
				else
				{
					dataL = *((volatile uint32_t*)(DBSC_PLL_LOCK_0+phychno*0x100));	//Check PLL is unlock again
					if((dataL & 0x0000001F) == 0x00)
					{
						pll_status[phychno]=1;
						pvt_freq_change_ack(freq,phychno,0);
						wait_count[phychno]=1000;
						retry=0;
					}
					else
					{
						wait_count[phychno]=1000;
					}
				}
			}
		}

		if( (DDR_PHYVALID& phytrainingok&0xf)==0xf )
		{
			break;
		}
		retry++;
		if(retry==10000)
		{
			break;
		}
	}

	for(phychno=0;phychno<4;phychno++)
	{
		dataL = REG_DDRPHY_READ ( phychno, 0x0000029B);
		REG_DDRPHY_WRITE( phychno, 0x29c, dataL);
	}

	return phytrainingok;
}


inline void PLL3_FREQ(uint32_t freq, uint32_t freq_flag)
{
	uint32_t dataL;

#ifdef	DDR_FREQCHG

	dsb_sev();

	if(freq_flag==0)
	{
#ifdef	DDR_LOWFIX
		dataL = DDR_LOWFIX;
#else
		dataL = (freq+1)/DDR_LOWDIV-1;
#endif
	}
	else
	{
		dataL = (freq);
	}

	dataL = (dataL<<24);
	*((volatile uint32_t*)CPG_CPGWPR) = ~dataL;
	*((volatile uint32_t*)CPG_PLL3CR) =  dataL;

	dsb_sev();
	do {
		dataL=*((volatile uint32_t*)CPG_PLLECR);
	} while( (dataL&0x1f00)!=0x1f00 );

#endif	// DDR_FREQCHG
}

#define TR_ERR 4000
#define TR_MAX (TR_ERR*2)
#define TE_ERR 10000
#define TE_MAX (TE_ERR*2)

uint32_t pvt_pi_training_go_all(uint32_t freq)
{
	uint32_t flag,i,j;
	uint32_t dataL;
	uint32_t phytrainingok;
	uint32_t retry;
	uintptr_t phychno;
	uint32_t FREQ_H;

	FREQ_H=1;

	PLL3_FREQ(freq,0);

	for(phychno=0;phychno<4;phychno++)
	{
		//dfi_init_start negate
		*((volatile uint32_t*)(DBSC_DBDFICNT_0+0x40*phychno)) = 0x00000010;
		dsb_sev();
	}

	if(DDR_TVAL0&0x100)
	{
//		//CHANGE IMPEDANCE CODE to CMOS MODE
		change_lpddr4_en(0xf,0);
	}

	for(phychno=0;phychno<4;phychno++)
	{
		if(DDR_TVAL0&0x1)
		{
//		0x00000268, 0x09090605, // PI_TDFI_CALVL_STROBE_F2:RW:24:4:=0x09 PI_TDFI_CALVL_STROBE_F1:RW:16:4:=0x09 PI_TDFI_CALVL_STROBE_F0:RW:8:4:=0x06 PI_TDFI_INIT_COMPLETE_MIN:RW:0:8:=0x05
//NOTE:TDFI_INIT_COMPLETE_MIN: wait cycle for init_complete: should be max value
			REG_DDRPHY_WRITE(phychno, 0x268, (REG_DDRPHY_READ(phychno,0x268)&0xffffff00) | 0xff);
		}

		if(DDR_TVAL0&0x2)
		{
//		0x0000026D, 0x18020100, // PI_TCKEHDQS_F1:RW:24:6:=0x18 PI_TCKEHDQS_F0:RW:16:6:=0x02 PI_REFRESH_BETWEEN_SEGMENT_DISABLE:RW_D:8:1:=0x01 PI_DRAM_CLK_DISABLE_DEASSERT_SEL:RW:0:1:=0x00
//		0x0000026E, 0x00010118, // PI_WDQLVL_BST_NUM:RW:16:3:=0x01 PI_WDQLVL_VREF_EN:RW:8:1:=0x01 PI_TCKEHDQS_F2:RW:0:6:=0x18
//NOTE:TCKEHDQS
			REG_DDRPHY_WRITE(phychno, 0x26D, (REG_DDRPHY_READ(phychno,0x26D)&0xc0c0ffff) | (0x18<<24) | (0x18<<16));
			REG_DDRPHY_WRITE(phychno, 0x26E, (REG_DDRPHY_READ(phychno,0x26E)&0xffffffc0) | (0x18<<0));
		}

		if(DDR_TVAL0&0x4)
		{
//		0x00000262, 0x00060100, // PI_TVREF_SHORT_F0:RW:16:10:=0x0006 PI_TDFI_CASEL_F0:RW:8:5:=0x01 PI_TDFI_CACSCA_F0:RW:0:5:=0x00
//		0x00000263, 0x01000006, // PI_TDFI_CASEL_F1:RW:24:5:=0x01 PI_TDFI_CACSCA_F1:RW:16:5:=0x00 PI_TVREF_LONG_F0:RW:0:10:=0x0006
//		0x00000264, 0x018E018E, // PI_TVREF_LONG_F1:RW:16:10:=0x018e PI_TVREF_SHORT_F1:RW:0:10:=0x018e
//		0x00000265, 0x018E0100, // PI_TVREF_SHORT_F2:RW:16:10:=0x018e PI_TDFI_CASEL_F2:RW:8:5:=0x01 PI_TDFI_CACSCA_F2:RW:0:5:=0x00
//		0x00000266, 0x0F0F018E, // PI_CALVL_VREF_INITIAL_STOP_POINT:RW:24:7:=0x1e PI_CALVL_VREF_INITIAL_START_POINT:RW:16:7:=0x1a PI_TVREF_LONG_F2:RW:0:10:=0x018e
//NOTE:TDFI_TVREF_LONG: wait cycle from CKE assert to MRW :
//			REG_DDRPHY_WRITE(phychno, 0x263, REG_DDRPHY_READ(phychno,0x263)&0xfffffc00 | (0x3ff<<0));
//			REG_DDRPHY_WRITE(phychno, 0x264, REG_DDRPHY_READ(phychno,0x264)&0xfc00ffff | (0x3ff<<16));
//			REG_DDRPHY_WRITE(phychno, 0x266, REG_DDRPHY_READ(phychno,0x266)&0xfffffc00 | (0x3ff<<0));

			REG_DDRPHY_WRITE(phychno, 0x262, (REG_DDRPHY_READ(phychno,0x262)&0xfc00ffff) | (0x18e<<0));
			REG_DDRPHY_WRITE(phychno, 0x263, (REG_DDRPHY_READ(phychno,0x263)&0xfffffc00) | (0x18e<<0));
			REG_DDRPHY_WRITE(phychno, 0x264, (REG_DDRPHY_READ(phychno,0x264)&0xfc00fc00) | (0x18e<<16) | (0x18e<<0));
			REG_DDRPHY_WRITE(phychno, 0x266, (REG_DDRPHY_READ(phychno,0x266)&0xfffffc00) | (0x18e<<0));
		}

		if(DDR_TVAL0&0x8)
		{
//		0x0000025F, 0x00FF010A, // PI_TCAENT_F0:RW:16:14:=0x0005 PI_TMRZ_F0:RW:8:5:=0x01 PI_TCACKEH:RW:0:5:=0x0a
//		0x00000260, 0x03018D03, // PI_TMRZ_F2:RW:24:5:=0x03 PI_TCAENT_F1:RW:8:14:=0x018d PI_TMRZ_F1:RW:0:5:=0x03
//		0x00000261, 0x000A018D, // PI_CA_TRAIN_VREF_EN:RW:24:1:=0x01 PI_TCAEXT:RW:16:5:=0x0a PI_TCAENT_F2:RW:0:14:=0x018d
//NOTE:TDFI_TCAENT: wait cycle to begin CA operation
			REG_DDRPHY_WRITE(phychno, 0x25f, (REG_DDRPHY_READ(phychno,0x25f)&0xc000ffff) | (0x3fff<<16));
			REG_DDRPHY_WRITE(phychno, 0x260, (REG_DDRPHY_READ(phychno,0x260)&0xffc000ff) | (0x3fff<<8));
			REG_DDRPHY_WRITE(phychno, 0x261, (REG_DDRPHY_READ(phychno,0x261)&0xffffc000) | (0x3fff<<0));
		}

		if(DDR_TVAL0&0x10)
		{
//		0x00000267, 0x10010204, // PI_TDFI_INIT_START_MIN:RW:24:8:=0x10 PI_CALVL_VREF_DELTA:RW:16:4:=0x01 PI_CALVL_VREF_NORMAL_STEPSIZE:RW:8:4:=0x02 PI_CALVL_VREF_INITIAL_STEPSIZE:RW:0:4:=0x04
//NOTE:TDFI_INIT_START_MIN: wait to assert init_start
			REG_DDRPHY_WRITE(phychno, 0x267, (REG_DDRPHY_READ(phychno,0x267)&0x00ffffff) | (0xff<<24));
		}
		if(DDR_TVAL0&0x20)
		{
//		0x00000269, 0x20000202, // PI_TDFI_INIT_START_F0:RW:24:8:=0x20 PI_SW_CA_TRAIN_VREF:RW:16:7:=0x00 PI_CALVL_STROBE_NUM:RW:8:5:=0x02 PI_TCKCKEH:RW:0:4:=0x02
//NOTE:PI_TCKCKEH :
			REG_DDRPHY_WRITE(phychno, 0x269, (REG_DDRPHY_READ(phychno,0x269)&0xfffffff0) | (0xf<<0));
		}

		if(DDR_TVAL0&0x40)
		{
//		0x0000026D, 0x18020100, // PI_TCKEHDQS_F1:RW:24:6:=0x18 PI_TCKEHDQS_F0:RW:16:6:=0x02 PI_REFRESH_BETWEEN_SEGMENT_DISABLE:RW_D:8:1:=0x01 PI_DRAM_CLK_DISABLE_DEASSERT_SEL:RW:0:1:=0x00
			REG_DDRPHY_WRITE(phychno, 0x26d, (REG_DDRPHY_READ(phychno,0x26d)&0xfffffffe) | (0x1<<0));
		}

		if(DDR_TVAL0&0x100)
		{
			for(i=0;i<4;i++)
			{
				REG_DDRPHY_WRITE(phychno, 0x44e + 0x80*i, REG_DDRPHY_READ(phychno, 0x44e + 0x80*i)|1);
			}
		}
//		0x00000414, 0x04080000, // PHY_WRLVL_UPDT_WAIT_CNT_0:RW:24:4:=0x04 PHY_WRLVL_CAPTURE_CNT_0:RW:16:6:=0x08 SC_PHY_LVL_DEBUG_CONT_0:WR:8:1:=0x00 PHY_LVL_DEBUG_MODE_0:RW:0:1:=0x00
//		0x00000415, 0x04080a04, // PHY_RDLVL_UPDT_WAIT_CNT_0:RW:24:4:=0x04 PHY_RDLVL_CAPTURE_CNT_0:RW:16:6:=0x08 PHY_GTLVL_UPDT_WAIT_CNT_0:RW:8:4:=0x04 PHY_GTLVL_CAPTURE_CNT_0:RW:0:6:=0x00
		for(i=0;i<4;i++)
		{
			j=((REG_DDRPHY_READ(phychno, 0x414 + 0x80*i)>>24)+((DDR_TVAL0>>12)&0x0f))&0x0f;
			REG_DDRPHY_WRITE(phychno, 0x414 + 0x80*i, (REG_DDRPHY_READ(phychno, 0x414 + 0x80*i)&0xf0ffffff) | (j<<24));
		}
	}

	if(DDR_TVAL0&0x80)
	{
		for(phychno=0;phychno<4;phychno++)
		{
//		0x00000700, 0x00000001, // PHY_SW_GRP_SHIFT_1:RW+:24:5:=0x00 PHY_SW_GRP_SHIFT_0:RW+:16:5:=0x00 PHY_FREQ_SEL_INDEX:RW:8:2:=0x00 PHY_FREQ_SEL_MULTICAST_EN:RW_D:0:1:=0x01
			REG_DDRPHY_WRITE(phychno, 0x700, (REG_DDRPHY_READ(phychno,0x700)&0xfffffcfe) | (0x2<<8) | (0x0<<0));
		}
	}

	for(phychno=0;phychno<4;phychno++)
	{
		int slice;

		if(DDR_PHYVALID&(1<<phychno))
		{
		}
		else
		{
			continue;
		}

		for(slice=0;slice<4;slice++)
		{
			for(i=0;i<4;i++)
			{
//		0x00000409, 0x000700C0, // SC_PHY_SNAP_OBS_REGS_0:WR:24:1:=0x00 PHY_GATE_ERROR_DELAY_SELECT_0:RW:16:4:=0x07 PHY_RDDQS_DQ_BYPASS_SLAVE_DELAY_0:RW:0:10:=0x00c0
				REG_DDRPHY_WRITE(phychno, 0x409+0x80*slice, (REG_DDRPHY_READ(phychno, 0x409+0x80*slice) | (1<<24)));
//		0x00000412, 0x00000000, // PHY_RDDQ_ENC_OBS_SELECT_0:RW:24:3:=0x00 PHY_MASTER_DLY_LOCK_OBS_SELECT_0:RW:16:3:=0x00 PHY_SW_FIFO_PTR_RST_DISABLE_0:RW:8:1:=0x00 PHY_SLAVE_LOOP_CNT_UPDATE_0:RW:0:3:=0x00
				REG_DDRPHY_WRITE(phychno, 0x412+0x80*slice, ((REG_DDRPHY_READ(phychno, 0x412+0x80*slice)  & 0xfff8ffff) | 0));
//		0x00000413, 0x00000000, // PHY_FIFO_PTR_OBS_SELECT_0:RW:24:4:=0x00 PHY_WR_SHIFT_OBS_SELECT_0:RW:16:4:=0x00 PHY_WR_ENC_OBS_SELECT_0:RW:8:4:=0x00 PHY_RDDQS_DQ_ENC_OBS_SELECT_0:RW:0:4:=0x00
				REG_DDRPHY_WRITE(phychno, 0x413+0x80*slice, ((REG_DDRPHY_READ(phychno, 0x413+0x80*slice)  & 0xfff0f0ff) | (9<<16) | (9<<16)));


//		0x00000423, 0x00000000, // PHY_RDDQS_BASE_SLV_DLY_ENC_OBS_0:RD:24:7:=0x00 PHY_RDDQ_SLV_DLY_ENC_OBS_0:RD:16:6:=0x00 PHY_MASTER_DLY_LOCK_OBS_0:RD:0:10:=0x0000
//		0x00000424, 0x00000000, // PHY_RDDQS_GATE_SLV_DLY_ENC_OBS_0:RD:16:10:=0x0000 PHY_RDDQS_DQ_FALL_ADDER_SLV_DLY_ENC_OBS_0:RD:8:8:=0x00 PHY_RDDQS_DQ_RISE_ADDER_SLV_DLY_ENC_OBS_0:RD:0:8:=0x00

				dataL=REG_DDRPHY_READ(phychno, 0x423+0x80*slice);
//		0x00000425, 0x00000000, // PHY_WR_SHIFT_OBS_0:RD:24:3:=0x00 PHY_WR_ADDER_SLV_DLY_ENC_OBS_0:RD:16:8:=0x00 PHY_WRDQ_BASE_SLV_DLY_ENC_OBS_0:RD:8:8:=0x00 PHY_WRDQS_BASE_SLV_DLY_ENC_OBS_0:RD:0:7:=0x00
				dataL=REG_DDRPHY_READ(phychno, 0x425+0x80*slice);
			}
		}
	}

	phytrainingok=DDR_PHYVALID;
	if(DDR_UPDT_WA&0x1000)
	{
		for(phychno=0;phychno<4;phychno++)
		{
			if(!(phytrainingok&(1<<phychno)))continue;
			//////////////////////
			*((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x040 * phychno)) = 0x200;
			dsb_sev();
			*((volatile uint32_t*)(DBSC_DBPDRGD_0 + 0x040 * phychno)) = 0x20410B01;
			retry=0;
			do {
				dataL  = *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*phychno)) & 0x1f;
				retry++;
			} while(dataL && retry<TR_MAX);
			if(retry>=TR_ERR)
			{
				phytrainingok&=~(1<<phychno);
			}
		}
	}
	else
	{

		for(phychno=0;phychno<4;phychno++)
		{
			//////////////////////
			if(!(DDR_PHYVALID&(1<<phychno)))continue;
			*((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x040 * phychno)) = 0x200;
			dsb_sev();
//BEGIN:timing critical:
			*((volatile uint32_t*)(DBSC_DBPDRGD_0 + 0x040 * phychno)) = 0x20410B01;
		}
		do {
			dataL  = 0;
			if(DDR_PHYVALID&(1<<0)) dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*0)) & 0x1f;
			if(DDR_PHYVALID&(1<<1)) dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*1)) & 0x1f;
			if(DDR_PHYVALID&(1<<2)) dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*2)) & 0x1f;
			if(DDR_PHYVALID&(1<<3)) dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*3)) & 0x1f;
		} while(dataL);
	}

	if(DDR_LVLEN&0x21)
	{

		if(DDR_TVAL0&0x100)
		{
		//CHANGE IMPEDANCE CODE to LP4 MODE
			change_lpddr4_en(0xf,0);
		}

		dsb_sev();
		dsb_sev();

		if(!(DDR_UPDT_WA&0x2000))
		{
			PLL3_FREQ(freq,FREQ_H);
			
			dsb_sev();
			dsb_sev();
		}

		for(phychno=0;phychno<4;phychno++)
		{
			if(!(phytrainingok&(1<<phychno)))continue;
			//DFI FREQ
			*((volatile uint32_t*)(DBSC_DFI_FREQ_0 +0x040 * phychno)) = FREQ_H;
	//		*((volatile uint32_t*)(DBSC_DFI_FREQ_0 +0x040 * phychno)) = 0;
			dsb_sev();
			//CHG_ACK
			*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0 + 0x40 * phychno)) = 0x0CF20000;
			dsb_sev();
	//		//CHG_ACK
			*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0 + 0x40 * phychno)) = 0x0;
			dsb_sev();
			if(DDR_UPDT_WA&0x800)	 //SEQ mode
			{
				do {
					dataL  = *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*phychno)) & 0x1f;
				} while(dataL!=0x1f);
				do {
					dataL  = *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*phychno)) & 0x1f;
				} while(dataL);
			}
		}


		if(!(DDR_UPDT_WA&0x800))	//PARA mode
		{
			//lock check
			do {
				dataL=0x1f;
				if(phytrainingok&(1<<0))dataL &= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*0)) & 0x1f;
				if(phytrainingok&(1<<1))dataL &= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*1)) & 0x1f;
				if(phytrainingok&(1<<2))dataL &= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*2)) & 0x1f;
				if(phytrainingok&(1<<3))dataL &= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*3)) & 0x1f;
			} while(dataL!=0x1f);

			///////////////////////////////////////////////////////////////////////
			//unlock check
			do {
				dataL  = 0;
				if(phytrainingok&(1<<0))dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*0)) & 0x1f;
				if(phytrainingok&(1<<1))dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*1)) & 0x1f;
				if(phytrainingok&(1<<2))dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*2)) & 0x1f;
				if(phytrainingok&(1<<3))dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*3)) & 0x1f;
			} while(dataL);
		}

		if(DDR_TVAL0&0x200)
		{
		//CHANGE IMPEDANCE CODE to CMOS MODE
			change_lpddr4_en(0xf,0);
		}

		dsb_sev();
		dsb_sev();
		dsb_sev();

		PLL3_FREQ(freq,0);

		dsb_sev();
		dsb_sev();
		dsb_sev();

if(0){	//////// Retry check NG -----
		for(phychno=0;phychno<4;phychno++)
		{
			if(!(phytrainingok&(1<<phychno)))continue;

			//DFI FREQ
			*((volatile uint32_t*)(DBSC_DFI_FREQ_0 +0x040 * phychno)) =0;	// 0;
	//		*((volatile uint32_t*)(DBSC_DFI_FREQ_0 +0x040 * phychno)) =2;	// 2
			//CHG_ACK
			*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0 + 0x40 * phychno)) = 0x0CF20000;
			//CHG_ACK
			*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0 + 0x40 * phychno)) = 0x0;
			retry=0;
			do {
				dataL  = *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*phychno)) & 0x1f;
				retry++;
			} while(dataL!=0x1f && retry<1000);
			if(retry<1000) {
				// pi_start_status |= (0x10<<phychno);
			}
			else {
				phytrainingok&=~(1<<phychno);
			}
		}
		//------------NG/////
} else {
		for(phychno=0;phychno<4;phychno++){
			if(!(phytrainingok&(1<<phychno)))continue;

			//DFI FREQ
			*((volatile uint32_t*)(DBSC_DFI_FREQ_0 +0x040 * phychno)) =0;	// 0;
	//		*((volatile uint32_t*)(DBSC_DFI_FREQ_0 +0x040 * phychno)) =2;	// 2
			dsb_sev();

			//CHG_ACK
			*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0 + 0x40 * phychno)) = 0x0CF20000;
			dsb_sev();

			//CHG_ACK
			*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0 + 0x40 * phychno)) = 0x0;
			dsb_sev();
			
			//NOTE: this detecting may be failed, because asserting period is very short (retry = 1 or 2)
			// therefore, do not treat as error when timeout.
			retry=0;
			do {
				dataL  = *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*phychno)) & 0x1f;
				retry++;
			} while(dataL!=0x1f && retry<32);
		}
}

//END:timing critical:
		//unlock check
		do {
			dataL  = 0;
			if(phytrainingok&(1<<0))dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*0)) & 0x1f;
			if(phytrainingok&(1<<1))dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*1)) & 0x1f;
			if(phytrainingok&(1<<2))dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*2)) & 0x1f;
			if(phytrainingok&(1<<3))dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*3)) & 0x1f;
		} while(dataL);

		if(DDR_TVAL0&0x200)
		{
		//CHANGE IMPEDANCE CODE to LP4 MODE
			change_lpddr4_en(0xf,1);
		}

		dsb_sev();
		dsb_sev();

		if(DDR_TVAL0&0x80)
		{
			for(phychno=0;phychno<4;phychno++)
			{
//		0x00000700, 0x00000001, // PHY_SW_GRP_SHIFT_1:RW+:24:5:=0x00 PHY_SW_GRP_SHIFT_0:RW+:16:5:=0x00 PHY_FREQ_SEL_INDEX:RW:8:2:=0x00 PHY_FREQ_SEL_MULTICAST_EN:RW_D:0:1:=0x01
				REG_DDRPHY_WRITE(phychno, 0x700, (REG_DDRPHY_READ(phychno,0x700)&0xfffffcfe) | (0x2<<8) | (0x1<<0));
			}
		}

		if(DDR_UPDT_WA&0x8000)
		{
			if(phytrainingok&(1<<0))		copy_csdelay(0,0);
			if(phytrainingok&(1<<1))		copy_csdelay(1,2);
			if(phytrainingok&(1<<2))		copy_csdelay(2,0);
			if(phytrainingok&(1<<3))		copy_csdelay(3,0);
			if(phytrainingok&(1<<0))*((volatile uint32_t*)DBSC_DBPDCNT_0) = 0x0000CF00;	//dll_rst_n1 -> 0
			if(phytrainingok&(1<<1))*((volatile uint32_t*)DBSC_DBPDCNT_1) = 0x0000CF00;
			if(phytrainingok&(1<<2))*((volatile uint32_t*)DBSC_DBPDCNT_2) = 0x0000CF00;
			if(phytrainingok&(1<<3))*((volatile uint32_t*)DBSC_DBPDCNT_3) = 0x0000CF00;
			dsb_sev();
			if(phytrainingok&(1<<0))*((volatile uint32_t*)DBSC_DBPDCNT_0) = 0x0000CF01;	//dll_rst_n0 -> 1
			if(phytrainingok&(1<<1))*((volatile uint32_t*)DBSC_DBPDCNT_1) = 0x0000CF01;
			if(phytrainingok&(1<<2))*((volatile uint32_t*)DBSC_DBPDCNT_2) = 0x0000CF01;
			if(phytrainingok&(1<<3))*((volatile uint32_t*)DBSC_DBPDCNT_3) = 0x0000CF01;

			dsb_sev();
			dsb_sev();
		}

	}	/////////////////////////////////////////lvlen&0x21

	dsb_sev();
	dsb_sev();
	dsb_sev();

	PLL3_FREQ(freq,FREQ_H);

	dsb_sev();
	dsb_sev();
	dsb_sev();

//	for(phychno=0;phychno<4;phychno++)
//	{
//		if(!(phytrainingok&(1<<phychno)))continue;
///////////////////
	for(i=0;i<4;i++){
		if(DDR_TVAL1&0x10){
			phychno=3-i;
		} else {
			phychno=i;
		}
		if(!(phytrainingok&(1<<phychno)))continue;

		if(DDR_TVAL1&0x03){
//		0x00000408, 0x00000133, // PHY_LPBK_CONTROL_0:RW:24:7:=0x00 PHY_LP4_BOOT_RDDQS_LATENCY_ADJUST_0:RW:16:4:=0x00 PHY_LP4_BOOT_RDDQS_GATE_SLAVE_DELAY_0:RW:0:10:=0x0133
			uint32_t o_ch,o_slice,o_tmp;
			o_ch=phychno^0x1;
			if((DDR_TVAL1&0x03)==1)
					o_tmp=0x43;
			else	o_tmp=0x47;
			for(o_slice=0;o_slice<4;o_slice++){
				REG_DDRPHY_WRITE(o_ch, 0x408+0x80*o_slice,(REG_DDRPHY_READ(o_ch, 0x408+0x80*o_slice) & 0x00ffffff) | (o_tmp<<24));
			}
		}
////////////////////
		//DFI FREQ
		*((volatile uint32_t*)(DBSC_DFI_FREQ_0 +0x040 * phychno)) = FREQ_H;
		dsb_sev();
		//CHG_ACK
		*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0 + 0x40 * phychno)) = 0x0CF20000;
		dsb_sev();
		//CHG_ACK
		*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0 + 0x40 * phychno)) = 0x0;
		dsb_sev();

		if(0)
		{
			int slice;

			for(slice=0;slice<4;slice++)
			{
				for(i=0;i<4;i++)
				{
//		0x00000409, 0x000700C0, // SC_PHY_SNAP_OBS_REGS_0:WR:24:1:=0x00 PHY_GATE_ERROR_DELAY_SELECT_0:RW:16:4:=0x07 PHY_RDDQS_DQ_BYPASS_SLAVE_DELAY_0:RW:0:10:=0x00c0
					REG_DDRPHY_WRITE(phychno, 0x409+0x80*slice, (REG_DDRPHY_READ(phychno, 0x409+0x80*slice) | (1<<24)));
//		0x00000412, 0x00000000, // PHY_RDDQ_ENC_OBS_SELECT_0:RW:24:3:=0x00 PHY_MASTER_DLY_LOCK_OBS_SELECT_0:RW:16:3:=0x00 PHY_SW_FIFO_PTR_RST_DISABLE_0:RW:8:1:=0x00 PHY_SLAVE_LOOP_CNT_UPDATE_0:RW:0:3:=0x00
					REG_DDRPHY_WRITE(phychno, 0x412+0x80*slice, ((REG_DDRPHY_READ(phychno, 0x412+0x80*slice)  & 0xfff8ffff) | 0));
//		0x00000413, 0x00000000, // PHY_FIFO_PTR_OBS_SELECT_0:RW:24:4:=0x00 PHY_WR_SHIFT_OBS_SELECT_0:RW:16:4:=0x00 PHY_WR_ENC_OBS_SELECT_0:RW:8:4:=0x00 PHY_RDDQS_DQ_ENC_OBS_SELECT_0:RW:0:4:=0x00
					REG_DDRPHY_WRITE(phychno, 0x413+0x80*slice, ((REG_DDRPHY_READ(phychno, 0x413+0x80*slice)  & 0xfff0f0ff) | (9<<16) | (9<<16)));


//		0x00000423, 0x00000000, // PHY_RDDQS_BASE_SLV_DLY_ENC_OBS_0:RD:24:7:=0x00 PHY_RDDQ_SLV_DLY_ENC_OBS_0:RD:16:6:=0x00 PHY_MASTER_DLY_LOCK_OBS_0:RD:0:10:=0x0000
//		0x00000424, 0x00000000, // PHY_RDDQS_GATE_SLV_DLY_ENC_OBS_0:RD:16:10:=0x0000 PHY_RDDQS_DQ_FALL_ADDER_SLV_DLY_ENC_OBS_0:RD:8:8:=0x00 PHY_RDDQS_DQ_RISE_ADDER_SLV_DLY_ENC_OBS_0:RD:0:8:=0x00

					dataL=REG_DDRPHY_READ(phychno, 0x423+0x80*slice);
//		0x00000425, 0x00000000, // PHY_WR_SHIFT_OBS_0:RD:24:3:=0x00 PHY_WR_ADDER_SLV_DLY_ENC_OBS_0:RD:16:8:=0x00 PHY_WRDQ_BASE_SLV_DLY_ENC_OBS_0:RD:8:8:=0x00 PHY_WRDQS_BASE_SLV_DLY_ENC_OBS_0:RD:0:7:=0x00
					dataL=REG_DDRPHY_READ(phychno, 0x425+0x80*slice);
				}
			}
		}

		if(DDR_UPDT_WA&0x400)	//SEQ MODE
		{
			retry=0;
			while(retry<TE_MAX)
			{
				dataL = REG_DDRPHY_READ ( phychno, 0x0000029B);
				if(dataL&0x1)break;
				retry++;
			}
			if(retry>=TE_ERR)
			{
				phytrainingok&=~(1<<phychno);
			}
		}

		if(DDR_TVAL1&0x03)
		{
//		0x00000408, 0x00000133, // PHY_LPBK_CONTROL_0:RW:24:7:=0x00 PHY_LP4_BOOT_RDDQS_LATENCY_ADJUST_0:RW:16:4:=0x00 PHY_LP4_BOOT_RDDQS_GATE_SLAVE_DELAY_0:RW:0:10:=0x0133
			uint32_t o_ch,o_slice,o_tmp;
			o_ch=phychno^0x1;
			o_tmp=0x0;

			for(o_slice=0;o_slice<4;o_slice++){
				REG_DDRPHY_WRITE(o_ch, 0x408+0x80*o_slice, 
					(REG_DDRPHY_READ (o_ch, 0x408+0x80*o_slice) & 0x00ffffff) | (o_tmp<<24));
			}
		}
	}

	retry=10000;
	flag=0;
	while(retry>0)
	{
		for(phychno=0;phychno<4;phychno++)
		{
			if(!(phytrainingok&(1<<phychno)))continue;
			if(flag & (1<<phychno))continue;
			dataL = REG_DDRPHY_READ ( phychno, 0x0000029B);
			
			if(dataL&0x1)
			{
				flag |= (0x1<<phychno);
				// TRAING OK
			}
		}
		if((flag&phytrainingok)==phytrainingok)break;
		retry--;
	}

	phytrainingok&=flag;


	for(phychno=0;phychno<4;phychno++)
	{
		dataL = REG_DDRPHY_READ ( phychno, 0x0000029B);
		REG_DDRPHY_WRITE( phychno, 0x29c, dataL);
	}

	return phytrainingok;
}

static uint32_t pvt_pi_training(uint32_t freq)
{
	uint32_t phytrainingok=0;

	if(1)
	{
		pvt_lvl_enable_set(0x3f);

		if(DDR_UPDT_WA&0x4000)
		{
			phytrainingok = pvt_pi_training_go_all_soft(freq);
		}
		else if(DDR_UPDT_WA&0x100)
		{
			phytrainingok = pvt_pi_training_go_all_o(freq);
		}
		else
		{
			phytrainingok = pvt_pi_training_go_all(freq);
		}

		//FREQ_SEL_INDEX=3 (not needed? because MULTICAST_EN=1) ==> NEED!! otherwise, copy will not copy
		REG_DDRPHY_WRITE (0,0x00000700,0x0301);
		REG_DDRPHY_WRITE (1,0x00000700,0x0301);
		REG_DDRPHY_WRITE (2,0x00000700,0x0301);
		REG_DDRPHY_WRITE (3,0x00000700,0x0301);

		copy_dqdelay(1 ,1,2);		// ch1 2:0:1:3
		copy_dqdelay(1 ,3,0);
		copy_dqdelay(2 ,1,3);		// ch2 3:1:2:0
		copy_dqdelay(2 ,2,0);
		copy_dqdelay(3 ,1,3);		// ch3 3:1:2:0
		copy_dqdelay(3 ,2,0);


		if(DDR_LVLEN&0x20)
		{
			if(DDR_CALVLSIDE== 0x0)
			{
				copy_csdelay(0,0);
				copy_csdelay(1,0);
				copy_csdelay(2,0);
				copy_csdelay(3,0);
			}
			else if(DDR_CALVLSIDE== 0x1)
			{
				copy_csdelay(0,2);
				copy_csdelay(1,2);
				copy_csdelay(2,2);
				copy_csdelay(3,2);
			}
			else
			{
				copy_csdelay(0,0);
				copy_csdelay(1,2);
				copy_csdelay(2,0);
				copy_csdelay(3,0);
			}
		}

		if(DDR_LVLEN&0x4)
		{
			adjust_rddqsgatedelay(0);
			adjust_rddqsgatedelay(1);
			adjust_rddqsgatedelay(2);
			adjust_rddqsgatedelay(3);
		}
	}

	return phytrainingok;
}

static void WaitDBCMD(void)
{
	uint32_t dataL;

	while(1)
	{
		dataL = *((volatile uint32_t*)DBSC_DBWAIT);	//wait DBCMD 1=busy, 0=ready
		if((dataL & 0x00000001) == 0x0)	break;
	}
}


static void REG_DDRPHY_WRITE ( uintptr_t phyno, uint32_t regadd, uint32_t regdata)
{
	uint32_t i;

	dsb_sev();
	*((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno)) = regadd;
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));	//
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));	//
	dsb_sev();
	*((volatile uint32_t*)(DBSC_DBPDRGD_0 + 0x40 * phyno)) = regdata;
	i = *((volatile uint32_t*)(DBSC_DBPDRGD_0 + 0x40 * phyno));	//
	i = *((volatile uint32_t*)(DBSC_DBPDRGD_0 + 0x40 * phyno));	//
	dsb_sev();
	(void)i;
}

static void change_lpddr4_en(uintptr_t phychno, uint32_t mode)
{
/*
		0x0000071A, 0x027F6E00, // PHY_PAD_FDBK_DRIVE:RW+:8:18:=0x000411 PHY_TDFI_PHY_WRDELAY:RW:0:1:=0x00
		0x0000071B, 0x047F027F, // PHY_PAD_DQS_DRIVE:RW+:16:12:=0x0040 PHY_PAD_DATA_DRIVE:RW+:0:10:=0x0040
		0x0000071C, 0x00027F6E, // PHY_PAD_ADDR_DRIVE:RW+:0:18:=0x000411 // LPDDR4_EN bit 14 is it needed?
		0x0000071D, 0x00047F6E, // PHY_PAD_CLK_DRIVE:RW+:0:20:=0x000411
		0x0000071E, 0x00030F68, // PHY_PAD_FDBK_TERM:RW+:0:18:=0x004410 //PVTP = 0x28, PVTN=0x3d, PVTr=0x10
		0x0000071F, 0x00010F68, // PHY_PAD_DATA_TERM:RW+:0:17:=0x004410
		0x00000720, 0x00010F68, // PHY_PAD_DQS_TERM:RW+:0:17:=0x004410
		0x00000721, 0x00010F68, // PHY_PAD_ADDR_TERM:RW+:0:18:=0x004410
		0x00000722, 0x00010F68, // PHY_PAD_CLK_TERM:RW+:0:18:=0x004410
		0x00000723, 0x00027F6E, // PHY_PAD_CKE_DRIVE:RW+:0:18:=0x000411
		0x00000724, 0x00010F68, // PHY_PAD_CKE_TERM:RW+:0:18:=0x004410
		0x00000725, 0x00027F6E, // PHY_PAD_RST_DRIVE:RW+:0:18:=0x000411
		0x00000726, 0x00010F68, // PHY_PAD_RST_TERM:RW+:0:18:=0x004410
		0x00000727, 0x00027F6E, // PHY_PAD_CS_DRIVE:RW+:0:18:=0x000411
		0x00000728, 0x00010F68, // PHY_PAD_CS_TERM:RW+:0:18:=0x004410
*/

	uint32_t i,from,to;
	uint32_t set,clr;
	uint32_t dataL;
	uint32_t addr;

#ifdef	DDR_LPDDR4_EN_OFF_FIRST
#else
	return;
#endif

	if(phychno>=4)
	{
		from=0; to=3;
	}
	else
	{
		from=phychno; to=phychno;
	}


	for(i=from;i<=to;i++)
	{
#ifdef	DDR_LPDDR4_EN_OFF_FIRST
		set = 1ULL <<14;
		clr = ~set;
		
		if(!mode)
		{
			set = 0;
		};
		
		addr=0x71c;	REG_DDRPHY_WRITE(i,addr,dataL=((REG_DDRPHY_READ(i,addr)&clr)|set) );
		addr=0x71d;	REG_DDRPHY_WRITE(i,addr,dataL=((REG_DDRPHY_READ(i,addr)&clr)|set) );
		addr=0x723;	REG_DDRPHY_WRITE(i,addr,dataL=((REG_DDRPHY_READ(i,addr)&clr)|set) );
		addr=0x727;	REG_DDRPHY_WRITE(i,addr,dataL=((REG_DDRPHY_READ(i,addr)&clr)|set) );
#else
		addr=0x71c;	REG_DDRPHY_WRITE(i,addr,dataL=0x00027B22 );
		addr=0x71d;	REG_DDRPHY_WRITE(i,addr,dataL=0x00047B22 );
		addr=0x723;	REG_DDRPHY_WRITE(i,addr,dataL=0x00027B22 );
		addr=0x727;	REG_DDRPHY_WRITE(i,addr,dataL=0x00027B22 );
#endif
	}

}

static void copy_csdelay(uint32_t	phychno, uint32_t s_byte)
{
	uint32_t	dataL[4];

	dataL[0] = ((REG_DDRPHY_READ ( phychno, 0x00000712))& 0x000fff00) >> 8;
	dataL[1] = ((REG_DDRPHY_READ ( phychno, 0x00000713))& 0x00000fff);
	dataL[2] = ((REG_DDRPHY_READ ( phychno, 0x00000713))& 0x0fff0000) >> 16;
	dataL[3] = ((REG_DDRPHY_READ ( phychno, 0x00000714))& 0x00000fff);

	REG_DDRPHY_WRITE (phychno,0x00000712,( dataL[s_byte] << 8) |((REG_DDRPHY_READ ( phychno, 0x00000712)) & 0x0ff));
	REG_DDRPHY_WRITE (phychno,0x00000713,((dataL[s_byte] << 16)| dataL[s_byte]));
	REG_DDRPHY_WRITE (phychno,0x00000714,  dataL[s_byte]);

}

static void adjust_rddqsgatedelay(uintptr_t phychno)
{
	uint32_t	i;
	uint32_t	s_dataL;
	uint32_t	d_dataL;
	uint32_t	teL;
	uint32_t	leL;
	uint32_t	meL;
	uint32_t	fsL;
	uint32_t	latL,dlyL;
	uint32_t	maxlatL =0x0;
	uint32_t	maxdlyL =0x0;

#ifdef	DDR_DBS_DFI_WA
	for(i=0;i<4;i++)
	{
		leL = (REG_DDRPHY_READ ( phychno, 0x00000429 + 0x80*i)>>16) & 0x3fff;
		teL = (REG_DDRPHY_READ ( phychno, 0x0000042A + 0x80*i)>>0 ) & 0x3fff;
		fsL = (REG_DDRPHY_READ ( phychno, 0x00000456 + 0x80*i)>>16) & 0x3ff;
		meL = (leL+teL)>>1;
		dlyL = (meL-fsL)&0x1ff;
		latL = ((meL-fsL)>>9) & 7;
		if(maxlatL < (dlyL + latL* 0x200))
		{
			maxdlyL =dlyL;
			maxlatL =dlyL + latL* 0x200;
		}
	}
#endif

	for(i=0;i<4;i++)
	{
		leL = (REG_DDRPHY_READ ( phychno, 0x00000429 + 0x80*i)>>16) & 0x3fff;
		teL = (REG_DDRPHY_READ ( phychno, 0x0000042A + 0x80*i)>>0 ) & 0x3fff;
		fsL = (REG_DDRPHY_READ ( phychno, 0x00000456 + 0x80*i)>>16 ) & 0x3ff;
		meL = (leL+teL)>>1;
		if(meL<fsL)meL=fsL;///CLIP
		dlyL = (meL-fsL)&0x1ff;
		latL = ((meL-fsL)>>9) & 7;
#ifdef	DDR_DBS_DFI_WA
		if((maxdlyL > 0xff) & (dlyL < 0x100))
		{
			REG_DDRPHY_WRITE ( phychno, 0x0454 + 0x80*i, (REG_DDRPHY_READ ( phychno, 0x00000454 + 0x80*i) + 0x00010000));
		}
#endif
		s_dataL = REG_DDRPHY_READ ( phychno, 0x0000044C + 0x80*i);
		d_dataL = (s_dataL & 0xff000000) | (latL<<16) | dlyL;
		REG_DDRPHY_WRITE ( phychno, 0x0000044C + 0x80*i,d_dataL);
	}
}


static void copy_dqdelay(uintptr_t phychno, uint32_t s_byte, uint32_t d_byte)
{
	uint32_t	i;
	uint32_t	s_dataL;
	uint32_t	d_dataL;

	for (i=0x43A;i<=0x43E;i++)
	{
								// 43A-43E:PHY_CLK_WRDQx_SLAVE_DELAY_0
								// 43F-442:PHY_RDDQx_SLAVE_DELAY_0
								// 443-44B:PHY_RDDQS_DQx_FALL_SLAVE_DELAY_0
		if(i==0x43E)
		{			// 43E:PHY_CLK_WRDQS_SLAVE_DELAY_0:RW+:16:10
			s_dataL = (REG_DDRPHY_READ ( phychno, i+s_byte*0x80)) & 0x0000ffff; // DM code
			d_dataL = (REG_DDRPHY_READ ( phychno, i+d_byte*0x80)) & 0xffff0000; // DQS code
			REG_DDRPHY_WRITE (phychno,i+d_byte*0x80,(s_dataL|d_dataL));
		}
		else
		{
			s_dataL = REG_DDRPHY_READ ( phychno, i+s_byte*0x80);
			REG_DDRPHY_WRITE (phychno,i+d_byte*0x80,s_dataL);
		}
	}

}

uint32_t pvt_pi_training_go_all_soft(uint32_t freq)
{
	uint32_t flag;
	uint32_t dataL;
	uint32_t phytrainingok;
	uint32_t retry;
	uintptr_t phychno;

	PLL3_FREQ(freq,0);

	for(phychno=0;phychno<4;phychno++)
	{
		//dfi_init_start negate
		*((volatile uint32_t*)(DBSC_DBDFICNT_0+0x40*phychno)) = 0x00000010;
		dsb_sev();
	}

	for(phychno=0;phychno<4;phychno++)
	{
		//////////////////////
		*((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x040 * phychno)) = 0x200;
		dsb_sev();
		*((volatile uint32_t*)(DBSC_DBPDRGD_0 + 0x040 * phychno)) = 0x20410B01;
		dsb_sev();

		dsb_sev();
		dsb_sev();
	}
	do {
		dataL  = *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*0)) & 0x1f;
		dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*1)) & 0x1f;
		dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*2)) & 0x1f;
		dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*3)) & 0x1f;
		dsb_sev();
	} while(dataL);

	dsb_sev();
	dsb_sev();

	if(!(DDR_UPDT_WA&0x2000))
	{
		PLL3_FREQ(freq,2);
		dsb_sev();
		dsb_sev();
	}

//	//CHANGE IMPEDANCE CODE to LP4 MODE
//	change_lpddr4_en(0xf,1);

	for(phychno=0;phychno<4;phychno++)
	{
		//DFI FREQ
		*((volatile uint32_t*)(DBSC_DFI_FREQ_0 +0x040 * phychno)) = 2;
//		*((volatile uint32_t*)(DBSC_DFI_FREQ_0 +0x040 * phychno)) = 0;
		dsb_sev();
		//CHG_ACK
		*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0 + 0x40 * phychno)) = 0x0CF20000;
		dsb_sev();
//		//CHG_ACK
		*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0 + 0x40 * phychno)) = 0x0;
		dsb_sev();

		dsb_sev();
		dsb_sev();
	}


	///////////////////////////////////////////////////////////////////////
	//unlock check
	do {
		dataL  = *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*0)) & 0x1f;
		dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*1)) & 0x1f;
		dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*2)) & 0x1f;
		dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*3)) & 0x1f;
		dsb_sev();
	} while(dataL);

	dsb_sev();
	dsb_sev();

	PLL3_FREQ(freq,0);

	dsb_sev();
	dsb_sev();

	for(phychno=0;phychno<4;phychno++)
	{
		//DFI FREQ
		*((volatile uint32_t*)(DBSC_DFI_FREQ_0 +0x040 * phychno)) =0;	// 0;
//		*((volatile uint32_t*)(DBSC_DFI_FREQ_0 +0x040 * phychno)) =2;	// 2
		//CHG_ACK
		*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0 + 0x40 * phychno)) = 0x0CF20000;
		//CHG_ACK
		*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0 + 0x40 * phychno)) = 0x0;

		dsb_sev();
		dsb_sev();
	}

//END:timing critical:
	//unlock check
	do {
		dataL  = *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*0)) & 0x1f;
		dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*1)) & 0x1f;
		dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*2)) & 0x1f;
		dataL |= *((volatile uint32_t*)(DBSC_PLL_LOCK_0+0x100*3)) & 0x1f;
	} while(dataL);

	dsb_sev();
	dsb_sev();

	PLL3_FREQ(freq,2);

	dsb_sev();
	dsb_sev();

	for(phychno=0;phychno<4;phychno++)
	{
		//DFI FREQ
		*((volatile uint32_t*)(DBSC_DFI_FREQ_0 +0x040 * phychno)) = 2;
		dsb_sev();
		//CHG_ACK
		*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0 + 0x40 * phychno)) = 0x0CF20000;
		dsb_sev();
		//CHG_ACK
		*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0 + 0x40 * phychno)) = 0x0;

		dsb_sev();
		dsb_sev();
	}

	retry=100000;
	flag=0;
	while(retry>0)
	{
		for(phychno=0;phychno<4;phychno++)
		{
			if(flag & (1<<phychno))continue;
			dataL = REG_DDRPHY_READ ( phychno, 0x0000029B);
			if(dataL&0x1)
			{
				flag |= (0x1<<phychno);
				// TRAING OK
			}
		}
		if((flag&DDR_PHYVALID)==DDR_PHYVALID)break;
		retry--;
	}

	phytrainingok=flag;

	for(phychno=0;phychno<4;phychno++)
	{
		dataL = REG_DDRPHY_READ ( phychno, 0x0000029B);
		REG_DDRPHY_WRITE( phychno, 0x29c, dataL);
	}

	return phytrainingok;
}

inline void dsb_sev(void)
{
	__asm__ __volatile__ ("dsb sy");

}

static uint32_t REG_DDRPHY_READ ( uintptr_t phyno, uint32_t regadd)
{
	uint32_t i;

	dsb_sev();
	*((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno)) = regadd;
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));	//
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));	//
	i = *((volatile uint32_t*)(DBSC_DBPDRGD_0 + 0x40 * phyno));
	i = *((volatile uint32_t*)(DBSC_DBPDRGD_0 + 0x40 * phyno));
	dsb_sev();
	return i ;
}

static uint32_t InitDDR_0917(uint32_t freq)
{
	uint32_t i=0;
	uint32_t j=0;
	uint32_t k=0;
	uint32_t dataL=0;
	uintptr_t phychno=0x0;
	uint32_t phytrainingok=0x0;
	uint32_t retry;
	uint32_t ch,slice;


//	if(mode==0 || mode==1|| mode==3)
	{
		//DDR RESET ASSERT
		*((volatile uint32_t*)DBSC_DBPDCNT0_0) = 0x01;
		*((volatile uint32_t*)DBSC_DBPDCNT0_1) = 0x01;
		*((volatile uint32_t*)DBSC_DBPDCNT0_2) = 0x01;
		*((volatile uint32_t*)DBSC_DBPDCNT0_3) = 0x01;

		dsb_sev();

#ifdef	DDR_PLL3ONLY
		dataL = *((volatile uint32_t*)DBSC_DBKIND);

/*
//		//reset pulse
		*((volatile uint32_t*)CPG_CPGWPR) = ~0x40000000;
		dsb_sev();
		do {
			*((volatile uint32_t*)CPG_SRCR4) =  0x40000000;
//			dsb_sev();
			*((volatile uint32_t*)CPG_SRSTCLR4) = 0x40000000;
			dsb_sev();
			dataL = *((volatile uint32_t*)DBSC_DBKIND);
		} while (dataL!=0);
*/
		*((volatile uint32_t*)DBSC_DBPDLK_0) = 0x0000A55A;	//Unlock DDRPHY register
		*((volatile uint32_t*)DBSC_DBPDLK_1) = 0x0000A55A;
		*((volatile uint32_t*)DBSC_DBPDLK_2) = 0x0000A55A;
		*((volatile uint32_t*)DBSC_DBPDLK_3) = 0x0000A55A;

		REG_DDRPHY_WRITE(0,0x229,0x0);
		REG_DDRPHY_WRITE(1,0x229,0x0);
		REG_DDRPHY_WRITE(2,0x229,0x0);
		REG_DDRPHY_WRITE(3,0x229,0x0);

		dataL=	*((volatile uint32_t*)CPG_PLLECR);
		*((volatile uint32_t*)CPG_CPGWPR) = ~(dataL & 0xFFFFFFF7);
		*((volatile uint32_t*)CPG_PLLECR) =   dataL & 0xFFFFFFF7;

		dsb_sev();

		// PLL3
#ifdef	DDR_FREQCHG
		j = *((volatile uint32_t*)CPG_PLL3CR);

#ifdef	DDR_LOWFIX
		j = (j&0x00ffff7f) | ((DDR_LOWFIX & 0xff) << 24);
#else	// DDR_LOWFIX
		j = (j&0x00ffff7f) | ((freq & 0xff) << 24) | 0x080;
#endif	// DDR_LOWFIX

#else	// DDR_FREQCHG
		j = *((volatile uint32_t*)CPG_PLL3CR);
		j = (j&0x00ffff7f) | ((freq << 24)&0xff000000)| ((DDR_FDIV << 7)&0x00000080);
#endif	// DDR_FREQCHG

		*((volatile uint32_t*)CPG_CPGWPR) = ~j;
		*((volatile uint32_t*)CPG_PLL3CR) =  j;		// CPG_PLL3CL

		*((volatile uint32_t*)CPG_CPGWPR) = ~(dataL |0x8);
		*((volatile uint32_t*)CPG_PLLECR) =   dataL |0x8;

		do {
			dataL=*((volatile uint32_t*)CPG_PLLECR);
		} while( (dataL&0x800)==0 );

		*((volatile uint32_t*)CPG_CPGWPR) = ~0x40000000;
		do {
			*((volatile uint32_t*)CPG_SRCR4) = 0x40000000;
			*((volatile uint32_t*)CPG_SRSTCLR4) = 0x40000000;
			dataL = *((volatile uint32_t*)DBSC_DBKIND);
		} while (dataL!=0);

		REG_DDRPHY_WRITE(0,0x229,0x100);
		REG_DDRPHY_WRITE(1,0x229,0x100);
		REG_DDRPHY_WRITE(2,0x229,0x100);
		REG_DDRPHY_WRITE(3,0x229,0x100);

#endif	//	DDR_PLL3ONLY

		pvt_dbsc_regset(freq);

		//DDR RESET NEGATE
		*((volatile uint32_t*)DBSC_DBPDCNT0_0) = 0x00;
		*((volatile uint32_t*)DBSC_DBPDCNT0_1) = 0x00;
		*((volatile uint32_t*)DBSC_DBPDCNT0_2) = 0x00;
		*((volatile uint32_t*)DBSC_DBPDCNT0_3) = 0x00;

		//(DDRPHY Kick Initialize Sequence)
		*((volatile uint32_t*)DBSC_DBPDLK_0) = 0x0000A55A;	//Unlock DDRPHY register(AGAIN)
		*((volatile uint32_t*)DBSC_DBPDLK_1) = 0x0000A55A;
		*((volatile uint32_t*)DBSC_DBPDLK_2) = 0x0000A55A;
		*((volatile uint32_t*)DBSC_DBPDLK_3) = 0x0000A55A;

		//(PHY parameter and PI parameter Initialize)	Initialize Leveling parameter including WDQLVL

		for( i=0; i<DDR_PHY_NUM; i++ )
		{
			REG_DDRPHY_WRITE(0,DDR_PHY_suresh[i][0],DDR_PHY_suresh[i][1]);
			REG_DDRPHY_WRITE(1,DDR_PHY_suresh[i][0],DDR_PHY_suresh[i][1]);
			REG_DDRPHY_WRITE(2,DDR_PHY_suresh[i][0],DDR_PHY_suresh[i][1]);
			REG_DDRPHY_WRITE(3,DDR_PHY_suresh[i][0],DDR_PHY_suresh[i][1]);
		}

#ifdef	DDR_DRIVE
		for( i=0; i<DDR_PHY_DRIVE_TERM_OVERWRITE_NUM; i++ )
		{
			REG_DDRPHY_WRITE(0,DDR_PHY_DDR_PHY_DRIVE_TERM_OVERWITE_1600[i][0],DDR_PHY_DDR_PHY_DRIVE_TERM_OVERWITE_1600[i][1]);
			REG_DDRPHY_WRITE(1,DDR_PHY_DDR_PHY_DRIVE_TERM_OVERWITE_1600[i][0],DDR_PHY_DDR_PHY_DRIVE_TERM_OVERWITE_1600[i][1]);
			REG_DDRPHY_WRITE(2,DDR_PHY_DDR_PHY_DRIVE_TERM_OVERWITE_1600[i][0],DDR_PHY_DDR_PHY_DRIVE_TERM_OVERWITE_1600[i][1]);
			REG_DDRPHY_WRITE(3,DDR_PHY_DDR_PHY_DRIVE_TERM_OVERWITE_1600[i][0],DDR_PHY_DDR_PHY_DRIVE_TERM_OVERWITE_1600[i][1]);
		}
#endif
//		0x00000404, 0x03006E0E, // PHY_DQS_TSEL_ENABLE_0:RW:24:3:=0x01 PHY_DQ_TSEL_SELECT_0:RW:0:24:=0x665555
//		0x00000405, 0x02006E0E, // PHY_TWO_CYC_PREAMBLE_0:RW:24:2:=0x02 PHY_DQS_TSEL_SELECT_0:RW:0:24:=0x665555
#ifdef	DDR_TSEL_SELECT
		{
			uint32_t adr;

			for(ch=0;ch<4;ch++)
			{
				for(slice=0;slice<4;slice++)
				{
					for(i=0;i<2;i++)
					{
						adr=0x404+i+0x80*slice;
						dataL=REG_DDRPHY_READ(ch,adr);
						dataL=(dataL&0xff000000) | DDR_TSEL_SELECT;
						REG_DDRPHY_WRITE(ch,adr,dataL);
					}
				}
			}
		}
#endif

		for( i=0; i<DDR_PI_NUM; i++ )
		{
			REG_DDRPHY_WRITE(0,DDR_PI_suresh[i][0],DDR_PI_suresh[i][1]);
			REG_DDRPHY_WRITE(1,DDR_PI_suresh[i][0],DDR_PI_suresh[i][1]);
			REG_DDRPHY_WRITE(2,DDR_PI_suresh[i][0],DDR_PI_suresh[i][1]);
			REG_DDRPHY_WRITE(3,DDR_PI_suresh[i][0],DDR_PI_suresh[i][1]);
		}

// Traning Reg setting
		if(freq>0x53)
		{
			//							// def 3200
		}
		else if(freq>0x47)
		{
//			freq2800_reg_set();			// need more tuning
		}
		else if(freq>0x2f)
		{
//			freq2400_reg_set();			// need more tuning
		}
		else
		{
			freq1600_reg_set();
		}

		//Set PCB bit swap connection 
		for( i=0; i<9+5; i++ )
		{
			REG_DDRPHY_WRITE(0,DDR_SIP_SWAP_CH0[i][0],DDR_SIP_SWAP_CH0[i][1]);
			REG_DDRPHY_WRITE(1,DDR_SIP_SWAP_CH1[i][0],DDR_SIP_SWAP_CH1[i][1]);
			REG_DDRPHY_WRITE(2,DDR_SIP_SWAP_CH2[i][0],DDR_SIP_SWAP_CH2[i][1]);
			REG_DDRPHY_WRITE(3,DDR_SIP_SWAP_CH3[i][0],DDR_SIP_SWAP_CH3[i][1]);
		}

		for (i=0;i<4;i++)
		{
			REG_DDRPHY_WRITE(i,0x40B,REG_DDRPHY_READ(i,0x40B) & (0xfffcffff));
			REG_DDRPHY_WRITE(i,0x48B,REG_DDRPHY_READ(i,0x48B) & (0xfffcffff));
			REG_DDRPHY_WRITE(i,0x50B,REG_DDRPHY_READ(i,0x50B) & (0xfffcffff));
			REG_DDRPHY_WRITE(i,0x58B,REG_DDRPHY_READ(i,0x58B) & (0xfffcffff));
		}

		if(DDR_CALVLSIDE== 0x0)
		{
			REG_DDRPHY_WRITE(0,0x709,((REG_DDRPHY_READ(0,0x709) & 0xffffff00) | 0x0000010));
			REG_DDRPHY_WRITE(0,0x257,((REG_DDRPHY_READ(0,0x257) & 0xfffffff0) | 0x0000001));

			REG_DDRPHY_WRITE(1,0x709,((REG_DDRPHY_READ(1,0x709) & 0xffffff00) | 0x0000011));
			REG_DDRPHY_WRITE(1,0x257,((REG_DDRPHY_READ(1,0x257) & 0xfffffff0) | 0x0000001));
			REG_DDRPHY_WRITE(1,0x60c,0x00543210);
			REG_DDRPHY_WRITE(1,0x421,((REG_DDRPHY_READ(1,0x421) & 0xfffffffe) | 0x0000001));
			REG_DDRPHY_WRITE(1,0x4a1,((REG_DDRPHY_READ(1,0x4a1) & 0xfffffffe) | 0x0000001));
			REG_DDRPHY_WRITE(1,0x521,((REG_DDRPHY_READ(1,0x521) & 0xfffffffe) | 0x0000000));
			REG_DDRPHY_WRITE(1,0x5a1,((REG_DDRPHY_READ(1,0x5a1) & 0xfffffffe) | 0x0000000));

			REG_DDRPHY_WRITE(2,0x709,((REG_DDRPHY_READ(2,0x709) & 0xffffff00) | 0x0000000));
			REG_DDRPHY_WRITE(2,0x257,((REG_DDRPHY_READ(2,0x257) & 0xfffffff0) | 0x0000001));
			REG_DDRPHY_WRITE(2,0x60c,0x00DCBA98);
			REG_DDRPHY_WRITE(2,0x421,((REG_DDRPHY_READ(2,0x421) & 0xfffffffe) | 0x0000000));
			REG_DDRPHY_WRITE(2,0x4a1,((REG_DDRPHY_READ(2,0x4a1) & 0xfffffffe) | 0x0000000));
			REG_DDRPHY_WRITE(2,0x521,((REG_DDRPHY_READ(2,0x521) & 0xfffffffe) | 0x0000001));
			REG_DDRPHY_WRITE(2,0x5a1,((REG_DDRPHY_READ(2,0x5a1) & 0xfffffffe) | 0x0000001));

			REG_DDRPHY_WRITE(3,0x709,((REG_DDRPHY_READ(3,0x709) & 0xffffff00) | 0x0000000));
			REG_DDRPHY_WRITE(3,0x257,((REG_DDRPHY_READ(3,0x257) & 0xfffffff0) | 0x0000001));
			REG_DDRPHY_WRITE(3,0x60c,0x00DCBA98);
			REG_DDRPHY_WRITE(3,0x421,((REG_DDRPHY_READ(3,0x421) & 0xfffffffe) | 0x0000000));
			REG_DDRPHY_WRITE(3,0x4a1,((REG_DDRPHY_READ(3,0x4a1) & 0xfffffffe) | 0x0000000));
			REG_DDRPHY_WRITE(3,0x521,((REG_DDRPHY_READ(3,0x521) & 0xfffffffe) | 0x0000001));
			REG_DDRPHY_WRITE(3,0x5a1,((REG_DDRPHY_READ(3,0x5a1) & 0xfffffffe) | 0x0000001));
		}

		if(DDR_CALVLSIDE== 0x1)
		{
			REG_DDRPHY_WRITE(0,0x709,((REG_DDRPHY_READ(0,0x709) & 0xffffff00) | 0x0000010));
			REG_DDRPHY_WRITE(0,0x257,((REG_DDRPHY_READ(0,0x257) & 0xfffffff0) | 0x0000004));

			REG_DDRPHY_WRITE(1,0x709,((REG_DDRPHY_READ(1,0x709) & 0xffffff00) | 0x0000011));
			REG_DDRPHY_WRITE(1,0x257,((REG_DDRPHY_READ(1,0x257) & 0xfffffff0) | 0x0000004));
			REG_DDRPHY_WRITE(1,0x60c,0x00DCBA98);
			REG_DDRPHY_WRITE(1,0x421,((REG_DDRPHY_READ(1,0x421) & 0xfffffffe) | 0x0000001));
			REG_DDRPHY_WRITE(1,0x4a1,((REG_DDRPHY_READ(1,0x4a1) & 0xfffffffe) | 0x0000001));
			REG_DDRPHY_WRITE(1,0x521,((REG_DDRPHY_READ(1,0x521) & 0xfffffffe) | 0x0000000));
			REG_DDRPHY_WRITE(1,0x5a1,((REG_DDRPHY_READ(1,0x5a1) & 0xfffffffe) | 0x0000000));

			REG_DDRPHY_WRITE(2,0x709,((REG_DDRPHY_READ(2,0x709) & 0xffffff00) | 0x0000000));
			REG_DDRPHY_WRITE(2,0x257,((REG_DDRPHY_READ(2,0x257) & 0xfffffff0) | 0x0000004));
			REG_DDRPHY_WRITE(2,0x60c,0x00543210);
			REG_DDRPHY_WRITE(2,0x421,((REG_DDRPHY_READ(2,0x421) & 0xfffffffe) | 0x0000000));
			REG_DDRPHY_WRITE(2,0x4a1,((REG_DDRPHY_READ(2,0x4a1) & 0xfffffffe) | 0x0000000));
			REG_DDRPHY_WRITE(2,0x521,((REG_DDRPHY_READ(2,0x521) & 0xfffffffe) | 0x0000001));
			REG_DDRPHY_WRITE(2,0x5a1,((REG_DDRPHY_READ(2,0x5a1) & 0xfffffffe) | 0x0000001));

			REG_DDRPHY_WRITE(3,0x709,((REG_DDRPHY_READ(3,0x709) & 0xffffff00) | 0x0000000));
			REG_DDRPHY_WRITE(3,0x257,((REG_DDRPHY_READ(3,0x257) & 0xfffffff0) | 0x0000004));
			REG_DDRPHY_WRITE(3,0x60c,0x00543210);
			REG_DDRPHY_WRITE(3,0x421,((REG_DDRPHY_READ(3,0x421) & 0xfffffffe) | 0x0000000));
			REG_DDRPHY_WRITE(3,0x4a1,((REG_DDRPHY_READ(3,0x4a1) & 0xfffffffe) | 0x0000000));
			REG_DDRPHY_WRITE(3,0x521,((REG_DDRPHY_READ(3,0x521) & 0xfffffffe) | 0x0000001));
			REG_DDRPHY_WRITE(3,0x5a1,((REG_DDRPHY_READ(3,0x5a1) & 0xfffffffe) | 0x0000001));
		}
#ifdef	DDR_VREF
		for (i=0;i<4;i++)
		{
			REG_DDRPHY_WRITE(i,0x70F,((REG_DDRPHY_READ(i,0x70f) & 0x000000ff)|(DDR_VREF << 8) ));
			REG_DDRPHY_WRITE(i,0x710,(DDR_VREF |(DDR_VREF << 16) ));
			REG_DDRPHY_WRITE(i,0x711,(DDR_VREF |(DDR_VREF << 16) ));
		}
#endif


#ifdef	DDR_PAD_BOOST
		for (i=0;i<4;i++)
		{
			REG_DDRPHY_WRITE(i,0x71B,((REG_DDRPHY_READ(i,0x71B) & 0xfbfffdff) ));	//phy_pad_dqs_drive ,phy_pad_data_drive
		}
#else
		for (i=0;i<4;i++)
		{
			REG_DDRPHY_WRITE(i,0x71B,((REG_DDRPHY_READ(i,0x71B) | 0x04000200) ));	//phy_pad_dqs_drive ,phy_pad_data_drive
		}
#endif

#ifdef	DDR_PAD_ADDR_CS_DRIVE
		for (i=0;i<4;i++){
			REG_DDRPHY_WRITE(i,0x71C,DDR_PAD_ADDR_CS_DRIVE);	//pad_addr_drive
			REG_DDRPHY_WRITE(i,0x727,DDR_PAD_ADDR_CS_DRIVE);	//pad_cs_drive
		}
#endif

//#define CH0B
#ifdef  CH0B
		// CH0 CALVL for swap
		// CH0 CALVL to CS[2] ( M2 dq_h -> soc M2 dq_h )
		REG_DDRPHY_WRITE(0,0x0257,0x00181704);		// PI_CALVL_CS_MAP:RW:0:4:= 0x4
		//0x0000060F, 0x01000000, // PHY_ADR_CALVL_DEVICE_MAP_0:RW:24:4:=0x01 PHY_ADR_CALVL_SWIZZLE1_1_0:RW:0:24:=0x000000	// SoC Byte3   -> DRAM Byte 3
		REG_DDRPHY_WRITE(0,0x060F,0x02DCBA98);
#endif
		// CH1 CALVL for swap
		// CH1 CALVL can be done only for CS[2] ( M2 dq_h -> soc M2 dq_h )
		REG_DDRPHY_WRITE(1,0x0257,0x00181704);		// PI_CALVL_CS_MAP:RW:0:4:= 0x4

		*((volatile uint32_t*)DBSC_DBDFIPMSTRCNF) = 0x00000001;	//DFI_PHYMSTR_ACK = 1
	}	// (mode==0 || mode==1|| mode==3)


	///////////////////////////////////////////////////////////////////////////////////////////////////////////end_of_mode1
	pvt_lvl_enable_set(0x3f);

	set_cacs_delay();

	REG_DDRPHY_WRITE(0,0x44c,0x0004000c);
	REG_DDRPHY_WRITE(0,0x4cc,0x0004000c);
	REG_DDRPHY_WRITE(0,0x54c,0x00040042);
	REG_DDRPHY_WRITE(0,0x5cc,0x0004004e);
	REG_DDRPHY_WRITE(1,0x44c,0x0004004e);
	REG_DDRPHY_WRITE(1,0x4cc,0x0004005a);
	REG_DDRPHY_WRITE(1,0x54c,0x00040078);
	REG_DDRPHY_WRITE(1,0x5cc,0x00040042);
	REG_DDRPHY_WRITE(2,0x44c,0x00040012);
	REG_DDRPHY_WRITE(2,0x4cc,0x00040066);
	REG_DDRPHY_WRITE(2,0x54c,0x000301f4);
	REG_DDRPHY_WRITE(2,0x5cc,0x0004004e);
	REG_DDRPHY_WRITE(3,0x44c,0x0004001e);
	REG_DDRPHY_WRITE(3,0x4cc,0x00040042);
	REG_DDRPHY_WRITE(3,0x54c,0x00040066);
	REG_DDRPHY_WRITE(3,0x5cc,0x000400c6);

// 9/24
//cacs_capture_cnt//cacs_resp_wait_cnt
//		0x00000257, 0x00181701, // PI_TDFI_CALVL_CC_F0:RW:16:10:=0x0018 PI_TDFI_CALVL_EN:RW:8:8:=0x17 PI_CALVL_CS_MAP:RW:0:4:=0x03
//		0x00000258, 0x00280006, // PI_TDFI_CALVL_CC_F1:RW:16:10:=0x0028 PI_TDFI_CALVL_CAPTURE_F0:RW:0:10:=0x0006
//		0x00000259, 0x00280016, // PI_TDFI_CALVL_CC_F2:RW:16:10:=0x0028 PI_TDFI_CALVL_CAPTURE_F1:RW:0:10:=0x0016
//		0x0000025A, 0x00000016, // PI_TDFI_CALVL_CAPTURE_F2:RW:0:10:=0x0016
	for(ch=0;ch<4;ch++){
//		0x00000610, 0x00020010, // PHY_ADR_CALVL_RESP_WAIT_CNT_0:RW:24:4:=0x00 PHY_ADR_CALVL_CAPTURE_CNT_0:RW:16:4:=0x02 PHY_ADR_CALVL_NUM_PATTERNS_0:RW:8:2:=0x00 PHY_ADR_CALVL_RANK_CTRL_0:RW:0:5:=0x10
//		0x0000060A, 0x00400320, // PHY_ADR_CALVL_COARSE_DLY_0:RW:16:11:=0x0040 PHY_ADR_CALVL_START_0:RW:0:11:=0x0320
//		0x0000060B, 0x00000040, // PHY_ADR_CALVL_QTR_0:RW:0:11:=0x0040
//		0x00000624, 0x00000003, // PHY_ADR_CALVL_DLY_STEP_0:RW+:0:4:=0x03

		uint32_t cc=(DDR_CACS_CCCP>>16)&0xff;
		uint32_t cp=(DDR_CACS_CCCP>>0)&0xff;

		REG_DDRPHY_WRITE(ch,0x257,(REG_DDRPHY_READ(ch,0x257) & 0x0000ffff) | (cc<<16)); 
		REG_DDRPHY_WRITE(ch,0x258,(REG_DDRPHY_READ(ch,0x258) & 0x00000000) | (cc<<16) | cp); 
		REG_DDRPHY_WRITE(ch,0x259,(REG_DDRPHY_READ(ch,0x259) & 0x00000000) | (cc<<16) | cp); 
		REG_DDRPHY_WRITE(ch,0x25a,(REG_DDRPHY_READ(ch,0x25a) & 0x00000000) | cp); 


		for(slice=0;slice<2;slice++){
			REG_DDRPHY_WRITE(ch,0x610+0x80*slice,(REG_DDRPHY_READ(ch,0x610+0x80*slice) & 0xf0f0ffff )
				| ((DDR_CACS_RESP_WAIT_CNT&0x0f)<<24)
				| ((DDR_CACS_CAPTURE_CNT&0x0f)<<16)
			);
			REG_DDRPHY_WRITE(ch,0x60a+0x80*slice,(REG_DDRPHY_READ(ch,0x60a+0x80*slice) & 0xfffff000 )
				| DDR_CACS_START
			);
			REG_DDRPHY_WRITE(ch,0x60b+0x80*slice,(REG_DDRPHY_READ(ch,0x60b+0x80*slice) & 0xfffff000 )
				| DDR_CACS_QTR
			);
			REG_DDRPHY_WRITE(ch,0x624+0x80*slice,(REG_DDRPHY_READ(ch,0x624+0x80*slice) & 0xfffffff0 )
				| DDR_CACS_STEP
			);
		}

//		0x00000704, 0x00020040, // PHY_CSLVL_CAPTURE_CNT:RW:16:4:=0x02 PHY_CSLVL_QTR:RW:0:11:=0x0040
//		0x00000705, 0x00020055, // PHY_CSLVL_DEBUG_MODE:RW:24:1:=0x00 PHY_CSLVL_COARSE_CAPTURE_CNT:RW:16:4:=0x02 PHY_CSLVL_COARSE_DLY:RW:0:11:=0x0055
		slice=0;
		REG_DDRPHY_WRITE(ch,0x704+0x80*slice,(REG_DDRPHY_READ(ch,0x704+0x80*slice) & 0xfff0ffff) | ((DDR_CACS_CAPTURE_CNT&0x0f)<<16));
		REG_DDRPHY_WRITE(ch,0x705+0x80*slice,(REG_DDRPHY_READ(ch,0x705+0x80*slice) & 0xfff0ffff) | ((DDR_CACS_CAPTURE_CNT&0x0f)<<16));
	}
//-------

	dsb_sev();
	dsb_sev();
	dsb_sev();

	PLL3_FREQ(freq,0);

	dsb_sev();
	dsb_sev();
	dsb_sev();

#ifdef	DDR_PAD_CAL_WA
	for(i=0;i<4;i++)
	{
		REG_DDRPHY_WRITE(i,0x72c,REG_DDRPHY_READ(i,0x72c)& ~(1<<24) ); //init_start_disable clear
	}

	// Rx Cal IE issue w/a
	for(ch=0;ch<4;ch++)
	{
		for(slice=0;slice<4;slice++)
		{
			if(DDR_TVAL2&0x1)
			{
				REG_DDRPHY_WRITE(ch,0x452+0x80*slice,REG_DDRPHY_READ(ch,0x452+0x80*slice) | (1<<16));
			}
//		0x00000433, 0x00200000, // PHY_RX_CAL_SAMPLE_WAIT_0:RW:16:8:=0x20 PHY_RX_CAL_OVERRIDE_0:RW:8:1:=0x00 SC_PHY_RX_CAL_START_0:WR:0:1:=0x00
			if(DDR_TVAL2&0x2)
			{
				REG_DDRPHY_WRITE(ch,0x433+slice*0x80,REG_DDRPHY_READ(ch,0x433+slice*0x80)|0x100);
			}
			else
			{
				REG_DDRPHY_WRITE(ch,0x433+slice*0x80,REG_DDRPHY_READ(ch,0x433+slice*0x80)&0xfffffeff);
			}
		}
	}
	dsb_sev();
#endif

	*((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x00000010;	//dbdficnt0 freq_ratio = 01 (2:1)init_start =1
	*((volatile uint32_t*)DBSC_DBDFICNT_1) = 0x00000010;
	*((volatile uint32_t*)DBSC_DBDFICNT_2) = 0x00000010;
	*((volatile uint32_t*)DBSC_DBDFICNT_3) = 0x00000010;
	dsb_sev();

	dsb_sev();
	dsb_sev();

	*((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x00000011;	//dbdficnt0 freq_ratio = 01 (2:1)init_start =1
	*((volatile uint32_t*)DBSC_DBDFICNT_1) = 0x00000011;
	*((volatile uint32_t*)DBSC_DBDFICNT_2) = 0x00000011;
	*((volatile uint32_t*)DBSC_DBDFICNT_3) = 0x00000011;
	dsb_sev();

	dsb_sev();
	dsb_sev();

	*((volatile uint32_t*)DBSC_DBPDCNT_0) = 0x000000;	//dll_rst_n0 -> 1
	*((volatile uint32_t*)DBSC_DBPDCNT_1) = 0x000000;
	*((volatile uint32_t*)DBSC_DBPDCNT_2) = 0x000000;
	*((volatile uint32_t*)DBSC_DBPDCNT_3) = 0x000000;
	dsb_sev();

	*((volatile uint32_t*)DBSC_DBPDCNT_0) = 0x0000CF00;	//dll_rst_n0 -> 1
	*((volatile uint32_t*)DBSC_DBPDCNT_1) = 0x0000CF00;
	*((volatile uint32_t*)DBSC_DBPDCNT_2) = 0x0000CF00;
	*((volatile uint32_t*)DBSC_DBPDCNT_3) = 0x0000CF00;
	dsb_sev();

	*((volatile uint32_t*)DBSC_DBPDCNT_0) = 0x0000CF01;	//dll_rst_n0 -> 1
	*((volatile uint32_t*)DBSC_DBPDCNT_1) = 0x0000CF01;
	*((volatile uint32_t*)DBSC_DBPDCNT_2) = 0x0000CF01;
	*((volatile uint32_t*)DBSC_DBPDCNT_3) = 0x0000CF01;
	dsb_sev();

	SoftDelay(1*1000); 	//wait for 1ms

	phytrainingok=0;
	//Need Handshake sequence betweed DBSC and PHY
	if(DDR_UPDT_WA&0x40)
	{
		for(ch=0;ch<4;ch++)
		{
			for(slice=0;slice<4;slice++)
			{
//		0x00000433, 0x00200000, // PHY_RX_CAL_SAMPLE_WAIT_0:RW:16:8:=0x20 PHY_RX_CAL_OVERRIDE_0:RW:8:1:=0x00 SC_PHY_RX_CAL_START_0:WR:0:1:=0x00
				if(DDR_TVAL2&0x2)
				{
					REG_DDRPHY_WRITE(ch,0x433+slice*0x80,REG_DDRPHY_READ(ch,0x433+slice*0x80)|0x1|0x100);
				}
				else
				{
					REG_DDRPHY_WRITE(ch,0x433+slice*0x80,REG_DDRPHY_READ(ch,0x433+slice*0x80)|0x1);
				}
			}
		}
		dsb_sev();
	}

	j=(DDR_UPDT_WA>>24)&0xff;
	k=0;
	for(phychno=0;phychno<4;phychno++)
	{
		if(!(DDR_PHYVALID&(1<<phychno)))continue;
		for(i=0;i<=j;i++)
		{
			if(i>0)
			{ //after first init, init_start should be negated so as to wakeup pll
				*((volatile uint32_t*)(DBSC_DBDFICNT_0+0x40*phychno)) = 0x00000011;	//dbdficnt0 freq_ratio = 01 (2:1)init_start =1
				dsb_sev();
				*((volatile uint32_t*)(DBSC_DBDFICNT_0+0x40*phychno)) = 0x00000010;	//dbdficnt0 freq_ratio = 01 (2:1)init_start =0
				dsb_sev();
//			0x000004B3, 0x00200000, // PHY_RX_CAL_SAMPLE_WAIT_1:RW:16:8:=0x20 PHY_RX_CAL_OVERRIDE_1:RW:8:1:=0x00 SC_PHY_RX_CAL_START_1:WR:0:1:=0x00
				for(retry=0;retry<4;retry++)
				{
					REG_DDRPHY_WRITE(phychno,0x433+retry*0x80,REG_DDRPHY_READ(phychno,0x433+retry*0x80)|0x1);
				}
			}

			retry=0;
			while(retry<2048)
			{
				dataL = *((volatile uint32_t*)(DBSC_INITCOMP_0+phychno*0x40));	//Wait for DBSC_INITCOMP_0[0] is 1
				if((dataL & 0x00000001) == 0x1)	break;
				retry++;
			}
			if(retry<2048)
			{
				phytrainingok|=(1<<phychno);
			}
			else
			{
				k|=(1<<phychno);
			}
		}
	}

	phytrainingok &=~k;
	if((phytrainingok&DDR_PHYVALID)!=DDR_PHYVALID)
	{
		return phytrainingok;
	}

#ifdef	DDR_PAD_CAL_WA
	{
		uint32_t pvtn_h,pvtn_l;
		uint32_t pvtp_h,pvtp_l;
		uint32_t pvtr;
		uint32_t o_dataL;

		for(j=0;j<4;j++){
			dataL=REG_DDRPHY_READ(j,0x731);
			pvtn_h=(dataL >>18)& 0x03f;
			pvtn_l=(dataL >>12)& 0x03f;
			pvtp_h=(dataL >> 6)& 0x03f;
			pvtp_l=(dataL >> 0)& 0x03f;
			for( i=0; i<DDR_PHY_PVT_OVERWRITE_NUM; i++ )
			{
				o_dataL=REG_DDRPHY_READ(j,DDR_PHY_DDR_PHY_PVT_OVERWITE_1600[i][0]);
				pvtr=(o_dataL >>12)& 0x01f;
				dataL=(o_dataL & 0xffe00000) | (((pvtr+DDR_PVTR_ADJ)&0x1f)<<12) | (((pvtn_h+pvtn_l)>>1)<<6) | ((pvtp_h+pvtp_l)>>1);
				REG_DDRPHY_WRITE(j,DDR_PHY_DDR_PHY_PVT_OVERWITE_1600[i][0],dataL);

				dataL=REG_DDRPHY_READ(j,DDR_PHY_DDR_PHY_PVT_OVERWITE_1600[i][0]);
			}
		}

		for(ch=0;ch<4;ch++)
		{
			for(slice=0;slice<4;slice++)
			{
				REG_DDRPHY_WRITE(ch,0x452+0x80*slice,REG_DDRPHY_READ(ch,0x452+0x80*slice) & ~(1<<16));//PAD_PHY_IE_MODE OFF
//		0x00000433, 0x00200000, // PHY_RX_CAL_SAMPLE_WAIT_0:RW:16:8:=0x20 PHY_RX_CAL_OVERRIDE_0:RW:8:1:=0x00 SC_PHY_RX_CAL_START_0:WR:0:1:=0x00
				REG_DDRPHY_WRITE(ch,0x433+slice*0x80,REG_DDRPHY_READ(ch,0x433+slice*0x80)&0xffff0000);
			}
		}

	}
#endif

	if(DDR_RX_CAL_MAN&0x100)
	{
		rx_cal_manual1(phytrainingok);
	}

	//Need Handshake sequence betweed DBSC and PHY

	if(DDR_UPDT_WA&0x2)
	{
		pvt_manual_update(0x0f);
	}
	if(DDR_UPDT_WA&0x4)
	{
		pvt_manual_update(0xf0);
	}

	//CHANGE IMPEDANCE CODE to CMOS MODE
	change_lpddr4_en(0xf,0);

	for(phychno=0;phychno<4;phychno+=1)
	{
		if((DDR_PHYVALID&0x0f)==0x0f)
		{
			phychno=0x0f;
		}
		else
		{
			if( (DDR_PHYVALID&(1<<phychno))==0 )
			{
				continue;
			}
		}

		//(SDRAM Initalize)
		*((volatile uint32_t*)DBSC_DBCMD) = 0x01040001|(0x00100000 * phychno);	//RSX chA rkA
		WaitDBCMD();

		dsb_sev();

		*((volatile uint32_t*)DBSC_DBCMD) = 0x08040000|(0x00100000 * phychno);	//PDE chA rkA
		WaitDBCMD();

		*((volatile uint32_t*)DBSC_DBCMD) = 0x08040000|(0x00100000 * phychno);	//PDE chA rkA
		WaitDBCMD();

		*((volatile uint32_t*)DBSC_DBCMD) = 0x08040001|(0x00100000 * phychno);	//PDX ch0 rk0
		WaitDBCMD();
		*((volatile uint32_t*)DBSC_DBCMD) = 0x08040001|(0x00100000 * phychno);	//PDX ch0 rk0
		WaitDBCMD();

		*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040200|(0x00100000 * phychno);	//MRW chA rkA 02:00
		WaitDBCMD();

//		*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040331|(0x00100000 * phychno);	//MRW chA rkA 03:31
//		*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040311|(0x00100000 * phychno);	//MRW chA rkA 03:31
		*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040300|(0x00100000 * phychno)|DDR_MR3;	//MRW chA rkA 03:31
		WaitDBCMD();

		*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040100|(0x00100000 * phychno);	//MRW chA rkA 01:00
		WaitDBCMD();

		if((DDR_PHYMRW & (1<<phychno)) || (phychno==0x0f))
		{/////////////////////////////////////////////////////////// phymrw
			mode_register_set(freq, phychno, 0x40);
			mode_register_set(freq, phychno, 0x00);
		} /////////////////////////////////////////////////////////// phymrw

		dsb_sev();

		*((volatile uint32_t*)DBSC_DBCMD) = 0x0d04004F|(0x00100000 * phychno);	//MPC chA rkA 4FH (ZQCAL start)
		WaitDBCMD();

		*((volatile uint32_t*)DBSC_DBCMD) = 0x0d040051|(0x00100000 * phychno);	//MPC chA rkA 51H (ZQCAL latch)
		WaitDBCMD();
	}
	{
		for(ch=0;ch<4;ch++)
		{
			j=0x276; dataL=REG_DDRPHY_READ(ch,j);
			dataL=(dataL&0x00ffffff) | (DDR_MR3<<24);
			REG_DDRPHY_WRITE(ch,j,dataL);

			j=0x278; dataL=REG_DDRPHY_READ(ch,j);
			dataL=(dataL&0xffff00ff) | (DDR_MR3<<8);
			REG_DDRPHY_WRITE(ch,j,dataL);

			j=0x279; dataL=REG_DDRPHY_READ(ch,j);
			dataL=(dataL&0x00ffffff) | (DDR_MR3<<24);
			REG_DDRPHY_WRITE(ch,j,dataL);

			j=0x27b; dataL=REG_DDRPHY_READ(ch,j);
			dataL=(dataL&0xff00ffff) | (DDR_MR3<<16);
			REG_DDRPHY_WRITE(ch,j,dataL);

			j=0x27d; dataL=REG_DDRPHY_READ(ch,j);
			dataL=(dataL&0xffffff00) | (DDR_MR3<<0);
			REG_DDRPHY_WRITE(ch,j,dataL);

			j=0x27e; dataL=REG_DDRPHY_READ(ch,j);
			dataL=(dataL&0xff00ffff) | (DDR_MR3<<16);
			;REG_DDRPHY_WRITE(ch,j,dataL);

			j=0x280; dataL=REG_DDRPHY_READ(ch,j);
			dataL=(dataL&0xffff00ff) | (DDR_MR3<<8);
			REG_DDRPHY_WRITE(ch,j,dataL);

			j=0x281; dataL=REG_DDRPHY_READ(ch,j);
			dataL=(dataL&0x00ffffff) | (DDR_MR3<<24);
			REG_DDRPHY_WRITE(ch,j,dataL);

			j=0x283; dataL=REG_DDRPHY_READ(ch,j);
			dataL=(dataL&0xffff00ff) | (DDR_MR3<<8);
			REG_DDRPHY_WRITE(ch,j,dataL);

			j=0x285; dataL=REG_DDRPHY_READ(ch,j);
			dataL=(dataL&0xffffff00) | (DDR_MR3<<0);
			REG_DDRPHY_WRITE(ch,j,dataL);

			j=0x286; dataL=REG_DDRPHY_READ(ch,j);
			dataL=(dataL&0xff00ffff) | (DDR_MR3<<16);
			REG_DDRPHY_WRITE(ch,j,dataL);

			j=0x288; dataL=REG_DDRPHY_READ(ch,j);
			dataL=(dataL&0xffffff00) | (DDR_MR3<<0);
			REG_DDRPHY_WRITE(ch,j,dataL);

		}

		_set_reg(0x0f,0x277, 0,8,DDR_MR11);
		_set_reg(0x0f,0x278,16,8,DDR_MR11);
		_set_reg(0x0f,0x27a, 0,8,DDR_MR11);
		_set_reg(0x0f,0x27b,24,8,DDR_MR11);
		_set_reg(0x0f,0x27d, 8,8,DDR_MR11);
		_set_reg(0x0f,0x27e,24,8,DDR_MR11);
		_set_reg(0x0f,0x280,16,8,DDR_MR11);
		_set_reg(0x0f,0x282, 0,8,DDR_MR11);
		_set_reg(0x0f,0x283,16,8,DDR_MR11);
		_set_reg(0x0f,0x285, 8,8,DDR_MR11);
		_set_reg(0x0f,0x286,24,8,DDR_MR11);
		_set_reg(0x0f,0x288, 8,8,DDR_MR11);
	}

	//CHANGE IMPEDANCE CODE to LPDDR4 MODE
	change_lpddr4_en(0xf,1);

	phytrainingok&=pvt_pi_training(freq);
	if(DDR_UPDT_WA&0x2)
	{
		pvt_manual_update(0x0f);
	}
	if(DDR_UPDT_WA&0x4)
	{
		pvt_manual_update(0xf0);
	}

	for(phychno=0;phychno<4;phychno+=1)
	{
		if((DDR_PHYVALID&phytrainingok& 0x0f)==0x0f)
		{
			phychno=0x0f;
		}
		else
		{
			if( (DDR_PHYVALID&phytrainingok&(1<<phychno))==0 )
			{
				continue;
			}
		}
		mode_register_set(freq, phychno, 0x40);
		mode_register_set(freq, phychno, 0x00);
	}

	if(DDR_UPDT_WA&0x38)
	{
		for(i=0;i<1000;i++)
		{
			//dummy
			REG_DDRPHY_READ(0,0x229);
			REG_DDRPHY_READ(1,0x229);
			REG_DDRPHY_READ(2,0x229);
			REG_DDRPHY_READ(3,0x229);
		}
	}

	if(DDR_UPDT_WA&0x20)
	{
		REG_DDRPHY_WRITE(0,0x229,0x000);
		REG_DDRPHY_WRITE(1,0x229,0x000);
		REG_DDRPHY_WRITE(2,0x229,0x000);
		REG_DDRPHY_WRITE(3,0x229,0x000);

		REG_DDRPHY_WRITE(0,0x229,0x100);
		REG_DDRPHY_WRITE(1,0x229,0x100);
		REG_DDRPHY_WRITE(2,0x229,0x100);
		REG_DDRPHY_WRITE(3,0x229,0x100);
	}

	if(DDR_UPDT_WA&0x08)
	{
		*((volatile uint32_t*)DBSC_DBPDCNT_0) = 0x0000CF00;	//dll_rst_n0 -> 1
		*((volatile uint32_t*)DBSC_DBPDCNT_1) = 0x0000CF00;
		*((volatile uint32_t*)DBSC_DBPDCNT_2) = 0x0000CF00;
		*((volatile uint32_t*)DBSC_DBPDCNT_3) = 0x0000CF00;
		dsb_sev();

		for(i=0;i<1000;i++)
		{
			//dummy
			REG_DDRPHY_READ(0,0x229);
			REG_DDRPHY_READ(1,0x229);
			REG_DDRPHY_READ(2,0x229);
			REG_DDRPHY_READ(3,0x229);
		}
	}

	if(DDR_UPDT_WA&0x10)
	{
		*((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x00000010;	//dbdficnt0 freq_ratio = 01 (2:1)init_start =1 
		*((volatile uint32_t*)DBSC_DBDFICNT_1) = 0x00000010;
		*((volatile uint32_t*)DBSC_DBDFICNT_2) = 0x00000010;
		*((volatile uint32_t*)DBSC_DBDFICNT_3) = 0x00000010;
		*((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x00000011;	//dbdficnt0 freq_ratio = 01 (2:1)init_start =1 
		*((volatile uint32_t*)DBSC_DBDFICNT_1) = 0x00000011;
		*((volatile uint32_t*)DBSC_DBDFICNT_2) = 0x00000011;
		*((volatile uint32_t*)DBSC_DBDFICNT_3) = 0x00000011;
		dsb_sev();
	}

	if(DDR_UPDT_WA&0x08)
	{
		*((volatile uint32_t*)DBSC_DBPDCNT_0) = 0x0000CF01;	//dll_rst_n0 -> 1
		*((volatile uint32_t*)DBSC_DBPDCNT_1) = 0x0000CF01;
		*((volatile uint32_t*)DBSC_DBPDCNT_2) = 0x0000CF01;
		*((volatile uint32_t*)DBSC_DBPDCNT_3) = 0x0000CF01;
		dsb_sev();

		for(i=0;i<1000;i++){//dummy
			REG_DDRPHY_READ(0,0x229);
			REG_DDRPHY_READ(1,0x229);
			REG_DDRPHY_READ(2,0x229);
			REG_DDRPHY_READ(3,0x229);
		}
	}

	if(DDR_UPDT_WA&0x18)
	{
		for(i=0;i<1000;i++)
		{
			//dummy
			REG_DDRPHY_READ(0,0x229);
			REG_DDRPHY_READ(1,0x229);
			REG_DDRPHY_READ(2,0x229);
			REG_DDRPHY_READ(3,0x229);
		}
	}

	if(DDR_UPDT_WA&0x10)
	{
		*((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x00000010;	//dbdficnt0 freq_ratio = 01 (2:1)init_start =1 
		*((volatile uint32_t*)DBSC_DBDFICNT_1) = 0x00000010;
		*((volatile uint32_t*)DBSC_DBDFICNT_2) = 0x00000010;
		*((volatile uint32_t*)DBSC_DBDFICNT_3) = 0x00000010;
		dsb_sev();

		//Need Handshake sequence betweed DBSC and PHY
		while(1)
		{
			dataL = *((volatile uint32_t*)DBSC_INITCOMP_0);	//Wait for DBSC_INITCOMP_0[0] is 1
			if((dataL & 0x00000001) == 0x1)	break;
		}

		while(1)
		{
			dataL = *((volatile uint32_t*)DBSC_INITCOMP_1);	//Wait for DBSC_INITCOMP_1[0] is 1
			if((dataL & 0x00000001) == 0x1)	break;
		}

		while(1)
		{
			dataL = *((volatile uint32_t*)DBSC_INITCOMP_2);	//Wait for DBSC_INITCOMP_2[0] is 1
			if((dataL & 0x00000001) == 0x1)	break;
		}

		while(1)
		{
			dataL = *((volatile uint32_t*)DBSC_INITCOMP_3);	//Wait for DBSC_INITCOMP_3[0] is 1
			if((dataL & 0x00000001) == 0x1)	break;
		}
	}

	*((volatile uint32_t*)DBSC_DBBUS0CNF1) = 0x70000100;	//dbbus0cnf1
	*((volatile uint32_t*)DBSC_DBBUS0CNF0) = 0x18010001;	//dbbus0cnf

//Auto Refresh setting
//	*((volatile uint32_t*)DBSC_DBRFCNF1) = 0x00081860;	//dbrfcnf1 refpmax=8 refint=6240
	if(freq>0x53) *((volatile uint32_t*)DBSC_DBRFCNF1) = 0x00081860;	//dbrfcnf1 refpmax=8 refint=6240
	else if(freq>0x47) *((volatile uint32_t*)DBSC_DBRFCNF1) = 0x00081554;	//dbrfcnf1 refpmax=8 refint=5460
	else if(freq>0x2f) *((volatile uint32_t*)DBSC_DBRFCNF1) = 0x00081248;	//dbrfcnf1 refpmax=8 refint=4680
	else *((volatile uint32_t*)DBSC_DBRFCNF1) = 0x00080C30;	//dbrfcnf1 refpmax=8 refint=3120

	*((volatile uint32_t*)DBSC_DBRFCNF2) = 0x00010000;	//dbrfcnf2 refpmin=1 refints=0

	*((volatile uint32_t*)DBSC_DBRFEN) = 0x00000001;	//dbrfen

//DRAM ACCESS enable
	*((volatile uint32_t*)DBSC_DBACEN) = 0x00000001;	//dbacen

	return phytrainingok;
}

#if 0
void freq2800_reg_set()
{
	uint32_t i;

	for(i=0;i<4;i++)
	{
		REG_DDRPHY_WRITE(i,0x22B, (DDR2800_PI_CASLAT_LIN<<24)|(DDR2800_PI_WRLAT<<16)|(DDR2800_PI_CASLAT_LIN<<8)|(DDR2800_PI_WRLAT<<0));
		REG_DDRPHY_WRITE(i,0x22C, 0x00020000|(DDR2800_PI_CASLAT_LIN<<8)|(DDR2800_PI_WRLAT<<0));

		REG_DDRPHY_WRITE(i,0x252, (DDR2800_PI_RDLAT_ADJ<<16)|(DDR2800_PI_RDLAT_ADJ<<8)|(DDR2800_PI_RDLAT_ADJ<<0));
		REG_DDRPHY_WRITE(i,0x253, (DDR2800_PI_WRLAT_ADJ<<16)|(DDR2800_PI_WRLAT_ADJ<<8)|(DDR2800_PI_WRLAT_ADJ<<0));

		REG_DDRPHY_WRITE(i,0x28A, (DDR2800_PI_TFC<<16)|(DDR2800_PI_TFC<<0));
		REG_DDRPHY_WRITE(i,0x28B, (DDR2800_PI_TRTP<<24)|(DDR2800_PI_TCCD<<16)|(DDR2800_PI_TFC<<0));
		REG_DDRPHY_WRITE(i,0x28C, (DDR2800_PI_TWR<<24) |(DDR2800_PI_TWTR<<16)|(DDR2800_PI_TRCD<<8)|(DDR2800_PI_TRP<<0));
		REG_DDRPHY_WRITE(i,0x28D, (DDR2800_PI_TRAS_MIN<<24)|(DDR2800_PI_TRAS_MAX<<0));
		REG_DDRPHY_WRITE(i,0x28E, (DDR2800_PI_TMRW<<24) | (DDR2800_PI_TMRD<<16)|(DDR2800_PI_TCCDMW<<8)|(DDR2800_PI_TDQSCK_MAX<<0));
		REG_DDRPHY_WRITE(i,0x28F, (DDR2800_PI_TWTR<<24) | (DDR2800_PI_TRCD<<16)|(DDR2800_PI_TRP<<8)   |(DDR2800_PI_TRTP<<0));
		REG_DDRPHY_WRITE(i,0x290, (DDR2800_PI_TRAS_MAX<<8) | (DDR2800_PI_TWR<<0));
		REG_DDRPHY_WRITE(i,0x291, (DDR2800_PI_TMRD<<24) | (DDR2800_PI_TCCDMW<<16) | (DDR2800_PI_TDQSCK_MAX<<8)|(DDR2800_PI_TRAS_MIN<<0));
		REG_DDRPHY_WRITE(i,0x292, (DDR2800_PI_TRCD<<24) | (DDR2800_PI_TRP<<16) | (DDR2800_PI_TRTP<<8)|(DDR2800_PI_TMRW<<0));
		REG_DDRPHY_WRITE(i,0x293, (DDR2800_PI_TWR<<8)|(DDR2800_PI_TWTR<<0));
		REG_DDRPHY_WRITE(i,0x294, (DDR2800_PI_TRAS_MIN<<24)|(DDR2800_PI_TRAS_MAX<<0));
		REG_DDRPHY_WRITE(i,0x295, (DDR2800_PI_TMRW<<24) | (DDR2800_PI_TMRD<<16)|(DDR2800_PI_TCCDMW<<8)|(DDR2800_PI_TDQSCK_MAX<<0));

		REG_DDRPHY_WRITE(i,0x276, (DDR_MR3 <<24)|(DDR2800_PI_MR2 <<16)|(DDR2800_PI_MR1 <<8)|0x00);
		REG_DDRPHY_WRITE(i,0x277, (DDR2800_PI_MR1 <<24)|(PI_MR14<<16)|(PI_MR12<<8)|(PI_MR11<<0));
		REG_DDRPHY_WRITE(i,0x278, (PI_MR12<<24)|(PI_MR11<<16)|(DDR_MR3 <<8)|(DDR2800_PI_MR2 <<0));
		REG_DDRPHY_WRITE(i,0x279, (DDR_MR3 <<24)|(DDR2800_PI_MR2 <<16)|(DDR2800_PI_MR1 <<8)|(PI_MR14<<0));
		REG_DDRPHY_WRITE(i,0x27A, (PI_MR13<<24)|(PI_MR14<<16)|(PI_MR12<<8)|(PI_MR11<<0));
		REG_DDRPHY_WRITE(i,0x27B, (PI_MR11<<24)|(DDR_MR3 <<16)|(DDR2800_PI_MR2 <<8)|(DDR2800_PI_MR1 <<0));
		REG_DDRPHY_WRITE(i,0x27C, (DDR2800_PI_MR2 <<24)|(DDR2800_PI_MR1 <<16)|(PI_MR14<<8)|(PI_MR12<<0));
		REG_DDRPHY_WRITE(i,0x27D, (PI_MR14<<24)|(PI_MR12<<16)|(PI_MR11<<8)|(DDR_MR3 <<0));
		REG_DDRPHY_WRITE(i,0x27E, (PI_MR11<<24)|(DDR_MR3 <<16)|(DDR2800_PI_MR2 <<8)|(DDR2800_PI_MR1 <<0));
		REG_DDRPHY_WRITE(i,0x27F, (DDR2800_PI_MR1 <<24)|(PI_MR13<<16)|(PI_MR14<<8)|(PI_MR12<<0));
		REG_DDRPHY_WRITE(i,0x280, (PI_MR12<<24)|(PI_MR11<<16)|(DDR_MR3 <<8)|(DDR2800_PI_MR2 <<0));
		REG_DDRPHY_WRITE(i,0x281, (DDR_MR3 <<24)|(DDR2800_PI_MR2 <<16)|(DDR2800_PI_MR1 <<8)|(PI_MR14<<0));
		REG_DDRPHY_WRITE(i,0x282, (DDR2800_PI_MR1 <<24)|(PI_MR14<<16)|(PI_MR12<<8)|(PI_MR11<<0));
		REG_DDRPHY_WRITE(i,0x283, (PI_MR12<<24)|(PI_MR11<<16)|(DDR_MR3 <<8)|(DDR2800_PI_MR2 <<0));
		REG_DDRPHY_WRITE(i,0x284, (DDR2800_PI_MR2 <<24)|(DDR2800_PI_MR1 <<16)|(PI_MR13<<8)|(PI_MR14<<0));
		REG_DDRPHY_WRITE(i,0x285, (PI_MR14<<24)|(PI_MR12<<16)|(PI_MR11<<8)|(DDR_MR3 <<0));
		REG_DDRPHY_WRITE(i,0x286, (PI_MR11<<24)|(DDR_MR3 <<16)|(DDR2800_PI_MR2 <<8)|(DDR2800_PI_MR1 <<0));
		REG_DDRPHY_WRITE(i,0x287, (DDR2800_PI_MR2 <<24)|(DDR2800_PI_MR1 <<16)|(PI_MR14<<8)|(PI_MR12<<0));
		REG_DDRPHY_WRITE(i,0x288, (PI_MR14<<24)|(PI_MR12<<16)|(PI_MR11<<8)|(DDR_MR3 <<0));
		REG_DDRPHY_WRITE(i,0x289, 0x00020000                              |(PI_MR13<<0));

		REG_DDRPHY_WRITE(i,0x247, 0x0a0a0a05);	//PI_RD_TO_ODTH_F2:RW:24:6:=0x15 PI_RD_TO_ODTH_F1:RW:16:6:=0x15 PI_RD_TO_ODTH_F0:RW:8:6:=0x04 PI_WR_TO_ODTH_F2:RW:0:6:=0x08
		REG_DDRPHY_WRITE(i,0x254, 0x02030303);	//PI_TDFI_PHY_WRDATA:RW:24:3:=0x02 PI_TDFI_WRCSLAT_F2:RW:16:7:=0x06 PI_TDFI_WRCSLAT_F1:RW:8:7:=0x06 PI_TDFI_WRCSLAT_F0:RW:0:7:=0x01
		REG_DDRPHY_WRITE(i,0x257, 0x00201701);	//PI_TDFI_CALVL_CC_F0:RW:16:10:=0x0018 PI_TDFI_CALVL_EN:RW:8:8:=0x17 PI_CALVL_CS_MAP:RW:0:4:=0x03
		REG_DDRPHY_WRITE(i,0x258, 0x0020000e);	//PI_TDFI_CALVL_CC_F1:RW:16:10:=0x0028 PI_TDFI_CALVL_CAPTURE_F0:RW:0:10:=0x000e
		REG_DDRPHY_WRITE(i,0x259, 0x0020000e);	//PI_TDFI_CALVL_CC_F2:RW:16:10:=0x0028 PI_TDFI_CALVL_CAPTURE_F1:RW:0:10:=0x000e
		REG_DDRPHY_WRITE(i,0x25A, 0x0000000e);	//PI_TDFI_CALVL_CAPTURE_F2:RW:0:10:=0x0016
		REG_DDRPHY_WRITE(i,0x25F, 0x00C8020A);	//PI_TCAENT_F0:RW:16:14:=0x0005 PI_TMRZ_F0:RW:8:5:=0x01 PI_TCACKEH:RW:0:5:=0x0a
		REG_DDRPHY_WRITE(i,0x260, 0x0200C802);	//PI_TMRZ_F2:RW:24:5:=0x03 PI_TCAENT_F1:RW:8:14:=0x018d PI_TMRZ_F1:RW:0:5:=0x03
		REG_DDRPHY_WRITE(i,0x261, 0x000A00C8);	//PI_CA_TRAIN_VREF_EN:RW:24:1:=0x01 PI_TCAEXT:RW:16:5:=0x0a PI_TCAENT_F2:RW:0:14:=0x018d
		REG_DDRPHY_WRITE(i,0x262, 0x00C90100);	//PI_TVREF_SHORT_F0:RW:16:10:=0x0006 PI_TDFI_CASEL_F0:RW:8:5:=0x01 PI_TDFI_CACSCA_F0:RW:0:5:=0x00
		REG_DDRPHY_WRITE(i,0x263, 0x010000C9);	//PI_TDFI_CASEL_F1:RW:24:5:=0x01 PI_TDFI_CACSCA_F1:RW:16:5:=0x00 PI_TVREF_LONG_F0:RW:0:10:=0x0006
		REG_DDRPHY_WRITE(i,0x268, 0x07070705);	//PI_TDFI_CALVL_STROBE_F2:RW:24:4:=0x09 PI_TDFI_CALVL_STROBE_F1:RW:16:4:=0x09 PI_TDFI_CALVL_STROBE_F0:RW:8:4:=0x06 PI_TDFI_INIT_COMPLETE_MIN:RW:0:8:=0x05
		REG_DDRPHY_WRITE(i,0x26D, 0x10100100);	//PI_TCKEHDQS_F1:RW:24:6:=0x18 PI_TCKEHDQS_F0:RW:16:6:=0x02 PI_REFRESH_BETWEEN_SEGMENT_DISABLE:RW_D:8:1:=0x01 PI_DRAM_CLK_DISABLE_DEASSERT_SEL:RW:0:1:=0x00
		REG_DDRPHY_WRITE(i,0x26E, 0x00010110);	//PI_WDQLVL_BST_NUM:RW:16:3:=0x01 PI_WDQLVL_VREF_EN:RW:8:1:=0x01 PI_TCKEHDQS_F2:RW:0:6:=0x18
		REG_DDRPHY_WRITE(i,0x2A8, 0x00185000);	//PI_TDFI_CTRLUPD_MAX_F0:RW:8:16:=0x008c PI_TDFI_CTRLUPD_MIN:RD:0:4:=0x00
		REG_DDRPHY_WRITE(i,0x2A9, 0x0000f320);	//PI_TDFI_CTRLUPD_INTERVAL_F0:RW:0:32:=0x00000578
		REG_DDRPHY_WRITE(i,0x2AA, 0x00001850);	//PI_TDFI_CTRLUPD_MAX_F1:RW:0:16:=0x304c
		REG_DDRPHY_WRITE(i,0x2AB, 0x0000f320);	//PI_TDFI_CTRLUPD_INTERVAL_F1:RW:0:32:=0x0001e2f8
		REG_DDRPHY_WRITE(i,0x2AC, 0x00001850);	//PI_TDFI_CTRLUPD_MAX_F2:RW:0:16:=0x304c
		REG_DDRPHY_WRITE(i,0x2AD, 0x0000f320);	//PI_TDFI_CTRLUPD_INTERVAL_F2:RW:0:32:=0x0001e2f8

		REG_DDRPHY_WRITE(i,0x44C, 0x00020109);	//PHY_WRITE_PATH_LAT_ADD_0:RW+:24:3:=0x00 PHY_RDDQS_LATENCY_ADJUST_0:RW+:16:4:=0x04 PHY_RDDQS_GATE_SLAVE_DELAY_0:RW+:0:10:=0x0109
		REG_DDRPHY_WRITE(i,0x452, 0x070000C0);	//PHY_RDDATA_EN_DLY_0:RW+:24:4:=0x0e PHY_IE_MODE_0:RW+:16:2:=0x00 PHY_RDDATA_EN_IE_DLY_0:RW+:8:2:=0x00 PHY_DQS_IE_TIMING_0:RW+:0:8:=0xc0
		REG_DDRPHY_WRITE(i,0x453, 0x00100006);	//PHY_MASTER_DELAY_START_0:RW+:16:10:=0x0010 PHY_SW_MASTER_MODE_0:RW+:8:4:=0x00 PHY_RDDATA_EN_TSEL_DLY_0:RW+:0:4:=0x0c
		REG_DDRPHY_WRITE(i,0x454, 0x0C054208);	//PHY_WRLVL_DLY_STEP_0:RW+:24:4:=0x0c PHY_RPTR_UPDATE_0:RW+:16:4:=0x06 PHY_MASTER_DELAY_WAIT_0:RW+:8:8:=0x42 PHY_MASTER_DELAY_STEP_0:RW+:0:6:=0x08
		REG_DDRPHY_WRITE(i,0x4CC, 0x00020109);	//PHY_WRITE_PATH_LAT_ADD_1:RW+:24:3:=0x00 PHY_RDDQS_LATENCY_ADJUST_1:RW+:16:4:=0x04 PHY_RDDQS_GATE_SLAVE_DELAY_1:RW+:0:10:=0x0109
		REG_DDRPHY_WRITE(i,0x4D2, 0x070000C0);	//PHY_RDDATA_EN_DLY_1:RW+:24:4:=0x0e PHY_IE_MODE_1:RW+:16:2:=0x00 PHY_RDDATA_EN_IE_DLY_1:RW+:8:2:=0x00 PHY_DQS_IE_TIMING_1:RW+:0:8:=0xc0
		REG_DDRPHY_WRITE(i,0x4D3, 0x00100006);	//PHY_MASTER_DELAY_START_1:RW+:16:10:=0x0010 PHY_SW_MASTER_MODE_1:RW+:8:4:=0x00 PHY_RDDATA_EN_TSEL_DLY_1:RW+:0:4:=0x0c
		REG_DDRPHY_WRITE(i,0x4D4, 0x0C054208);	//PHY_WRLVL_DLY_STEP_1:RW+:24:4:=0x0c PHY_RPTR_UPDATE_1:RW+:16:4:=0x06 PHY_MASTER_DELAY_WAIT_1:RW+:8:8:=0x42 PHY_MASTER_DELAY_STEP_1:RW+:0:6:=0x08
		REG_DDRPHY_WRITE(i,0x54C, 0x00020109);	//PHY_WRITE_PATH_LAT_ADD_2:RW+:24:3:=0x00 PHY_RDDQS_LATENCY_ADJUST_2:RW+:16:4:=0x04 PHY_RDDQS_GATE_SLAVE_DELAY_2:RW+:0:10:=0x0109
		REG_DDRPHY_WRITE(i,0x552, 0x070000C0);	//PHY_RDDATA_EN_DLY_2:RW+:24:4:=0x0e PHY_IE_MODE_2:RW+:16:2:=0x00 PHY_RDDATA_EN_IE_DLY_2:RW+:8:2:=0x00 PHY_DQS_IE_TIMING_2:RW+:0:8:=0xc0
		REG_DDRPHY_WRITE(i,0x553, 0x00100006);	//PHY_MASTER_DELAY_START_2:RW+:16:10:=0x0010 PHY_SW_MASTER_MODE_2:RW+:8:4:=0x00 PHY_RDDATA_EN_TSEL_DLY_2:RW+:0:4:=0x0c
		REG_DDRPHY_WRITE(i,0x554, 0x0C054208);	//PHY_WRLVL_DLY_STEP_2:RW+:24:4:=0x0c PHY_RPTR_UPDATE_2:RW+:16:4:=0x06 PHY_MASTER_DELAY_WAIT_2:RW+:8:8:=0x42 PHY_MASTER_DELAY_STEP_2:RW+:0:6:=0x08
		REG_DDRPHY_WRITE(i,0x5CC, 0x00020109);	//PHY_WRITE_PATH_LAT_ADD_3:RW+:24:3:=0x00 PHY_RDDQS_LATENCY_ADJUST_3:RW+:16:4:=0x04 PHY_RDDQS_GATE_SLAVE_DELAY_3:RW+:0:10:=0x0109
		REG_DDRPHY_WRITE(i,0x5D2, 0x070000C0);	//PHY_RDDATA_EN_DLY_3:RW+:24:4:=0x0e PHY_IE_MODE_3:RW+:16:2:=0x00 PHY_RDDATA_EN_IE_DLY_3:RW+:8:2:=0x00 PHY_DQS_IE_TIMING_3:RW+:0:8:=0xc0
		REG_DDRPHY_WRITE(i,0x5D3, 0x00100006);	//PHY_MASTER_DELAY_START_3:RW+:16:10:=0x0010 PHY_SW_MASTER_MODE_3:RW+:8:4:=0x00 PHY_RDDATA_EN_TSEL_DLY_3:RW+:0:4:=0x0c
		REG_DDRPHY_WRITE(i,0x5D4, 0x0C054208);	//PHY_WRLVL_DLY_STEP_3:RW+:24:4:=0x0c PHY_RPTR_UPDATE_3:RW+:16:4:=0x06 PHY_MASTER_DELAY_WAIT_3:RW+:8:8:=0x42 PHY_MASTER_DELAY_STEP_3:RW+:0:6:=0x08
		REG_DDRPHY_WRITE(i,0x719, 0x00000303);	//PHY_LP_WAKEUP:RW:24:4:=0x00 PHY_LP4_BOOT_LOW_FREQ_SEL:RW:16:1:=0x01 PHY_TCKSRE_WAIT:RW:8:4:=0x05 PHY_PLL_TESTOUT_SEL:RW:0:3:=0x03
	}
}


void freq2400_reg_set()
{
	uint32_t i;

	for(i=0;i<4;i++)
	{
		REG_DDRPHY_WRITE(i,0x22B, (DDR2400_PI_CASLAT_LIN<<24)|(DDR2400_PI_WRLAT<<16)|(DDR2400_PI_CASLAT_LIN<<8)|(DDR2400_PI_WRLAT<<0));
		REG_DDRPHY_WRITE(i,0x22C, 0x00020000|(DDR2400_PI_CASLAT_LIN<<8)|(DDR2400_PI_WRLAT<<0));

		REG_DDRPHY_WRITE(i,0x252, (DDR2400_PI_RDLAT_ADJ<<16)|(DDR2400_PI_RDLAT_ADJ<<8)|(DDR2400_PI_RDLAT_ADJ<<0));
		REG_DDRPHY_WRITE(i,0x253, (DDR2400_PI_WRLAT_ADJ<<16)|(DDR2400_PI_WRLAT_ADJ<<8)|(DDR2400_PI_WRLAT_ADJ<<0));

		REG_DDRPHY_WRITE(i,0x28A, (DDR2400_PI_TFC<<16)|(DDR2400_PI_TFC<<0));
		REG_DDRPHY_WRITE(i,0x28B, (DDR2400_PI_TRTP<<24)|(DDR2400_PI_TCCD<<16)|(DDR2400_PI_TFC<<0));
		REG_DDRPHY_WRITE(i,0x28C, (DDR2400_PI_TWR<<24) |(DDR2400_PI_TWTR<<16)|(DDR2400_PI_TRCD<<8)|(DDR2400_PI_TRP<<0));
		REG_DDRPHY_WRITE(i,0x28D, (DDR2400_PI_TRAS_MIN<<24)|(DDR2400_PI_TRAS_MAX<<0));
		REG_DDRPHY_WRITE(i,0x28E, (DDR2400_PI_TMRW<<24) | (DDR2400_PI_TMRD<<16)|(DDR2400_PI_TCCDMW<<8)|(DDR2400_PI_TDQSCK_MAX<<0));
		REG_DDRPHY_WRITE(i,0x28F, (DDR2400_PI_TWTR<<24) | (DDR2400_PI_TRCD<<16)|(DDR2400_PI_TRP<<8)   |(DDR2400_PI_TRTP<<0));
		REG_DDRPHY_WRITE(i,0x290, (DDR2400_PI_TRAS_MAX<<8) | (DDR2400_PI_TWR<<0));
		REG_DDRPHY_WRITE(i,0x291, (DDR2400_PI_TMRD<<24) | (DDR2400_PI_TCCDMW<<16) | (DDR2400_PI_TDQSCK_MAX<<8)|(DDR2400_PI_TRAS_MIN<<0));
		REG_DDRPHY_WRITE(i,0x292, (DDR2400_PI_TRCD<<24) | (DDR2400_PI_TRP<<16) | (DDR2400_PI_TRTP<<8)|(DDR2400_PI_TMRW<<0));
		REG_DDRPHY_WRITE(i,0x293, (DDR2400_PI_TWR<<8)|(DDR2400_PI_TWTR<<0));
		REG_DDRPHY_WRITE(i,0x294, (DDR2400_PI_TRAS_MIN<<24)|(DDR2400_PI_TRAS_MAX<<0));
		REG_DDRPHY_WRITE(i,0x295, (DDR2400_PI_TMRW<<24) | (DDR2400_PI_TMRD<<16)|(DDR2400_PI_TCCDMW<<8)|(DDR2400_PI_TDQSCK_MAX<<0));

		REG_DDRPHY_WRITE(i,0x276, (DDR_MR3 <<24)|(DDR2400_PI_MR2 <<16)|(DDR2400_PI_MR1 <<8)|0x00);
		REG_DDRPHY_WRITE(i,0x277, (DDR2400_PI_MR1 <<24)|(PI_MR14<<16)|(PI_MR12<<8)|(PI_MR11<<0));
		REG_DDRPHY_WRITE(i,0x278, (PI_MR12<<24)|(PI_MR11<<16)|(DDR_MR3 <<8)|(DDR2400_PI_MR2 <<0));
		REG_DDRPHY_WRITE(i,0x279, (DDR_MR3 <<24)|(DDR2400_PI_MR2 <<16)|(DDR2400_PI_MR1 <<8)|(PI_MR14<<0));
		REG_DDRPHY_WRITE(i,0x27A, (PI_MR13<<24)|(PI_MR14<<16)|(PI_MR12<<8)|(PI_MR11<<0));
		REG_DDRPHY_WRITE(i,0x27B, (PI_MR11<<24)|(DDR_MR3 <<16)|(DDR2400_PI_MR2 <<8)|(DDR2400_PI_MR1 <<0));
		REG_DDRPHY_WRITE(i,0x27C, (DDR2400_PI_MR2 <<24)|(DDR2400_PI_MR1 <<16)|(PI_MR14<<8)|(PI_MR12<<0));
		REG_DDRPHY_WRITE(i,0x27D, (PI_MR14<<24)|(PI_MR12<<16)|(PI_MR11<<8)|(DDR_MR3 <<0));
		REG_DDRPHY_WRITE(i,0x27E, (PI_MR11<<24)|(DDR_MR3 <<16)|(DDR2400_PI_MR2 <<8)|(DDR2400_PI_MR1 <<0));
		REG_DDRPHY_WRITE(i,0x27F, (DDR2400_PI_MR1 <<24)|(PI_MR13<<16)|(PI_MR14<<8)|(PI_MR12<<0));
		REG_DDRPHY_WRITE(i,0x280, (PI_MR12<<24)|(PI_MR11<<16)|(DDR_MR3 <<8)|(DDR2400_PI_MR2 <<0));
		REG_DDRPHY_WRITE(i,0x281, (DDR_MR3 <<24)|(DDR2400_PI_MR2 <<16)|(DDR2400_PI_MR1 <<8)|(PI_MR14<<0));
		REG_DDRPHY_WRITE(i,0x282, (DDR2400_PI_MR1 <<24)|(PI_MR14<<16)|(PI_MR12<<8)|(PI_MR11<<0));
		REG_DDRPHY_WRITE(i,0x283, (PI_MR12<<24)|(PI_MR11<<16)|(DDR_MR3 <<8)|(DDR2400_PI_MR2 <<0));
		REG_DDRPHY_WRITE(i,0x284, (DDR2400_PI_MR2 <<24)|(DDR2400_PI_MR1 <<16)|(PI_MR13<<8)|(PI_MR14<<0));
		REG_DDRPHY_WRITE(i,0x285, (PI_MR14<<24)|(PI_MR12<<16)|(PI_MR11<<8)|(DDR_MR3 <<0));
		REG_DDRPHY_WRITE(i,0x286, (PI_MR11<<24)|(DDR_MR3 <<16)|(DDR2400_PI_MR2 <<8)|(DDR2400_PI_MR1 <<0));
		REG_DDRPHY_WRITE(i,0x287, (DDR2400_PI_MR2 <<24)|(DDR2400_PI_MR1 <<16)|(PI_MR14<<8)|(PI_MR12<<0));
		REG_DDRPHY_WRITE(i,0x288, (PI_MR14<<24)|(PI_MR12<<16)|(PI_MR11<<8)|(DDR_MR3 <<0));
		REG_DDRPHY_WRITE(i,0x289, 0x00020000                              |(PI_MR13<<0));

		REG_DDRPHY_WRITE(i,0x247, 0x0a0a0a05);	//PI_RD_TO_ODTH_F2:RW:24:6:=0x15 PI_RD_TO_ODTH_F1:RW:16:6:=0x15 PI_RD_TO_ODTH_F0:RW:8:6:=0x04 PI_WR_TO_ODTH_F2:RW:0:6:=0x08
		REG_DDRPHY_WRITE(i,0x254, 0x02030303);	//PI_TDFI_PHY_WRDATA:RW:24:3:=0x02 PI_TDFI_WRCSLAT_F2:RW:16:7:=0x06 PI_TDFI_WRCSLAT_F1:RW:8:7:=0x06 PI_TDFI_WRCSLAT_F0:RW:0:7:=0x01
		REG_DDRPHY_WRITE(i,0x257, 0x00201701);	//PI_TDFI_CALVL_CC_F0:RW:16:10:=0x0018 PI_TDFI_CALVL_EN:RW:8:8:=0x17 PI_CALVL_CS_MAP:RW:0:4:=0x03
		REG_DDRPHY_WRITE(i,0x258, 0x0020000e);	//PI_TDFI_CALVL_CC_F1:RW:16:10:=0x0028 PI_TDFI_CALVL_CAPTURE_F0:RW:0:10:=0x000e
		REG_DDRPHY_WRITE(i,0x259, 0x0020000e);	//PI_TDFI_CALVL_CC_F2:RW:16:10:=0x0028 PI_TDFI_CALVL_CAPTURE_F1:RW:0:10:=0x000e
		REG_DDRPHY_WRITE(i,0x25A, 0x0000000e);	//PI_TDFI_CALVL_CAPTURE_F2:RW:0:10:=0x0016
		REG_DDRPHY_WRITE(i,0x25F, 0x00C8020A);	//PI_TCAENT_F0:RW:16:14:=0x0005 PI_TMRZ_F0:RW:8:5:=0x01 PI_TCACKEH:RW:0:5:=0x0a
		REG_DDRPHY_WRITE(i,0x260, 0x0200C802);	//PI_TMRZ_F2:RW:24:5:=0x03 PI_TCAENT_F1:RW:8:14:=0x018d PI_TMRZ_F1:RW:0:5:=0x03
		REG_DDRPHY_WRITE(i,0x261, 0x000A00C8);	//PI_CA_TRAIN_VREF_EN:RW:24:1:=0x01 PI_TCAEXT:RW:16:5:=0x0a PI_TCAENT_F2:RW:0:14:=0x018d
		REG_DDRPHY_WRITE(i,0x262, 0x00C90100);	//PI_TVREF_SHORT_F0:RW:16:10:=0x0006 PI_TDFI_CASEL_F0:RW:8:5:=0x01 PI_TDFI_CACSCA_F0:RW:0:5:=0x00
		REG_DDRPHY_WRITE(i,0x263, 0x010000C9);	//PI_TDFI_CASEL_F1:RW:24:5:=0x01 PI_TDFI_CACSCA_F1:RW:16:5:=0x00 PI_TVREF_LONG_F0:RW:0:10:=0x0006
		REG_DDRPHY_WRITE(i,0x268, 0x07070705);	//PI_TDFI_CALVL_STROBE_F2:RW:24:4:=0x09 PI_TDFI_CALVL_STROBE_F1:RW:16:4:=0x09 PI_TDFI_CALVL_STROBE_F0:RW:8:4:=0x06 PI_TDFI_INIT_COMPLETE_MIN:RW:0:8:=0x05
		REG_DDRPHY_WRITE(i,0x26D, 0x10100100);	//PI_TCKEHDQS_F1:RW:24:6:=0x18 PI_TCKEHDQS_F0:RW:16:6:=0x02 PI_REFRESH_BETWEEN_SEGMENT_DISABLE:RW_D:8:1:=0x01 PI_DRAM_CLK_DISABLE_DEASSERT_SEL:RW:0:1:=0x00
		REG_DDRPHY_WRITE(i,0x26E, 0x00010110);	//PI_WDQLVL_BST_NUM:RW:16:3:=0x01 PI_WDQLVL_VREF_EN:RW:8:1:=0x01 PI_TCKEHDQS_F2:RW:0:6:=0x18
		REG_DDRPHY_WRITE(i,0x2A8, 0x00185000);	//PI_TDFI_CTRLUPD_MAX_F0:RW:8:16:=0x008c PI_TDFI_CTRLUPD_MIN:RD:0:4:=0x00
		REG_DDRPHY_WRITE(i,0x2A9, 0x0000f320);	//PI_TDFI_CTRLUPD_INTERVAL_F0:RW:0:32:=0x00000578
		REG_DDRPHY_WRITE(i,0x2AA, 0x00001850);	//PI_TDFI_CTRLUPD_MAX_F1:RW:0:16:=0x304c
		REG_DDRPHY_WRITE(i,0x2AB, 0x0000f320);	//PI_TDFI_CTRLUPD_INTERVAL_F1:RW:0:32:=0x0001e2f8
		REG_DDRPHY_WRITE(i,0x2AC, 0x00001850);	//PI_TDFI_CTRLUPD_MAX_F2:RW:0:16:=0x304c
		REG_DDRPHY_WRITE(i,0x2AD, 0x0000f320);	//PI_TDFI_CTRLUPD_INTERVAL_F2:RW:0:32:=0x0001e2f8

		REG_DDRPHY_WRITE(i,0x44C, 0x00020109);	//PHY_WRITE_PATH_LAT_ADD_0:RW+:24:3:=0x00 PHY_RDDQS_LATENCY_ADJUST_0:RW+:16:4:=0x04 PHY_RDDQS_GATE_SLAVE_DELAY_0:RW+:0:10:=0x0109
		REG_DDRPHY_WRITE(i,0x452, 0x070000C0);	//PHY_RDDATA_EN_DLY_0:RW+:24:4:=0x0e PHY_IE_MODE_0:RW+:16:2:=0x00 PHY_RDDATA_EN_IE_DLY_0:RW+:8:2:=0x00 PHY_DQS_IE_TIMING_0:RW+:0:8:=0xc0
		REG_DDRPHY_WRITE(i,0x453, 0x00100006);	//PHY_MASTER_DELAY_START_0:RW+:16:10:=0x0010 PHY_SW_MASTER_MODE_0:RW+:8:4:=0x00 PHY_RDDATA_EN_TSEL_DLY_0:RW+:0:4:=0x0c
		REG_DDRPHY_WRITE(i,0x454, 0x0C054208);	//PHY_WRLVL_DLY_STEP_0:RW+:24:4:=0x0c PHY_RPTR_UPDATE_0:RW+:16:4:=0x06 PHY_MASTER_DELAY_WAIT_0:RW+:8:8:=0x42 PHY_MASTER_DELAY_STEP_0:RW+:0:6:=0x08
		REG_DDRPHY_WRITE(i,0x4CC, 0x00020109);	//PHY_WRITE_PATH_LAT_ADD_1:RW+:24:3:=0x00 PHY_RDDQS_LATENCY_ADJUST_1:RW+:16:4:=0x04 PHY_RDDQS_GATE_SLAVE_DELAY_1:RW+:0:10:=0x0109
		REG_DDRPHY_WRITE(i,0x4D2, 0x070000C0);	//PHY_RDDATA_EN_DLY_1:RW+:24:4:=0x0e PHY_IE_MODE_1:RW+:16:2:=0x00 PHY_RDDATA_EN_IE_DLY_1:RW+:8:2:=0x00 PHY_DQS_IE_TIMING_1:RW+:0:8:=0xc0
		REG_DDRPHY_WRITE(i,0x4D3, 0x00100006);	//PHY_MASTER_DELAY_START_1:RW+:16:10:=0x0010 PHY_SW_MASTER_MODE_1:RW+:8:4:=0x00 PHY_RDDATA_EN_TSEL_DLY_1:RW+:0:4:=0x0c
		REG_DDRPHY_WRITE(i,0x4D4, 0x0C054208);	//PHY_WRLVL_DLY_STEP_1:RW+:24:4:=0x0c PHY_RPTR_UPDATE_1:RW+:16:4:=0x06 PHY_MASTER_DELAY_WAIT_1:RW+:8:8:=0x42 PHY_MASTER_DELAY_STEP_1:RW+:0:6:=0x08
		REG_DDRPHY_WRITE(i,0x54C, 0x00020109);	//PHY_WRITE_PATH_LAT_ADD_2:RW+:24:3:=0x00 PHY_RDDQS_LATENCY_ADJUST_2:RW+:16:4:=0x04 PHY_RDDQS_GATE_SLAVE_DELAY_2:RW+:0:10:=0x0109
		REG_DDRPHY_WRITE(i,0x552, 0x070000C0);	//PHY_RDDATA_EN_DLY_2:RW+:24:4:=0x0e PHY_IE_MODE_2:RW+:16:2:=0x00 PHY_RDDATA_EN_IE_DLY_2:RW+:8:2:=0x00 PHY_DQS_IE_TIMING_2:RW+:0:8:=0xc0
		REG_DDRPHY_WRITE(i,0x553, 0x00100006);	//PHY_MASTER_DELAY_START_2:RW+:16:10:=0x0010 PHY_SW_MASTER_MODE_2:RW+:8:4:=0x00 PHY_RDDATA_EN_TSEL_DLY_2:RW+:0:4:=0x0c
		REG_DDRPHY_WRITE(i,0x554, 0x0C054208);	//PHY_WRLVL_DLY_STEP_2:RW+:24:4:=0x0c PHY_RPTR_UPDATE_2:RW+:16:4:=0x06 PHY_MASTER_DELAY_WAIT_2:RW+:8:8:=0x42 PHY_MASTER_DELAY_STEP_2:RW+:0:6:=0x08
		REG_DDRPHY_WRITE(i,0x5CC, 0x00020109);	//PHY_WRITE_PATH_LAT_ADD_3:RW+:24:3:=0x00 PHY_RDDQS_LATENCY_ADJUST_3:RW+:16:4:=0x04 PHY_RDDQS_GATE_SLAVE_DELAY_3:RW+:0:10:=0x0109
		REG_DDRPHY_WRITE(i,0x5D2, 0x070000C0);	//PHY_RDDATA_EN_DLY_3:RW+:24:4:=0x0e PHY_IE_MODE_3:RW+:16:2:=0x00 PHY_RDDATA_EN_IE_DLY_3:RW+:8:2:=0x00 PHY_DQS_IE_TIMING_3:RW+:0:8:=0xc0
		REG_DDRPHY_WRITE(i,0x5D3, 0x00100006);	//PHY_MASTER_DELAY_START_3:RW+:16:10:=0x0010 PHY_SW_MASTER_MODE_3:RW+:8:4:=0x00 PHY_RDDATA_EN_TSEL_DLY_3:RW+:0:4:=0x0c
		REG_DDRPHY_WRITE(i,0x5D4, 0x0C054208);	//PHY_WRLVL_DLY_STEP_3:RW+:24:4:=0x0c PHY_RPTR_UPDATE_3:RW+:16:4:=0x06 PHY_MASTER_DELAY_WAIT_3:RW+:8:8:=0x42 PHY_MASTER_DELAY_STEP_3:RW+:0:6:=0x08
		REG_DDRPHY_WRITE(i,0x719, 0x00000303);	//PHY_LP_WAKEUP:RW:24:4:=0x00 PHY_LP4_BOOT_LOW_FREQ_SEL:RW:16:1:=0x01 PHY_TCKSRE_WAIT:RW:8:4:=0x05 PHY_PLL_TESTOUT_SEL:RW:0:3:=0x03
	}
}
#endif

static void freq1600_reg_set()
{
	uint32_t i;

	for(i=0;i<4;i++)
	{
		REG_DDRPHY_WRITE(i,0x22B, (DDR1600_PI_CASLAT_LIN<<24)|(DDR1600_PI_WRLAT<<16)|(DDR1600_PI_CASLAT_LIN<<8)|(DDR1600_PI_WRLAT<<0));
		REG_DDRPHY_WRITE(i,0x22C, 0x00020000|(DDR1600_PI_CASLAT_LIN<<8)|(DDR1600_PI_WRLAT<<0));

		REG_DDRPHY_WRITE(i,0x252, (DDR1600_PI_RDLAT_ADJ<<16)|(DDR1600_PI_RDLAT_ADJ<<8)|(DDR1600_PI_RDLAT_ADJ<<0));
		REG_DDRPHY_WRITE(i,0x253, (DDR1600_PI_WRLAT_ADJ<<16)|(DDR1600_PI_WRLAT_ADJ<<8)|(DDR1600_PI_WRLAT_ADJ<<0));

		REG_DDRPHY_WRITE(i,0x28A, (DDR1600_PI_TFC<<16)|(DDR1600_PI_TFC<<0));
		REG_DDRPHY_WRITE(i,0x28B, (DDR1600_PI_TRTP<<24)|(DDR1600_PI_TCCD<<16)|(DDR1600_PI_TFC<<0));
		REG_DDRPHY_WRITE(i,0x28C, (DDR1600_PI_TWR<<24) |(DDR1600_PI_TWTR<<16)|(DDR1600_PI_TRCD<<8)|(DDR1600_PI_TRP<<0));
		REG_DDRPHY_WRITE(i,0x28D, (DDR1600_PI_TRAS_MIN<<24)|(DDR1600_PI_TRAS_MAX<<0));
		REG_DDRPHY_WRITE(i,0x28E, (DDR1600_PI_TMRW<<24) | (DDR1600_PI_TMRD<<16)|(DDR1600_PI_TCCDMW<<8)|(DDR1600_PI_TDQSCK_MAX<<0));
		REG_DDRPHY_WRITE(i,0x28F, (DDR1600_PI_TWTR<<24) | (DDR1600_PI_TRCD<<16)|(DDR1600_PI_TRP<<8)   |(DDR1600_PI_TRTP<<0));
		REG_DDRPHY_WRITE(i,0x290, (DDR1600_PI_TRAS_MAX<<8) | (DDR1600_PI_TWR<<0));
		REG_DDRPHY_WRITE(i,0x291, (DDR1600_PI_TMRD<<24) | (DDR1600_PI_TCCDMW<<16) | (DDR1600_PI_TDQSCK_MAX<<8)|(DDR1600_PI_TRAS_MIN<<0));
		REG_DDRPHY_WRITE(i,0x292, (DDR1600_PI_TRCD<<24) | (DDR1600_PI_TRP<<16) | (DDR1600_PI_TRTP<<8)|(DDR1600_PI_TMRW<<0));
		REG_DDRPHY_WRITE(i,0x293, (DDR1600_PI_TWR<<8)|(DDR1600_PI_TWTR<<0));
		REG_DDRPHY_WRITE(i,0x294, (DDR1600_PI_TRAS_MIN<<24)|(DDR1600_PI_TRAS_MAX<<0));
		REG_DDRPHY_WRITE(i,0x295, (DDR1600_PI_TMRW<<24) | (DDR1600_PI_TMRD<<16)|(DDR1600_PI_TCCDMW<<8)|(DDR1600_PI_TDQSCK_MAX<<0));

		REG_DDRPHY_WRITE(i,0x276, (PI_MR3 <<24)|(DDR1600_PI_MR2 <<16)|(DDR1600_PI_MR1 <<8)|0x00);
		REG_DDRPHY_WRITE(i,0x277, (DDR1600_PI_MR1 <<24)|(PI_MR14<<16)|(PI_MR12<<8)|(PI_MR11<<0));
		REG_DDRPHY_WRITE(i,0x278, (PI_MR12<<24)|(PI_MR11<<16)|(PI_MR3 <<8)|(DDR1600_PI_MR2 <<0));
		REG_DDRPHY_WRITE(i,0x279, (PI_MR3 <<24)|(DDR1600_PI_MR2 <<16)|(DDR1600_PI_MR1 <<8)|(PI_MR14<<0));
		REG_DDRPHY_WRITE(i,0x27A, (PI_MR13<<24)|(PI_MR14<<16)|(PI_MR12<<8)|(PI_MR11<<0));
		REG_DDRPHY_WRITE(i,0x27B, (PI_MR11<<24)|(PI_MR3 <<16)|(DDR1600_PI_MR2 <<8)|(DDR1600_PI_MR1 <<0));
		REG_DDRPHY_WRITE(i,0x27C, (DDR1600_PI_MR2 <<24)|(DDR1600_PI_MR1 <<16)|(PI_MR14<<8)|(PI_MR12<<0));
		REG_DDRPHY_WRITE(i,0x27D, (PI_MR14<<24)|(PI_MR12<<16)|(PI_MR11<<8)|(PI_MR3 <<0));
		REG_DDRPHY_WRITE(i,0x27E, (PI_MR11<<24)|(PI_MR3 <<16)|(DDR1600_PI_MR2 <<8)|(DDR1600_PI_MR1 <<0));
		REG_DDRPHY_WRITE(i,0x27F, (DDR1600_PI_MR1 <<24)|(PI_MR13<<16)|(PI_MR14<<8)|(PI_MR12<<0));
		REG_DDRPHY_WRITE(i,0x280, (PI_MR12<<24)|(PI_MR11<<16)|(PI_MR3 <<8)|(DDR1600_PI_MR2 <<0));
		REG_DDRPHY_WRITE(i,0x281, (PI_MR3 <<24)|(DDR1600_PI_MR2 <<16)|(DDR1600_PI_MR1 <<8)|(PI_MR14<<0));
		REG_DDRPHY_WRITE(i,0x282, (DDR1600_PI_MR1 <<24)|(PI_MR14<<16)|(PI_MR12<<8)|(PI_MR11<<0));
		REG_DDRPHY_WRITE(i,0x283, (PI_MR12<<24)|(PI_MR11<<16)|(PI_MR3 <<8)|(DDR1600_PI_MR2 <<0));
		REG_DDRPHY_WRITE(i,0x284, (DDR1600_PI_MR2 <<24)|(DDR1600_PI_MR1 <<16)|(PI_MR13<<8)|(PI_MR14<<0));
		REG_DDRPHY_WRITE(i,0x285, (PI_MR14<<24)|(PI_MR12<<16)|(PI_MR11<<8)|(PI_MR3 <<0));
		REG_DDRPHY_WRITE(i,0x286, (PI_MR11<<24)|(PI_MR3 <<16)|(DDR1600_PI_MR2 <<8)|(DDR1600_PI_MR1 <<0));
		REG_DDRPHY_WRITE(i,0x287, (DDR1600_PI_MR2 <<24)|(DDR1600_PI_MR1 <<16)|(PI_MR14<<8)|(PI_MR12<<0));
		REG_DDRPHY_WRITE(i,0x288, (PI_MR14<<24)|(PI_MR12<<16)|(PI_MR11<<8)|(PI_MR3 <<0));
		REG_DDRPHY_WRITE(i,0x289, 0x00020000                              |(PI_MR13<<0));

		REG_DDRPHY_WRITE(i,0x247, 0x0a0a0a05);	//PI_RD_TO_ODTH_F2:RW:24:6:=0x15 PI_RD_TO_ODTH_F1:RW:16:6:=0x15 PI_RD_TO_ODTH_F0:RW:8:6:=0x04 PI_WR_TO_ODTH_F2:RW:0:6:=0x08
		REG_DDRPHY_WRITE(i,0x254, 0x02030303);	//PI_TDFI_PHY_WRDATA:RW:24:3:=0x02 PI_TDFI_WRCSLAT_F2:RW:16:7:=0x06 PI_TDFI_WRCSLAT_F1:RW:8:7:=0x06 PI_TDFI_WRCSLAT_F0:RW:0:7:=0x01
		REG_DDRPHY_WRITE(i,0x257, 0x00201701);	//PI_TDFI_CALVL_CC_F0:RW:16:10:=0x0018 PI_TDFI_CALVL_EN:RW:8:8:=0x17 PI_CALVL_CS_MAP:RW:0:4:=0x03
		REG_DDRPHY_WRITE(i,0x258, 0x0020000e);	//PI_TDFI_CALVL_CC_F1:RW:16:10:=0x0028 PI_TDFI_CALVL_CAPTURE_F0:RW:0:10:=0x000e
		REG_DDRPHY_WRITE(i,0x259, 0x0020000e);	//PI_TDFI_CALVL_CC_F2:RW:16:10:=0x0028 PI_TDFI_CALVL_CAPTURE_F1:RW:0:10:=0x000e
		REG_DDRPHY_WRITE(i,0x25A, 0x0000000e);	//PI_TDFI_CALVL_CAPTURE_F2:RW:0:10:=0x0016
		REG_DDRPHY_WRITE(i,0x25F, 0x00C8020A);	//PI_TCAENT_F0:RW:16:14:=0x0005 PI_TMRZ_F0:RW:8:5:=0x01 PI_TCACKEH:RW:0:5:=0x0a
		REG_DDRPHY_WRITE(i,0x260, 0x0200C802);	//PI_TMRZ_F2:RW:24:5:=0x03 PI_TCAENT_F1:RW:8:14:=0x018d PI_TMRZ_F1:RW:0:5:=0x03
		REG_DDRPHY_WRITE(i,0x261, 0x000A00C8);	//PI_CA_TRAIN_VREF_EN:RW:24:1:=0x01 PI_TCAEXT:RW:16:5:=0x0a PI_TCAENT_F2:RW:0:14:=0x018d
		REG_DDRPHY_WRITE(i,0x262, 0x00C90100);	//PI_TVREF_SHORT_F0:RW:16:10:=0x0006 PI_TDFI_CASEL_F0:RW:8:5:=0x01 PI_TDFI_CACSCA_F0:RW:0:5:=0x00
		REG_DDRPHY_WRITE(i,0x263, 0x010000C9);	//PI_TDFI_CASEL_F1:RW:24:5:=0x01 PI_TDFI_CACSCA_F1:RW:16:5:=0x00 PI_TVREF_LONG_F0:RW:0:10:=0x0006
		REG_DDRPHY_WRITE(i,0x268, 0x07070705);	//PI_TDFI_CALVL_STROBE_F2:RW:24:4:=0x09 PI_TDFI_CALVL_STROBE_F1:RW:16:4:=0x09 PI_TDFI_CALVL_STROBE_F0:RW:8:4:=0x06 PI_TDFI_INIT_COMPLETE_MIN:RW:0:8:=0x05
		REG_DDRPHY_WRITE(i,0x26D, 0x10100100);	//PI_TCKEHDQS_F1:RW:24:6:=0x18 PI_TCKEHDQS_F0:RW:16:6:=0x02 PI_REFRESH_BETWEEN_SEGMENT_DISABLE:RW_D:8:1:=0x01 PI_DRAM_CLK_DISABLE_DEASSERT_SEL:RW:0:1:=0x00
		REG_DDRPHY_WRITE(i,0x26E, 0x00010110);	//PI_WDQLVL_BST_NUM:RW:16:3:=0x01 PI_WDQLVL_VREF_EN:RW:8:1:=0x01 PI_TCKEHDQS_F2:RW:0:6:=0x18
		REG_DDRPHY_WRITE(i,0x2A8, 0x00185000);	//PI_TDFI_CTRLUPD_MAX_F0:RW:8:16:=0x008c PI_TDFI_CTRLUPD_MIN:RD:0:4:=0x00
		REG_DDRPHY_WRITE(i,0x2A9, 0x0000f320);	//PI_TDFI_CTRLUPD_INTERVAL_F0:RW:0:32:=0x00000578
		REG_DDRPHY_WRITE(i,0x2AA, 0x00001850);	//PI_TDFI_CTRLUPD_MAX_F1:RW:0:16:=0x304c
		REG_DDRPHY_WRITE(i,0x2AB, 0x0000f320);	//PI_TDFI_CTRLUPD_INTERVAL_F1:RW:0:32:=0x0001e2f8
		REG_DDRPHY_WRITE(i,0x2AC, 0x00001850);	//PI_TDFI_CTRLUPD_MAX_F2:RW:0:16:=0x304c
		REG_DDRPHY_WRITE(i,0x2AD, 0x0000f320);	//PI_TDFI_CTRLUPD_INTERVAL_F2:RW:0:32:=0x0001e2f8

		REG_DDRPHY_WRITE(i,0x44C, 0x00020109);	//PHY_WRITE_PATH_LAT_ADD_0:RW+:24:3:=0x00 PHY_RDDQS_LATENCY_ADJUST_0:RW+:16:4:=0x04 PHY_RDDQS_GATE_SLAVE_DELAY_0:RW+:0:10:=0x0109
		REG_DDRPHY_WRITE(i,0x452, 0x070000C0);	//PHY_RDDATA_EN_DLY_0:RW+:24:4:=0x0e PHY_IE_MODE_0:RW+:16:2:=0x00 PHY_RDDATA_EN_IE_DLY_0:RW+:8:2:=0x00 PHY_DQS_IE_TIMING_0:RW+:0:8:=0xc0
		REG_DDRPHY_WRITE(i,0x453, 0x00100006);	//PHY_MASTER_DELAY_START_0:RW+:16:10:=0x0010 PHY_SW_MASTER_MODE_0:RW+:8:4:=0x00 PHY_RDDATA_EN_TSEL_DLY_0:RW+:0:4:=0x0c
		REG_DDRPHY_WRITE(i,0x454, 0x0C054208);	//PHY_WRLVL_DLY_STEP_0:RW+:24:4:=0x0c PHY_RPTR_UPDATE_0:RW+:16:4:=0x06 PHY_MASTER_DELAY_WAIT_0:RW+:8:8:=0x42 PHY_MASTER_DELAY_STEP_0:RW+:0:6:=0x08
		REG_DDRPHY_WRITE(i,0x4CC, 0x00020109);	//PHY_WRITE_PATH_LAT_ADD_1:RW+:24:3:=0x00 PHY_RDDQS_LATENCY_ADJUST_1:RW+:16:4:=0x04 PHY_RDDQS_GATE_SLAVE_DELAY_1:RW+:0:10:=0x0109
		REG_DDRPHY_WRITE(i,0x4D2, 0x070000C0);	//PHY_RDDATA_EN_DLY_1:RW+:24:4:=0x0e PHY_IE_MODE_1:RW+:16:2:=0x00 PHY_RDDATA_EN_IE_DLY_1:RW+:8:2:=0x00 PHY_DQS_IE_TIMING_1:RW+:0:8:=0xc0
		REG_DDRPHY_WRITE(i,0x4D3, 0x00100006);	//PHY_MASTER_DELAY_START_1:RW+:16:10:=0x0010 PHY_SW_MASTER_MODE_1:RW+:8:4:=0x00 PHY_RDDATA_EN_TSEL_DLY_1:RW+:0:4:=0x0c
		REG_DDRPHY_WRITE(i,0x4D4, 0x0C054208);	//PHY_WRLVL_DLY_STEP_1:RW+:24:4:=0x0c PHY_RPTR_UPDATE_1:RW+:16:4:=0x06 PHY_MASTER_DELAY_WAIT_1:RW+:8:8:=0x42 PHY_MASTER_DELAY_STEP_1:RW+:0:6:=0x08
		REG_DDRPHY_WRITE(i,0x54C, 0x00020109);	//PHY_WRITE_PATH_LAT_ADD_2:RW+:24:3:=0x00 PHY_RDDQS_LATENCY_ADJUST_2:RW+:16:4:=0x04 PHY_RDDQS_GATE_SLAVE_DELAY_2:RW+:0:10:=0x0109
		REG_DDRPHY_WRITE(i,0x552, 0x070000C0);	//PHY_RDDATA_EN_DLY_2:RW+:24:4:=0x0e PHY_IE_MODE_2:RW+:16:2:=0x00 PHY_RDDATA_EN_IE_DLY_2:RW+:8:2:=0x00 PHY_DQS_IE_TIMING_2:RW+:0:8:=0xc0
		REG_DDRPHY_WRITE(i,0x553, 0x00100006);	//PHY_MASTER_DELAY_START_2:RW+:16:10:=0x0010 PHY_SW_MASTER_MODE_2:RW+:8:4:=0x00 PHY_RDDATA_EN_TSEL_DLY_2:RW+:0:4:=0x0c
		REG_DDRPHY_WRITE(i,0x554, 0x0C054208);	//PHY_WRLVL_DLY_STEP_2:RW+:24:4:=0x0c PHY_RPTR_UPDATE_2:RW+:16:4:=0x06 PHY_MASTER_DELAY_WAIT_2:RW+:8:8:=0x42 PHY_MASTER_DELAY_STEP_2:RW+:0:6:=0x08
		REG_DDRPHY_WRITE(i,0x5CC, 0x00020109);	//PHY_WRITE_PATH_LAT_ADD_3:RW+:24:3:=0x00 PHY_RDDQS_LATENCY_ADJUST_3:RW+:16:4:=0x04 PHY_RDDQS_GATE_SLAVE_DELAY_3:RW+:0:10:=0x0109
		REG_DDRPHY_WRITE(i,0x5D2, 0x070000C0);	//PHY_RDDATA_EN_DLY_3:RW+:24:4:=0x0e PHY_IE_MODE_3:RW+:16:2:=0x00 PHY_RDDATA_EN_IE_DLY_3:RW+:8:2:=0x00 PHY_DQS_IE_TIMING_3:RW+:0:8:=0xc0
		REG_DDRPHY_WRITE(i,0x5D3, 0x00100006);	//PHY_MASTER_DELAY_START_3:RW+:16:10:=0x0010 PHY_SW_MASTER_MODE_3:RW+:8:4:=0x00 PHY_RDDATA_EN_TSEL_DLY_3:RW+:0:4:=0x0c
		REG_DDRPHY_WRITE(i,0x5D4, 0x0C054208);	//PHY_WRLVL_DLY_STEP_3:RW+:24:4:=0x0c PHY_RPTR_UPDATE_3:RW+:16:4:=0x06 PHY_MASTER_DELAY_WAIT_3:RW+:8:8:=0x42 PHY_MASTER_DELAY_STEP_3:RW+:0:6:=0x08
		REG_DDRPHY_WRITE(i,0x719, 0x00000303);	//PHY_LP_WAKEUP:RW:24:4:=0x00 PHY_LP4_BOOT_LOW_FREQ_SEL:RW:16:1:=0x01 PHY_TCKSRE_WAIT:RW:8:4:=0x05 PHY_PLL_TESTOUT_SEL:RW:0:3:=0x03
	}
}

