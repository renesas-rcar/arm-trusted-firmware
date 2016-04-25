#include <stdint.h>		//for uint32_t
#include <string.h>		//for uint32_t
#include <stdio.h>		//for uint32_t
#ifdef RCAR_LSI
#include <debug.h>
#endif

#define RCAR_DDR_VERSION        "rev.0.15"
#define DRAM_CH_CNT		0x02

///////////////////////////////////////////////////////////
//BOARD CONFIGRATION
//PLEASE DEFINE THE FOLLOWING MACRO
///////////////////////////////////////////////////////////
//number of board configurations(PLEASE SET)
#define BOARDNUM 2
///////////////////////////////////////////////////////////
//PLEASE SET _def_BOARDTYPE value
#define	BOARD_SALVATOR_X
//#define	BOARD_KRIEK

#ifdef	BOARD_SALVATOR_X
#define _def_BOARDTYPE     (0)
#else
#define _def_BOARDTYPE     (1)
#endif

#undef BOARD_KRIEK
#undef BOARD_SALVATOR_X

///////////////////////////////////////////////////////////
//BOARD CONFIGRATION
//PLEASE DEFINE THE FOLLOWING ARRAY
///////////////////////////////////////////////////////////
//SoC caX([5][4][3][2][1][0]) -> MEM caY: 
//SoC caX([5][4][3][2][1][0]) -> Dfi caY:
static const uint32_t BOARDCNF_CA_SWAP[BOARDNUM][DRAM_CH_CNT]= 
{
	{ //BOARDTYPE:0
		0x00543210,//CH0
		0x00543210 //CH1
	},
	{ //BOARDTYPE:1
		0x00345201,//CH0
		0x00302154 //CH1
	}
};
//SoC dqsX([3][2][1][0]) -> MEM dqsY: 
static const uint16_t BOARDCNF_DQS_SWAP[BOARDNUM][DRAM_CH_CNT]= 
{
	{ //BOARDTYPE:0
		0x3201,//CH0
		0x2310 //CH1
	},
	{ //BOARDTYPE:1
		0x3201,//CH0
		0x2310 //CH1
	}
};
//SoC dq([7][6][5][4][3][2][1][0]) -> MEM dqY:  (8 means DM)
static const uint32_t BOARDCNF_DQ_SWAP[BOARDNUM][DRAM_CH_CNT][4]= 
{
	{ //BOARDTYPE0
		{ //CH0 SoC_DQS0    SoC_DQS1    SoC_DQS2    SoC_DQS3
			0x70612543, 0x43251670, 0x45326170, 0x10672534
		},
		{ //CH1 SoC_DQS0    SoC_DQS1    SoC_DQS2    SoC_DQS3
			0x01327654, 0x34526107, 0x35421670, 0x70615324
		}
	},
	{//BOARDTYPE1
		{ //CH0 SoC_DQS0    SoC_DQS1    SoC_DQS2    SoC_DQS3
			0x01672543, 0x45361207, 0x45632107, 0x60715234
		},
		{ //CH1 SoC_DQS0    SoC_DQS1    SoC_DQS2    SoC_DQS3
			0x01672543, 0x45361207, 0x45632107, 0x60715234
		}
	}
};
//SoC dm -> MEM dqY:  (8 means DM)
static const uint8_t BOARDCNF_DM_SWAP[BOARDNUM][DRAM_CH_CNT][4]= 
{
	{ //BOARDTYPE0
		{ //CH0 SoC_DQS0    SoC_DQS1    SoC_DQS2    SoC_DQS3
			0x08,       0x08,       0x08,       0x08
		},
		{ //CH1 SoC_DQS0    SoC_DQS1    SoC_DQS2    SoC_DQS3
			0x08,       0x08,       0x08,       0x08
		}
	},
	{//BOARDTYPE1
		{ //CH0 SoC_DQS0    SoC_DQS1    SoC_DQS2    SoC_DQS3
			0x08,       0x08,       0x08,       0x08
		},
		{ //CH1 SoC_DQS0    SoC_DQS1    SoC_DQS2    SoC_DQS3
			0x08,       0x08,       0x08,       0x08
		}
	}
};

///////////////////////////////////////////////////////////
//MEMORY CONFIGRATION
//PLEASE DEFINE THE FOLLOWING MACRO
///////////////////////////////////////////////////////////
//0x00... 4Gbit/die
//0x01... 6Gbit/die
//0x02... 8Gbit/die
//0x03...12Gbit/die
//0x04...16Gbit/die
//0x05...24Gbit/die
//0x06...32Gbit/die
//0xff...NO_MEMORY

//example 1 (8Gbit 2rank device)
#define DBMEMCONF_DENS(ch,cs) (0x02)

//example 2 (12Gbit 1rank device)
//#define DBMEMCONF_DENS(ch,cs) (!(cs)? 0x03 : 0xff)

//example 3 (8Gbit 2rank device + 8Gbit 1rank device)
//#define DBMEMCONF_DENS(ch,cs) (!(ch)? 0x02 : !(cs)? 0x02 : 0xff)

///////////////////////////////////////////////////////////
//CLOCK DEFINITION
//PLEASE DEFINE
#if 0 //pre-define
//example : 16.66MHz BRD_CLKx3=50  (md14,md13==0,0)
//example : 20.00MHz BRD_CLKx3=60  (md14,md13==0,1)
//example : 25.00MHz BRD_CLKx3=75  (md14,md13==1,0)
//example : 33.33MHz BRD_CLKx3=100 (md14,md13==1,1)
#define _def_BRD_CLKx3  50
#else
//judge by md14,md13 value (please specify 0)
#define _def_BRD_CLKx3 0
#endif

#if 0 //pre-define
//           16.66MHz 20.00MHz 25.00MHz 33.33MHz << board clk
//DDR3200 :      0x5f     0x4f     0x3f    0x2f (md19,17==0,0)
//DDR2800 :      0x53     0x45     0x37    0x29 (md19,17==0,1)
//DDR2400 :      0x47     0x3b     0x2f    0x23 (md19,17==1,0)
//DDR1600 :      0x2f     0x27     0x1f    0x17 (md19,17==1,1)
#define _def_DDR_CLK 0x5f
#else
//judge by md19,md17 value (please specify 0)
#define _def_DDR_CLK 0
#endif
///////////////////////////////////////////////////////////
//REFRESH DEFINITION (DO NOT MODIFY)
#if 0
//define by TCK cycle
//3200Mbps: 1890ns / 625ps = 3024 cycle
//2800Mbps: 1890ns / 714ps = 2646 cycle
//2400Mbps: 1890ns / 833ps = 2268 cycle
//1600Mbps: 1890ns /1300ps = 1512 cycle
#define _def_REFCYCLE  3024 
#define _def_REFPERIOD  0	//in ns (30ns * 63)
#else
//calculated by PERIOD/TCK 
//refcycle should be set as 0
//period should be multiple of (period of board clk)
#define _def_REFCYCLE      0
#define _def_REFPERIOD  1890	//in ns (30ns * 63)
#endif
///////////////////////////////////////////////////////////
//DO NOT TOUCH BELOW
///////////////////////////////////////////////////////////
#define DDRXXXX_CALC(tgt,boardclk,boardclkdiv) ((tgt)*(boardclkdiv)/(boardclk*2)-1)
#define DDRXXXX_RECALC(freq,boardclk,boardclkdiv) (((freq)+1)*(boardclk*2)/(boardclkdiv))
///
#define DBMEMCONF_REG(d3,row,bank,col,dw) ((d3)<<30 | ((row)<<24) | ((bank)<<16) | ((col)<<8) | (dw))
#define DBMEMCONF_REGD(density) (DBMEMCONF_REG((density)%2,((density)+1)/2+(29-3-10-2),3,10,2))
#define DBMEMCONF_VAL(ch,cs) (DBMEMCONF_REGD(DBMEMCONF_DENS(ch,cs)))
///////////////////////////////////////////////////////////
#define	DDR_PHYVALID		0x03

#define	DBSC_REFINTS			0x1		// 0: Average interval is REFINT. / 1: Average interval is 1/2 REFINT.

///////////////////////////////////////////////////////////
/* add start */
#define	RST_BASE				(0xE6160000U)
#define	RST_MODEMR				(RST_BASE + 0x0060U)
#define	CPG_BASE				(0xE6150000U)
#define	CPG_PLLECR				(CPG_BASE + 0x00D0U)
#define	CPG_CPGWPR				(CPG_BASE + 0x900U)
#define	CPG_PLL3CR				(CPG_BASE + 0x0DCU)
#define	CPG_SRCR4				(CPG_BASE + 0x0BCU)
#define	CPG_SRSTCLR4				(CPG_BASE + 0x950U)
///////////////////////////////////////////////////////////
#define DBSC_DBSYSCONF1			0xE6790004
#define DBSC_DBPHYCONF0			0xE6790010
#define DBSC_DBKIND			0xE6790020

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

#define DBSC_DBMEMCONF(ch,cs)		(0xE6790030ULL+0x10*ch+0x4*cs)

#define DBSC_DBSTATE0			0xE6790108

#define DBSC_DBACEN			0xE6790200
#define DBSC_DBRFEN			0xE6790204
#define DBSC_DBCMD			0xE6790208
#define DBSC_DBWAIT			0xE6790210	//wait DBCMD 1=busy, 0=ready
#define DBSC_DBSYSCTRL0			0xE6790280

#define DBSC_DBTR0			0xE6790300
#define DBSC_DBTR1			0xE6790304
#define DBSC_DBTR3			0xE679030C
#define DBSC_DBTR4			0xE6790310
#define DBSC_DBTR5			0xE6790314
#define DBSC_DBTR6			0xE6790318
#define DBSC_DBTR7			0xE679031C
#define DBSC_DBTR8			0xE6790320
#define DBSC_DBTR9			0xE6790324
#define DBSC_DBTR10			0xE6790328
#define DBSC_DBTR11			0xE679032C
#define DBSC_DBTR12			0xE6790330
#define DBSC_DBTR13			0xE6790334
#define DBSC_DBTR14			0xE6790338
#define DBSC_DBTR15			0xE679033C
#define DBSC_DBTR16			0xE6790340
#define DBSC_DBTR17			0xE6790344
#define DBSC_DBTR18			0xE6790348
#define DBSC_DBTR19			0xE679034C
#define DBSC_DBTR20			0xE6790350
#define DBSC_DBTR21			0xE6790354
#define DBSC_DBTR22			0xE6790358
#define DBSC_DBTR23			0xE679035C
#define DBSC_DBTR24			0xE6790360
#define DBSC_DBTR25			0xE6790364

#define DBSC_DBBL			0xE6790400

#define DBSC_DBRFCNF1			0xE6790414
#define DBSC_DBRFCNF2			0xE6790418

#define DBSC_DBRNK0			0xE6790430
#define DBSC_DBRNK1			0xE6790434
#define DBSC_DBRNK2			0xE6790438
#define DBSC_DBRNK3			0xE679043C
#define DBSC_DBRNK4			0xE6790440
#define DBSC_DBRNK5			0xE6790444
#define DBSC_DBRNK6			0xE6790448

#define DBSC_DBADJ0			0xE6790500
#define DBSC_DBADJ2			0xE6790508
#define DBSC_DBDBICNT			0xE6790518

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

#define DBSC_DBPDRGA_0			0xE6790624
#define DBSC_DBPDRGD_0			0xE6790628
#define DBSC_DBPDRGA_1			0xE6790664
#define DBSC_DBPDRGD_1			0xE6790668
#define DBSC_DBPDRGA_2			0xE67906A4
#define DBSC_DBPDRGD_2			0xE67906A8
#define DBSC_DBPDRGA_3			0xE67906E4
#define DBSC_DBPDRGD_3			0xE67906E8
#define DBSC_DBPDRGA(ch)		(0xE6790624ULL+0x40*(ch))
#define DBSC_DBPDRGD(ch)		(0xE6790628ULL+0x40*(ch))

#define DBSC_DBPDSTAT_0			0xE6790630
#define DBSC_DBPDSTAT_1			0xE6790670
#define DBSC_DBPDSTAT_2			0xE67906B0
#define DBSC_DBPDSTAT_3			0xE67906F0
#define DBSC_DBPDSTAT(ch)		(0xE6790630ULL+0x40*(ch))

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

#define DBSC_SCFCTST0			0xE6791700	//Schedule timing setting register 0
#define DBSC_SCFCTST1			0xE6791708	//Schedule timing setting register 1
#define DBSC_SCFCTST2			0xE679170C	//Schedule timing setting register 2

#define DBSC_DBMRRDR(ch)		(0xE6791800ULL+0x4*(ch))
#define DBSC_DBMRRDR_0			0xE6791800
#define DBSC_DBMRRDR_1			0xE6791804
#define DBSC_DBMRRDR_2			0xE6791808
#define DBSC_DBMRRDR_3			0xE679180C

#define DBSC_DBMEMSWAPCONF0		0xE6792000

#define DBSC_DBMONCONF4			0xE6793010

#define DBSC_PLL_LOCK(ch)		(0xE6794054ULL+0x100*(ch))
#define DBSC_PLL_LOCK_0			0xE6794054
#define DBSC_PLL_LOCK_1			0xE6794154
#define DBSC_PLL_LOCK_2			0xE6794254
#define DBSC_PLL_LOCK_3			0xE6794354

#define DBSC_DBTSTCONF1			0xE6794804
	
#define DBSC_FREQ_CHG_ACK_0		0xE6790618
#define DBSC_FREQ_CHG_ACK_1		0xE6790658
#define DBSC_FREQ_CHG_ACK_2		0xE6790698
#define DBSC_FREQ_CHG_ACK_3		0xE67906D8

#define DBSC_DFI_FREQ_0			0xE6790614
#define DBSC_DFI_FREQ_1			0xE6790654
#define DBSC_DFI_FREQ_2			0xE6790694
#define DBSC_DFI_FREQ_3			0xE67906D4

#include "init_dram_tbl_m3_es10.h"
uint32_t BRD_CLKx3;
uint32_t DDR1600_CLK;
uint32_t DDR2400_CLK;
uint32_t DDR2800_CLK;
uint32_t DDR3200_CLK;
uint32_t DDR_CLK;
uint32_t _cnf_DDR_PHY_SLICE_REGSET[128][2];
uint32_t _cnf_DDR_PHY_ADR_V_REGSET[128][2];
uint32_t _cnf_DDR_PHY_ADR_I_REGSET[128][2];
uint32_t _cnf_DDR_PHY_ADR_G_REGSET[128][2];
uint32_t _cnf_DDR_PI_REGSET[255 +1][2];
static uint32_t InitDDR_start(uint32_t freq);
static void SoftDelay_ddr(uint32_t loop);
static inline void dsb_sev(void);
static uint32_t PLL3_MODE;
uint32_t InitResetCount;
uint32_t PadCalFailCount;
uint32_t TrainingFailCount;
static inline uint32_t PLL3_FREQ_01();
static inline uint32_t PLL3_FREQ_10();
static inline uint32_t PLL3_FREQ(uint32_t freq_flag_b,uint32_t freq_flag_a);
static void WaitDBCMD(void);
static void SendDBCMD(uint32_t cmd);
static void REG_DDRPHY_WRITE_A ( uint32_t regadd, uint32_t regdata);
static void REG_DDRPHY_WRITE ( uintptr_t phyno, uint32_t regadd, uint32_t regdata);
static uint32_t REG_DDRPHY_READ ( uintptr_t phyno, uint32_t regadd);
static void change_lpddr4_en(uintptr_t phychno, uint32_t mode);
static void pvt_dbsc_regset(uint32_t freq);
static void mode_register_set(uint32_t freq,uintptr_t phychno,uint32_t mr13_val,uint32_t postflag);
static inline void ResetDram()
{
 *((volatile uint32_t*)CPG_CPGWPR) = ~0x40000000;
 dsb_sev();
 *((volatile uint32_t*)CPG_SRCR4) = 0x40000000;
 *((volatile uint32_t*)CPG_SRSTCLR4) = 0x40000000;
 dsb_sev();
}
static void SoftDelay_ddr(uint32_t loop)
{
 volatile uint32_t i;
 for(i=0;i<(loop*2);i++){};
}
static inline void dsb_sev(void)
{
 __asm__ __volatile__ ("dsb sy");
}
static inline void ddrphy_regif_wa()
{
 *((volatile uint32_t*)(DBSC_DBPDRGA_0)) = 0x02A9;
 *((volatile uint32_t*)(DBSC_DBPDRGA_1)) = 0x02A9;
 dsb_sev();
}
static void SendDBCMD(uint32_t cmd)
{
 uint32_t dataL;
 uint32_t count;
 count=0;
 dataL = *((volatile uint32_t*)DBSC_DBCMD);
 dsb_sev();
 while(1)
 {
  dataL = *((volatile uint32_t*)DBSC_DBWAIT);
  dsb_sev();
  if((dataL & 0x00000001) == 0x0) break;
  count++;
 }
 *((volatile uint32_t*)DBSC_DBCMD) = cmd;
 dsb_sev();
}
static void WaitDBCMD(void)
{
 uint32_t dataL;
 dataL = *((volatile uint32_t*)DBSC_DBCMD);
 dsb_sev();
 while(1)
 {
  dataL = *((volatile uint32_t*)DBSC_DBWAIT);
  dsb_sev();
  if((dataL & 0x00000001) == 0x0) break;
 }
}
static uint32_t REG_DDRPHY_READ ( uintptr_t phyno, uint32_t regadd)
{
 uint32_t val,loop;
 *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno)) = regadd;
 dsb_sev();
 val = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
 dsb_sev();
 for(loop=0;loop<(PLL3_MODE?1:8);loop++){
  val = *((volatile uint32_t*)(DBSC_DBPDRGD_0 + 0x40 * phyno));
  dsb_sev();
 }
 (void)val;
 return val ;
}
static void REG_DDRPHY_WRITE ( uintptr_t phyno, uint32_t regadd, uint32_t regdata)
{
 uint32_t val,loop;
 *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno)) = regadd;
 dsb_sev();
 *((volatile uint32_t*)(DBSC_DBPDRGD_0 + 0x40 * phyno)) = regdata;
 dsb_sev();
 for(loop=0;loop<(PLL3_MODE?1:30);loop++){
  val = *((volatile uint32_t*)(DBSC_DBPDRGD_0 + 0x40 * phyno));
  dsb_sev();
 }
 (void)val;
}
static void REG_DDRPHY_WRITE_A ( uint32_t regadd, uint32_t regdata)
{
 uint32_t val,loop;
 uintptr_t phyno;
 for(phyno=0;phyno<DRAM_CH_CNT;phyno++)
  *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno)) = regadd;
 dsb_sev();
 for(phyno=0;phyno<DRAM_CH_CNT;phyno++)
  *((volatile uint32_t*)(DBSC_DBPDRGD_0 + 0x40 * phyno)) = regdata;
 dsb_sev();
 for(loop=0;loop<(PLL3_MODE?1:30);loop++){
  val = *((volatile uint32_t*)(DBSC_DBPDRGD_0 + 0x40 * (phyno-1)));
  dsb_sev();
 }
 (void)val;
}
static void _TBLCOPY(uint32_t to[][2], const uint32_t from[][2]){
 uint32_t i,tmp;
 i=0;
 while(1){
  tmp=to[i][0]=from[i][0];
  to[i][1]=from[i][1];
  if(tmp==0xffffffff)break;
  i++;
 }
}
static uint32_t REG_DDRTBL_READ(uint32_t tbl[][2], uint32_t a){
 uint32_t i,tmp;
 i=0;
 tmp=0;
 while(tmp!=0xffffffff) {
  tmp=tbl[i][0];
  if(tmp==a)return tbl[i][1];
  i++;
 }
 NOTICE("REG_DDRTBL_READ: REGADD=%08x not found\n",a);
 while(1);
}
static void REG_DDRTBL_WRITE(uint32_t tbl[][2], uint32_t a,uint32_t d){
 uint32_t i,tmp;
 i=0;
 tmp=0;
 while(tmp!=0xffffffff) {
  tmp=tbl[i][0];
  if(tmp==a){
   tbl[i][1]=d;
   return;
  }
  i++;
 };
 NOTICE("REG_DDRTBL_WRITE: REGADD=%08x not found\n",a);
 while(1);
}
static void REG_DDRTBL_MOD(uint32_t tbl[][2], uint32_t a,uint32_t mask, uint32_t d){
 uint32_t i,tmp;
 i=0;
 tmp=0;
 while(tmp!=0xffffffff) {
  tmp=tbl[i][0];
  if(tmp==a){
   tbl[i][1]=(tbl[i][1]&~mask)| (d&mask);
   return;
  }
  i++;
 };
 NOTICE("REG_DDRTBL_MOD: REGADD=%08x not found\n",a);
 while(1);
}
uint32_t ddr_density[DRAM_CH_CNT][4];
static void DDR_CHK(void){
 uint32_t DBTR0_BAK;
 uint32_t DBTR16_BAK;
 uint32_t ch,cs,slice,i;
 uint32_t dataL;
 uint32_t ddr_density_e1[DRAM_CH_CNT*2][2];
 uint32_t ddr_density_e2[DRAM_CH_CNT*2][2];
 uint32_t ddr_density_tp[DRAM_CH_CNT*2][2];
 for(cs=0;cs<2;cs++){
  for(ch=0;ch<DRAM_CH_CNT*2;ch++){
   ddr_density_tp[ch][cs]=0x0;
   ddr_density_e1[ch][cs]=0x0;
   ddr_density_e2[ch][cs]=0x0;
  }
 }
 DBTR0_BAK = *((volatile uint32_t*)DBSC_DBTR0);
 DBTR16_BAK = *((volatile uint32_t*)DBSC_DBTR16);
 *((volatile uint32_t*)DBSC_DBTR0) = 0x00000006;
 *((volatile uint32_t*)DBSC_DBTR16) = 0x001f0406 | (((5)&0xff)<<24);
 for(ch=0;ch<DRAM_CH_CNT;ch++){
  for(slice=0;slice<4;slice++){
   REG_DDRPHY_WRITE(ch,0x0806+0x80*slice,REG_DDRPHY_READ(ch,0x806+0x80*slice) & 0xff000000);
   REG_DDRPHY_WRITE(ch, 0x820+0x80*slice,REG_DDRPHY_READ(ch,0x820+0x80*slice)|0xFF000000);
  }
 }
 for(cs=0;cs<2;cs++){
 for(i=0;i<4;i++){
  for(ch=0;ch<DRAM_CH_CNT;ch++)
   SendDBCMD(0x0F000C00 | (ch<<20) | (cs<<16));
  WaitDBCMD();
  for (ch=0;ch<DRAM_CH_CNT*2;ch++) {
   dataL = (*((volatile uint32_t*)DBSC_DBMRRDR(ch)) >> (cs*8))&0xff;
   if(dataL!=0x4d){
    ddr_density_tp[ch][cs]=0xff;
    ddr_density_e1[ch][cs]++;
   }
  }
  for(ch=0;ch<DRAM_CH_CNT;ch++)
   SendDBCMD(0x0F000800 | (ch<<20) | (cs<<16));
  WaitDBCMD();
  for (ch=0;ch<DRAM_CH_CNT*2;ch++) {
   dataL = ((*((volatile uint32_t*)DBSC_DBMRRDR(ch)) >> (cs*8))&0xff)>>2;
   if(ddr_density_tp[ch][cs]==0xff || (i>0 && (ddr_density_tp[ch][cs]!=dataL))){
    ddr_density_e2[ch][cs]++;
    ddr_density_tp[ch][cs]=0xff;
   } else {
    ddr_density_tp[ch][cs]=dataL;
   }
  }
 }
 }
 for(ch=0;ch<DRAM_CH_CNT;ch++){
  for(slice=0;slice<4;slice++){
   REG_DDRPHY_WRITE(ch,0x0806+0x80*slice,(REG_DDRPHY_READ(ch,0x806+0x80*slice) & 0xff000000) | (0x006E6E0E) | 0x02000000);
  }
 }
 *((volatile uint32_t*)DBSC_DBTR0) = DBTR0_BAK;
 *((volatile uint32_t*)DBSC_DBTR16) = DBTR16_BAK;
 for(ch=0;ch<DRAM_CH_CNT;ch++){
  for(cs=0;cs<2;cs++){
   ddr_density[ch][cs] = ddr_density_tp[ch*2+0][cs];
   ddr_density[ch][cs+2] = ddr_density_tp[ch*2+1][cs];
  }
  for(cs=0;cs<2;cs++){
   if(ddr_density[ch][cs]==0xff){
    dataL=ddr_density[ch][cs];
    ddr_density[ch][cs]=ddr_density[ch][cs+2];
    ddr_density[ch][cs+2]=dataL;
   }
  }
 }
}
static void change_lpddr4_en(uintptr_t phychno, uint32_t mode)
{
 uint32_t i,from,to;
 uint32_t set,clr;
 uint32_t dataL;
 uint32_t addr;
 if(phychno>=2)
 {
  from=0; to=1;
 }
 else
 {
  from=phychno; to=phychno;
 }
 for(i=from;i<=to;i++)
 {
  set = 1ULL <<14;
  clr = ~set;
  if(!mode)
  {
   set = 0;
  };
  addr=0x0BA0; REG_DDRPHY_WRITE(i,addr,dataL=((REG_DDRPHY_READ(i,addr)&clr)|set) );
  addr=0x0BA1; REG_DDRPHY_WRITE(i,addr,dataL=((REG_DDRPHY_READ(i,addr)&clr)|set) );
  addr=0x0BA7; REG_DDRPHY_WRITE(i,addr,dataL=((REG_DDRPHY_READ(i,addr)&clr)|set) );
  addr=0x0BAB; REG_DDRPHY_WRITE(i,addr,dataL=((REG_DDRPHY_READ(i,addr)&clr)|set) );
 }
 ddrphy_regif_wa();
}
static void pvt_dbsc_regset(uint32_t freq)
{
 *((volatile uint32_t*)DBSC_DBPDCNT0_0) = 0x00;
 *((volatile uint32_t*)DBSC_DBPDCNT0_1) = 0x00;
 *((volatile uint32_t*)DBSC_DBSTATE0) = 0x00000010;
 *((volatile uint32_t*)DBSC_DBKIND) = 0x0000000a;
 *((volatile uint32_t*)DBSC_DBBL) = 0x00000002;
 {
  uint32_t ch,cs;
  for(ch=0;ch<DRAM_CH_CNT;ch++)
   for(cs=0;cs<4;cs++)
    *((volatile uint32_t*)DBSC_DBMEMCONF(ch,cs)) = DBMEMCONF_REGD(0);
 }
 *((volatile uint32_t*)DBSC_DBPHYCONF0) = 0x00000001;
 if(freq>DDR3200_CLK && 0)
 {
  *((volatile uint32_t*)DBSC_DBTR0) = 0x00000020;
  *((volatile uint32_t*)DBSC_DBTR1) = 0x0000000e;
  *((volatile uint32_t*)DBSC_DBTR3) = 0x00000022;
  *((volatile uint32_t*)DBSC_DBTR4) = 0x00280022;
  *((volatile uint32_t*)DBSC_DBTR5) = 0x00000071;
  *((volatile uint32_t*)DBSC_DBTR6) = 0x0000004f;
  *((volatile uint32_t*)DBSC_DBTR7) = 0x00130013;
  *((volatile uint32_t*)DBSC_DBTR8) = 0x0000004B;
  *((volatile uint32_t*)DBSC_DBTR9) = 0x0000000E;
  *((volatile uint32_t*)DBSC_DBTR10) = 0x00000023;
  *((volatile uint32_t*)DBSC_DBTR11) = 0x00000026;
  *((volatile uint32_t*)DBSC_DBTR12) = 0x002A002A;
  *((volatile uint32_t*)DBSC_DBTR13) = 0x00900120;
  *((volatile uint32_t*)DBSC_DBTR14) = 0x000c000c;
  *((volatile uint32_t*)DBSC_DBTR15) = 0x001e000c;
  *((volatile uint32_t*)DBSC_DBTR16) = (0x0d1f0c0e);
  *((volatile uint32_t*)DBSC_DBTR17) = 0x080a0004;
  *((volatile uint32_t*)DBSC_DBTR18) = 0x00000000;
  *((volatile uint32_t*)DBSC_DBTR19) = 0x00000000;
  *((volatile uint32_t*)DBSC_DBTR20) = 0x012c012c;
  *((volatile uint32_t*)DBSC_DBTR21) = (8) |((8)<<16);
  *((volatile uint32_t*)DBSC_DBTR22) = 0x06400030;
  *((volatile uint32_t*)DBSC_DBTR23) = 0x00000002;
  *((volatile uint32_t*)DBSC_DBTR24) = (0x06100509);
 }
 else if(freq>DDR2800_CLK || 1)
 {
  *((volatile uint32_t*)DBSC_DBTR0) = 0x00000020;
  *((volatile uint32_t*)DBSC_DBTR1) = 0x0000000e;
  *((volatile uint32_t*)DBSC_DBTR3) = 0x0000001d;
  *((volatile uint32_t*)DBSC_DBTR4) = 0x0022001d;
  *((volatile uint32_t*)DBSC_DBTR5) = 0x00000060;
  *((volatile uint32_t*)DBSC_DBTR6) = 0x00000044;
  *((volatile uint32_t*)DBSC_DBTR7) = 0x00100010;
  *((volatile uint32_t*)DBSC_DBTR8) = 0x00000040;
  *((volatile uint32_t*)DBSC_DBTR9) = 0x0000000c;
  *((volatile uint32_t*)DBSC_DBTR10) = 0x0000001d;
  *((volatile uint32_t*)DBSC_DBTR11) = 0x00000025;
  *((volatile uint32_t*)DBSC_DBTR12) = 0x00270027;
  *((volatile uint32_t*)DBSC_DBTR13) = 0x00900120;
  *((volatile uint32_t*)DBSC_DBTR14) = 0x000c000c;
  *((volatile uint32_t*)DBSC_DBTR15) = 0x001e000c;
  *((volatile uint32_t*)DBSC_DBTR16) = (0x0d1f0c0e);
  *((volatile uint32_t*)DBSC_DBTR17) = 0x080a0004;
  *((volatile uint32_t*)DBSC_DBTR18) = 0x00000000;
  *((volatile uint32_t*)DBSC_DBTR19) = 0x00000000;
  *((volatile uint32_t*)DBSC_DBTR20) = 0x012c012c;
  *((volatile uint32_t*)DBSC_DBTR21) = (8) |((8)<<16);
  *((volatile uint32_t*)DBSC_DBTR22) = 0x06400030;
  *((volatile uint32_t*)DBSC_DBTR23) = 0x00000002;
  *((volatile uint32_t*)DBSC_DBTR24) = (0x06100509);
 }
 else if(freq>DDR2400_CLK)
 {
  *((volatile uint32_t*)DBSC_DBTR0) = 0x0000001c;
  *((volatile uint32_t*)DBSC_DBTR1) = 0x0000000e;
  *((volatile uint32_t*)DBSC_DBTR3) = 0x0000001a;
  *((volatile uint32_t*)DBSC_DBTR4) = 0x001e001a;
  *((volatile uint32_t*)DBSC_DBTR5) = 0x00000055;
  *((volatile uint32_t*)DBSC_DBTR6) = 0x0000003b;
  *((volatile uint32_t*)DBSC_DBTR7) = 0x000f000f;
  *((volatile uint32_t*)DBSC_DBTR8) = 0x00000039;
  *((volatile uint32_t*)DBSC_DBTR9) = 0x0000000c;
  *((volatile uint32_t*)DBSC_DBTR10) = 0x0000001E;
  *((volatile uint32_t*)DBSC_DBTR11) = 0x00000022;
  *((volatile uint32_t*)DBSC_DBTR12) = 0x00260026;
  *((volatile uint32_t*)DBSC_DBTR13) = 0x007f00fd;
  *((volatile uint32_t*)DBSC_DBTR14) = 0x000c000c;
  *((volatile uint32_t*)DBSC_DBTR15) = 0x0016000b;
  *((volatile uint32_t*)DBSC_DBTR16) = (0x0d1f0c0e);
  *((volatile uint32_t*)DBSC_DBTR17) = 0x0c0f0020;
  *((volatile uint32_t*)DBSC_DBTR18) = 0x00000000;
  *((volatile uint32_t*)DBSC_DBTR19) = 0x00000000;
  *((volatile uint32_t*)DBSC_DBTR20) = 0x01070107;
  *((volatile uint32_t*)DBSC_DBTR21) = (8) |((8)<<16);
  *((volatile uint32_t*)DBSC_DBTR22) = 0x06400030;
  *((volatile uint32_t*)DBSC_DBTR23) = 0x00000002;
  *((volatile uint32_t*)DBSC_DBTR24) = (0x06100509);
 }
 else if(freq>DDR1600_CLK)
 {
  *((volatile uint32_t*)DBSC_DBTR0) = 0x00000018;
  *((volatile uint32_t*)DBSC_DBTR1) = 0x0000000c;
  *((volatile uint32_t*)DBSC_DBTR3) = 0x00000016;
  *((volatile uint32_t*)DBSC_DBTR4) = 0x001a0016;
  *((volatile uint32_t*)DBSC_DBTR5) = 0x00000049;
  *((volatile uint32_t*)DBSC_DBTR6) = 0x00000033;
  *((volatile uint32_t*)DBSC_DBTR7) = 0x000d000d;
  *((volatile uint32_t*)DBSC_DBTR8) = 0x00000031;
  *((volatile uint32_t*)DBSC_DBTR9) = 0x0000000a;
  *((volatile uint32_t*)DBSC_DBTR10) = 0x00000016;
  *((volatile uint32_t*)DBSC_DBTR11) = 0x0000001c;
  *((volatile uint32_t*)DBSC_DBTR12) = 0x00220022;
  *((volatile uint32_t*)DBSC_DBTR13) = 0x006d00d9;
  *((volatile uint32_t*)DBSC_DBTR14) = 0x000a000a;
  *((volatile uint32_t*)DBSC_DBTR15) = 0x0013000c;
  *((volatile uint32_t*)DBSC_DBTR16) = (0x0d1f0c0e);
  *((volatile uint32_t*)DBSC_DBTR17) = 0x080a0004;
  *((volatile uint32_t*)DBSC_DBTR18) = 0x00000000;
  *((volatile uint32_t*)DBSC_DBTR19) = 0x00000000;
  *((volatile uint32_t*)DBSC_DBTR20) = 0x00e200e2;
  *((volatile uint32_t*)DBSC_DBTR21) = (8) |((8)<<16);
  *((volatile uint32_t*)DBSC_DBTR22) = 0x04b10025;
  *((volatile uint32_t*)DBSC_DBTR23) = 0x00000002;
  *((volatile uint32_t*)DBSC_DBTR24) = (0x06100509);
 }
 else
 {
  *((volatile uint32_t*)DBSC_DBTR0) = 0x0000000e;
  *((volatile uint32_t*)DBSC_DBTR1) = 0x0000000c;
  *((volatile uint32_t*)DBSC_DBTR3) = 0x0000000f;
  *((volatile uint32_t*)DBSC_DBTR4) = 0x0011000f;
  *((volatile uint32_t*)DBSC_DBTR5) = 0x00000030;
  *((volatile uint32_t*)DBSC_DBTR6) = 0x00000022;
  *((volatile uint32_t*)DBSC_DBTR7) = 0x00080008;
  *((volatile uint32_t*)DBSC_DBTR8) = 0x00000020;
  *((volatile uint32_t*)DBSC_DBTR9) = 0x00000006;
  *((volatile uint32_t*)DBSC_DBTR10) = 0x0000000f;
  *((volatile uint32_t*)DBSC_DBTR11) = 0x00000011;
  *((volatile uint32_t*)DBSC_DBTR12) = 0x001D001D;
  *((volatile uint32_t*)DBSC_DBTR13) = 0x00480090;
  *((volatile uint32_t*)DBSC_DBTR14) = 0x00060006;
  *((volatile uint32_t*)DBSC_DBTR15) = 0x000c0006;
  *((volatile uint32_t*)DBSC_DBTR16) = (0x0d1f0c0e);
  *((volatile uint32_t*)DBSC_DBTR17) = 0x080a0004;
  *((volatile uint32_t*)DBSC_DBTR18) = 0x00000000;
  *((volatile uint32_t*)DBSC_DBTR19) = 0x00000000;
  *((volatile uint32_t*)DBSC_DBTR20) = 0x00b00096;
  *((volatile uint32_t*)DBSC_DBTR21) = (8) |((8)<<16);
  *((volatile uint32_t*)DBSC_DBTR22) = 0x03200018;
  *((volatile uint32_t*)DBSC_DBTR23) = 0x00000002;
  *((volatile uint32_t*)DBSC_DBTR24) = (0x06100509);
 }
 *((volatile uint32_t*)DBSC_DBTR16) = (0x0d1f0c0e);
 *((volatile uint32_t*)DBSC_DBRNK0) = 0x00000000;
 *((volatile uint32_t*)DBSC_DBRNK1) = 0x00000000;
 *((volatile uint32_t*)DBSC_DBRNK2) = ( ((((0x8888)>>0)&0xf)<<12) | ((((0x8888)>>0)&0xf)<<8) | ((((0x8888)>>0)&0xf)<<4) | (((0x8888)>>0)&0xf));
 *((volatile uint32_t*)DBSC_DBRNK3) = ( ((((0x8888)>>4)&0xf)<<12) | ((((0x8888)>>4)&0xf)<<8) | ((((0x8888)>>4)&0xf)<<4) | (((0x8888)>>4)&0xf));
 *((volatile uint32_t*)DBSC_DBRNK4) = ( ((((0x8888)>>8)&0xf)<<12) | ((((0x8888)>>8)&0xf)<<8) | ((((0x8888)>>8)&0xf)<<4) | (((0x8888)>>8)&0xf));
 *((volatile uint32_t*)DBSC_DBRNK5) = ( ((((0x8888)>>12)&0xf)<<12) | ((((0x8888)>>12)&0xf)<<8) | ((((0x8888)>>12)&0xf)<<4) | (((0x8888)>>12)&0xf));
 *((volatile uint32_t*)DBSC_DBRNK6) = 0x00000000;
 *((volatile uint32_t*)DBSC_DBADJ0) = 0x00000000;
 *((volatile uint32_t*)DBSC_DBADJ2) = 0x00000000;
 *((volatile uint32_t*)DBSC_DBSYSCONF1) = 0x00000002;
 *((volatile uint32_t*)DBSC_DBCAM0CNF1) = 0x00044218;
 *((volatile uint32_t*)DBSC_DBCAM0CNF2) = 0x00000284;
 if(freq>DDR3200_CLK || 1)
 {
  *((volatile uint32_t*)DBSC_SCFCTST0) = 0x18030d09;
  *((volatile uint32_t*)DBSC_SCFCTST1) = 0x090a080c;
 }
 else if(freq>DDR2800_CLK)
 {
  *((volatile uint32_t*)DBSC_SCFCTST0) = 0x18030d09;
  *((volatile uint32_t*)DBSC_SCFCTST1) = 0x090a080c;
 }
 else if(freq>DDR2400_CLK)
 {
  *((volatile uint32_t*)DBSC_SCFCTST0) = 0x180b1408;
  *((volatile uint32_t*)DBSC_SCFCTST1) = 0x0a0b080C;
 }
 else if(freq>DDR1600_CLK)
 {
  *((volatile uint32_t*)DBSC_SCFCTST0) = 0x180B1708;
  *((volatile uint32_t*)DBSC_SCFCTST1) = 0x0808070C;
 }
 else
 {
  *((volatile uint32_t*)DBSC_SCFCTST0) = 0x0c020905;
  *((volatile uint32_t*)DBSC_SCFCTST1) = 0x0508040C;
 }
 *((volatile uint32_t*)DBSC_DBMONCONF4 ) = 0x00700000 | (*((volatile uint32_t*)DBSC_DBMONCONF4 ));
 if((1)){
  *((volatile uint32_t*)DBSC_DBTR16) = *((volatile uint32_t*)DBSC_DBTR16)-0x0101;
  *((volatile uint32_t*)DBSC_DBTR24) = *((volatile uint32_t*)DBSC_DBTR24)-0x0001;
 }
}
static void mode_register_set(uint32_t freq,uintptr_t phychno,uint32_t mr13_val,uint32_t postflag)
{
 uint32_t DDR_MR1, DDR_MR2;
 SendDBCMD(0x0e040d00|(0x00100000 * phychno)|mr13_val);
 if(0 && freq>DDR3200_CLK){
  DDR_MR1 = 0xe4;
  DDR_MR2 = (2) ? 0x36 :
       0x37 ;
 } else if(1 || freq>DDR2400_CLK) {
  DDR_MR1 = 0xd4;
  DDR_MR2 = (2) ? 0x2d :
       0x2e ;
 } else if(freq>DDR1600_CLK) {
  DDR_MR1 = 0xc4;
  DDR_MR2 = 0x24;
 } else {
  DDR_MR1 = 0xa4;
  DDR_MR2 = 0x52;
 }
 SendDBCMD(0x0e040100|(0x00100000 * phychno)|DDR_MR1);
 SendDBCMD(0x0e040200|(0x00100000 * phychno)|DDR_MR2);
 if((postflag && ((2)==2)) || ((2)==1)) {
  SendDBCMD(0x0e040300|(0x00100000 * phychno)|0x31|0xC0);
 } else {
  SendDBCMD(0x0e040300|(0x00100000 * phychno)|0x31);
 }
 if((postflag == 1) && (0)){
 } else {
  SendDBCMD(0x0e040b00|(0x00100000 * phychno)|(0x36));
  SendDBCMD(0x0e040c00|(0x00100000 * phychno)|(0x11));
  SendDBCMD(0x0e040e00|(0x00100000 * phychno)|(0x11));
 }
 SendDBCMD(0x0e041600|(0x00100000 * phychno)|0x16);
}
static inline uint32_t PLL3_FREQ_01()
{
 uint32_t dataL;
 uint32_t count;
 count = 100000;
 do {
  dataL = (*((volatile uint32_t*)DBSC_DBPDSTAT_0)) &
   (*((volatile uint32_t*)DBSC_DBPDSTAT_1)) & 0x00000001;
  count = count -1;
 } while( (dataL!=0x00000001) & (count !=0));
 if(count == 0) {
  NOTICE("PLL3_FREQ_01:Time out[1]");
  return (1);
 }
 *((volatile uint32_t*)CPG_CPGWPR) = ~(0x001f001f);
 *((volatile uint32_t*)CPG_PLLECR) = 0x001f001f ;
 PLL3_MODE=1;
 dsb_sev();
 do {
  dataL=*((volatile uint32_t*)CPG_PLLECR);
 } while( (dataL&0x1f00)!=0x1f00 );
 SoftDelay_ddr(1000);
 *((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x01000010;
 *((volatile uint32_t*)DBSC_DBDFICNT_1) = 0x01000010;
 dsb_sev();
 *((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0)) = 0x0CF20000;
 *((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_1)) = 0x0CF20000;
 dsb_sev();
 count = 1000000;
 do {
  dataL = ((*((volatile uint32_t*)DBSC_DBPDSTAT_0)) |
   (*((volatile uint32_t*)DBSC_DBPDSTAT_1))) & 0x00000001;
  count = count -1;
 } while( (dataL==0x00000001) & (count !=0));
 *((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0)) = 0x00000000;
 *((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_1)) = 0x00000000;
 dsb_sev();
 if(count == 0) {
  NOTICE("PLL3_FREQ_01:Time out[2]");
  return (1);
 }
 return (0);
}
static inline uint32_t PLL3_FREQ_10()
{
 uint32_t dataL;
 uint32_t count;
 count = 10000000;
 do {
  dataL = (*((volatile uint32_t*)DBSC_DBPDSTAT_0)) &
   (*((volatile uint32_t*)DBSC_DBPDSTAT_1)) & 0x00000001;
  count = count -1;
 } while( (dataL!=0x00000001) & (count !=0));
 if(count == 0) {
  NOTICE("PLL3_FREQ_10:Time out[1]");
  return (1);
 }
 *((volatile uint32_t*)CPG_CPGWPR) = ~(0x001f0017);
 *((volatile uint32_t*)CPG_PLLECR) = 0x001f0017 ;
 PLL3_MODE=0;
 dsb_sev();
 *((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x00000010;
 *((volatile uint32_t*)DBSC_DBDFICNT_1) = 0x00000010;
 dsb_sev();
 *((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0)) = 0x0CF20000;
 *((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_1)) = 0x0CF20000;
 dsb_sev();
 count = 100000;
 do {
  dataL = ((*((volatile uint32_t*)DBSC_DBPDSTAT_0)) |
   (*((volatile uint32_t*)DBSC_DBPDSTAT_1))) & 0x00000001;
  count = count -1;
 } while( (dataL==0x00000001) & (count !=0));
 *((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0)) = 0x00000000;
 *((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_1)) = 0x00000000;
 if(count == 0) {
  NOTICE("PLL3_FREQ_10:Time out[2]");
  return (1);
 }
 return (0);
}
static inline uint32_t PLL3_FREQ(uint32_t freq_flag_b,uint32_t freq_flag_a)
{
 uint32_t dataL;
 uint32_t count;
 count = 100000;
 do {
  dataL = (*((volatile uint32_t*)DBSC_DBPDSTAT_0)) &
   (*((volatile uint32_t*)DBSC_DBPDSTAT_1)) & 0x00000001;
  count = count -1;
 } while( (dataL!=0x00000001) & (count !=0));
 if(count == 0) {
  return (1);
 }
 if(freq_flag_a == 0){
  *((volatile uint32_t*)CPG_CPGWPR) = ~(0x001f0017);
  *((volatile uint32_t*)CPG_PLLECR) = 0x001f0017 ;
  PLL3_MODE=0;
  dsb_sev();
 } else if((freq_flag_b == 0)&(freq_flag_a == 1)){
  *((volatile uint32_t*)CPG_CPGWPR) = ~(0x001f001f);
  *((volatile uint32_t*)CPG_PLLECR) = 0x001f001f ;
  PLL3_MODE=1;
  dsb_sev();
  do {
   dataL=*((volatile uint32_t*)CPG_PLLECR);
  } while( (dataL&0x1f00)!=0x1f00 );
 }
 dsb_sev();
 *((volatile uint32_t*)DBSC_DBDFICNT_0) = freq_flag_a << 24| 0x10;
 *((volatile uint32_t*)DBSC_DBDFICNT_1) = freq_flag_a << 24| 0x10;
 dsb_sev();
 *((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0)) = 0x0CF20000;
 *((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_1)) = 0x0CF20000;
 count = 100000;
 do {
  dataL = ((*((volatile uint32_t*)DBSC_DBPDSTAT_0)) |
   (*((volatile uint32_t*)DBSC_DBPDSTAT_1))) & 0x00000001;
  count = count -1;
 } while( (dataL==0x00000001) & (count !=0));
 *((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0)) = 0x00000000;
 *((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_1)) = 0x00000000;
 if(count == 0) {
  NOTICE("Time out[2]");
  return (1);
 }
 return (0);
}
static uint32_t pvt_pi_training_go_all(uint32_t freq)
{
 uint32_t flag;
 uint32_t dataL;
 uint32_t retry;
 uintptr_t ch;
 uint32_t mst_ch;
 uint32_t cur_frq;
 uint32_t complete;
 uint32_t frqchg_req;
 *((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x01000010;
 *((volatile uint32_t*)DBSC_DBDFICNT_1) = 0x01000010;
 dsb_sev();
 for(ch=0;ch<DRAM_CH_CNT;ch++){
  REG_DDRPHY_WRITE(ch, 0x0200, 0x20410B01);
  *(volatile uint32_t*)DBSC_DBPDRGA(ch) = 0x2A9;
 }
 *((volatile uint32_t*)DBSC_DBDFIPMSTRCNF) = 0x00000001;
 dsb_sev();
 mst_ch=0;
 flag=0;
 complete=0;
 cur_frq=0;
 retry=4096;
 do {
  frqchg_req = ((*((volatile uint32_t*)DBSC_DBPDSTAT(mst_ch))>>0)&0x1);
  if(frqchg_req){
   if(cur_frq){
    flag=PLL3_FREQ_10();
    cur_frq=0;
   } else {
    flag=PLL3_FREQ_01();
    cur_frq=1;
   }
   retry=4096;
   if(flag)break;
  } else {
   if(cur_frq){
    for(ch=0;ch<DRAM_CH_CNT;ch++){
     if(complete & (1<<ch))continue;
     if((*(volatile uint32_t*)DBSC_DBPDRGD(ch)) & (1<<8)){
      complete |= (1<<ch);
     }
    }
    if(complete==DDR_PHYVALID)break;
   }
  }
 } while(--retry);
 if(flag){
  NOTICE("FREQCHG ERROR");
 }
 if(retry==0){
  NOTICE("PI TIMEOUT");
 }
 for(ch=0;ch<DRAM_CH_CNT;ch++)
 {
  dataL = REG_DDRPHY_READ ( ch, 0x02A9) >> 8;
  REG_DDRPHY_WRITE( ch, 0x02AA, dataL);
 }
 return complete;
}
static void ConfigTBL(void){
 _TBLCOPY(_cnf_DDR_PHY_SLICE_REGSET, DDR_PHY_SLICE_REGSET);
 _TBLCOPY(_cnf_DDR_PHY_ADR_V_REGSET, DDR_PHY_ADR_V_REGSET);
 _TBLCOPY(_cnf_DDR_PHY_ADR_I_REGSET, DDR_PHY_ADR_I_REGSET);
 _TBLCOPY(_cnf_DDR_PHY_ADR_G_REGSET, DDR_PHY_ADR_G_REGSET);
 _TBLCOPY(_cnf_DDR_PI_REGSET, DDR_PI_REGSET);
 if((1)){
  REG_DDRTBL_MOD(_cnf_DDR_PHY_SLICE_REGSET,0x0805,0x00FFFFFF,(0x006E6E0E));
  REG_DDRTBL_MOD(_cnf_DDR_PHY_SLICE_REGSET,0x0806,0x00FFFFFF,(0x006E6E0E));
 }
 if((4)==4){
  REG_DDRTBL_MOD(_cnf_DDR_PHY_SLICE_REGSET,0x084D, 0x03FF0000, 0x00000200<<16);
 }
 if((1)){
  REG_DDRTBL_MOD(_cnf_DDR_PHY_SLICE_REGSET,0x084D, 0x00000300, 0x100);
  REG_DDRTBL_WRITE(_cnf_DDR_PI_REGSET,0x0255 ,REG_DDRTBL_READ(_cnf_DDR_PI_REGSET,0x0255)-0x010101);
  REG_DDRTBL_WRITE(_cnf_DDR_PI_REGSET,0x022B ,REG_DDRTBL_READ(_cnf_DDR_PI_REGSET,0x022B)-0x01000001);
  REG_DDRTBL_WRITE(_cnf_DDR_PI_REGSET,0x022C ,REG_DDRTBL_READ(_cnf_DDR_PI_REGSET,0x022C)-0x00010000);
 }
 if((3) != (3)){
  REG_DDRTBL_MOD(_cnf_DDR_PHY_ADR_V_REGSET,0x0A10, 0x00000003, (3)<<0);
 }
}
static uint32_t InitDDR_start(uint32_t freq)
{
 uint32_t i=0;
 uint32_t k=0;
 uint32_t dataL=0;
 uint32_t phytrainingok=0x0;
 uint32_t retry;
 uintptr_t ch,slice;
 uint32_t ddr_csn;
 uint32_t cs;
 *((volatile uint32_t*)DBSC_DBPDCNT0_0) = 0x01;
 *((volatile uint32_t*)DBSC_DBPDCNT0_1) = 0x01;
 dsb_sev();
 pvt_dbsc_regset(freq);
 *((volatile uint32_t*)DBSC_DBPDCNT0_0) = 0x00;
 *((volatile uint32_t*)DBSC_DBPDCNT0_1) = 0x00;
 *((volatile uint32_t*)DBSC_DBPDLK_0) = 0x0000A55A;
 *((volatile uint32_t*)DBSC_DBPDLK_1) = 0x0000A55A;
 for (ch=0;ch<DRAM_CH_CNT;ch++)
 {
  REG_DDRPHY_WRITE(ch,0x0B80 , 0x00000001);
  for(slice=0;slice<3;slice++)
  {
   REG_DDRPHY_WRITE(ch,0x0807 +0x80*slice, 0x00010F00);
  }
 }
 ConfigTBL();
 for( slice=0; slice<4; slice++){
  for( i=0; i<128; i++ ) {
   if(DDR_PHY_SLICE_REGSET[i][0]== 0xFFFFFFFF)break;
   REG_DDRPHY_WRITE_A(_cnf_DDR_PHY_SLICE_REGSET[i][0]+0x80*slice,_cnf_DDR_PHY_SLICE_REGSET[i][1]);
  }
 }
 for( i=0; i<128; i++ ) {
  if(DDR_PHY_ADR_V_REGSET[i][0]== 0xFFFFFFFF)break;
  REG_DDRPHY_WRITE_A(_cnf_DDR_PHY_ADR_V_REGSET[i][0] ,_cnf_DDR_PHY_ADR_V_REGSET[i][1]);
 }
 for( slice=0; slice<2; slice++){
  for( i=0; i<128; i++ ) {
   if(DDR_PHY_ADR_I_REGSET[i][0]== 0xFFFFFFFF)break;
   REG_DDRPHY_WRITE_A(_cnf_DDR_PHY_ADR_I_REGSET[i][0]+0x80*slice,_cnf_DDR_PHY_ADR_I_REGSET[i][1]);
  }
 }
 for( i=0; i<128; i++ ) {
  if(DDR_PHY_ADR_G_REGSET[i][0]== 0xFFFFFFFF)break;
  REG_DDRPHY_WRITE_A(_cnf_DDR_PHY_ADR_G_REGSET[i][0] ,_cnf_DDR_PHY_ADR_G_REGSET[i][1]);
 }
 for( i=0; i<255; i++ )
 {
  if(_cnf_DDR_PI_REGSET[i][0]== 0xFFFFFFFF)break;
  REG_DDRPHY_WRITE_A(_cnf_DDR_PI_REGSET[i][0],_cnf_DDR_PI_REGSET[i][1]);
 }
 if(_def_BOARDTYPE>=BOARDNUM){
  NOTICE("DDR:Unknown Board");
 } else {
  for(ch=0;ch<DRAM_CH_CNT;ch++){
   for(slice=0;slice<4;slice++){
    REG_DDRPHY_WRITE(ch,0x800+slice*0x80,BOARDCNF_DQ_SWAP[_def_BOARDTYPE][ch][slice]);
    REG_DDRPHY_WRITE(ch,0x801+slice*0x80, (REG_DDRTBL_READ(_cnf_DDR_PHY_SLICE_REGSET,0x801) & 0xffffff00) | BOARDCNF_DM_SWAP[_def_BOARDTYPE][ch][slice]);
    if( (BOARDCNF_DQS_SWAP[_def_BOARDTYPE][ch] >> (8*slice)) %2 ) {
     REG_DDRPHY_WRITE(ch,0x820+slice*0x80,REG_DDRTBL_READ(_cnf_DDR_PHY_SLICE_REGSET,0x820) & 0xff00ffff);
    } else {
     REG_DDRPHY_WRITE(ch,0x820+slice*0x80,(REG_DDRTBL_READ(_cnf_DDR_PHY_SLICE_REGSET,0x820) & 0xff00ffff) | 0x00010000);
    }
   }
   REG_DDRPHY_WRITE(ch,0xa0c,(BOARDCNF_CA_SWAP[_def_BOARDTYPE][ch] | 0x00888888));
   REG_DDRPHY_WRITE(ch,0xa0d,0x00000000);
   REG_DDRPHY_WRITE(ch,0xa0e,(BOARDCNF_CA_SWAP[_def_BOARDTYPE][ch] | 0x00888888));
   REG_DDRPHY_WRITE(ch,0xa0f,((1)<<24) | 0x00000000);
   dataL=0;
   k=BOARDCNF_CA_SWAP[_def_BOARDTYPE][ch];
   for(i=0;i<6;i++){
    dataL|= ((k&0xf)<<(i*5));
    k=k>>4;
   }
   REG_DDRPHY_WRITE(ch,0xa1c,dataL);
   dataL=0;
   k=BOARDCNF_DQS_SWAP[_def_BOARDTYPE][ch];
   for(i=0;i<4;i++){
    dataL|= ((k&0x3)<<(i*2));
    k=k>>4;
   }
   REG_DDRPHY_WRITE(ch,0xbbc,(REG_DDRTBL_READ(_cnf_DDR_PHY_ADR_G_REGSET,0xbbc) & 0xff00ffff) | (dataL<<16));
  }
  for(ch=0;ch<DRAM_CH_CNT;ch++){
   for( i=0; i<0; i++ )
   {
    if(DDR_BOARD_SWAP[_def_BOARDTYPE][ch][i][0]== 0xFFFFFFFF) break;
     REG_DDRPHY_WRITE(ch,
     DDR_BOARD_SWAP[_def_BOARDTYPE][ch][i][0],
     DDR_BOARD_SWAP[_def_BOARDTYPE][ch][i][1]
    );
   }
  }
 }
 for(ch=0;ch<DRAM_CH_CNT;ch++)
 {
  for(slice=0;slice<4;slice++)
  {
   REG_DDRPHY_WRITE(ch,0x0853+0x80*slice,REG_DDRPHY_READ(ch,0x0853+0x80*slice) | (1<<16));
   *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x040*ch)) = 0x02A9;
  }
 }
 dsb_sev();
 ddrphy_regif_wa();
 *((volatile uint32_t*)CPG_CPGWPR) = ~(0x001f0017);
 *((volatile uint32_t*)CPG_PLLECR) = 0x001f0017 ;
 PLL3_MODE=0;
 dsb_sev();
 *((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x00000F10;
 *((volatile uint32_t*)DBSC_DBDFICNT_1) = 0x00000F10;
 dsb_sev();
 *((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x00000F10;
 *((volatile uint32_t*)DBSC_DBDFICNT_1) = 0x00000F10;
 dsb_sev();
 *((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x00000F11;
 *((volatile uint32_t*)DBSC_DBDFICNT_1) = 0x00000F11;
 dsb_sev();
 *((volatile uint32_t*)DBSC_DBPDCNT_0) = 0x0000CF01;
 *((volatile uint32_t*)DBSC_DBPDCNT_1) = 0x0000CF01;
 dsb_sev();
 for(phytrainingok=0,k=0,ch=0;ch<DRAM_CH_CNT;ch++)
 {
  if(!(DDR_PHYVALID&(1<<ch)))continue;
  {
   retry=0;
   while(retry<4096*16)
   {
    dataL = *((volatile uint32_t*)(DBSC_INITCOMP_0+ch*0x40));
    if((dataL & 0x00000001) == 0x1) break;
    retry++;
   }
   if(retry<4096*16)
   {
    phytrainingok|=(1<<ch);
   }
   else
   {
    k|=(1<<ch);
   }
  }
 }
 *(volatile uint32_t*)(DBSC_DBPDRGA_0) = 0x02A9;
 *(volatile uint32_t*)(DBSC_DBPDRGA_1) = 0x02A9;
 *((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x00000010;
 *((volatile uint32_t*)DBSC_DBDFICNT_1) = 0x00000010;
 dsb_sev();
 if((phytrainingok & DDR_PHYVALID) != DDR_PHYVALID){
  return (0xff);
 }
if((0) || !(0)){
 if((0)){
  dataL = (0x0001554F);
 } else {
  dataL = (0x0001554F);
 }
 REG_DDRPHY_WRITE_A(0x0BA2 , dataL|0x00020000 );
 REG_DDRPHY_WRITE_A(0x0BA3 , dataL );
 REG_DDRPHY_WRITE_A(0x0BA4 , dataL );
 REG_DDRPHY_WRITE_A(0x0BA5 , dataL );
 REG_DDRPHY_WRITE_A(0x0BA6 , dataL );
 REG_DDRPHY_WRITE_A(0x0BA8 , dataL );
 REG_DDRPHY_WRITE_A(0x0BAA , dataL );
 REG_DDRPHY_WRITE_A(0x0BAC , dataL );
 *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x040*0)) = 0x02A9;
 *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x040*1)) = 0x02A9;
}
if((1)){
 for(slice=0;slice<4;slice+=1){
  REG_DDRPHY_WRITE_A(0x0838 +slice*0x80 ,0);
  REG_DDRPHY_WRITE_A(0x0833+0x80*slice,0x000F0101);
 }
 for(ch=0;ch<DRAM_CH_CNT;ch+=1) {
  for(slice=0;slice<4;slice+=1){
   retry=0;
   while((REG_DDRPHY_READ(ch,0x0838 +slice*0x80)&0x3ff00000)==0){
    retry++;
    if(retry>4096){
     phytrainingok &= ~(1<<ch);
     break;
    }
   }
  }
 }
 for(slice=0;slice<4;slice+=1){
  REG_DDRPHY_WRITE_A(0x0833+0x80*slice,0x000F0000);
 }
} else {
 dataL = ((0x20 <<6)| (0x20)) * 0x10001;
 for(slice=0;slice<4;slice+=1){
  REG_DDRPHY_WRITE_A(0x0834 +slice*0x80 , dataL );
  REG_DDRPHY_WRITE_A(0x0835 +slice*0x80 , dataL );
  REG_DDRPHY_WRITE_A(0x0836 +slice*0x80 , dataL );
  REG_DDRPHY_WRITE_A(0x0837 +slice*0x80 , dataL );
  REG_DDRPHY_WRITE_A(0x0838 +slice*0x80 , dataL );
  REG_DDRPHY_WRITE_A(0x0839 +slice*0x80 , dataL );
 }
}
 for(slice=0;slice<4;slice++)
 {
  REG_DDRPHY_WRITE_A(0x0853+0x80*slice,(REG_DDRPHY_READ(0,0x0853+0x80*slice)) & (~(1<<16)));
 }
 *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x040*0)) = 0x02A9;
 *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x040*1)) = 0x02A9;
 for(ch=0;ch<DRAM_CH_CNT;ch++){
  dataL=REG_DDRPHY_READ(ch,0x200);
  if(dataL!=0x20410B00){
   phytrainingok &= ~(1<<ch);
  }
 }
 if((phytrainingok & DDR_PHYVALID) != DDR_PHYVALID){
  return (0xfe);
 }
 change_lpddr4_en(0xf,0);
 ch=0x0f;
 SendDBCMD(0x01040001|(0x00100000 * ch));
 SendDBCMD(0x08040000|(0x00100000 * ch));
 SendDBCMD(0x08040001|(0x00100000 * ch));
 SendDBCMD(0x0e041600|(0x00100000 * ch)|0x16);
 SendDBCMD(0x0d04004F|(0x00100000 * ch));
 SendDBCMD(0x0d040051|(0x00100000 * ch));
 if(DBMEMCONF_DENS(0,0)==0xfe){
  DDR_CHK();
 } else {
  for(ch=0;ch<DRAM_CH_CNT;ch++){
   for(cs=0;cs<2;cs++){
    ddr_density[ch][cs]=DBMEMCONF_DENS(ch,cs);
    ddr_density[ch][cs+2]=DBMEMCONF_DENS(ch,cs);
   }
  }
 }
 mode_register_set(freq, ch=0xf, 0x40,0);
 mode_register_set(freq, ch=0xf, 0x00,0);
 change_lpddr4_en(0xf,1);
 for(ch=0;ch<DRAM_CH_CNT;ch++){
  if(ddr_density[ch][1]==0xff){
   REG_DDRPHY_WRITE(ch,0x229,REG_DDRPHY_READ(ch,0x229) & ~0x0a000000);
  }
 }
 for (ch=0;ch<DRAM_CH_CNT;ch++)
 {
  for(slice=0;slice<4;slice++)
  {
   REG_DDRPHY_WRITE(ch,0x0807 +0x80*slice, 0x00000F00);
  }
 }
 phytrainingok = pvt_pi_training_go_all(freq);
 if(phytrainingok!=3)return(0xe0|phytrainingok);
if((4)==4){
 for(ch=0;ch<DRAM_CH_CNT;ch+=1)
 {
  for(slice=0;slice<4;slice+=1)
  {
   for(cs=0;cs<2;cs++){
   uint32_t dly;
   REG_DDRPHY_WRITE(ch,0x0807 +slice*0x80,(REG_DDRPHY_READ(ch,0x0807 +slice*0x80) & ~(1<<24)) | (cs<<24));
   dly =(REG_DDRPHY_READ(ch,0x083E +slice*0x80))>>16;
   if(dly>(0x180))
    REG_DDRPHY_WRITE(ch,0x084D +slice*0x80,REG_DDRPHY_READ(ch,0x084D +slice*0x80)- 0x0100);
   }
  }
 }
}
 for(ch=0;ch<DRAM_CH_CNT;ch+=1)
 {
  REG_DDRPHY_WRITE(ch,0x0234,0x01000000);
  REG_DDRPHY_WRITE(ch,0x0234,0x00000000);
  for(slice=0;slice<4;slice+=1){
   REG_DDRPHY_WRITE(ch,0x0807 +slice*0x80,REG_DDRPHY_READ(ch,0x0807 +slice*0x80)&(~(1<<24)));
   REG_DDRPHY_WRITE(ch,0x081A +slice*0x80,REG_DDRPHY_READ(ch,0x081A +slice*0x80)|((1<<24)));
   REG_DDRPHY_WRITE(ch,0x0807 +slice*0x80,REG_DDRPHY_READ(ch,0x0807 +slice*0x80)|( 1<<24));
   REG_DDRPHY_WRITE(ch,0x081A +slice*0x80,REG_DDRPHY_READ(ch,0x081A +slice*0x80)|((1<<24)));
  }
 }
 for(ddr_csn=0;ddr_csn<4;ddr_csn+=1)
 {
  uint32_t ch_have_this_cs;
  ch_have_this_cs=0;
  for(ch=0;ch<DRAM_CH_CNT;ch+=1)
  {
   if(ddr_density[ch][ddr_csn%2]!=0xff){
    ch_have_this_cs|=(1<<ch);
   }
  }
  for(ch=0;ch<DRAM_CH_CNT;ch+=1)
  {
   if(ch_have_this_cs & (1<<ch)){
    REG_DDRPHY_WRITE(ch,0x0274,0x34000100|(ddr_csn<<16));
   }
  }
  for(ch=0;ch<DRAM_CH_CNT;ch++){
   *((volatile uint32_t*)(DBSC_DBPDRGA_0+0x40*ch))=0x231;
  }
  i=ch_have_this_cs;
  dsb_sev();
  retry=0x100000;
  do {
   for(ch=0;ch<DRAM_CH_CNT;ch++){
    if(i&(1<<ch)){
     dataL=*((volatile uint32_t*)(DBSC_DBPDRGD_0+0x40*ch));
     if(dataL&0x01000000) i&=~(1<<ch);
    }
   }
   retry--;
  } while ((i)!=0x0 && retry>0);
  if(retry==0){
  }
  else
  {
  }
  dsb_sev();
  for(ch=0;ch<DRAM_CH_CNT;ch++){
   REG_DDRPHY_WRITE(ch,0x0234,0x01000000);
   REG_DDRPHY_WRITE(ch,0x0234,0x00000000);
  }
  dsb_sev();
 }
 for(ch=0;ch<DRAM_CH_CNT;ch++){
  REG_DDRPHY_WRITE(ch,0x0B82,REG_DDRPHY_READ(ch,0x0B82) | (1<<16));
  REG_DDRPHY_WRITE(ch,0x0BB6,REG_DDRPHY_READ(ch,0x0BB6) | (1<<16));
  REG_DDRPHY_WRITE(ch,0x0B82,REG_DDRPHY_READ(ch,0x0B82) &~(1<<16));
  REG_DDRPHY_WRITE(ch,0x0BB6,REG_DDRPHY_READ(ch,0x0BB6) &~(1<<16));
 }
 for(ch=0;ch<DRAM_CH_CNT;ch++){
  for(cs=0;cs<2;cs++){
   if(ddr_density[ch][cs]==0xff) {
    *((volatile uint32_t*)DBSC_DBMEMCONF(ch,cs))=0;
   } else {
    *((volatile uint32_t*)DBSC_DBMEMCONF(ch,cs))=DBMEMCONF_REGD(ddr_density[ch][cs]);
   }
  }
  *((volatile uint32_t*)DBSC_DBMEMCONF(ch,2)) = 0x00000000;
  *((volatile uint32_t*)DBSC_DBMEMCONF(ch,3)) = 0x00000000;
 }
 *((volatile uint32_t*)DBSC_DBBUS0CNF1) = 0x00000000;
 *((volatile uint32_t*)DBSC_DBBUS0CNF0) = 0x00010000;
 for(ch=0;ch<DRAM_CH_CNT;ch+=1)
 {
  if((DDR_PHYVALID&phytrainingok& 0x0f)==DDR_PHYVALID)
  {
   ch=0x0f;
  }
  else
  {
   if( (DDR_PHYVALID&phytrainingok&(1<<ch))==0 )
   {
    continue;
   }
  }
  mode_register_set(freq, ch, 0x40,1);
  mode_register_set(freq, ch, 0x00,1);
 }
 if((2) == 2){
  for(ch=0;ch<DRAM_CH_CNT;ch++){
   for(slice=0;slice<4;slice++){
    REG_DDRPHY_WRITE(ch,0x0807+0x80*slice,(REG_DDRPHY_READ(ch,0x0807+0x80*slice) | 0x1 ));
   }
  }
 }
 if((2) == 1 || (2) == 2){
  *((volatile uint32_t*)DBSC_DBDBICNT) = 0x00000003;
 }
 if(_def_REFCYCLE==0){
  dataL = _def_REFPERIOD*DDRXXXX_RECALC(DDR_CLK,BRD_CLKx3,3)/2/1000;
  *((volatile uint32_t*)DBSC_DBRFCNF1) = 0x00080000 | (dataL&0xffff);
 } else {
       *((volatile uint32_t*)DBSC_DBRFCNF1) = 0x00080000 | (_def_REFCYCLE&0xffff);
 }
 *((volatile uint32_t*)DBSC_DBRFCNF2) = 0x00010000|DBSC_REFINTS;
 *((volatile uint32_t*)DBSC_DBRFEN) = 0x00000001;
 *((volatile uint32_t*)DBSC_DBACEN) = 0x00000001;
 return phytrainingok;
}
void InitDram_m3_es10(void)
{
 uint32_t md;
 uint32_t dataL;
 InitResetCount=0;
 PadCalFailCount=0;
 TrainingFailCount=0;
 if(_def_BRD_CLKx3==0) {
  md = ((*((volatile uint32_t*)RST_MODEMR))>>13)&0x3;
  switch(md){
   case 0x0 : BRD_CLKx3 = 50; break;
   case 0x1 : BRD_CLKx3 = 60; break;
   case 0x2 : BRD_CLKx3 = 75; break;
   case 0x3 : BRD_CLKx3 =100; break;
  }
 } else {
  BRD_CLKx3 = _def_BRD_CLKx3;
 }
 DDR1600_CLK = DDRXXXX_CALC(1600,BRD_CLKx3,3);
 DDR2400_CLK = DDRXXXX_CALC(2400,BRD_CLKx3,3);
 DDR2800_CLK = DDRXXXX_CALC(2800,BRD_CLKx3,3);
 DDR3200_CLK = DDRXXXX_CALC(3200,BRD_CLKx3,3);
 if(_def_DDR_CLK==0) {
  md = ((*((volatile uint32_t*)RST_MODEMR))>>17) & 0x5;
  md = (md|(md>>1))&0x3;
  switch(md){
   case 0x0 : DDR_CLK = DDR3200_CLK; break;
   case 0x1 : DDR_CLK = DDR2800_CLK; break;
   case 0x2 : DDR_CLK = DDR2400_CLK; break;
   case 0x3 : DDR_CLK = DDR1600_CLK; break;
  }
 } else {
  DDR_CLK = _def_DDR_CLK;
 }
 NOTICE("BL2: DDR%d(%s)\n", DDRXXXX_RECALC(DDR_CLK,BRD_CLKx3,3), RCAR_DDR_VERSION);
 *((volatile uint32_t*)CPG_CPGWPR) = ~(DDR_CLK<<24);
 *((volatile uint32_t*)CPG_PLL3CR) = DDR_CLK<<24 ;
 PLL3_MODE=1;
 dsb_sev();
 do {
  dataL=*((volatile uint32_t*)CPG_PLLECR);
 } while( (dataL&0x1f00)!=0x1f00 );
 while (1){
  dataL=InitDDR_start(DDR_CLK);
  if(dataL==DDR_PHYVALID)break;
  if(dataL==0xFF){
   InitResetCount++;
  } else if(dataL==0xFE){
   PadCalFailCount++;
  } else {
   TrainingFailCount++;
  }
  if((InitResetCount+PadCalFailCount)==0x0f || TrainingFailCount==0x0f){
   NOTICE("Training Fail");
   break;
  }
  *((volatile uint32_t*)CPG_CPGWPR) = ~(0x001f001f);
  *((volatile uint32_t*)CPG_PLLECR) = 0x001f001f ;
  PLL3_MODE=1;
  dsb_sev();
  do {
   dataL=*((volatile uint32_t*)CPG_PLLECR);
  } while( (dataL&0x1f00)!=0x1f00 );
  SoftDelay_ddr(1000);
  ResetDram();
  ResetDram();
  dsb_sev();
 };
}
