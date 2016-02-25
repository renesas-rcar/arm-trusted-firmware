
#include <stdint.h>		//for uint32_t
#include <string.h>		//for uint32_t
#include <stdio.h>		//for uint32_t
#include "boot_init_dram_m3_es10.h"

#define	RCAR_DDR_VERSION	"rev.0.11"

///////////////////////////////////////////////////////////
// Board define

#define	BOARD_SALVATOR_X
//#define	BOARD_KRIEK


///////////////////////////////////////////////////////////
#include "init_dram_tbl_m3_es10.h"
//#include "bit.h"
//#include "reg_rcarh3.h"

///////////////////////////////////////////////////////////
/* add start */
#include <debug.h>
#define	RST_BASE				(0xE6160000U)
#define	RST_MODEMR				(RST_BASE + 0x0060U)
#define	CPG_BASE				(0xE6150000U)
#define	CPG_PLLECR				(CPG_BASE + 0x00D0U)
#define	CPG_CPGWPR				(CPG_BASE + 0x900U)
#define	CPG_PLL3CR				(CPG_BASE + 0x0DCU)
#define	CPG_SRCR4				(CPG_BASE + 0x0BCU)
#define	CPG_SRSTCLR4				(CPG_BASE + 0x950U)
///////////////////////////////////////////////////////////

#define	DEBUG_WDQSWA7

//OK?
//   debug_term
//   non debug_term_over_pre
//   non debug_cadelay
//   kriek + DDR_2RANK,DDR_RANK0
//   debug_term_over

#define	debug_cadelay
#define	debug_term_over
#define	debug_term_over_pre
//#define debug_pad_cal
//#define debug_vref
//#define debug_pad_cal_freq


///////////////////////////////////////////////////////////
//#define	DDR_DEBUG_ES1P1				// ES1.1 debug code
//#define	DDR_DEBUG_WL
//#define	DDR_DEBUG_REG_DDRPHY
//#define	DDR_DEBUG_MEMCHK
//#define	DDR_DEBUG_ODTMODE
//#define	DDR_DEBUG_VREF
//#define	DDR_DEBUG_FREQ		0
//#define	DDR_DEBUG_ODT2
//#define	DDR_DEBUG_PST_AMBLE	1
//#define	DDR_DEBUG_REF
//#define	DDR_DEBUG_DBI		0x02

///////////////////////////////////////////////////////////
// Kriek board settings
#ifdef	BOARD_KRIEK
//#define DDR_REF_PR
//#define DDR_PAD_NORMAL
#define DDR_2RANK
//#define DDR_RANK0
//#define DDR_RANK0
//#define DDR_RANK1
//#define DDR_KRIEK_MEMSWAP
#endif 

#ifdef	BOARD_SALVATOR_X
#define DDR_2RANK
//#define DDR_RANK0
#endif 

///////////////////////////////////////////////////////////
#define DRAM_CH_CNT		0x02
#define	DDR_PHYVALID	0x03
#define	DDR_PHYMRW		0x03
#define	DDR_PAD_CAL_IEWA



//#define	DDR_WDQLVL_CHAB_SWAP_WA
//#define	DDR_ODT_SET			0x7170

#ifdef DDR_2RANK			// 2Rank
#define	DDR_CSMAP		0xF
#define	DDR_CSMAP_WDQLVL	0xF
#define	DDR_CSMAP_RDLVL		0xF
#define	DDR_CSMAP_GTLVL		0xF
#define	DDR_CSMAP_WRLVL		0xF
#define	DDR_CSMAP_CALVL		0xF
#endif
#ifdef DDR_RANK0			// Rank0
#define	DDR_CSMAP			0x5
#define	DDR_CSMAP_WDQLVL	0x5
#define	DDR_CSMAP_RDLVL		0x1
#define	DDR_CSMAP_GTLVL		0x1
#define	DDR_CSMAP_WRLVL		0x1
#define	DDR_CSMAP_CALVL		0x1
#endif
#ifdef DDR_RANK1			// Rank1
#define	DDR_CSMAP			0xA
#define	DDR_CSMAP_WDQLVL	0xA
#define	DDR_CSMAP_RDLVL		0xF
#define	DDR_CSMAP_GTLVL		0xF
#define	DDR_CSMAP_WRLVL		0xF
#define	DDR_CSMAP_CALVL		0xF
#endif


//#define	DDR_LVLEN				0x1e


#ifdef	DDR_DEBUG_DBI
#define	DDR_MR3					0xF1	//0xF1 W/DBI , 0x31 noDBI
#else
#define	DDR_MR3					0x31
#endif

#define	DDR_MR11				0x36	// ca.odt=80 ohm / dq.odt=40 ohm
#ifdef DDR_2RANK			// 2Rank
#define	DDR_MR22				0x16
#else
#define	DDR_MR22				0x06
#endif

#define DDR1600_CLK				0x2F		// 1600
#define DDR2400_CLK				0x47		// 2400
#define DDR2800_CLK				0x53		// 2800
#define DDR3200_CLK				0x5f		// 3200

#ifdef DDR_DEBUG_FREQ
#define DDR1433_CLK				0x2A		// 1433(1433.3333)
#define DDR1466_CLK				0x2B		// 1466(1466.6652)
#define DDR1500_CLK				0x2C		// 1500
#define DDR1533_CLK				0x2D		// 1533(1533.3333)
#define DDR2900_CLK				0x56		// 2900
#define DDR3000_CLK				0x59		// 3000
#define DDR3100_CLK				0x5C		// 3100
#define DDR3133_CLK				0x5D		// 3133
#define DDR3166_CLK				0x5E		// 3166
#endif

#define DDR_CACS_CAPTURE_CNT	0x02
#define DDR_CACS_RESP_WAIT_CNT	0x00
#define DDR_CACS_CCCP			0x2816
#define DDR_CACS_START			0x280		// 0x320
#define DDR_CACS_QTR			0x80		// 0x40
#define DDR_CACS_STEP			0x03

#define	DBSC_REFINTS			0x1;		// 0: Average interval is REFINT. / 1: Average interval is 1/2 REFINT.

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

#define DBSC_DBSTATE0			0xE6790108

#define DBSC_DBACEN			0xE6790200
#define DBSC_DBRFEN			0xE6790204
#define DBSC_DBCMD			0xE6790208
#define DBSC_DBWAIT			0xE6790210	//wait DBCMD 1=busy, 0=ready

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

#define DBSC_DBPDSTAT_0			0xE6790630
#define DBSC_DBPDSTAT_1			0xE6790670
#define DBSC_DBPDSTAT_2			0xE67906B0
#define DBSC_DBPDSTAT_3			0xE67906F0

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

#define DBSC_DBMEMSWAPCONF0		0xE6792000

#define DBSC_DBMONCONF4			0xE6793010

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

////////////////////////////////////////////////////////////
// 2800: RL=28,WL=14,nWR=30,nRTP=12
#define DDR2800_PI_MR1			0xd4	//DeviceFeature1(Post=1.5tck nWR=30 RDpre=static WRPre=2tCK BL=16		// 1 101 0100
#define DDR2800_PI_MR2			0x2d	//DeviceFeature2(0,0SetA,101=WL14,101=RL28)					// 0 0 101 101
#define DDR2800_PI_TFC			0x00c8	//	xxxx
#define DDR2800_PI_TRTP			0x0B
#define DDR2800_PI_TCCD			0x08
#define DDR2800_PI_TWR			0x1A
#define DDR2800_PI_TWTR			0x0E
#define DDR2800_PI_TRCD			0x1A
#define DDR2800_PI_TRP			0x1E
#define DDR2800_PI_TRAS_MIN		0x3B
#define DDR2800_PI_TRAS_MAX		0x0db60	//	xxxx
#define DDR2800_PI_TMRW			0x08	//	xxxx
#define DDR2800_PI_TMRD			0x0c	//	xxxx
#define DDR2800_PI_TCCDMW		0x20
#define DDR2800_PI_TDQSCK_MAX		0x03
#define DDR2800_PI_RDLAT_ADJ		0x10	//???
#define DDR2800_PI_CASLAT_LIN		0x50
#define DDR2800_PI_WRLAT		0x0e
#define DDR2800_PI_WRLAT_ADJ		0x0c

///////////////////////////////////////////////////////////
// 2400: RL=24,WL=12,nWR=24,nRTP=10
#define DDR2400_PI_MR1			0xc4	//DeviceFeature1(Post=1.5tck nWR=24 RDpre=static WRPre=2tCK BL=16		// 1 100 0100
#define DDR2400_PI_MR2			0x24	//DeviceFeature2(0,0SetA,100=WL12,100=RL24)					// 0 0 100 100
#define DDR2400_PI_TFC			0x00c8	//	xxxx
#define DDR2400_PI_TRTP			0x09
#define DDR2400_PI_TCCD			0x08
#define DDR2400_PI_TWR			0x16
#define DDR2400_PI_TWTR			0x0c
#define DDR2400_PI_TRCD			0x16
#define DDR2400_PI_TRP			0x1a
#define DDR2400_PI_TRAS_MIN		0x33
#define DDR2400_PI_TRAS_MAX		0x0db60	//	xxxx
#define DDR2400_PI_TMRW			0x08	//	xxxx
#define DDR2400_PI_TMRD			0x0c	//	xxxx
#define DDR2400_PI_TCCDMW		0x20
#define DDR2400_PI_TDQSCK_MAX		0x03
#define DDR2400_PI_RDLAT_ADJ		0x0e
#define DDR2400_PI_CASLAT_LIN		0x50	//PI_CASLAT_LIN_F2:RW:0:7:=0x1c
#define DDR2400_PI_WRLAT		0x0c	//PI_WRLAT_F2:RW:16:5:=0x08
#define DDR2400_PI_WRLAT_ADJ		0x0a
/////////////////
#define DDR1600_PI_MR1			0xa4	//MRW DeviceFeature1(Post=1.5tck nWR=16 RDpre=static WRPre=2tCK BL=16//OK		// 1 010 0100
#define DDR1600_PI_MR2			0x52	//MRW DeviceFeature2(0,1SetB,010=WL12,010=RL14(nRTP14))//				// 01 010 010
#define DDR1600_PI_TFC			0x00c8
#define DDR1600_PI_TRTP			0x08
#define DDR1600_PI_TCCD			0x08
#define DDR1600_PI_TWR			0x11
#define DDR1600_PI_TWTR			0x0a
#define DDR1600_PI_TRCD			0x0f
#define DDR1600_PI_TRP			0x11
#define DDR1600_PI_TRAS_MIN		0x22
#define DDR1600_PI_TRAS_MAX		0x0db60
#define DDR1600_PI_TMRW			0x08	//10
#define DDR1600_PI_TMRD			0x0c
#define DDR1600_PI_TCCDMW		0x20
#define DDR1600_PI_TDQSCK_MAX		0x3
#define DDR1600_PI_RDLAT_ADJ		0x08	//PI_RDLAT_ADJ_F2:RW:16:8:=0x07       //try 07+2 1600
#define DDR1600_PI_CASLAT_LIN		0x50	//PI_CASLAT_LIN_F2:RW:0:7:=0x1c
#define DDR1600_PI_WRLAT		0x0c	//PI_WRLAT_F2:RW:16:5:=0x08
#define DDR1600_PI_WRLAT_ADJ		0x0a

//////////////////////////////////////////////////////////////////
void InitDram(void);
void InitDDR_start_testtesttest(uint32_t freq);

///------------
void SoftDelay_ddr(uint32_t loop);
inline void dsb_sev(void);
inline uint32_t PLL3_FREQ_01();
inline uint32_t PLL3_FREQ_10();
inline uint32_t PLL3_FREQ(uint32_t freq_flag_b,uint32_t freq_flag_a);

void WaitDBCMD(void);
void REG_DDRPHY_WRITE ( uintptr_t phyno, uint32_t regadd, uint32_t regdata);
uint32_t REG_DDRPHY_READ ( uintptr_t phyno, uint32_t regadd);
void change_lpddr4_en(uintptr_t phychno, uint32_t mode);

//---	Reg Setting	----
void pvt_dbsc_regset(uint32_t freq);
void mode_register_set(uint32_t freq,uintptr_t phychno,uint32_t mr13_val);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
void PutHex(const char *string , uint32_t value, uint32_t valsize, uint32_t cr)
{
	char buf[256];
	Data2HexAscii_64(value,buf,valsize);
	PutStr(string,0);
	PutStr(buf,cr);
}
*/
void SoftDelay_ddr(uint32_t loop)
{
	volatile uint32_t i;
	for(i=0;i<(loop*2);i++){};
}

inline void dsb_sev(void)
//void dsb_sev(void)
{
	__asm__ __volatile__ ("dsb sy");
}

inline void ddrphy_regif_wa()
{
	// REG-IF:W/A
//	PutHex("CH0 RegIF CHECK:0x0200 = ",REG_DDRPHY_READ(0,0x0200),4,0);	PutHex(", 0x02A9 = ",REG_DDRPHY_READ(0,0x02A9),4,1);
//	PutHex("CH1 RegIF CHECK:0x0200 = ",REG_DDRPHY_READ(1,0x0200),4,0);	PutHex(", 0x02A9 = ",REG_DDRPHY_READ(1,0x02A9),4,1);
	*((volatile uint32_t*)(DBSC_DBPDRGA_0)) = 0x02A9;
	*((volatile uint32_t*)(DBSC_DBPDRGA_1)) = 0x02A9;
	dsb_sev();
}



inline uint32_t PLL3_FREQ_01()
{
	uint32_t dataL;
	uint32_t count;

	// FREQ_CHG_REQ status
//	PutHex("Freq_chg_req_0: 0x",(*((volatile uint32_t*)(DBSC_DBPDSTAT_0))& 0x01),1,0);
//	PutHex(", type_0: 0x",(*((volatile uint32_t*)(DBSC_DBPDSTAT_0)) >> 8)& 0x0f ,1,0);
//	PutHex(", req_1: 0x" ,(*((volatile uint32_t*)(DBSC_DBPDSTAT_1))      & 0x01),1,0);
//	PutHex(", type_1: 0x",(*((volatile uint32_t*)(DBSC_DBPDSTAT_0)) >> 8)& 0x0f ,1,1);
//	PutHex("PLL Status: ",((*( (volatile uint32_t*)DBSC_PLL_LOCK_0 ))|(*((volatile uint32_t*)DBSC_PLL_LOCK_1)<<8)),2,1);
	// Waiting  FREQ_CHG_REQ
	count = 100000;
	do {
		dataL =	(*((volatile uint32_t*)DBSC_DBPDSTAT_0)) &
			(*((volatile uint32_t*)DBSC_DBPDSTAT_1)) & 0x00000001;
//		dataL =	(*((volatile uint32_t*)DBSC_DBPDSTAT_0))  & 0x00000001;
		count = count -1;
	} while( (dataL!=0x00000001) & (count !=0));
	if(count == 0) {
//		PutStr("Time out",1);
		return (1);
	}


	//PLL3 : Enable(non bypass)
	*((volatile uint32_t*)CPG_CPGWPR)		= ~(0x001f001f);
	*((volatile uint32_t*)CPG_PLLECR)		=   0x001f001f ;
	dsb_sev();
	do {
		dataL=*((volatile uint32_t*)CPG_PLLECR);
	} while( (dataL&0x1f00)!=0x1f00 );
//	*((volatile uint32_t*)(DBSC_DBTSTCONF1))	= 0x00000000;
	SoftDelay_ddr(1000);


	// FREQ = 1
//	*((volatile uint32_t*)(DBSC_DFI_FREQ_0))	= 0x00000001;
//	*((volatile uint32_t*)(DBSC_DFI_FREQ_1))	= 0x00000001;
	*((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x01000010;	//dbdficnt0: DFIFREQUENCY = 1, freq_ratio = 01 (2:1), init_start =0
	*((volatile uint32_t*)DBSC_DBDFICNT_1) = 0x01000010;
	dsb_sev();

	// freq_change_ack:  0 -> 1
	// freq_change_ack:  1 -> 0
	*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0))	= 0x0CF20000;
	*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_1))	= 0x0CF20000;
	dsb_sev();

	count = 1000000;
	do {
		dataL =	((*((volatile uint32_t*)DBSC_DBPDSTAT_0)) |
			(*((volatile uint32_t*)DBSC_DBPDSTAT_1))) & 0x00000001;
//		dataL =	((*((volatile uint32_t*)DBSC_DBPDSTAT_0)) ) & 0x00000001;
		count = count -1;
	} while( (dataL==0x00000001) & (count !=0));
	*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0))	= 0x00000000;
	*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_1))	= 0x00000000;
	dsb_sev();

	if(count == 0) {
//		PutStr("Time out",1);
		return (1);
	}
//	PutHex("PLL Status: ",((*( (volatile uint32_t*)DBSC_PLL_LOCK_0 ))|(*((volatile uint32_t*)DBSC_PLL_LOCK_1)<<8)),2,1);

	return (0);
}



inline uint32_t PLL3_FREQ_10()
{
	uint32_t dataL;
	uint32_t count;

	count = 10000000;
	do {
		dataL =	(*((volatile uint32_t*)DBSC_DBPDSTAT_0)) &
			(*((volatile uint32_t*)DBSC_DBPDSTAT_1)) & 0x00000001;
		count = count -1;
	} while( (dataL!=0x00000001) & (count !=0));
	if(count == 0) {
//		PutStr("Time out",1);
		return (1);
	}

//	PutHex("Freq_chg_req_0: 0x",(*((volatile uint32_t*)(DBSC_DBPDSTAT_0))& 0x01),1,0);
//	PutHex(", type_0: 0x",(*((volatile uint32_t*)(DBSC_DBPDSTAT_0)) >> 8)& 0x0f ,1,0);
//	PutHex(", req_1: 0x" ,(*((volatile uint32_t*)(DBSC_DBPDSTAT_1))      & 0x01),1,0);
//	PutHex(", type_1: 0x",(*((volatile uint32_t*)(DBSC_DBPDSTAT_0)) >> 8)& 0x0f ,1,1);

	//PLL3 : Disable(bypass)
	*((volatile uint32_t*)CPG_CPGWPR)		= ~(0x001f0017);
	*((volatile uint32_t*)CPG_PLLECR)		=   0x001f0017 ;
//	*((volatile uint32_t*)(DBSC_DBTSTCONF1))	= 0x00000001;
	dsb_sev();

//	*((volatile uint32_t*)(DBSC_DBTSTCONF1))	= 0x00000001;

	// FREQ = 0
//	*((volatile uint32_t*)(DBSC_DFI_FREQ_0))	= 0x00000000;
//	*((volatile uint32_t*)(DBSC_DFI_FREQ_1))	= 0x00000000;
	*((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x00000010;	//dbdficnt0: DFIFREQUENCY = 0, freq_ratio = 01 (2:1), init_start =0
	*((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x00000010;
	dsb_sev();

	// freq_change_ack:  0 -> 1
	*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0))	= 0x0CF20000;
	*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_1))	= 0x0CF20000;
	dsb_sev();

	// PHY-PLL lock waiting....
	count = 100000;
	do {
		dataL =	((*((volatile uint32_t*)DBSC_DBPDSTAT_0)) |
			(*((volatile uint32_t*)DBSC_DBPDSTAT_1))) & 0x00000001;
		count = count -1;
	} while( (dataL==0x00000001) & (count !=0));
	// freq_change_ack:  1 -> 0
	*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0))	= 0x00000000;
	*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_1))	= 0x00000000;
	if(count == 0) {
//		PutStr("Time out",1);
		return (1);
	}
//	PutHex("PLL Status: ",((*( (volatile uint32_t*)DBSC_PLL_LOCK_0 ))|(*((volatile uint32_t*)DBSC_PLL_LOCK_1)<<8)),2,1);
	return (0);

}

inline uint32_t PLL3_FREQ(uint32_t freq_flag_b,uint32_t freq_flag_a)
{
	uint32_t dataL;
	uint32_t count;

	// PHY-PLL unlock waiting....
	count = 100000;
	do {
		dataL =	(*((volatile uint32_t*)DBSC_DBPDSTAT_0)) &
			(*((volatile uint32_t*)DBSC_DBPDSTAT_1)) & 0x00000001;
//		dataL =	(*((volatile uint32_t*)DBSC_DBPDSTAT_0)) & 0x00000001;
		count = count -1;
	} while( (dataL!=0x00000001) & (count !=0));
	if(count == 0) {
//		PutStr("Time out",1);
		return (1);
	}

//	PutHex("Freq_chg_req_0: 0x",(*((volatile uint32_t*)(DBSC_DBPDSTAT_0))& 0x01),1,0);
//	PutHex(", type_0: 0x",(*((volatile uint32_t*)(DBSC_DBPDSTAT_0)) >> 8)& 0x0f ,1,0);
//	PutHex(", req_1: 0x" ,(*((volatile uint32_t*)(DBSC_DBPDSTAT_1))      & 0x01),1,0);
//	PutHex(", type_1: 0x",(*((volatile uint32_t*)(DBSC_DBPDSTAT_0)) >> 8)& 0x0f ,1,1);

	if(freq_flag_a == 0){
		//PLL3 : Disable(bypass)
		*((volatile uint32_t*)CPG_CPGWPR)		= ~(0x001f0017);
		*((volatile uint32_t*)CPG_PLLECR)		=   0x001f0017 ;
		dsb_sev();
//		*((volatile uint32_t*)(DBSC_DBTSTCONF1))	= 0x00000001;
	} else if((freq_flag_b == 0)&(freq_flag_a == 1)){
		//PLL3 : Enable
		*((volatile uint32_t*)CPG_CPGWPR)		= ~(0x001f001f);
		*((volatile uint32_t*)CPG_PLLECR)		=   0x001f001f ;
//		*((volatile uint32_t*)(DBSC_DBTSTCONF1))	= 0x00000000;
		dsb_sev();

		do {
			dataL=*((volatile uint32_t*)CPG_PLLECR);
		} while( (dataL&0x1f00)!=0x1f00 );
//		*((volatile uint32_t*)(DBSC_DBTSTCONF1))	= 0x00000000;
	}
	dsb_sev();

	// FREQ = freq_flag
//	*((volatile uint32_t*)(DBSC_DFI_FREQ_0))	= freq_flag_a;
//	*((volatile uint32_t*)(DBSC_DFI_FREQ_1))	= freq_flag_a;
	*((volatile uint32_t*)DBSC_DBDFICNT_0) = freq_flag_a << 24| 0x10;	//dbdficnt0: DFIFREQUENCY = freq_flag_a, freq_ratio = 01 (2:1), init_start =0
	*((volatile uint32_t*)DBSC_DBDFICNT_1) = freq_flag_a << 24| 0x10;
	dsb_sev();

	// freq_change_ack:  0 -> 1
	*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0))	= 0x0CF20000;
	*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_1))	= 0x0CF20000;

	count = 100000;
	do {
		dataL =	((*((volatile uint32_t*)DBSC_DBPDSTAT_0)) |
			(*((volatile uint32_t*)DBSC_DBPDSTAT_1))) & 0x00000001;
//		dataL =	(*((volatile uint32_t*)DBSC_DBPDSTAT_0)) & 0x00000001;
		count = count -1;
	} while( (dataL==0x00000001) & (count !=0));
	// freq_change_ack:  1 -> 0
	*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_0))	= 0x00000000;
	*((volatile uint32_t*)(DBSC_FREQ_CHG_ACK_1))	= 0x00000000;

	if(count == 0) {
//		PutStr("Time out",1);
		return (1);
	}
//	PutHex("PLL Status: ",((*( (volatile uint32_t*)DBSC_PLL_LOCK_0 ))|(*((volatile uint32_t*)DBSC_PLL_LOCK_1)<<8)),2,1);
	return (0);
}


void WaitDBCMD(void)
{
	uint32_t dataL;

	while(1)
	{
		dataL = *((volatile uint32_t*)DBSC_DBWAIT);	//wait DBCMD 1=busy, 0=ready
		if((dataL & 0x00000001) == 0x0)	break;
	}
}

uint32_t REG_DDRPHY_READ ( uintptr_t phyno, uint32_t regadd)
{
	uint32_t i;

//	dsb_sev();
	*((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno)) = regadd;
	SoftDelay_ddr(1000);

	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGD_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGD_0 + 0x40 * phyno));
	dsb_sev();

	return i ;
}

void REG_DDRPHY_WRITE ( uintptr_t phyno, uint32_t regadd, uint32_t regdata)
{
	uint32_t i;

	*((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno)) = regadd;
	SoftDelay_ddr(1000);

	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();

	*((volatile uint32_t*)(DBSC_DBPDRGD_0 + 0x40 * phyno)) = regdata;
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();
	i = *((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x40 * phyno));
	dsb_sev();

	(void)i;
}

void change_lpddr4_en(uintptr_t phychno, uint32_t mode)
{
/*
	{	0x0B9E ,	0x00000411	} ,	// PHY_PAD_FDBK_DRIVE:RW+:0:20:=0x000411
	{	0x0B9F ,	0x00400040	} ,	// PHY_PAD_DQS_DRIVE:RW+:16:12:=0x0040 PHY_PAD_DATA_DRIVE:RW+:0:10:=0x0040
	{	0x0BA0 ,	0x00000411	} ,	// PHY_PAD_ADDR_DRIVE:RW+:0:18:=0x000411
	{	0x0BA1 ,	0x00000411	} ,	// PHY_PAD_CLK_DRIVE:RW+:0:20:=0x000411
	{	0x0BA2 ,	0x00004410	} ,	// PHY_PAD_FDBK_TERM:RW+:0:18:=0x004410
	{	0x0BA3 ,	0x00004410	} ,	// PHY_PAD_DATA_TERM:RW+:0:17:=0x004410
	{	0x0BA4 ,	0x00004410	} ,	// PHY_PAD_DQS_TERM:RW+:0:17:=0x004410
	{	0x0BA5 ,	0x00004410	} ,	// PHY_PAD_ADDR_TERM:RW+:0:18:=0x004410
	{	0x0BA6 ,	0x00004410	} ,	// PHY_PAD_CLK_TERM:RW+:0:18:=0x004410
	{	0x0BA7 ,	0x00000411	} ,	// PHY_PAD_CKE_DRIVE:RW+:0:18:=0x000411
	{	0x0BA8 ,	0x00004410	} ,	// PHY_PAD_CKE_TERM:RW+:0:18:=0x004410
	{	0x0BA9 ,	0x00000411	} ,	// PHY_PAD_RST_DRIVE:RW+:0:18:=0x000411
	{	0x0BAA ,	0x00004410	} ,	// PHY_PAD_RST_TERM:RW+:0:18:=0x004410
	{	0x0BAB ,	0x00000411	} ,	// PHY_PAD_CS_DRIVE:RW+:0:18:=0x000411
	{	0x0BAC ,	0x00004410	} ,	// PHY_PAD_CS_TERM:RW+:0:18:=0x004410
*/

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

		addr=0x0BA0;	REG_DDRPHY_WRITE(i,addr,dataL=((REG_DDRPHY_READ(i,addr)&clr)|set) );	// PHY_PAD_ADDR_DRIVE
		addr=0x0BA1;	REG_DDRPHY_WRITE(i,addr,dataL=((REG_DDRPHY_READ(i,addr)&clr)|set) );	// PHY_PAD_CLK_DRIVE
		addr=0x0BA7;	REG_DDRPHY_WRITE(i,addr,dataL=((REG_DDRPHY_READ(i,addr)&clr)|set) );	// PHY_PAD_CKE_DRIVE
		addr=0x0BAB;	REG_DDRPHY_WRITE(i,addr,dataL=((REG_DDRPHY_READ(i,addr)&clr)|set) );	// PHY_PAD_CS_DRIVE
	}
	ddrphy_regif_wa();	// REG-IF:W/A
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DBSC Register Setting
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void pvt_dbsc_regset(uint32_t freq)
{

	*((volatile uint32_t*)DBSC_DBPDCNT0_0)	= 0x00;
	*((volatile uint32_t*)DBSC_DBPDCNT0_1)	= 0x00;

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

	*((volatile uint32_t*)DBSC_DBMEMCONF_0_2)	= 0x0f030a02;	//memconf01(ch0/rank2) 4GB
	*((volatile uint32_t*)DBSC_DBMEMCONF_0_3)	= 0x0f030a02;	//memconf01(ch0/rank3) 4GB
	*((volatile uint32_t*)DBSC_DBMEMCONF_1_2)	= 0x0f030a02;	//memconf01(ch1/rank2) 4GB
	*((volatile uint32_t*)DBSC_DBMEMCONF_1_3)	= 0x0f030a02;	//memconf01(ch1/rank3) 4GB

	*((volatile uint32_t*)DBSC_DBPHYCONF0)	= 0x00000001;	//phyconf0

//	if(freq>0x5f)
	if(freq>0xf)
	{
#ifdef	DDR_DEBUG_WL
		// DDR3733 ???
		*((volatile uint32_t*)DBSC_DBTR0)	= 0x00000020;	//dbtr0  cl=32
		*((volatile uint32_t*)DBSC_DBTR1)	= 0x0000000e;	//dbtr1  cwl=14
		*((volatile uint32_t*)DBSC_DBTR3)	= 0x00000022;	//dbtr3  trcd=34
		*((volatile uint32_t*)DBSC_DBTR4)	= 0x00280022;	//dbtr4  trpa=40 trp=34
		*((volatile uint32_t*)DBSC_DBTR5)	= 0x00000071;	//dbtr5  trc=113
		*((volatile uint32_t*)DBSC_DBTR6)	= 0x0000004f;	//dbtr6  tras=79
		*((volatile uint32_t*)DBSC_DBTR7)	= 0x00130013;	//dbtr7  trrd_s=19 trrd=19
		*((volatile uint32_t*)DBSC_DBTR8)	= 0x0000004B;	//dbtr8  tfaw=75
		*((volatile uint32_t*)DBSC_DBTR9)	= 0x0000000E;	//dbtr9  trdpr=14
		*((volatile uint32_t*)DBSC_DBTR10)	= 0x00000023;	//dbtr10 twr=38
		*((volatile uint32_t*)DBSC_DBTR11)	= 0x00000026;	//dbtr11 trdwr=41
		*((volatile uint32_t*)DBSC_DBTR12)	= 0x002A002A;	//dbtr12 twrrd_s=42 twrrd=42
		*((volatile uint32_t*)DBSC_DBTR13)	= 0x00900120;	//dbtr13 trfcpb=96 trfc=208
		*((volatile uint32_t*)DBSC_DBTR14)	= 0x000c000c;	//dbtr14 tckehdll=12 tckeh=12
		*((volatile uint32_t*)DBSC_DBTR15)	= 0x001e000c;	//dbtr15 tckesr=30 tckel=12   currently tckesr is TBD.

//		*((volatile uint32_t*)DBSC_DBTR16)	= 0x10400c0e;	//dbtr16 latency?  CL=32 & PHY_RDDATA_EN_DLY_X = 0e (32+2-14=20), CWL=14 & TDFI_PHY_WRDATA=2
		*((volatile uint32_t*)DBSC_DBTR16)	= 0x10400c0e;	//dbtr16 latency?  CL=32 & PHY_RDDATA_EN_DLY_X = 0e (32+2-14=20), CWL=14 & TDFI_PHY_WRDATA=2
		*((volatile uint32_t*)DBSC_DBTR17)	= 0x080a0004;	//dbtr17 tmodrd=8  tmod=10 trdmr=4
		*((volatile uint32_t*)DBSC_DBTR18)	= 0x00000000;	//dbtr18 No odt for DQ exists in LPDDR4.
		*((volatile uint32_t*)DBSC_DBTR19)	= 0x00000000;	//dbtr19 tzqcl=0 tzqcs=0 because LPDDR4 doesn't support ZQC[SL].
		*((volatile uint32_t*)DBSC_DBTR20)	= 0x012c012c;	//dbtr20 txsdll=trfc+12=220 txs=220
		*((volatile uint32_t*)DBSC_DBTR21)	= 0x00080008;	//dbtr21 tccd_s=8 tccd=8
		*((volatile uint32_t*)DBSC_DBTR22)	= 0x06400030;	//dbtr22 tzqcal=1600 tzqlat=48
//		*((volatile uint32_t*)DBSC_DBTR23)	= 0x00000002;	//dbtr23
		*((volatile uint32_t*)DBSC_DBTR23)	= 0x00000003;	//dbtr23
#else
		// DDR3733 ???
		*((volatile uint32_t*)DBSC_DBTR0)	= 0x00000020;	//dbtr0  cl=32
		*((volatile uint32_t*)DBSC_DBTR1)	= 0x00000010;	//dbtr1  cwl=16
		*((volatile uint32_t*)DBSC_DBTR3)	= 0x00000022;	//dbtr3  trcd=29
		*((volatile uint32_t*)DBSC_DBTR4)	= 0x00280022;	//dbtr4  trpa=34 trp=29
		*((volatile uint32_t*)DBSC_DBTR5)	= 0x00000076;	//dbtr5  trc=102
		*((volatile uint32_t*)DBSC_DBTR6)	= 0x0000004F;	//dbtr6  tras=68
		*((volatile uint32_t*)DBSC_DBTR7)	= 0x00130013;	//dbtr7  trrd_s=16 trrd=16
		*((volatile uint32_t*)DBSC_DBTR8)	= 0x0000004B;	//dbtr8  tfaw=64
		*((volatile uint32_t*)DBSC_DBTR9)	= 0x0000000e;	//dbtr9  trdpr=12
		*((volatile uint32_t*)DBSC_DBTR10)	= 0x00000023;	//dbtr10 twr=30
		*((volatile uint32_t*)DBSC_DBTR11)	= 0x00000025;	//dbtr11 trdwr=37
		*((volatile uint32_t*)DBSC_DBTR12)	= 0x002c002c;	//dbtr12 twrrd_s=44 twrrd=44
		*((volatile uint32_t*)DBSC_DBTR13)	= 0x00900120;	//dbtr13 trfcpb=96 trfc=208
		*((volatile uint32_t*)DBSC_DBTR14)	= 0x000c000c;	//dbtr14 tckehdll=12 tckeh=12
		*((volatile uint32_t*)DBSC_DBTR15)	= 0x001e000c;	//dbtr15 tckesr=30 tckel=12   currently tckesr is TBD.

//		*((volatile uint32_t*)DBSC_DBTR16)	= 0x10290e10;	//dbtr16 latency?  CL=32 & PHY_RDDATA_EN_DLY_X = 0e (32+2-14=20), CWL=14 & TDFI_PHY_WRDATA=2
		*((volatile uint32_t*)DBSC_DBTR16)	= 0x10290c0e;	//dbtr16 latency?  CL=32 & PHY_RDDATA_EN_DLY_X = 0e (32+2-14=20), CWL=14 & TDFI_PHY_WRDATA=2: // 2016/03/24
		*((volatile uint32_t*)DBSC_DBTR17)	= 0x080a0004;	//dbtr17 tmodrd=8  tmod=10 trdmr=4
		*((volatile uint32_t*)DBSC_DBTR18)	= 0x00000000;	//dbtr18 No odt for DQ exists in LPDDR4.
		*((volatile uint32_t*)DBSC_DBTR19)	= 0x00000000;	//dbtr19 tzqcl=0 tzqcs=0 because LPDDR4 doesn't support ZQC[SL].
		*((volatile uint32_t*)DBSC_DBTR20)	= 0x012c012c;	//dbtr20 txsdll=trfc+12=220 txs=220
		*((volatile uint32_t*)DBSC_DBTR21)	= 0x00080008;	//dbtr21 tccd_s=8 tccd=8
		*((volatile uint32_t*)DBSC_DBTR22)	= 0x06400030;	//dbtr22 tzqcal=1600 tzqlat=48
//		*((volatile uint32_t*)DBSC_DBTR23)	= 0x00000002;	//dbtr23
		*((volatile uint32_t*)DBSC_DBTR23)	= 0x00000003;	//dbtr23 // 2016/03/24
#endif

	}
	else if(freq>0x53)
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
		*((volatile uint32_t*)DBSC_DBTR21)	= 0x00080008;	//dbtr21 tccd_s=8 tccd=8
		*((volatile uint32_t*)DBSC_DBTR22)	= 0x06400030;	//dbtr22 tzqcal=1600 tzqlat=48
//		*((volatile uint32_t*)DBSC_DBTR23)	= 0x00000002;	//dbtr23
		*((volatile uint32_t*)DBSC_DBTR23)	= 0x00000003;	//dbtr23

	}
	else if(freq>0x47)
	{
		// DDR2800
		*((volatile uint32_t*)DBSC_DBTR0)	= 0x0000001c;	//dbtr0  cl=28
		*((volatile uint32_t*)DBSC_DBTR1)	= 0x0000000e;	//dbtr1  cwl=14
		*((volatile uint32_t*)DBSC_DBTR3)	= 0x0000001a;	//dbtr3  trcd=26
		*((volatile uint32_t*)DBSC_DBTR4)	= 0x001e001a;	//dbtr4  trpa=30 trp=26
		*((volatile uint32_t*)DBSC_DBTR5)	= 0x00000055;	//dbtr5  trc=85
		*((volatile uint32_t*)DBSC_DBTR6)	= 0x0000003b;	//dbtr6  tras=59
		*((volatile uint32_t*)DBSC_DBTR7)	= 0x000f000f;	//dbtr7  trrd_s=15 trrd=15
		*((volatile uint32_t*)DBSC_DBTR8)	= 0x00000039;	//dbtr8  tfaw=57
		*((volatile uint32_t*)DBSC_DBTR9)	= 0x0000000c;	//dbtr9  trdpr=12
		*((volatile uint32_t*)DBSC_DBTR10)	= 0x0000001E;	//dbtr10 twr=30, nWR is 26, but its value is not defined in MR1.nWR
		*((volatile uint32_t*)DBSC_DBTR11)	= 0x00000022;	//dbtr11 trdwr=34
		*((volatile uint32_t*)DBSC_DBTR12)	= 0x00260026;	//dbtr12 twrrd_s=38 twrrd=38
		*((volatile uint32_t*)DBSC_DBTR13)	= 0x007f00fd;	//dbtr13 trfcpb=127 trfc=253
		*((volatile uint32_t*)DBSC_DBTR14)	= 0x000c000c;	//dbtr14 tckehdll=12 tckeh=12
		*((volatile uint32_t*)DBSC_DBTR15)	= 0x0016000b;	//dbtr15 tckesr=22 tckel=11

		*((volatile uint32_t*)DBSC_DBTR16)	= 0x101f0c0e;	//dbtr16 latency?  CL=32 & PHY_RDDATA_EN_DLY_X = 0e (32+2-14=20), CWL=14 & TDFI_PHY_WRDATA=2
		*((volatile uint32_t*)DBSC_DBTR17)	= 0x0c0f0020;	//dbtr17 tmodrd=12  tmod=15 trdmr=32
		*((volatile uint32_t*)DBSC_DBTR18)	= 0x00000000;	//dbtr18 No odt for DQ exists in LPDDR4.
		*((volatile uint32_t*)DBSC_DBTR19)	= 0x00000000;	//dbtr19 tzqcl=0 tzqcs=0 because LPDDR4 doesn't support ZQC[SL].
		*((volatile uint32_t*)DBSC_DBTR20)	= 0x01070107;	//dbtr20 txsdll=txs=tRFCab+7.5ns=263
		*((volatile uint32_t*)DBSC_DBTR21)	= 0x00080008;	//dbtr21 tccd_s=8 tccd=8
		*((volatile uint32_t*)DBSC_DBTR22)	= 0x06400030;	//dbtr22 tzqcal=1600 tzqlat=48
//		*((volatile uint32_t*)DBSC_DBTR23)	= 0x00000002;	//dbtr23
		*((volatile uint32_t*)DBSC_DBTR23)	= 0x00000003;	//dbtr23

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
		*((volatile uint32_t*)DBSC_DBTR11)	= 0x0000001c;	//dbtr11 trdwr=28
		*((volatile uint32_t*)DBSC_DBTR12)	= 0x00220022;	//dbtr12 twrrd_s=34 twrrd=34
		*((volatile uint32_t*)DBSC_DBTR13)	= 0x006d00d9;	//dbtr13 trfcpb=109 trfc=217
		*((volatile uint32_t*)DBSC_DBTR14)	= 0x000a000a;	//dbtr14 tckehdll=10 tckeh=10
		*((volatile uint32_t*)DBSC_DBTR15)	= 0x0013000c;	//dbtr15 tckesr=19 tckel=12

		*((volatile uint32_t*)DBSC_DBTR16)	= 0x08400a0c;	//dbtr16 latency?  CL=24 & PHY_RDDATA_EN_DLY_X = 0e (???), CWL=12 & TDFI_PHY_WRDATA=2
		*((volatile uint32_t*)DBSC_DBTR17)	= 0x080a0004;	//dbtr17 tmodrd=8  tmod=10 trdmr=4
		*((volatile uint32_t*)DBSC_DBTR18)	= 0x00000000;	//dbtr18 No odt for DQ exists in LPDDR4.
		*((volatile uint32_t*)DBSC_DBTR19)	= 0x00000000;	//dbtr19 tzqcl=0 tzqcs=0 because LPDDR4 doesn't support ZQC[SL].
		*((volatile uint32_t*)DBSC_DBTR20)	= 0x00e200e2;	//dbtr20 txsdll=txs=tRFCab+7.5ns=226
		*((volatile uint32_t*)DBSC_DBTR21)	= 0x00080008;	//dbtr21 tccd_s=8 tccd=8
		*((volatile uint32_t*)DBSC_DBTR22)	= 0x04b10025;	//dbtr22 tzqcal=1201 tzqlat=37
//		*((volatile uint32_t*)DBSC_DBTR23)	= 0x00000002;	//dbtr23
		*((volatile uint32_t*)DBSC_DBTR23)	= 0x00000003;	//dbtr23

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
		*((volatile uint32_t*)DBSC_DBTR11)	= 0x00000011;	//dbtr11 trdwr=17
		*((volatile uint32_t*)DBSC_DBTR12)	= 0x001D001D;	//dbtr12 twrrd_s=29
		*((volatile uint32_t*)DBSC_DBTR13)	= 0x00480090;	//dbtr13 trfcpb=72  trfcab=144
		*((volatile uint32_t*)DBSC_DBTR14)	= 0x00060006;	//dbtr14 tckehdll=6
		*((volatile uint32_t*)DBSC_DBTR15)	= 0x000c0006;	//dbtr15 tckesr=12

		*((volatile uint32_t*)DBSC_DBTR16)	= 0x06200a0C;	//dbtr16 latency?
		*((volatile uint32_t*)DBSC_DBTR17)	= 0x080a0004;	//dbtr17 tmodrd=8  tmod=10 trdmr=4
		*((volatile uint32_t*)DBSC_DBTR18)	= 0x00000000;	//dbtr18 No odt for DQ exists in LPDDR4.
		*((volatile uint32_t*)DBSC_DBTR19)	= 0x00000000;	//dbtr19 tzqcl=0 tzqcs=0 because LPDDR4 doesn't support ZQC[SL].
		*((volatile uint32_t*)DBSC_DBTR20)	= 0x00b00096;	//dbtr20 txsdll=150
		*((volatile uint32_t*)DBSC_DBTR21)	= 0x00080008;	//dbtr21 tccd_s=8 tccd=8
		*((volatile uint32_t*)DBSC_DBTR22)	= 0x03200018;	//dbtr22 tzqcal=800 tzqlat=24
//		*((volatile uint32_t*)DBSC_DBTR23)	= 0x00000002;	//dbtr23
		*((volatile uint32_t*)DBSC_DBTR23)	= 0x00000003;	//dbtr23
	}



	*((volatile uint32_t*)DBSC_DBRNK0)	= 0x00000000;	//dbrnk0 No odt for DQ exists in LPDDR4(N.Honda).
	*((volatile uint32_t*)DBSC_DBRNK1)	= 0x00000000;	//dbrnk1 Don't need to set it cause we use all-rank refresh.
	*((volatile uint32_t*)DBSC_DBRNK2)	= 0x00008888;	//dbrnk2 rkrr=8
	*((volatile uint32_t*)DBSC_DBRNK3)	= 0x00008888;	//dbrnk3 rkrw=8
	*((volatile uint32_t*)DBSC_DBRNK4)	= 0x00008888;	//dbrnk4 rkwr=8
	*((volatile uint32_t*)DBSC_DBRNK5)	= 0x00008888;	//dbrnk5 rkww=8
#ifdef DDR_REF_PR
	*((volatile uint32_t*)DBSC_DBRNK6)	= 0x00000002;	//dbrnk6 Use Per Rank
#else
	*((volatile uint32_t*)DBSC_DBRNK6)	= 0x00000000;	//dbrnk6 Use ALL Rank
#endif
	*((volatile uint32_t*)DBSC_DBADJ0)	= 0x00000000;	//dbadj0 No Optional setting
	*((volatile uint32_t*)DBSC_DBADJ2)	= 0x00000000;	//dbadj2 No Optional setting

	//BUFCAM settings

#ifdef	DDR_QOSSETTING
//	*((volatile uint32_t*)DBSC_DBCAM0CNF0)	= 0x00000000;	//dbcam0cnf0 inoen=0, actually it is not used.
	*((volatile uint32_t*)DBSC_DBCAM0CNF1)	= 0x00044218;	//dbcam0cnf1 
	*((volatile uint32_t*)DBSC_DBCAM0CNF2)	= 0x000000F4;	//dbcam0cnf2 
//	*((volatile uint32_t*)DBSC_DBCAM0CNF3)	= 0x00000003;	//
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
#endif

	*((volatile uint32_t*)DBSC_DBSYSCONF1)		= 0x00000002;	//dbsysconf1 freqratio=2.

	*((volatile uint32_t*)DBSC_DBCAM0CNF1)		= 0x00044218;	//wbkwait(0004), wbkmdhi(4,2),wbkmdlo(1,8)             <- independent of dram freq
	*((volatile uint32_t*)DBSC_DBCAM0CNF2)		= 0x00000284;	//rg_scexp[15:0] 0,0(fillunit),8(dirtymax),4(dirtymin) <- independent of dram freq

//	if(freq>0x5f)
	if(freq>0xf)
	{
		// DDR3733???
#ifdef	DDR_QOSSETTING
		*((volatile uint32_t*)DBSC_DBSCHRW0)	= 0xF7311111;	//stop_tolerance, independent of dram freq
		*((volatile uint32_t*)DBSC_DBSCHRW1)	= 0x00000048;	//[15: 8]=trfcpb/4; [ 7: 0]=trfcab/4   ;
		*((volatile uint32_t*)DBSC_SCFCTST2)	= 0x111f1fff;	//rd-wr/wr-rd toggle count, independent of dram freq
#endif
		*((volatile uint32_t*)DBSC_SCFCTST0)	= 0x18030d09;	//[31:24]=trc/4   ; [23:16]=trda->act/4; [15:8]=twra->act/4; [7:0]=trp/4
		*((volatile uint32_t*)DBSC_SCFCTST1)	= 0x090a080c;	//[31:24]=trdwr/4 ; [23:16]=twrrd/4    ; [15:8]=trcd/4     ; [7:0]=asyncofs
	}
	else if(freq>0x53)
	{
		// DDR3200
#ifdef	DDR_QOSSETTING
		*((volatile uint32_t*)DBSC_DBSCHRW0)	= 0xF7311111;	//stop_tolerance, independent of dram freq
		*((volatile uint32_t*)DBSC_DBSCHRW1)	= 0x00000048;	//[15: 8]=trfcpb/4; [ 7: 0]=trfcab/4   ;
		*((volatile uint32_t*)DBSC_SCFCTST2)	= 0x111f1fff;	//rd-wr/wr-rd toggle count, independent of dram freq
#endif
		*((volatile uint32_t*)DBSC_SCFCTST0)	= 0x18030d09;	//[31:24]=trc/4   ; [23:16]=trda->act/4; [15:8]=twra->act/4; [7:0]=trp/4
		*((volatile uint32_t*)DBSC_SCFCTST1)	= 0x090a080c;	//[31:24]=trdwr/4 ; [23:16]=twrrd/4    ; [15:8]=trcd/4     ; [7:0]=asyncofs
	}
	else if(freq>0x47)
	{
		// DDR2800
#ifdef	DDR_QOSSETTING
		*((volatile uint32_t*)DBSC_DBSCHRW0)	= 0xF7311111;	//stop_tolerance, independent of dram freq
		*((volatile uint32_t*)DBSC_DBSCHRW1)	= 0x00018248;	//[15: 8]=trfcpb/4; [ 7: 0]=trfcab/4   ;
		*((volatile uint32_t*)DBSC_SCFCTST2)	= 0x000F0FFF;	//rd-wr/wr-rd toggle count, independent of dram freq
#endif
		*((volatile uint32_t*)DBSC_SCFCTST0)	= 0x180b1408;	//[31:24]=trc/2.5=18  ; [23:16]=trda->act/2.5=11; [15:8]=twra->act/2.5=21; [7:0]=trp/2.5=8
		*((volatile uint32_t*)DBSC_SCFCTST1)	= 0x0a0b080C;	//[31:24]=trdwr/2.5=9 ; [23:16]=twrrd/2.5=16    ; [15:8]=trcd/2.5=8      ; [7:0]=asyncofs
	}
	else if(freq>0x2f)
	{
		// DDR2400
#ifdef	DDR_QOSSETTING
		*((volatile uint32_t*)DBSC_DBSCHRW0)	= 0x22421111;	//stop_tolerance, independent of dram freq
		*((volatile uint32_t*)DBSC_DBSCHRW1)	= 0x00180034;	//[15: 8]=trfcpb/4; [ 7: 0]=trfcab/4   ;
		*((volatile uint32_t*)DBSC_SCFCTST2)	= 0x012F1123;	//rd-wr/wr-rd toggle count, independent of dram freq
#endif
		*((volatile uint32_t*)DBSC_SCFCTST0)	= 0x180B1708;	//[31:24]=trc/4   ; [23:16]=trda->act/4; [15:8]=twra->act/4; [7:0]=trp/4
		*((volatile uint32_t*)DBSC_SCFCTST1)	= 0x0808070C;	//[31:24]=trdwr/4 ; [23:16]=twrrd/4    ; [15:8]=trcd/4     ; [7:0]=asyncofs
	}
	else
	{
#ifdef	DDR_QOSSETTING
		*((volatile uint32_t*)DBSC_DBSCHRW0)	= 0xF7311111;	//stop_tolerance, independent of dram freq
		*((volatile uint32_t*)DBSC_DBSCHRW1)	= 0x00000024;	//[15: 8]=trfcpb/4; [ 7: 0]=trfcab/4   ;
		*((volatile uint32_t*)DBSC_SCFCTST2)	= 0x000F0FFF;	//rd-wr/wr-rd toggle count, independent of dram freq
#endif
		*((volatile uint32_t*)DBSC_SCFCTST0)	= 0x0c020905;	//[31:24]=trc/4   ; [23:16]=trda->act/4; [15:8]=twra->act/4; [7:0]=trp/4
		*((volatile uint32_t*)DBSC_SCFCTST1)	= 0x0508040C;	//[31:24]=trdwr/4 ; [23:16]=twrrd/4    ; [15:8]=trcd/4     ; [7:0]=asyncofs
	}

	*((volatile uint32_t*)DBSC_DBMONCONF4 )		= 0x00700000 | (*((volatile uint32_t*)DBSC_DBMONCONF4 ));

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DDR Mode Register setting

void mode_register_set(uint32_t freq,uintptr_t phychno,uint32_t mr13_val)
{

	*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040d00|(0x00100000 * phychno)|mr13_val;	//MRW 13 chA rkA 0d:00 //FSPOP&FSPWP is fixed to 0, VRO off
	WaitDBCMD();

//	if(freq>0x5f)
	if(freq>0xf)
	{
#ifdef	DDR_DEBUG_WL
		*((volatile uint32_t*)DBSC_DBCMD) = 0x0e0401d4|(0x00100000 * phychno);	//MRW 1 DeviceFeature1(Post=1.5tck nWR=30 RDpre=static WRPre=2tCK BL=16
#else
		*((volatile uint32_t*)DBSC_DBCMD) = 0x0e0401e4|(0x00100000 * phychno);	//MRW 1 DeviceFeature1(Post=1.5tck nWR=34 RDpre=static WRPre=2tCK BL=16
#endif
	}
	else if(freq>0x47)
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

//	if(freq>0x5f)
	if(freq>0xf)
	{
#ifdef	DDR_DEBUG_WL
		*((volatile uint32_t*)DBSC_DBCMD) = 0x0e04022e|(0x00100000 * phychno);	//MRW 2 DeviceFeature2(0,0SetA,101=WL14,110=RL32)
#else
		*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040236|(0x00100000 * phychno);	//MRW 2 DeviceFeature2(0,0SetA,110=WL16,110=RL32)
#endif
	}
	else if(freq>0x47)
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

	*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040c11|(0x00100000 * phychno);			//MRW 12
	WaitDBCMD();

	*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040e11|(0x00100000 * phychno);			//MRW 14
	WaitDBCMD();

	*((volatile uint32_t*)DBSC_DBCMD) = 0x0e041600|(0x00100000 * phychno)|DDR_MR22;	//MRW 22
	WaitDBCMD();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define TR_ERR 4000
#define TR_MAX (TR_ERR*2)
#define TE_ERR 10000
#define TE_MAX (TE_ERR*2)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//testtesttest

uint32_t pvt_pi_training_go_all_testtesttest(uint32_t freq)
{
//	uint32_t flag,i,j;
	uint32_t flag;
	uint32_t dataL;
	uint32_t phytrainingok;
	uint32_t retry;
	uintptr_t phychno;



//////////////////////
//dfi_init_start negate
	*((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x01000010;	//dbdficnt0: DFIFREQUENCY = 0, freq_ratio = 01 (2:1), init_start =0
	*((volatile uint32_t*)DBSC_DBDFICNT_1) = 0x01000010;
	dsb_sev();

	phytrainingok=DDR_PHYVALID;

//////////////////////
// PI START
//////////////////////

//	PutStr("PI START",1);
//	{	0x0200 ,	0x00000B00	} ,	// PI_VERSION:RD:16:16:=0x0000 PI_DRAM_CLASS:RW:8:4:=0x0b PI_START:RW:0:1:=0x00
	REG_DDRPHY_WRITE(0, 0x0200, 0x20410B01);
	REG_DDRPHY_WRITE(1, 0x0200, 0x20410B01);

// REG-IF:W/A
	ddrphy_regif_wa();

	*((volatile uint32_t*)DBSC_DBDFIPMSTRCNF) = 0x00000001;        //DFI_PHYMSTR_ACK = 1
	dsb_sev();

//	{	0x0B91 ,	0x000D1F00	} ,	// PHY_PAD_VREF_CTRL_DQ_0:RW+:8:14:=0x0d1f PHY_LOW_FREQ_SEL:RW+:0:1:=0x00
//	REG_DDRPHY_WRITE(0,0x0B91,0x000D1F00);
//	REG_DDRPHY_WRITE(0,0x0B91,0x000D1F00);

	flag=0;

//////////////////////
//	PutStr("Freq 0 to 1",1);
//	PLL3_FREQ(0,1);

	flag |=PLL3_FREQ_01();
	dsb_sev();

	if (flag) return (0);

	dsb_sev();

//////////////////////
//	PutStr("Freq 1 to 2",1);
	flag |=PLL3_FREQ(1,2);
	dsb_sev();

	if (flag) return (0);

	SoftDelay_ddr(2000000);

	if (flag) {
		return (0);
	}

	SoftDelay_ddr(40000);

	retry=10000;
	flag=0;
	while(retry>0)
	{
		for(phychno=0;phychno<DRAM_CH_CNT;phychno++)
		{
			if(!(phytrainingok&(1<<phychno)))continue;
			if(flag & (1<<phychno))continue;
//	{	0x02A9 ,	0x00000000	} ,	// PI_INT_STATUS:RD:8:18:=0x000000 RESERVED:RW_D:0:4:=0x00
			dataL = REG_DDRPHY_READ ( phychno, 0x02A9) >> 8 ;	//PI_INT_STATUS
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


	for(phychno=0;phychno<DRAM_CH_CNT;phychno++)
	{
//	{	0x02A9 ,	0x00000000	} ,	// PI_INT_STATUS:RD:8:18:=0x000000 RESERVED:RW_D:0:4:=0x00
//	{	0x02AA ,	0x00000000	} ,	// PI_INT_ACK:WR:0:17:=0x000000
		dataL = REG_DDRPHY_READ ( phychno, 0x02A9) >> 8;
		REG_DDRPHY_WRITE( phychno, 0x02AA, dataL);
	}

	return phytrainingok;
}

//----------------------------------------------------------------------------

void InitDDR_start_testtesttest(uint32_t freq)
{
	uint32_t	i=0;
//	uint32_t	j=0;
	uint32_t	k=0;
	uint32_t	dataL=0;
	uint32_t	phytrainingok=0x0;
	uint32_t	retry;
	uintptr_t	ch,slice;

	//DDR RESET ASSERT
	*((volatile uint32_t*)DBSC_DBPDCNT0_0) = 0x01;
	*((volatile uint32_t*)DBSC_DBPDCNT0_1) = 0x01;

	dsb_sev();

//////////////////////////////////////////

	pvt_dbsc_regset(freq);

//////////////////////////////////////////

	//DDR RESET NEGATE
	*((volatile uint32_t*)DBSC_DBPDCNT0_0) = 0x00;
	*((volatile uint32_t*)DBSC_DBPDCNT0_1) = 0x00;

	//(DDRPHY Kick Initialize Sequence)
	*((volatile uint32_t*)DBSC_DBPDLK_0) = 0x0000A55A;	//Unlock DDRPHY register(AGAIN)
	*((volatile uint32_t*)DBSC_DBPDLK_1) = 0x0000A55A;

//VREF cap
//	REG_DDRPHY_WRITE(0,0x1010,0x0000);
//	REG_DDRPHY_WRITE(1,0x1010,0x0000);

	//(PHY parameter and PI parameter Initialize)	Initialize Leveling parameter including WDQLVL
	for( i=0; i<DDR_PHY_REGSET_NUM; i++ )
	{
		if(DDR_PHY_REGSET[i][0]== 0xFFFFFFFF) {
			return ;
		}
		REG_DDRPHY_WRITE(0,DDR_PHY_REGSET[i][0],DDR_PHY_REGSET[i][1]);
		REG_DDRPHY_WRITE(1,DDR_PHY_REGSET[i][0],DDR_PHY_REGSET[i][1]);
	}
	dsb_sev();
	if(DDR_PHY_REGSET[i][0]!= 0xFFFFFFFF) {
//		PutHex("PHY Reg NUM Error (small):",DDR_PHY_REGSET[i][0],4,1);
		return ;
	}

	for( i=0; i<DDR_PI_REGSET_NUM; i++ )
	{
		if(DDR_PI_REGSET[i][0]== 0xFFFFFFFF) {
//			PutStr("PI Reg NUM Error",1);
			return ;
		}
		REG_DDRPHY_WRITE(0,DDR_PI_REGSET[i][0],DDR_PI_REGSET[i][1]);
		REG_DDRPHY_WRITE(1,DDR_PI_REGSET[i][0],DDR_PI_REGSET[i][1]);
	}
	dsb_sev();
	if(DDR_PI_REGSET[i][0]!= 0xFFFFFFFF) {
//		PutStr("PI Reg NUM Error",1);
		return ;
	}

	for( i=0; i<DDR_BOARD_REGSET_NUM; i++ )
	{
		if(DDR_BOARD_SWAP_CH0[i][0]== 0xFFFFFFFF) {
//			PutStr("SWAP_CH0 Reg NUM Error",1);
			return ;
		}
		if(DDR_BOARD_SWAP_CH1[i][0]== 0xFFFFFFFF) {
//			PutStr("SWAP_CH1 Reg NUM Error",1);
			return ;
		}
		REG_DDRPHY_WRITE(0,DDR_BOARD_SWAP_CH0[i][0],DDR_BOARD_SWAP_CH0[i][1]);
		REG_DDRPHY_WRITE(1,DDR_BOARD_SWAP_CH1[i][0],DDR_BOARD_SWAP_CH1[i][1]);
	}
	if(DDR_BOARD_SWAP_CH0[i][0]!= 0xFFFFFFFF) {
//		PutStr("SWAP_CH0 Reg NUM Error",1);
		return ;
	}
	if(DDR_BOARD_SWAP_CH1[i][0]!= 0xFFFFFFFF) {
//		PutStr("SWAP_CH1 Reg NUM Error",1);
		return ;
	}
/////////
#ifdef	debug_cadelay
	uint32_t	f,DDR_CAD,DDR_GRP_A,DDR_GRP_B,freq_sel_index,multicast_en;
// debug code
//	dataL =	*((volatile uint32_t*)0xE633800C);
//	if((dataL&0xFFFF0000) == 0xA5A50000){
//		DDR_CAD = dataL&0x0FFF;
//		DDR_GRP_A = dataL&0x0FFF;
//		DDR_GRP_B = dataL&0x0FFF;
//		freq_sel_index = dataL&0x0C000 >> 10;
//		multicast_en   = dataL&0x01000 >> 12;
//	}else {
		DDR_CAD = 0x0240;
		DDR_GRP_A = 0x0240;;
		DDR_GRP_B = 0x0240;;
		freq_sel_index = 0x00;
		multicast_en   = 0x01;
//	}

	{
		for (i=0;i<DRAM_CH_CNT;i++)
		{
			freq_sel_index = REG_DDRPHY_READ(i,0x0B80) & 0x300 ;
			for(f=0;f<3;f++)
			{
//	{	0x0B80 ,	0x00000001	} ,	// PHY_SW_GRP_SHIFT_1:RW+:24:5:=0x00 PHY_SW_GRP_SHIFT_0:RW+:16:5:=0x00 PHY_FREQ_SEL_INDEX:RW+:8:2:=0x00 PHY_FREQ_SEL_MULTICAST_EN:RW+:0:1:=0x01
				REG_DDRPHY_WRITE(i,0x0B80,dataL=((REG_DDRPHY_READ(i,0x0B80) & 0xfffffcfe) | (f<<8)) );

//	{	0x0A9F ,	0x0002406E	} ,	// PHY_ADR0_CLK_WR_SLAVE_DELAY_1:RW+:8:11:=0x0300 PHY_ADR_TSEL_SELECT_1:RW+:0:8:=0x55
//	{	0x0AA0 ,	0x02400240	} ,	// PHY_ADR2_CLK_WR_SLAVE_DELAY_1:RW+:16:11:=0x0300 PHY_ADR1_CLK_WR_SLAVE_DELAY_1:RW+:0:11:=0x0300
//	{	0x0AA1 ,	0x02400240	} ,	// PHY_ADR4_CLK_WR_SLAVE_DELAY_1:RW+:16:11:=0x0300 PHY_ADR3_CLK_WR_SLAVE_DELAY_1:RW+:0:11:=0x0300
//	{	0x0AA2 ,	0x00000240	} ,	// PHY_ADR_SW_MASTER_MODE_1:RW+:16:4:=0x00 PHY_ADR5_CLK_WR_SLAVE_DELAY_1:RW+:0:11:=0x0300
				for(slice=0;slice<3;slice++)
				{
					REG_DDRPHY_WRITE(i,0x0A1F+0x80*slice,0x0000006E    | (DDR_CAD<<8) );
					REG_DDRPHY_WRITE(i,0x0A20+0x80*slice,(DDR_CAD<<16) | (DDR_CAD<<0) );
					REG_DDRPHY_WRITE(i,0x0A21+0x80*slice,(DDR_CAD<<16) | (DDR_CAD<<0) );
					REG_DDRPHY_WRITE(i,0x0A22+0x80*slice,(DDR_CAD<<0) );
				}

//	{	0x0B95 ,	0x00024000	} ,	// PHY_GRP_SLAVE_DELAY_0:RW+:8:11:=0x0300 PHY_SET_DFI_INPUT_3:RW:0:4:=0x00
//	{	0x0B96 ,	0x02400240	} ,	// PHY_GRP_SLAVE_DELAY_2:RW+:16:11:=0x0300 PHY_GRP_SLAVE_DELAY_1:RW+:0:11:=0x0300
//	{	0x0B97 ,	0x00000240	} ,	// PHY_GRP_SLAVE_DELAY_3:RW+:0:11:=0x0300

				REG_DDRPHY_WRITE(i,0x0B95, (DDR_GRP_A<< 8) );
				REG_DDRPHY_WRITE(i,0x0B96, (DDR_GRP_B<<16) | (DDR_GRP_A<<0) );
				REG_DDRPHY_WRITE(i,0x0B97, (DDR_GRP_B<< 0) );
			}
			REG_DDRPHY_WRITE(i,0x0B80,dataL=((REG_DDRPHY_READ(i,0x0B80) & 0xfffffcfe) |freq_sel_index| multicast_en) );
		}
		ddrphy_regif_wa();	// REG-IF:W/A
	}

#endif		//	debug_cadelay

/////////////
	dsb_sev();

//	PutStr("RANK setting",1);
	for(i=0;i<DRAM_CH_CNT;i++)
	{
//	{	0x00000229, 0x05000101}, 		// PI_CS_MAP:RW:24:4:=0x0f RESERVED:RW:16:1:=0x00 PI_SW_RST_N:RW_D:8:1:=0x01 PI_WDT_DISABLE:RW_D:0:1:=0x01
//	{	0x0229 ,	0x0F000101	} ,	// PI_CS_MAP:RW:24:4:=0x0f RESERVED:RW:16:1:=0x00 PI_SW_RST_N:RW_D:8:1:=0x01 PI_WDT_DISABLE:RW_D:0:1:=0x01
		REG_DDRPHY_WRITE(i,0x0229,((REG_DDRPHY_READ(i,0x0229) & 0x00ffffff) | ((DDR_CSMAP & 0x0f) << 24)));

//	{	0x00000270, 0x0F050000}, 		// PI_WDQLVL_VREF_INITIAL_START_POINT:RW:24:7:=0x1a PI_WDQLVL_CS_MAP:RW:16:4:=0x01 PI_WDQLVL_ROTATE:RW:8:1:=0x00 PI_WDQLVL_RESP_MASK:RW:0:4:=0x00
//	{	0x0272 ,	0x1A0F0000	} ,	// PI_WDQLVL_VREF_INITIAL_START_POINT:RW:24:7:=0x1a PI_WDQLVL_CS_MAP:RW:16:4:=0x0f PI_WDQLVL_ROTATE:RW:8:1:=0x00 PI_WDQLVL_RESP_MASK:RW:0:4:=0x00
		REG_DDRPHY_WRITE(i,0x0272,((REG_DDRPHY_READ(i,0x0272) & 0xff00ffff) | ((DDR_CSMAP_WDQLVL & 0x0f) << 16  )));

//	{	0x0000024B, 0x001E0101}, 		// PI_TDFI_RDLVL_RR:RW:16:10:=0x001e PI_RDLVL_GATE_CS_MAP:RW:8:4:=0x03 PI_RDLVL_CS_MAP:RW:0:4:=0x03
//	{	0x024C ,	0x00030300	} ,	// PI_RDLVL_GATE_CS_MAP:RW:16:4:=0x03 PI_RDLVL_CS_MAP:RW:8:4:=0x03 PI_RDLVL_GATE_ROTATE:RW:0:1:=0x00
		REG_DDRPHY_WRITE(i,0x024C,((REG_DDRPHY_READ(i,0x024C) & 0xff00ffff) | ((DDR_CSMAP_GTLVL & 0x0f) << 16 )));
		REG_DDRPHY_WRITE(i,0x024C,((REG_DDRPHY_READ(i,0x024C) & 0xffff00ff) | ((DDR_CSMAP_RDLVL & 0x0f) << 8 )));

//	{	0x0000023C, 0x00010000}, 		// PI_WRLVL_ERROR_STATUS:RD:24:2:=0x00 PI_WRLVL_CS_MAP:RW:16:4:=0x03 PI_WRLVL_ROTATE:RW:8:1:=0x00 PI_WRLVL_RESP_MASK:RW:0:4:=0x00
//	{	0x023D ,	0x00030000	} ,	// PI_WRLVL_ERROR_STATUS:RD:24:2:=0x00 PI_WRLVL_CS_MAP:RW:16:4:=0x03 PI_WRLVL_ROTATE:RW:8:1:=0x00 PI_WRLVL_RESP_MASK:RW:0:4:=0x00
		REG_DDRPHY_WRITE(i,0x023D,((REG_DDRPHY_READ(i,0x023D) & 0xff00ffff) | ((DDR_CSMAP_WRLVL & 0x0f) << 16 )));

//		REG_DDRPHY_WRITE(i,0x703 ,((REG_DDRPHY_READ(i,0x703 ) & 0xffff00ff) | ((DDR_CSMAP_CSLVL & 0xff) << 8 )));
//		REG_DDRPHY_WRITE(i,0x0259,((REG_DDRPHY_READ(i,0x0259) & 0xffffff00) | ((DDR_CSMAP_CALVL & 0x03) << 0 ))); // TMP
		REG_DDRPHY_WRITE(i,0x0259,((REG_DDRPHY_READ(i,0x0259) & 0xffffff00) | ((DDR_CSMAP_CALVL & 0x0f) << 0 ))); // OK
	}
	ddrphy_regif_wa();	// REG-IF:W/A

////////////////////////////////////////////////////////////////////////////////////////
// PLL3 setting

//	PutStr("PLL3 Disable",1);
//	//PLL3 : Disable(bypass)
	*((volatile uint32_t*)CPG_CPGWPR)		= ~(0x001f0017);
	*((volatile uint32_t*)CPG_PLLECR)		=   0x001f0017 ;
	dsb_sev();

//	// FREQ = 0, RATIO = 01, init_start = 0
	*((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x00000F10;	//dbdficnt0:dfi_dram_clk_disable=1, DFIFREQUENCY = 0, freq_ratio = 01 (2:1), init_start =0
	*((volatile uint32_t*)DBSC_DBDFICNT_1) = 0x00000F10;
	dsb_sev();
	

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for(ch=0;ch<DRAM_CH_CNT;ch+=1)
	{


//	{	// ON_FLY_GATE_ADJUST_EN_0:RW:16:2:=0x03 PHY_GATE_SMPL2_SLAVE_DELAY_0:RW:0:9:=0x0066
//	{	0x0000040B, 0x00030066	} , // ON_FLY_GATE_ADJUST_EN_0:RW:16:2:=0x03 PHY_GATE_SMPL2_SLAVE_DELAY_0:RW:0:9:=0x0066
//	{	0x080C ,	0x00030066	} ,	// ON_FLY_GATE_ADJUST_EN_0:RW:16:2:=0x03 PHY_GATE_SMPL2_SLAVE_DELAY_0:RW:0:9:=0x0066
		for(slice=0;slice<4;slice++)
		{
			REG_DDRPHY_WRITE(ch,0x080C+0x80*slice,REG_DDRPHY_READ(i,0x080C) & (0xfffcffff));
		}

/////////////////////////////////////////////////
// PI_WRLAT debug

//	{	0x022B ,	0x00000000|(PI_WRLAT<<24)|(0x0C<<16)|(0x04<<0)	} ,	// PI_WRLAT_F1:RW:24:5:=0x06 PI_CASLAT_LIN_F0:RW:16:7:=0x0c PI_ADDITIVE_LAT_F0:RW:8:6:=0x00 PI_WRLAT_F0:RW:0:5:=0x04
//	{	0x022C ,	0x00000000|(PI_WRLAT<<16)|(PI_CASLAT_LIN<<8)	} ,	// PI_ADDITIVE_LAT_F2:RW:24:6:=0x00 PI_WRLAT_F2:RW:16:5:=0x0e PI_CASLAT_LIN_F1:RW:8:7:=0x14 PI_ADDITIVE_LAT_F1:RW:0:6:=0x00
//	{	0x0255, (PI_WRLAT_ADJ<<16)|(PI_WRLAT_ADJ<<8)|(PI_WRLAT_ADJ<<0)}, // PI_TDFI_PHY_WRLAT:RD:24:7:=0x00 PI_WRLAT_ADJ_F2:RW:16:7:=0x0c PI_WRLAT_ADJ_F1:RW:8:7:=0x0c PI_WRLAT_ADJ_F0:RW:0:7:=0x02

// debug code
//		dataL =	*((volatile uint32_t*)0xE633801C);
//		if((dataL&0xFFFF0000) == 0xA55A0000){
//			REG_DDRPHY_WRITE(ch,0x022B ,((dataL & 0xFF)<<24)|(0x0C<<16)|(0x04<<0));	// PI_WRLAT_F1:RW:24:5:=0x06 PI_CASLAT_LIN_F0:RW:16:7:=0x0c PI_ADDITIVE_LAT_F0:RW:8:6:=0x00 PI_WRLAT_F0:RW:0:5:=0x04
//			REG_DDRPHY_WRITE(ch,0x022C ,((dataL & 0xFF)<<16)|(PI_CASLAT_LIN<<8));	// PI_ADDITIVE_LAT_F2:RW:24:6:=0x00 PI_WRLAT_F2:RW:16:5:=0x0e PI_CASLAT_LIN_F1:RW:8:7:=0x14 PI_ADDITIVE_LAT_F1:RW:0:6:=0x00
//			REG_DDRPHY_WRITE(ch,0x0255 ,(((dataL & 0xFF00) >> 8)<<16)|(((dataL & 0xFF00) >> 8)<<8)|(((dataL & 0xFF00) >> 8)<<0)); // PI_TDFI_PHY_WRLAT:RD:24:7:=0x00 PI_WRLAT_ADJ_F2:RW:16:7:=0x0c PI_WRLAT_ADJ_F1:RW:8:7:=0x0c PI_WRLAT_ADJ_F0:RW:0:7:=0x02
//		}

//	M3 new swizzling settings

//	{	0x080F ,	0x5a365a3c	} ,	// PHY_LP4_RDLVL_PATT8_0:RW:0:32:=0x00000000
//	{	0x088F ,	0x5a365a3c	} ,	// PHY_LP4_RDLVL_PATT8_1:RW:0:32:=0x00000000
//	{	0x090F ,	0xaac55a3c	} ,	// PHY_LP4_RDLVL_PATT8_2:RW:0:32:=0x00000000
//	{	0x098F ,	0xaac55a3c	} ,	// PHY_LP4_RDLVL_PATT8_3:RW:0:32:=0x00000000

// debug code
//		dataL =	*((volatile uint32_t*)0xE6338010);
//		if((dataL) == 0xA55A){
//			dataL =	*((volatile uint32_t*)0xE6338014);
//			REG_DDRPHY_WRITE(ch,0x080F ,	0x5a3c|(dataL&0xFFFF)		);	// PHY_LP4_RDLVL_PATT8_0:RW:0:32:=0x00000000
//			REG_DDRPHY_WRITE(ch,0x088F ,	0x5a3c|(dataL&0xFFFF)		);	// PHY_LP4_RDLVL_PATT8_1:RW:0:32:=0x00000000
//			REG_DDRPHY_WRITE(ch,0x090F ,	0x5a3c|((dataL>>16)&0xFFFF)	);	// PHY_LP4_RDLVL_PATT8_2:RW:0:32:=0x00000000
//			REG_DDRPHY_WRITE(ch,0x098F ,	0x5a3c|((dataL>>16)&0xFFFF)	);	// PHY_LP4_RDLVL_PATT8_3:RW:0:32:=0x00000000
//		}

//	{	0x0252 ,	0x01080000	} ,	// PI_RDLVL_PATTERN_NUM:RW:24:4:=0x01 PI_RDLVL_PATTERN_START:RW:16:4:=0x00 PI_RDLVL_GATE_INTERVAL:RW:0:16:=0x0000
// debug code
//		dataL =	*((volatile uint32_t*)0xE6338018);
//		if((dataL&0xFFFF0000) == 0xA55A0000){
//			REG_DDRPHY_WRITE(ch,0x0252 ,	0x01000000|((dataL&0xF)<<16)		);	// PHY_LP4_RDLVL_PATT8_0:RW:0:32:=0x00000000
//		}


#ifdef DEBUG_WDQSWA7
//	{	0x084D ,	0x01D00004	} ,	// PHY_WRLVL_DELAY_EARLY_THRESHOLD_0:RW+:16:10:=0x01D0 PHY_WRITE_PATH_LAT_ADD_0:RW+:8:3:=0x00 PHY_RDDQS_LATENCY_ADJUST_0:RW+:0:4:=0x04
//	{	0x084E ,	0x00010000	} ,	// PHY_WRLVL_EARLY_FORCE_ZERO_0:RW+:16:1:=0x00 PHY_WRLVL_DELAY_PERIOD_THRESHOLD_0:RW+:0:10:=0x0000
		for(slice=0;slice<4;slice++)
		{
//			REG_DDRPHY_WRITE(ch,0x084D +0x80*slice,	0x01C00004	);
			REG_DDRPHY_WRITE(ch,0x084D +0x80*slice,	0x01800004	);
			REG_DDRPHY_WRITE(ch,0x084E +0x80*slice,	0x00010000	);
		}



#endif

///////////////////////////////////////////////////
#ifdef	debug_term_over_pre
///// TERM over write
		// TERM 10F68(PVTR=10,PVTN=3D,PVTP=28) -> 1154f(PVTR=11,PVTN=15,PVTP=0f
// debug code
//		dataL =	*((volatile uint32_t*)0xE6338008);
//		if((dataL&0xFFFF0000) == 0xA55A0000){
//			dataL =	*((volatile uint32_t*)0xE6338008);
//			dataL = ((dataL & 0xFF)<<12	) |0x0000054F;
//		} else {
			dataL = 0x0001154F;
//			dataL = 0x0001F54F;
//		}

		REG_DDRPHY_WRITE(ch,0x0BA2 ,	dataL|0x00020000	);	// PHY_PAD_FDBK_TERM:RW+:0:18:=0x004410
//		REG_DDRPHY_WRITE(ch,0x0BA3 ,	dataL	);	// PHY_PAD_DATA_TERM:RW+:0:17:=0x004410
		REG_DDRPHY_WRITE(ch,0x0BA3 ,	dataL|0x00020000	);	// PHY_PAD_DATA_TERM:RW+:0:17:=0x004410
		REG_DDRPHY_WRITE(ch,0x0BA4 ,	dataL	);	// PHY_PAD_DQS_TERM:RW+:0:17:=0x004410
		REG_DDRPHY_WRITE(ch,0x0BA5 ,	dataL	);	// PHY_PAD_ADDR_TERM:RW+:0:18:=0x004410
		REG_DDRPHY_WRITE(ch,0x0BA6 ,	dataL	);	// PHY_PAD_CLK_TERM:RW+:0:18:=0x004410
		REG_DDRPHY_WRITE(ch,0x0BA8 ,	dataL	);	// PHY_PAD_CKE_TERM:RW+:0:18:=0x004410
		REG_DDRPHY_WRITE(ch,0x0BAA ,	dataL	);	// PHY_PAD_RST_TERM:RW+:0:18:=0x004410
		REG_DDRPHY_WRITE(ch,0x0BAC ,	dataL	);	// PHY_PAD_CS_TERM:RW+:0:18:=0x004410
		*((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x040*ch)) = 0x02A9;


//		dataL = 0x0C710C71;	//0x31,0x31
//		dataL = 0x0C600C60;	//0x31/0x20
//		dataL = 0x08310831;	//0x20/0x31
//		dataL = ((0x31 <<6)| (0x31)) * 0x10001;
		dataL = ((0x20 <<6)| (0x20)) * 0x10001;

		for(slice=0;slice<4;slice++)
		{
			REG_DDRPHY_WRITE(ch,0x0834 +0x80*slice,	dataL	);	// PHY_RX_CAL_DQ1_0:RW+:16:12:=0x0000 PHY_RX_CAL_DQ0_0:RW+:0:12:=0x0000
			REG_DDRPHY_WRITE(ch,0x0835 +0x80*slice,	dataL	);	// PHY_RX_CAL_DQ3_0:RW+:16:12:=0x0000 PHY_RX_CAL_DQ2_0:RW+:0:12:=0x0000
			REG_DDRPHY_WRITE(ch,0x0836 +0x80*slice,	dataL	);	// PHY_RX_CAL_DQ5_0:RW+:16:12:=0x0000 PHY_RX_CAL_DQ4_0:RW+:0:12:=0x0000
			REG_DDRPHY_WRITE(ch,0x0837 +0x80*slice,	dataL	);	// PHY_RX_CAL_DQ7_0:RW+:16:12:=0x0000 PHY_RX_CAL_DQ6_0:RW+:0:12:=0x0000
			REG_DDRPHY_WRITE(ch,0x0838 +0x80*slice,	dataL	);	// PHY_RX_CAL_DQS_0:RW+:16:12:=0x0000 PHY_RX_CAL_DM_0:RW+:0:12:=0x0000
			REG_DDRPHY_WRITE(ch,0x0839 +0x80*slice,	dataL	);	// PHY_RX_CAL_OBS_0:RD:16:11:=0x0000 PHY_RX_CAL_FDBK_0:RW+:0:12:=0x0000
		}

#endif


		REG_DDRPHY_WRITE(ch,0x0B80 ,	0x00000100);	// PHY_SW_GRP_SHIFT_1:RW+:24:5:=0x00 PHY_SW_GRP_SHIFT_0:RW+:16:5:=0x00 PHY_FREQ_SEL_INDEX:RW+:8:2:=0x00 PHY_FREQ_SEL_MULTICAST_EN:RW+:0:1:=0x01
		for(slice=0;slice<4;slice++)
		{
			REG_DDRPHY_WRITE(ch,0x083A +slice*0x80 ,	0x03000300);	// PHY_CLK_WRDQ1_SLAVE_DELAY_0:RW+:16:11:=0x0280 PHY_CLK_WRDQ0_SLAVE_DELAY_0:RW+:0:11:=0x0280
			REG_DDRPHY_WRITE(ch,0x083B +slice*0x80 ,	0x03000300);	// PHY_CLK_WRDQ3_SLAVE_DELAY_0:RW+:16:11:=0x0280 PHY_CLK_WRDQ2_SLAVE_DELAY_0:RW+:0:11:=0x0280
			REG_DDRPHY_WRITE(ch,0x083C +slice*0x80 ,	0x03000300);	// PHY_CLK_WRDQ5_SLAVE_DELAY_0:RW+:16:11:=0x0280 PHY_CLK_WRDQ4_SLAVE_DELAY_0:RW+:0:11:=0x0280
			REG_DDRPHY_WRITE(ch,0x083D +slice*0x80 ,	0x03000300);	// PHY_CLK_WRDQ7_SLAVE_DELAY_0:RW+:16:11:=0x0280 PHY_CLK_WRDQ6_SLAVE_DELAY_0:RW+:0:11:=0x0280
			REG_DDRPHY_WRITE(ch,0x083E +slice*0x80 ,	0x00000300);	// PHY_CLK_WRDQS_SLAVE_DELAY_0:RW+:16:10:=0x0000 PHY_CLK_WRDM_SLAVE_DELAY_0:RW+:0:11:=0x0280

			REG_DDRPHY_WRITE(ch,0x0843 +slice*0x80 ,	0x00A00000);	// PHY_RDDQS_DQ0_RISE_SLAVE_DELAY_0:RW+:16:10:=0x00A0 PHY_RDDM_SLAVE_DELAY_0:RW+:0:10:=0x0000
			REG_DDRPHY_WRITE(ch,0x0844 +slice*0x80 ,	0x00A000A0);	// PHY_RDDQS_DQ1_RISE_SLAVE_DELAY_0:RW+:16:10:=0x00A0 PHY_RDDQS_DQ0_FALL_SLAVE_DELAY_0:RW+:0:10:=0x00a0
			REG_DDRPHY_WRITE(ch,0x0845 +slice*0x80 ,	0x00A000A0);	// PHY_RDDQS_DQ2_RISE_SLAVE_DELAY_0:RW+:16:10:=0x00A0 PHY_RDDQS_DQ1_FALL_SLAVE_DELAY_0:RW+:0:10:=0x00a0
			REG_DDRPHY_WRITE(ch,0x0846 +slice*0x80 ,	0x00A000A0);	// PHY_RDDQS_DQ3_RISE_SLAVE_DELAY_0:RW+:16:10:=0x00A0 PHY_RDDQS_DQ2_FALL_SLAVE_DELAY_0:RW+:0:10:=0x00a0
			REG_DDRPHY_WRITE(ch,0x0847 +slice*0x80 ,	0x00A000A0);	// PHY_RDDQS_DQ4_RISE_SLAVE_DELAY_0:RW+:16:10:=0x00A0 PHY_RDDQS_DQ3_FALL_SLAVE_DELAY_0:RW+:0:10:=0x00a0
			REG_DDRPHY_WRITE(ch,0x0848 +slice*0x80 ,	0x00A000A0);	// PHY_RDDQS_DQ5_RISE_SLAVE_DELAY_0:RW+:16:10:=0x00A0 PHY_RDDQS_DQ4_FALL_SLAVE_DELAY_0:RW+:0:10:=0x00a0
			REG_DDRPHY_WRITE(ch,0x0849 +slice*0x80 ,	0x00A000A0);	// PHY_RDDQS_DQ6_RISE_SLAVE_DELAY_0:RW+:16:10:=0x00A0 PHY_RDDQS_DQ5_FALL_SLAVE_DELAY_0:RW+:0:10:=0x00a0
			REG_DDRPHY_WRITE(ch,0x084A +slice*0x80 ,	0x00A000A0);	// PHY_RDDQS_DQ7_RISE_SLAVE_DELAY_0:RW+:16:10:=0x00A0 PHY_RDDQS_DQ6_FALL_SLAVE_DELAY_0:RW+:0:10:=0x00a0
			REG_DDRPHY_WRITE(ch,0x084B +slice*0x80 ,	0x00A000A0);	// PHY_RDDQS_DM_RISE_SLAVE_DELAY_0:RW+:16:10:=0x00A0 PHY_RDDQS_DQ7_FALL_SLAVE_DELAY_0:RW+:0:10:=0x00a0
			REG_DDRPHY_WRITE(ch,0x084C +slice*0x80 ,	0x010900A0);	// PHY_RDDQS_GATE_SLAVE_DELAY_0:RW+:16:10:=0x0109 PHY_RDDQS_DM_FALL_SLAVE_DELAY_0:RW+:0:10:=0x00a0

		}
/*
		for(slice=0;slice<3;slice++)
		{
			REG_DDRPHY_WRITE(ch,0x0A1F +slice*0x80 ,	0x0002406E);	// PHY_ADR0_CLK_WR_SLAVE_DELAY_0:RW+:8:11:=0x0300 PHY_ADR_TSEL_SELECT_0:RW+:0:8:=0x55
			REG_DDRPHY_WRITE(ch,0x0A20 +slice*0x80 ,	0x02400240);	// PHY_ADR2_CLK_WR_SLAVE_DELAY_0:RW+:16:11:=0x0300 PHY_ADR1_CLK_WR_SLAVE_DELAY_0:RW+:0:11:=0x0300
			REG_DDRPHY_WRITE(ch,0x0A21 +slice*0x80 ,	0x02400240);	// PHY_ADR4_CLK_WR_SLAVE_DELAY_0:RW+:16:11:=0x0300 PHY_ADR3_CLK_WR_SLAVE_DELAY_0:RW+:0:11:=0x0300
			REG_DDRPHY_WRITE(ch,0x0A22 +slice*0x80 ,	0x00000240);	// PHY_ADR_SW_MASTER_MODE_0:RW+:16:4:=0x00 PHY_ADR5_CLK_WR_SLAVE_DELAY_0:RW+:0:11:=0x0300
			REG_DDRPHY_WRITE(ch,0x0A23 +slice*0x80 ,	0x42080010);	// PHY_ADR_MASTER_DELAY_WAIT_0:RW+:24:8:=0x42 PHY_ADR_MASTER_DELAY_STEP_0:RW+:16:6:=0x08 PHY_ADR_MASTER_DELAY_START_0:RW+:0:10:=0x0010
		}
*/
		REG_DDRPHY_WRITE(ch,0x0B80 ,	0x00000001);	// PHY_SW_GRP_SHIFT_1:RW+:24:5:=0x00 PHY_SW_GRP_SHIFT_0:RW+:16:5:=0x00 PHY_FREQ_SEL_INDEX:RW+:8:2:=0x00 PHY_FREQ_SEL_MULTICAST_EN:RW+:0:1:=0x01
		ddrphy_regif_wa();	// REG-IF:W/A
	}

#ifdef	DDR_PAD_CAL_IEWA
//	PutStr("PAD CAL IE W/A : IE on",1);
	// Rx Cal IE issue w/a : IE=on
	for(ch=0;ch<DRAM_CH_CNT;ch++)
	{
		for(slice=0;slice<4;slice++)
		{
//	{	0x0853 ,	0x0E0000C0	} ,	// PHY_RDDATA_EN_DLY_0:RW+:24:4:=0x0e PHY_IE_MODE_0:RW+:16:2:=0x00 PHY_RDDATA_EN_IE_DLY_0:RW+:8:2:=0x00 PHY_DQS_IE_TIMING_0:RW+:0:8:=0xc0
//			REG_DDRPHY_WRITE(ch,0x0853+0x80*slice,REG_DDRPHY_READ(ch,0x0853+0x80*slice) | (1<<16));	//IE_on
			REG_DDRPHY_WRITE(ch,0x0853+0x80*slice,0x0E0100C0);	//IE_on
			*((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x040*ch)) = 0x02A9;
		}
	}
#endif

	dsb_sev();

	ddrphy_regif_wa();	// REG-IF:W/A

	*((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x00000F10;	//dbdficnt0: DFIFREQUENCY = 0, freq_ratio = 01 (2:1), init_start =0
	*((volatile uint32_t*)DBSC_DBDFICNT_1) = 0x00000F10;
	dsb_sev();

	*((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x00000F11;	//dbdficnt0: DFIFREQUENCY = 0, freq_ratio = 01 (2:1), init_start =1
	*((volatile uint32_t*)DBSC_DBDFICNT_1) = 0x00000F11;
	dsb_sev();

	*((volatile uint32_t*)DBSC_DBPDCNT_0)	= 0x0000CF01;	//dll_rst_n0 -> 1
	*((volatile uint32_t*)DBSC_DBPDCNT_1)	= 0x0000CF01;
	dsb_sev();


///////////////////	Wait InitCOMP	/////////////////////////////////////////
	for(phytrainingok=0,k=0,ch=0;ch<DRAM_CH_CNT;ch++)
	{
		if(!(DDR_PHYVALID&(1<<ch)))continue;
		{
			retry=0;
			while(retry<2048)
			{
				dataL = *((volatile uint32_t*)(DBSC_INITCOMP_0+ch*0x40));	//Wait for DBSC_INITCOMP_0[0] is 1
				if((dataL & 0x00000001) == 0x1)	break;
				retry++;
			}
			if(retry<2048)
			{
				phytrainingok|=(1<<ch);
			}
			else
			{
				k|=(1<<ch);
			}
		}
	}

////////////////////////////////////////////////////////////////////////////////
#ifdef	DDR_PAD_CAL_IEWA
	// Rx Cal IE issue w/a : IE=off 
	for(ch=0;ch<DRAM_CH_CNT;ch++)
	{
		for(slice=0;slice<4;slice++)
		{
//	{	0x0853 ,	0x0E0000C0	} ,	// PHY_RDDATA_EN_DLY_0:RW+:24:4:=0x0e PHY_IE_MODE_0:RW+:16:2:=0x00 PHY_RDDATA_EN_IE_DLY_0:RW+:8:2:=0x00 PHY_DQS_IE_TIMING_0:RW+:0:8:=0xc0
			REG_DDRPHY_WRITE(ch,0x0853+0x80*slice,0x0E0000C0);	//IE_off
			*((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x040*ch)) = 0x02A9;
		}
	}
#endif

////////////////////////////////////////////////////////////////////////////////
	*((volatile uint32_t*)DBSC_DBDFICNT_0) = 0x00000010;	//dbdficnt0: DFIFREQUENCY = 0, freq_ratio = 01 (2:1), init_start =0
	*((volatile uint32_t*)DBSC_DBDFICNT_1) = 0x00000010;
	dsb_sev();

////////////////////////////////////////////////////////////////////////////////
// Init Comp OK??
	phytrainingok &=~k;
/*
	PutHex("Init Comp OK status: ",phytrainingok,1,1);
	PutHex("CH0 PI_STATUS: ",REG_DDRPHY_READ(0,0x02A9) >> 8,4,1);
	PutHex("CH1 PI_STATUS: ",REG_DDRPHY_READ(1,0x02A9) >> 8,4,1);

	PutHex("Freq_chg_req_0: 0x",(*((volatile uint32_t*)(DBSC_DBPDSTAT_0))& 0x01),1,0);
	PutHex(", type_0: 0x",(*((volatile uint32_t*)(DBSC_DBPDSTAT_0)) >> 8)& 0x0f ,1,0);
	PutHex(", req_1: 0x" ,(*((volatile uint32_t*)(DBSC_DBPDSTAT_1))      & 0x01),1,0);
	PutHex(", type_1: 0x",(*((volatile uint32_t*)(DBSC_DBPDSTAT_0)) >> 8)& 0x0f ,1,1);
*/

	if((phytrainingok&DDR_PHYVALID)!=DDR_PHYVALID)
	{
		return;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef	debug_term_over
///// TERM over write
	for(ch=0;ch<DRAM_CH_CNT;ch+=1)
	{
		// TERM 10F68(PVTR=10,PVTN=3D,PVTP=28) -> 1154f(PVTR=11,PVTN=15,PVTP=0f
// debug code
//		dataL =	*((volatile uint32_t*)0xE6338008);
//		if((dataL&0xFFFF0000) == 0xA55A0000){
//			dataL =	*((volatile uint32_t*)0xE6338008);
//			dataL = ((dataL & 0xFF)<<12	) |0x0000054F;
//		} else {
			dataL = 0x0001154F;
//			dataL = 0x0001F54F;
//		}

		REG_DDRPHY_WRITE(ch,0x0BA2 ,	dataL|0x00020000	);	// PHY_PAD_FDBK_TERM:RW+:0:18:=0x004410
//		REG_DDRPHY_WRITE(ch,0x0BA3 ,	dataL	);	// PHY_PAD_DATA_TERM:RW+:0:17:=0x004410
		REG_DDRPHY_WRITE(ch,0x0BA3 ,	dataL|0x00020000	);	// PHY_PAD_DATA_TERM:RW+:0:17:=0x004410
		REG_DDRPHY_WRITE(ch,0x0BA4 ,	dataL	);	// PHY_PAD_DQS_TERM:RW+:0:17:=0x004410
		REG_DDRPHY_WRITE(ch,0x0BA5 ,	dataL	);	// PHY_PAD_ADDR_TERM:RW+:0:18:=0x004410
		REG_DDRPHY_WRITE(ch,0x0BA6 ,	dataL	);	// PHY_PAD_CLK_TERM:RW+:0:18:=0x004410
		REG_DDRPHY_WRITE(ch,0x0BA8 ,	dataL	);	// PHY_PAD_CKE_TERM:RW+:0:18:=0x004410
		REG_DDRPHY_WRITE(ch,0x0BAA ,	dataL	);	// PHY_PAD_RST_TERM:RW+:0:18:=0x004410
		REG_DDRPHY_WRITE(ch,0x0BAC ,	dataL	);	// PHY_PAD_CS_TERM:RW+:0:18:=0x004410
		*((volatile uint32_t*)(DBSC_DBPDRGA_0 + 0x040*ch)) = 0x02A9;

		dataL = ((0x20 <<6)| (0x20)) * 0x10001;
		for(slice=0;slice<4;slice+=1){
			REG_DDRPHY_WRITE(ch,0x0834 +slice*0x80 ,	dataL	);	// PHY_RX_CAL_DQ1_0:RW+:16:12:=0x0000 PHY_RX_CAL_DQ0_0:RW+:0:12:=0x0000
			REG_DDRPHY_WRITE(ch,0x0835 +slice*0x80 ,	dataL	);	// PHY_RX_CAL_DQ3_0:RW+:16:12:=0x0000 PHY_RX_CAL_DQ2_0:RW+:0:12:=0x0000
			REG_DDRPHY_WRITE(ch,0x0836 +slice*0x80 ,	dataL	);	// PHY_RX_CAL_DQ5_0:RW+:16:12:=0x0000 PHY_RX_CAL_DQ4_0:RW+:0:12:=0x0000
			REG_DDRPHY_WRITE(ch,0x0837 +slice*0x80 ,	dataL	);	// PHY_RX_CAL_DQ7_0:RW+:16:12:=0x0000 PHY_RX_CAL_DQ6_0:RW+:0:12:=0x0000
			REG_DDRPHY_WRITE(ch,0x0838 +slice*0x80 ,	dataL	);	// PHY_RX_CAL_DQS_0:RW+:16:12:=0x0000 PHY_RX_CAL_DM_0:RW+:0:12:=0x0000
			REG_DDRPHY_WRITE(ch,0x0839 +slice*0x80 ,	dataL	);	// PHY_RX_CAL_OBS_0:RD:16:11:=0x0000 PHY_RX_CAL_FDBK_0:RW+:0:12:=0x0000
		}
	}
#endif	//debug_term_over


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// DDR MODE REG setting
//CHANGE IMPEDANCE CODE to CMOS MODE

	change_lpddr4_en(0xf,0);

////////////////////////////////////////////////////////////////////////

	for(ch=0;ch<DRAM_CH_CNT;ch+=1)
	{
		if((DDR_PHYVALID&0x0f)==0x03)
//		if((DDR_PHYVALID&0x0f)==0x0f)
		{
			ch=0x0f;
		}
		else
		{
			if( (DDR_PHYVALID&(1<<ch))==0 )
			{
				continue;
			}
		}

		//(SDRAM Initalize)
		*((volatile uint32_t*)DBSC_DBCMD) = 0x01040001|(0x00100000 * ch);	//RSX chA rkA
		WaitDBCMD();

		*((volatile uint32_t*)DBSC_DBCMD) = 0x08040000|(0x00100000 * ch);	//PDE chA rkA
		WaitDBCMD();

		*((volatile uint32_t*)DBSC_DBCMD) = 0x08040001|(0x00100000 * ch);	//PDX ch0 rk0
		WaitDBCMD();

		*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040200|(0x00100000 * ch);	//MRW chA rkA 02:00
		WaitDBCMD();

		*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040300|(0x00100000 * ch)|DDR_MR3;	//MRW chA rkA 03:31
		WaitDBCMD();

		*((volatile uint32_t*)DBSC_DBCMD) = 0x0e040100|(0x00100000 * ch);	//MRW chA rkA 01:00
		WaitDBCMD();

		if((DDR_PHYMRW & (1<<ch)) || (ch==0x0f))
		{/////////////////////////////////////////////////////////// phymrw
			mode_register_set(freq, ch, 0x40);
			mode_register_set(freq, ch, 0x00);
		} /////////////////////////////////////////////////////////// phymrw

		dsb_sev();

		*((volatile uint32_t*)DBSC_DBCMD) = 0x0d04004F|(0x00100000 * ch);	//MPC chA rkA 4FH (ZQCAL start)
		WaitDBCMD();

		*((volatile uint32_t*)DBSC_DBCMD) = 0x0d040051|(0x00100000 * ch);	//MPC chA rkA 51H (ZQCAL latch)
		WaitDBCMD();
	}

	//CHANGE IMPEDANCE CODE to LPDDR4 MODE

	change_lpddr4_en(0xf,1);


////////////////////////////////////////////////////////////////////////////////////////
//	Training Start
////////////////////////////////////////////////////////////////////////////////////////

	phytrainingok = pvt_pi_training_go_all_testtesttest(freq);


/////////////////////////////////////////////////////////////////////////////////////////////////////////

//		return ;

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
// DDR MODE REG resetting

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
		mode_register_set(freq, ch, 0x40);
		mode_register_set(freq, ch, 0x00);
	}

/////////////////////////////////////////////////////

	*((volatile uint32_t*)DBSC_DBBUS0CNF1)	= 0x70000100;	//dbbus0cnf1
	*((volatile uint32_t*)DBSC_DBBUS0CNF0)	= 0x18010001;	//dbbus0cnf

/////////////////////////////////////////////////////
//Auto Refresh setting
//	if(freq>0x53)		*((volatile uint32_t*)DBSC_DBRFCNF1)	= 0x00081860;			//dbrfcnf1 refpmax=8 refint=6240
	if(freq>0xf)		*((volatile uint32_t*)DBSC_DBRFCNF1)	= 0x00081860;			//dbrfcnf1 refpmax=8 refint=6240
	else if(freq>0x47)	*((volatile uint32_t*)DBSC_DBRFCNF1)	= 0x00081554;			//dbrfcnf1 refpmax=8 refint=5460
	else if(freq>0x2f)	*((volatile uint32_t*)DBSC_DBRFCNF1)	= 0x00081248;			//dbrfcnf1 refpmax=8 refint=4680
	else			*((volatile uint32_t*)DBSC_DBRFCNF1)	= 0x00080C30;			//dbrfcnf1 refpmax=8 refint=3120
				*((volatile uint32_t*)DBSC_DBRFCNF2)	= 0x00010000|DBSC_REFINTS;	//dbrfcnf2 refpmin=1 refints=0

	if (DDR_CSMAP != 0xf) {
		*((volatile uint32_t*)DBSC_DBMEMCONF_0_1)	= 0x00000000;
		*((volatile uint32_t*)DBSC_DBMEMCONF_1_1)	= 0x00000000;
	}

	*((volatile uint32_t*)DBSC_DBMEMCONF_0_2)		= 0x00000000;
	*((volatile uint32_t*)DBSC_DBMEMCONF_0_3)		= 0x00000000;
	*((volatile uint32_t*)DBSC_DBMEMCONF_1_2)		= 0x00000000;
	*((volatile uint32_t*)DBSC_DBMEMCONF_1_3)		= 0x00000000;

	*((volatile uint32_t*)DBSC_DBRFEN)			= 0x00000001;	//dbrfen

//DRAM ACCESS enable
	*((volatile uint32_t*)DBSC_DBACEN) = 0x00000001;	//dbacen

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InitDram_m3_es10(void)
{
//	uint32_t md;
	uint32_t freq;
//	uint32_t i,slice,ch;

////////////////////////////////////////////////////////////////////////////////
// Change PLL3 Freq Setteing for DEBUG
////////////////////////////////////////////////////////////////////////////////

//	md = (*((volatile uint32_t*)RST_MODEMR) & 0x000A0000) >> 17;
//	if (md == 0x0) {
//		freq = DDR3200_CLK;	//MD19=0,MD17=0 : 3200 -> LPDDR4-3200
//	} else if (md == 0x1) {
//		freq = DDR2800_CLK;	//MD19=0,MD17=1 : 2133 -> LPDDR4-2800
//	} else if (md == 0x4) {
//		freq = DDR2400_CLK;	//MD19=1,MD17=0 : 3733 -> LPDDR4-2400
//	} else {
		freq = DDR1600_CLK;	//MD19=1,MD17=1 : 3200 -> LPDDR4-1600
		NOTICE("BL2: DDR1600(%s)\n", RCAR_DDR_VERSION);
//	}

	*((volatile uint32_t*)CPG_CPGWPR)		= ~(freq<<24);
	*((volatile uint32_t*)CPG_PLL3CR)		=   freq<<24 ;
	dsb_sev();

////////////////////////////////////////////////////////////////////////////////
//	Init Start!
////////////////////////////////////////////////////////////////////////////////
	InitDDR_start_testtesttest(freq);

////////////////////////////////////////////////////////////////////////////////
//	Finish!!
////////////////////////////////////////////////////////////////////////////////

//	PHY status Dunmp for DEBUG
/*
	PutStr("CH0",1);
	PutHex("phy_cal_result_obs_0: ",REG_DDRPHY_READ(0,0x0BB4),4,1);
	PutHex("phy_cal_result2_obs_0: ",REG_DDRPHY_READ(0,0x0BB5),4,1);
	PutHex("phy_pad_clk_term: ",REG_DDRPHY_READ(0,0x0BA6),4,1);
	*(volatile uint32_t*)(DBSC_DBPDRGA_0) = 0x02A9;

	PutStr("CH1",1);
	PutHex("phy_cal_result_obs_0: ",REG_DDRPHY_READ(1,0x0BB4),4,1);
	PutHex("phy_cal_result2_obs_0: ",REG_DDRPHY_READ(1,0x0BB5),4,1);
	PutHex("phy_pad_clk_term: ",REG_DDRPHY_READ(1,0x0BA6),4,1);
	*(volatile uint32_t*)(DBSC_DBPDRGA_1) = 0x02A9;

// Rx cal
//	{	0x0834 ,	0x02200220	} ,	// PHY_RX_CAL_DQ1_0:RW+:16:12:=0x0000 PHY_RX_CAL_DQ0_0:RW+:0:12:=0x0000
	PutHex(" PHY_RX_CAL_DQ0_0: up= ",	(REG_DDRPHY_READ(0,0x0834) & 0xfc0)>> 6 ,1,0);
	PutHex(",down= ",					(REG_DDRPHY_READ(0,0x0834) & 0x03f)     ,1,1);
	PutHex(" PHY_RX_CAL_DQ1_0: up= ",(REG_DDRPHY_READ(0,0x0834) & 0xfc00000)>>(6+16) ,1,0);
	PutHex(",down= ",(REG_DDRPHY_READ(0,0x0834) & 0x3f0000)>>16 ,1,1);
	PutHex(" PHY_RX_CAL_DQ2_0: up= ",(REG_DDRPHY_READ(0,0x0835) & 0xfc0)>> 6 ,1,0);
	PutHex(",down= ",(REG_DDRPHY_READ(0,0x0835) & 0x3f) ,1,1);
	PutHex(" PHY_RX_CAL_DQ3_0: up= ",(REG_DDRPHY_READ(0,0x0835) & 0xfc00000)>>(6+16) ,1,0);
	PutHex(",down= ",(REG_DDRPHY_READ(0,0x0835) & 0x3f0000)>>16 ,1,1);
	PutHex(" PHY_RX_CAL_DQ4_0: up= ",(REG_DDRPHY_READ(0,0x0836) & 0xfc0)>> 6 ,1,0);
	PutHex(",down= ",(REG_DDRPHY_READ(0,0x0836) & 0x3f) ,1,1);
	PutHex(" PHY_RX_CAL_DQ5_0: up= ",(REG_DDRPHY_READ(0,0x0836) & 0xfc00000)>>(6+16) ,1,0);
	PutHex(",down= ",(REG_DDRPHY_READ(0,0x0836) & 0x3f0000)>>16 ,1,1);
	PutHex(" PHY_RX_CAL_DQ6_0: up= ",(REG_DDRPHY_READ(0,0x0837) & 0xfc0)>> 6 ,1,0);
	PutHex(",down= ",(REG_DDRPHY_READ(0,0x0837) & 0x3f) ,1,1);
	PutHex(" PHY_RX_CAL_DQ7_0: up= ",(REG_DDRPHY_READ(0,0x0837) & 0xfc00000)>>(6+16) ,1,0);
	PutHex(",down= ",(REG_DDRPHY_READ(0,0x0837) & 0x3f0000)>>16 ,1,1);
	PutHex(" PHY_RX_CAL_DM_0 : up= ",(REG_DDRPHY_READ(0,0x0838) & 0xfc0)>> 6 ,1,0);
	PutHex(",down= ",(REG_DDRPHY_READ(0,0x0838) & 0x3f) ,1,1);
	PutHex(" PHY_RX_CAL_DQS_0: up= ",(REG_DDRPHY_READ(0,0x0838) & 0xfc00000)>>(6+16) ,1,0);
	PutHex(",down= ",(REG_DDRPHY_READ(0,0x0838) & 0x3f0000)>>16 ,1,1);
	
//	{	0x0839 ,	0x00000000	} ,	// PHY_RX_CAL_OBS_0:RD:16:11:=0x0000 PHY_RX_CAL_FDBK_0:RW+:0:12:=0x0000
	PutHex(" PHY_RX_CAL_OBS_0: from FDBK= ",(REG_DDRPHY_READ(0,0x0839) & 0x04000000)>> 26 ,1,0);
	PutHex(",from DQS= ",(REG_DDRPHY_READ(0,0x0839) & 0x02000000)>> 25 ,1,0);
	PutHex(",from DM= ", (REG_DDRPHY_READ(0,0x0839) & 0x01000000)>> 24 ,1,0);
	PutHex(",from DQ= ", (REG_DDRPHY_READ(0,0x0839) & 0x00F00000)>> 20 ,1,1);
	PutStr("",1);
*/
/*
// need update
	for(i=0;i<4;i++)
	{
		PutHex(" phy_master_dly_lock_obs_select_X = ", i<<24,1,1);
		REG_DDRPHY_WRITE(0,0x0893,i);
		REG_DDRPHY_WRITE(0,0x0913,i);
		REG_DDRPHY_WRITE(0,0x0993,i);
		REG_DDRPHY_WRITE(1,0x0813,i);
		REG_DDRPHY_WRITE(1,0x0893,i);
		REG_DDRPHY_WRITE(1,0x0913,i);
		REG_DDRPHY_WRITE(1,0x0993,i);
		PutHex(" phy_master_dly_lock_obs_X[0:3]: ",REG_DDRPHY_READ(0,0x0823) & 0x3ff,2,0);
		PutHex(", ",REG_DDRPHY_READ(0,0x08A3) & 0x3ff,2,0);
		PutHex(", ",REG_DDRPHY_READ(0,0x0923) & 0x3ff,2,0);
		PutHex(", ",REG_DDRPHY_READ(0,0x09A3) & 0x3ff,2,0);
		PutHex(", ",REG_DDRPHY_READ(1,0x0823) & 0x3ff,2,0);
		PutHex(", ",REG_DDRPHY_READ(1,0x08A3) & 0x3ff,2,0);
		PutHex(", ",REG_DDRPHY_READ(1,0x0923) & 0x3ff,2,0);
		PutHex(", ",REG_DDRPHY_READ(1,0x09A3) & 0x3ff,2,1);
	}

//	{	0x0A04 ,	0x00000000	} ,	// PHY_ADR_SLAVE_LOOP_CNT_UPDATE_0:RW:24:3:=0x00 PHY_ADR_MASTER_DLY_LOCK_OBS_0:RD:8:10:=0x0000 PHY_ADR_MASTER_DLY_LOCK_OBS_SELECT_0:RW:0:3:=0x00
	for(i=0;i<3;i++)
	{
		PutHex(" phy_adr_master_dly_lock_obs_select_N = ",i,1,1);
		REG_DDRPHY_WRITE(0,0x0A04,i);
		REG_DDRPHY_WRITE(0,0x0A84,i);
		REG_DDRPHY_WRITE(0,0x0B04,i);
		REG_DDRPHY_WRITE(1,0x0A04,i);
		REG_DDRPHY_WRITE(1,0x0A84,i);
		REG_DDRPHY_WRITE(1,0x0B04,i);

		PutHex("   phy_adr_master_dly_lock_obs_N[0:2]: ",(REG_DDRPHY_READ(0,0x0A04) >> 8) & 0x3ff,2,0);
		PutHex(", ",(REG_DDRPHY_READ(0,0x0A84) >> 8) & 0x3ff,2,0);
		PutHex(", ",(REG_DDRPHY_READ(0,0x0B04) >> 8) & 0x3ff,2,0);
		PutHex(", ",(REG_DDRPHY_READ(1,0x0A04) >> 8) & 0x3ff,2,0);
		PutHex(", ",(REG_DDRPHY_READ(1,0x0A84) >> 8) & 0x3ff,2,0);
		PutHex(", ",(REG_DDRPHY_READ(1,0x0B04) >> 8) & 0x3ff,2,1);
	}
*/
/*
// Traning : Gate Leveling

	PutHex(" PI_CONTRL_ERROR_STATUS: ",REG_DDRPHY_READ(1,0x0216) & 0x1ff,2,0);
	PutHex(", ",REG_DDRPHY_READ(1,0x0216) & 0x1ff,2,1);
	PutHex(" PI_UPDATE_ERROR_STATUS: ",REG_DDRPHY_READ(1,0x02C3) & 0x3f,1,0);
	PutHex(", ",REG_DDRPHY_READ(1,0x02C3) & 0x3f,1,1);
	PutHex(" PI_CALVL_ERROR_STATUS : ",(REG_DDRPHY_READ(1,0x025F) & 0x00030000)>>16,1,0);
	PutHex(", ",(REG_DDRPHY_READ(1,0x025F) & 0x00030000)>>16,1,1);
	PutHex(" PI_WRLVL_ERROR_STATUS : ",(REG_DDRPHY_READ(1,0x023D) & 0x00300000)>>23,1,0);
	PutHex(", ",(REG_DDRPHY_READ(1,0x023D) & 0x00300000)>>16,1,1);

	PutStr("",1);


// Traning : Gate Leveling
//	{	0x082A ,	0x00000000	} ,	// PHY_GTLVL_STATUS_OBS_0:RD:16:12:=0x0000 PHY_GTLVL_HARD1_DELAY_OBS_0:RD:0:14:=0x0000
	PutStr(" PHY_GTLVL_STATUS_OBS_X[0:3]: ",0);
	for(ch=0;ch<DRAM_CH_CNT;ch+=1){
		for(slice=0;slice<4;slice+=1){
			PutHex(", ",(REG_DDRPHY_READ(ch,0x082A +slice*0x80) >> 16) & 0xfff0000,2,0);
		}
	}
	PutStr("",1);


//	{	0x0829 ,	0x00000000	} ,	// PHY_GTLVL_HARD0_DELAY_OBS_0:RD:16:14:=0x0000 PHY_WRLVL_ERROR_OBS_0:RD:0:16:=0x0000
	PutStr(" PHY_GTLVL_HARD0_DELAY_OBS_X[0:3]: ",0);
	for(ch=0;ch<DRAM_CH_CNT;ch+=1)
	{
		for(slice=0;slice<4;slice+=1){
			PutHex(", ",(REG_DDRPHY_READ(ch,0x0829 +slice*0x80) >> 16) & 0x3fff,2,0);
		}
	}
	PutStr("",1);


//	{	0x082A ,	0x00000000	} ,	// PHY_GTLVL_STATUS_OBS_0:RD:16:12:=0x0000 PHY_GTLVL_HARD1_DELAY_OBS_0:RD:0:14:=0x0000
	PutStr(" PHY_GTLVL_HARD1_DELAY_OBS_X[0:3]: ",0);
	for(ch=0;ch<DRAM_CH_CNT;ch+=1)	
	{
		for(slice=0;slice<4;slice+=1){
			PutHex(", ",(REG_DDRPHY_READ(ch,0x082A +slice*0x80) >> 0),2,0);
		}
	}
	PutStr("",1);


//	{	0x084F ,	0x00010000	} ,	// PHY_GTLVL_LAT_ADJ_START_0:RW+:16:4:=0x01 PHY_GTLVL_RDDQS_SLV_DLY_START_0:RW+:0:10:=0x0000
	PutStr(" PHY_GTLVL_RDDQS_SLV_DLY_START_X[0:3]: ",0);
	for(ch=0;ch<DRAM_CH_CNT;ch+=1)	
	{
		for(slice=0;slice<4;slice+=1){
			PutHex(", ",((REG_DDRPHY_READ(ch,0x084F +slice*0x80) & 0x3ff) >> 0),2,0);
		}
	}
	PutStr("",1);

//	{	0x082A ,	0x00000000	} ,	// PHY_GTLVL_STATUS_OBS_0:RD:16:12:=0x0000 PHY_GTLVL_HARD1_DELAY_OBS_0:RD:0:14:=0x0000
	PutStr(" PHY_GTLVL_STATUS_OBS_X[0:3]: ",0);
	for(ch=0;ch<DRAM_CH_CNT;ch+=1)	
	{
		for(slice=0;slice<4;slice+=1){
			PutHex(", ",((REG_DDRPHY_READ(ch,0x082A +slice*0x80) ) >> 16),2,0);
		}
	}
	PutStr("",1);


	REG_DDRPHY_WRITE(0,0x0817,(REG_DDRPHY_READ(0,0x0817) & 0xfffff0ff)|(0x11 << 8));
	REG_DDRPHY_WRITE(0,0x0817,(REG_DDRPHY_READ(1,0x0817) & 0xfffff0ff)|(0x11 << 8));
	PutHex("DM:phy_rdlvl_rddqs_dq_le_dly_obs_X[0:3] ",(REG_DDRPHY_READ(0,0x082B) & 0x3FF) ,2,0);
	PutHex(", ",(REG_DDRPHY_READ(0,0x08AB) & 0x3FF) ,2,0);
	PutHex(", ",(REG_DDRPHY_READ(0,0x092B) & 0x3FF) ,2,0);
	PutHex(", ",(REG_DDRPHY_READ(0,0x09AB) & 0x3FF) ,2,0);
	PutHex(": ",(REG_DDRPHY_READ(1,0x082B) & 0x3FF) ,2,0);
	PutHex(", ",(REG_DDRPHY_READ(1,0x08AB) & 0x3FF) ,2,0);
	PutHex(", ",(REG_DDRPHY_READ(1,0x092B) & 0x3FF) ,2,0);
	PutHex(", ",(REG_DDRPHY_READ(1,0x09AB) & 0x3FF) ,2,1);
	PutStr("",1);

// phy_rddqs_latency_adjust   (h84d  Bit[3:0])
// phy_rddqs_gate_slave_delay (h809  Bit[9:0]) ??? 0x084C ???
//	{	0x084D ,	0x01D00004	} ,	// PHY_WRLVL_DELAY_EARLY_THRESHOLD_0:RW+:16:10:=0x01D0 PHY_WRITE_PATH_LAT_ADD_0:RW+:8:3:=0x00 PHY_RDDQS_LATENCY_ADJUST_0:RW+:0:4:=0x04
//	{	0x0809 ,	0x00000133	} ,	// PHY_LPBK_CONTROL_0:RW:24:7:=0x00 PHY_LP4_BOOT_RDDQS_LATENCY_ADJUST_0:RW:16:4:=0x00 PHY_LP4_BOOT_RDDQS_GATE_SLAVE_DELAY_0:RW:0:10:=0x0133
//	{	0x084C ,	0x010900A0	} ,	// PHY_RDDQS_GATE_SLAVE_DELAY_0:RW+:16:10:=0x0109 PHY_RDDQS_DM_FALL_SLAVE_DELAY_0:RW+:0:10:=0x00a0
	PutHex("PHY_RDDQS_LATENCY_ADJUST_X[0:3] ",(REG_DDRPHY_READ(0,0x084D) & 0x000F) ,1,0);
	PutHex(", ",(REG_DDRPHY_READ(0,0x08CD) & 0x000F) ,1,0);
	PutHex(", ",(REG_DDRPHY_READ(0,0x094D) & 0x000F) ,1,0);
	PutHex(": ",(REG_DDRPHY_READ(0,0x09CD) & 0x000F) ,1,0);
	PutHex(", ",(REG_DDRPHY_READ(1,0x084D) & 0x000F) ,1,0);
	PutHex(", ",(REG_DDRPHY_READ(1,0x08CD) & 0x000F) ,1,0);
	PutHex(", ",(REG_DDRPHY_READ(1,0x094D) & 0x000F) ,1,0);
	PutHex(": ",(REG_DDRPHY_READ(1,0x09CD) & 0x000F) ,1,1);
	PutHex("PHY_LP4_BOOT_RDDQS_GATE_SLAVE_DELAY_X[0:3] ",(REG_DDRPHY_READ(0,0x0809) & 0x03FF) ,2,0);
	PutHex(", ",(REG_DDRPHY_READ(0,0x0889) & 0x03FF) ,2,0);
	PutHex(": ",(REG_DDRPHY_READ(0,0x0909) & 0x03FF) ,2,0);
	PutHex(", ",(REG_DDRPHY_READ(0,0x0989) & 0x03FF) ,2,0);
	PutHex(", ",(REG_DDRPHY_READ(1,0x0809) & 0x03FF) ,2,0);
	PutHex(", ",(REG_DDRPHY_READ(1,0x0889) & 0x03FF) ,2,0);
	PutHex(", ",(REG_DDRPHY_READ(1,0x0909) & 0x03FF) ,2,0);
	PutHex(": ",(REG_DDRPHY_READ(1,0x0989) & 0x03FF) ,2,1);
	PutHex("PHY_RDDQS_GATE_SLAVE_DELAY_X[0:3] ",(REG_DDRPHY_READ(0,0x084C)>>16) & 0x03FF,2,0);
	PutHex(", ",(REG_DDRPHY_READ(0,0x08CC)>>16) & 0x03FF,2,0);
	PutHex(", ",(REG_DDRPHY_READ(0,0x094C)>>16) & 0x03FF,2,0);
	PutHex(", ",(REG_DDRPHY_READ(0,0x09CC)>>16) & 0x03FF,2,0);
	PutHex(", ",(REG_DDRPHY_READ(1,0x084C)>>16) & 0x03FF,2,0);
	PutHex(", ",(REG_DDRPHY_READ(1,0x08CC)>>16) & 0x03FF,2,0);
	PutHex(", ",(REG_DDRPHY_READ(1,0x094C)>>16) & 0x03FF,2,0);
	PutHex(", ",(REG_DDRPHY_READ(1,0x09CC)>>16) & 0x03FF,2,1);
	PutStr("",1);

//	{	0x082B ,	0x00000000	} ,	// PHY_RDLVL_RDDQS_DQ_TE_DLY_OBS_0:RD:16:10:=0x0000 PHY_RDLVL_RDDQS_DQ_LE_DLY_OBS_0:RD:0:10:=0x0000
//	{	0x0817 ,	0x00000000	} ,	// PHY_RDLVL_DATA_MASK_0:RW:16:8:=0x00 PHY_RDLVL_RDDQS_DQ_OBS_SELECT_0:RW:8:5:=0x00 PHY_RDLVL_OP_MODE_0:RW:0:2:=0x00

//	PutStr("CH0:PHY_RDDQS_DQX_RISE/FALL_SLAVE_DELAY_0",1);
	for(ch=0;ch<DRAM_CH_CNT;ch+=1){
		PutHex("CH",ch,1,0);
		PutStr(":PHY_RDDQS_DQX_RISE/FALL_SLAVE_DELAY_0",1);
		for(slice=0;slice<4;slice+=1){
			for(i=0;i<8;i+=1){
				PutHex(" DQ",i +slice*8,1,0);
				PutHex(" :R ",((REG_DDRPHY_READ(ch,0x0843 +i +slice*0x80) & 0x07FF0000) >>16),2,0);	// PHY_RDDQS_DQ0_RISE_SLAVE_DELAY_0:RW+:16:10:=0x00A0 PHY_RDDM_SLAVE_DELAY_0:RW+:0:10:=0x0000
				PutHex(" ,F ",((REG_DDRPHY_READ(ch,0x0844 +i +slice*0x80) & 0x000007FF)     ),2,0);	// PHY_RDDQS_DQ1_RISE_SLAVE_DELAY_0:RW+:16:10:=0x00A0 PHY_RDDQS_DQ0_FALL_SLAVE_DELAY_0:RW+:0:10:=0x00a0
				REG_DDRPHY_WRITE(ch,0x0817 +slice*0x80,i<<8);PutHex(" ,TE/LE ",(REG_DDRPHY_READ(ch,0x082B +slice*0x80)),4,1);
			}
		}
		PutStr("",1);
	}

	for(ch=0;ch<DRAM_CH_CNT;ch+=1){
		for(slice=0;slice<4;slice+=1){
			PutHex(" PHY_RDLVL_STATUS_OBS_",slice,1,0);
			PutHex(": ",(REG_DDRPHY_READ(ch,0x082D +slice*0x80))>>0,4,1);
		}
	}
	PutStr("",1);

	for(ch=0;ch<DRAM_CH_CNT;ch+=1){
		PutHex("CH",ch,1,0);
		PutStr(":PHY_CLK_WRDQn_SLAVE_DELAY_X",1);
		for(slice=0;slice<4;slice+=1){
			for(i=0;i<8;i=i+2){
				PutHex(" DQ",i+slice*8,1,0);
				PutHex(" : ",(
									(	REG_DDRPHY_READ(ch,0x083A +i/2 +slice *0x80)		& 0x000007FF)
									>>0),2,1);	// PHY_CLK_WRDQ1_SLAVE_DELAY_0:RW+:16:11:=0x0280 PHY_CLK_WRDQ0_SLAVE_DELAY_0:RW+:0:11:=0x0280
				PutHex(" DQ",i+1+slice*8,1,0);
				PutHex(" : ",((REG_DDRPHY_READ(ch,0x083A +i/2 +slice*0x80)&0x07FF0000)>>16),2,1);	// PHY_CLK_WRDQ1_SLAVE_DELAY_0:RW+:16:11:=0x0280 PHY_CLK_WRDQ0_SLAVE_DELAY_0:RW+:0:11:=0x0280
			}
		}
		for(slice=0;slice<4;slice+=1){
			PutHex(" DM",(slice),1,0);	// PHY_CLK_WRDQS_SLAVE_DELAY_0:RW+:16:10:=0x0000 PHY_CLK_WRDM_SLAVE_DELAY_0:RW+:0:11:=0x0280
			PutHex(" : ",((REG_DDRPHY_READ(ch,0x083E +slice*0x80)&0x000007FF)>>0),2,1);	// PHY_CLK_WRDQS_SLAVE_DELAY_0:RW+:16:10:=0x0000 PHY_CLK_WRDM_SLAVE_DELAY_0:RW+:0:11:=0x0280
		}
		for(slice=0;slice<4;slice+=1){
			PutHex(" DQS",(slice),1,0);	// PHY_CLK_WRDQS_SLAVE_DELAY_0:RW+:16:10:=0x0000 PHY_CLK_WRDM_SLAVE_DELAY_0:RW+:0:11:=0x0280
			PutHex(": ",((REG_DDRPHY_READ(ch,0x083E +slice*0x80)&0x07FF0000)>>16),2,1);	// PHY_CLK_WRDQS_SLAVE_DELAY_0:RW+:16:10:=0x0000 PHY_CLK_WRDM_SLAVE_DELAY_0:RW+:0:11:=0x0280
		}
		PutStr("",1);
	}
	PutStr("",1);
*/

/*
	PutStr("PHY/PI Register Checking",1);
	for( i=0; i<DDR_PHY_REGSET_NUM; i++ )
	{
		if((DDR_PHY_REGSET[i][0]> 0x0B7F) | (DDR_PHY_REGSET[i][0] < 0x0B00))
		{
		if(REG_DDRPHY_READ(0,DDR_PHY_REGSET[i][0]) != DDR_PHY_REGSET[i][1]){
			PutHex("0:ADD ",DDR_PHY_REGSET[i][0],2,0);
			PutHex(", Wrdata ",DDR_PHY_REGSET[i][1],4,0);
			PutHex(", Rddata ",REG_DDRPHY_READ(0,DDR_PHY_REGSET[i][0]),4,1);
		}
		if(REG_DDRPHY_READ(1,DDR_PHY_REGSET[i][0]) != DDR_PHY_REGSET[i][1]){
			PutHex("1:ADD ",DDR_PHY_REGSET[i][0],2,0);
			PutHex(", Wrdata ",DDR_PHY_REGSET[i][1],4,0);
			PutHex(", Rddata ",REG_DDRPHY_READ(1,DDR_PHY_REGSET[i][0]),4,1);
		}
		}
	}
*/
}
