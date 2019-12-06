/*
 * Copyright (c) 2015-2019, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 *	NUMBER OF BOARD CONFIGRATION
 *	PLEASE DEFINE
 ******************************************************************************/
#define BOARDNUM 22
/*******************************************************************************
 *	PLEASE SET board number or board judge function
 ******************************************************************************/
#define BOARD_JUDGE_AUTO
#ifdef BOARD_JUDGE_AUTO
static uint32_t _board_judge(void);
static uint32_t boardcnf_get_brd_type(void)
{
		return _board_judge();
}
#else /* BOARD_JUDGE_AUTO */
static uint32_t boardcnf_get_brd_type(void)
{
		return 1;
}
#endif /* BOARD_JUDGE_AUTO */

/*******************************************************************************
 *	Set DRAM ODT , VREFca , Derating  condtition
 ******************************************************************************/
/* #define _def_LPDDR4_ODT	0x36	// MR11 */
/* #define _def_LPDDR4_ODT	0x66	// MR11 */
/* #define _def_LPDDR4_VREFCA	0x11	// MR12 */
/* #define JS2_DERATE		1	// 1: Temperature Derating */
#define DDR_FAST_INIT

/*******************************************************************************
 *	BOARD CONFIGRATION
 *	PLEASE DEFINE boardcnfs[]
 ******************************************************************************/
struct _boardcnf_ch {
	/*0x00... 4Gbit/die ( 2Gb/channel)
	 *0x01... 6Gbit/die ( 3Gb/channel)
	 *0x02... 8Gbit/die ( 4Gb/channel) or 4Gb/die (4Gb/channel)
	 *0x03...12Gbit/die ( 6Gb/channel) or 6Gb/die (6Gb/channel)
	 *0x04...16Gbit/die ( 8Gb/channel) or 8Gb/die (8Gb/channel)
//non:	 *0x05...24Gbit/die (12Gb/channel)
//non:	 *0x06...32Gbit/die (16Gb/channel)
	 *0xff...NO_MEMORY
	 */
	uint8_t ddr_density[CS_CNT];
	/* SoC caX([15][14]....[3][2][1][0]) -> MEM caY: */
	uint64_t ca_swap;
	/* SoC dqsX([3][2][1][0]) -> MEM dqsY: */
	uint16_t dqs_swap;
	/* SoC dq([7][6][5][4][3][2][1][0]) -> MEM dqY/dm:  (8 means DM) */
	uint32_t dq_swap[SLICE_CNT];
	/* SoC dm -> MEM dqY/dm:  (8 means DM) */
	uint8_t dm_swap[SLICE_CNT];
	/* write traing pattern
	 * (DM,DQ7,....DQ0) x BL16
	 */
	uint16_t wdqlvl_patt[16];
	/* delay adjustment is ps */
	int8_t cacs_adj[16];
	int8_t dm_adj_w[SLICE_CNT];
	int8_t dq_adj_w[SLICE_CNT*8];
	int8_t dm_adj_r[SLICE_CNT];
	int8_t dq_adj_r[SLICE_CNT*8];
};

struct _boardcnf {
	/* ch in use */
	uint8_t phyvalid;
	/* use dbi mode */
	uint8_t dbi_en;
	/* default CA/CS delay value */
	uint16_t cacs_dly;
	/* default CA/CS delay adjust value in ps */
	int16_t cacs_dly_adj;
	/* default DQ/DM delay value for write */
	uint16_t dqdm_dly_w;
	/* default DQ/DM delay value for read */
	uint16_t dqdm_dly_r;
	struct _boardcnf_ch ch[DRAM_CH_CNT];
};
/* write traing pattern
 * (DM,DQ7,....DQ0) x BL16
 */
#define WDQLVL_PAT {\
	0x00AA,\
	0x0055,\
	0x00AA,\
	0x0155,\
	0x01CC,\
	0x0133,\
	0x00CC,\
	0x0033,\
	0x00F0,\
	0x010F,\
	0x01F0,\
	0x010F,\
	0x00F0,\
	0x00F0,\
	0x000F,\
	0x010F}

static const struct _boardcnf boardcnfs[BOARDNUM] = {
/*
 * boardcnf[0] RENESAS SALVATOR-X board with M3-W/SIP
 */
{
	0x03,		/* phyvalid */
	0x01,		/* dbi_en */
	0x02c0,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x70612543, 0x43251670, 0x45326170, 0x10672534 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01327654, 0x34526107, 0x35421670, 0x70615324 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[1] RENESAS KRIEK board with M3-W/SoC
 */
{
	0x03,		/* phyvalid */
	0x01,		/* dbi_en */
	0x02c0,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00345201,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00302154,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[2] RENESAS SALVATOR-X board with H3 Ver.1.x/SIP(8Gbit 1rank)
 */
{
	0x0f,		/* phyvalid */
	0x00,		/* dbi_en */
	0x0300,		/* cacs_dly */
	-320,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x3210,
/*dq_swap[]*/		{ 0x20741365, 0x34256107, 0x57460321, 0x70614532 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x3102,
/*dq_swap[]*/		{ 0x23547610, 0x34526107, 0x67452310, 0x32106754 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[2]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x0213,
/*dq_swap[]*/		{ 0x30216754, 0x67453210, 0x70165243, 0x07162345 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[3]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x0213,
/*dq_swap[]*/		{ 0x01327654, 0x70615432, 0x54760123, 0x07162345 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[3] RENESAS Starter Kit board with M3-W/SIP(8Gbit 1rank)
 */
{
	0x03,		/* phyvalid */
	0x01,		/* dbi_en */
	0x02c0,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },    /* M3-W/SIP(8Gbit 1rank) */
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x70612543, 0x43251670, 0x45326170, 0x10672534 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },    /* M3-W/SIP(8Gbit 1rank) */
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01327654, 0x34526107, 0x35421670, 0x70615324 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[4] RENESAS SALVATOR-M(1rank) board with H3 Ver.1.x/SoC
 */
{
	0x0f,		/* phyvalid */
	0x00,		/* dbi_en */
	0x02c0,		/* cacs_dly */
	-320,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },
/*ca_swap*/		0x00315024,
/*dqs_swap*/		0x3120,
/*dq_swap[]*/		{ 0x30671254, 0x26541037, 0x17054623, 0x12307645 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },
/*ca_swap*/		0x00025143,
/*dqs_swap*/		0x3210,
/*dq_swap[]*/		{ 0x70613542, 0x16245307, 0x30712645, 0x21706354 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[2]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },
/*ca_swap*/		0x00523104,
/*dqs_swap*/		0x2301,
/*dq_swap[]*/		{ 0x70613542, 0x16245307, 0x30712645, 0x21706354 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[3]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },
/*ca_swap*/		0x00153402,
/*dqs_swap*/		0x2031,
/*dq_swap[]*/		{ 0x30671254, 0x26541037, 0x17054623, 0x12307645 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[5] RENESAS KRIEK-1rank board with M3-W/SoC
 */
{
	0x03,		/* phyvalid */
	0x01,		/* dbi_en */
	0x02c0,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },
/*ca_swap*/		0x00345201,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },
/*ca_swap*/		0x00302154,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[6] RENESAS SALVATOR-X board with H3 Ver.1.x/SIP(8Gbit 2rank)
 */
{
	0x0f,		/* phyvalid */
	0x00,		/* dbi_en */
	0x0300,		/* cacs_dly */
	-320,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x3210,
/*dq_swap[]*/		{ 0x20741365, 0x34256107, 0x57460321, 0x70614532 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x3102,
/*dq_swap[]*/		{ 0x23547610, 0x34526107, 0x67452310, 0x32106754 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[2]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x0213,
/*dq_swap[]*/		{ 0x30216754, 0x67453210, 0x70165243, 0x07162345 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[3]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x0213,
/*dq_swap[]*/		{ 0x01327654, 0x70615432, 0x54760123, 0x07162345 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[7] RENESAS SALVATOR-X board with H3 Ver.2.0 or later/SIP(8Gbit 1rank)
 */
{
	0x0f,		/* phyvalid */
	0x01,		/* dbi_en */
	0x0300,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x70631425, 0x34527016, 0x43527610, 0x32104567 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },
/*ca_swap*/		0x00105432,
/*dqs_swap*/		0x3210,
/*dq_swap[]*/		{ 0x43256107, 0x07162354, 0x10234567, 0x01235467 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[2]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x2301,
/*dq_swap[]*/		{ 0x01327654, 0x02316457, 0x10234567, 0x01325467 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[3]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x2301,
/*dq_swap[]*/		{ 0x12034765, 0x23105467, 0x23017645, 0x32106745 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[8] RENESAS SALVATOR-X board with H3 Ver.2.0 or later/SIP(8Gbit 2rank)
 */
{
#if RCAR_DRAM_CHANNEL == 5
	0x05,		/* phyvalid */
#else /* RCAR_DRAM_CHANNEL!=5 */
	0x0f,		/* phyvalid */
#endif /* RCAR_DRAM_CHANNEL */
	0x01,		/* dbi_en */
	0x0300,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x70631425, 0x34527016, 0x43527610, 0x32104567 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
#if ((RCAR_DRAM_CHANNEL == 5) && (RCAR_DRAM_SPLIT == 2))
/*ch[1]*/	{	/* copy from ch[2] (for DRAM_SPLIT_2CH) */
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x2301,
/*dq_swap[]*/		{ 0x01327654, 0x02316457, 0x10234567, 0x01325467 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
#else /* (RCAR_DRAM_CHANNEL == 5) && (RCAR_DRAM_SPLIT == 2) */
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00105432,
/*dqs_swap*/		0x3210,
/*dq_swap[]*/		{ 0x43256107, 0x07162354, 0x10234567, 0x01235467 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
#endif /* (RCAR_DRAM_CHANNEL == 5) && (RCAR_DRAM_SPLIT == 2) */
/*ch[2]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x2301,
/*dq_swap[]*/		{ 0x01327654, 0x02316457, 0x10234567, 0x01325467 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[3]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x2301,
/*dq_swap[]*/		{ 0x12034765, 0x23105467, 0x23017645, 0x32106745 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[9] RENESAS SALVATOR-MS(1rank) board with H3 Ver.2.0 or later/SoC
 */
{
	0x0f,		/* phyvalid */
	0x01,		/* dbi_en */
	0x0300,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x3210,
/*dq_swap[]*/		{ 0x27645310, 0x75346210, 0x53467210, 0x23674510 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x2301,
/*dq_swap[]*/		{ 0x23764510, 0x43257610, 0x43752610, 0x37652401 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/	/*	{ 0, 0, 0, 0, 0, 0, 0, 0, */
/*cacs_adj*/		{ -128, -128, -128, -128, -128, -128, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[2]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },
/*ca_swap*/		0x00452103,
/*dqs_swap*/		0x3210,
/*dq_swap[]*/		{ 0x32764510, 0x43257610, 0x43752610, 0x26573401 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[3]*/	{
/*ddr_density[]*/	{ 0x02, 0xff },
/*ca_swap*/		0x00520413,
/*dqs_swap*/		0x2301,
/*dq_swap[]*/		{ 0x47652301, 0x75346210, 0x53467210, 0x32674501 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/	/*	{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 }, */
/*cacs_adj*/		{ 30, 30, 30, 30, 30, 30, 30, 30,
			  30, 30 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[10] RENESAS Kriek(2rank) board with M3-N/SoC
 */
{
	0x01,		/* phyvalid */
	0x01,		/* dbi_en */
	0x0300,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00345201,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[11] RENESAS SALVATOR-X board with M3-N/SIP(8Gbit 2rank)
 */
{
	0x01,		/* phyvalid */
	0x01,		/* dbi_en */
	0x0300,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
#if (RCAR_DRAM_LPDDR4_MEMCONF == 2)
/*ddr_density[]*/	{ 0x04, 0x04 },
#else
/*ddr_density[]*/	{ 0x02, 0x02 },
#endif
/*ca_swap*/		0x00342501,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x10672534, 0x43257106, 0x34527601, 0x71605243 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[12] RENESAS CONDOR board with V3H/SoC
 */
{
	0x01,		/* phyvalid */
	0x01,		/* dbi_en */
	0x0300,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*//*	{ 0x00, 0x00 }, */
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00501342,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x70562134, 0x34526071, 0x23147506, 0x12430567 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[13] RENESAS KRIEK board with PM3/SoC
 */
{
	0x05,		/* phyvalid */
	0x00,		/* dbi_en */
	0x02c0,		/* cacs_dly */
	-320,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00345201,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{	/* for DRAM_SPLIT_2CH */
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00302154,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[2]*/	{	/* for DRAM_SPLIT_NON */
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00302154,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[3]*/	{ /* Dummy */
/*ddr_density[]*/	{ 0xff, 0xff },
/*ca_swap*/		0,
/*dqs_swap*/		0,
/*dq_swap[]*/		{ 0, 0, 0, 0 },
/*dm_swap[]*/		{ 0, 0, 0, 0 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[14] RENESAS SALVATOR-X board with H3 Ver.2.0 or later/SIP(16Gbit 1rank)
 */
{
#if RCAR_DRAM_CHANNEL == 5
	0x05,		/* phyvalid */
#else /* RCAR_DRAM_CHANNEL!=5 */
	0x0f,		/* phyvalid */
#endif /* RCAR_DRAM_CHANNEL */
	0x01,		/* dbi_en */
	0x0300,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x70631425, 0x34527016, 0x43527610, 0x32104567 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
#if ((RCAR_DRAM_CHANNEL == 5) && (RCAR_DRAM_SPLIT == 2))
/*ch[1]*/	{	/* copy from ch[2] (for DRAM_SPLIT_2CH) */
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x2301,
/*dq_swap[]*/		{ 0x01327654, 0x02316457, 0x10234567, 0x01325467 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
#else /* (RCAR_DRAM_CHANNEL == 5) && (RCAR_DRAM_SPLIT == 2) */
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00105432,
/*dqs_swap*/		0x3210,
/*dq_swap[]*/		{ 0x43256107, 0x07162354, 0x10234567, 0x01235467 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
#endif /* (RCAR_DRAM_CHANNEL == 5) && (RCAR_DRAM_SPLIT == 2) */
/*ch[2]*/	{
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x2301,
/*dq_swap[]*/		{ 0x01327654, 0x02316457, 0x10234567, 0x01325467 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[3]*/	{
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x2301,
/*dq_swap[]*/		{ 0x12034765, 0x23105467, 0x23017645, 0x32106745 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[15] RENESAS KRIEK board with H3N
 */
{
	0x05,		/* phyvalid */
	0x01,		/* dbi_en */
	0x0300,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00345201,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x01672543, 0x45367012, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{	/* for DRAM_SPLIT_2CH */
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00302154,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[2]*/	{	/* for DRAM_SPLIT_NON */
/*ddr_density[]*/	{ 0x02, 0x02 },
/*ca_swap*/		0x00302154,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[3]*/	{ /* Dummy */
/*ddr_density[]*/	{ 0xff, 0xff },
/*ca_swap*/		0,
/*dqs_swap*/		0,
/*dq_swap[]*/		{ 0, 0, 0, 0 },
/*dm_swap[]*/		{ 0, 0, 0, 0 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[16] RENESAS KRIEK-P2P board with M3-W/SoC
 */
{
	0x03,		/* phyvalid */
	0x01,		/* dbi_en */
	0x0320,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x04, 0x04 },
/*ca_swap*/		0x520314FFFF523041,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234},
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x04, 0x04 },
/*ca_swap*/		0x314250FFFF312405,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[17] RENESAS KRIEK-P2P board with M3-N/SoC
 */
{
	0x01,		/* phyvalid */
	0x01,		/* dbi_en */
	0x0300,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x04, 0x04 },
/*ca_swap*/		0x520314FFFF523041,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234},
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[18] RENESAS SALVATOR-X board with M3-W/SIP(16Gbit 2rank)
 */
{
	0x03,		/* phyvalid */
	0x01,		/* dbi_en */
	0x02c0,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x04, 0x04 },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x70612543, 0x43251670, 0x45326170, 0x10672534 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x04, 0x04 },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01327654, 0x34526107, 0x35421670, 0x70615324 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[19] RENESAS SALVATOR-X board with M3-W/SIP(16Gbit 1rank)
 */
{
	0x03,		/* phyvalid */
	0x01,		/* dbi_en */
	0x02c0,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x70612543, 0x43251670, 0x45326170, 0x10672534 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00543210,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01327654, 0x34526107, 0x35421670, 0x70615324 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[20] RENESAS KRIEK 16Gbit/2rank/2ch board with M3-W/SoC
 */
{
	0x03,		/* phyvalid */
	0x01,		/* dbi_en */
	0x02c0,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x04, 0x04 },
/*ca_swap*/		0x00345201,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x04, 0x04 },
/*ca_swap*/		0x00302154,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
},
/*
 * boardcnf[21] RENESAS KRIEK 16Gbit/1rank/2ch board with M3-W/SoC
 */
{
	0x03,		/* phyvalid */
	0x01,		/* dbi_en */
	0x02c0,		/* cacs_dly */
	0,		/* cacs_dly_adj */
	0x0300,		/* dqdm_dly_w */
	0x00a0,		/* dqdm_dly_r */
	{
/*ch[0]*/	{
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00345201,
/*dqs_swap*/		0x3201,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		},
/*ch[1]*/	{
/*ddr_density[]*/	{ 0x04, 0xff },
/*ca_swap*/		0x00302154,
/*dqs_swap*/		0x2310,
/*dq_swap[]*/		{ 0x01672543, 0x45361207, 0x45632107, 0x60715234 },
/*dm_swap[]*/		{ 0x08, 0x08, 0x08, 0x08 },
/*wdqlvl_patt[]*/	WDQLVL_PAT,
/*cacs_adj*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0 },
/*dm_adj_w*/		{ 0, 0, 0, 0 },
/*dqdm_adj_w*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 },
/*dm_adj_r*/		{ 0, 0, 0, 0 },
/*dqdm_adj_r*/		{ 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0 }
		}
	}
}
};

/*******************************************************************************
 *	EXTAL CLOCK DEFINITION
 *	PLEASE DEFINE HOW TO JUDGE BOARD CLK
 ******************************************************************************/
/*
 * RENESAS SALVATOR/KRIEK BOARD EXAMPLE
 * judge by md14/md13
 *
 * 16.66MHz CLK,DIV= 50,3  (md14,md13==0,0)
 * 20.00MHz CLK,DIV= 60,3  (md14,md13==0,1)
 * 25.00MHz CLK,DIV= 75,3  (md14,md13==1,0)
 * 16.66MHz CLK,DIV= 50,3  (md14,md13==1,1) // only for H3 Ver.1.0
 * 33.33MHz CLK,DIV=100,3  (md14,md13==1,1)
 */
void boardcnf_get_brd_clk(uint32_t brd, uint32_t *clk, uint32_t *div)
{
	uint32_t md;

	if ((Prr_Product == PRR_PRODUCT_H3) && (Prr_Cut == PRR_PRODUCT_10)) {
		*clk = 50;
		*div = 3;
	} else {
		md = (mmio_read_32(RST_MODEMR) >> 13) & 0x3;
		switch (md) {
		case 0x0:
			*clk = 50;
			*div = 3;
			break;
		case 0x1:
			*clk = 60;
			*div = 3;
			break;
		case 0x2:
			*clk = 75;
			*div = 3;
			break;
		case 0x3:
			*clk = 100;
			*div = 3;
			break;
		}
	}
	(void)brd;
}

/*******************************************************************************
 *	DDR MBPS TARGET
 *	PLEASE DEFINE HOW TO JUDGE DDR BPS
 ******************************************************************************/
/*
 * DDRxxxx (judge_ by md19,17) : 0
 * DDR3200 (md19,17==0,0) : 3200
 * DDR2800 (md19,17==0,1) : 2800
 * DDR2400 (md19,17==1,0) : 2400
 * DDR1600 (md19,17==1,1) : 1600
 */
void boardcnf_get_ddr_mbps(uint32_t brd, uint32_t *mbps, uint32_t *div)
{
	uint32_t md;

	if (Prr_Product == PRR_PRODUCT_V3H) {
		md = (mmio_read_32(RST_MODEMR) >> 19) & 0x01;
		switch (md) {
		case 0x0:
			*mbps = 3200;
			*div = 1;
			break;
		case 0x1:
			*mbps = 1600;
			*div = 1;
			break;
		}
	} else {
		md = (mmio_read_32(RST_MODEMR) >> 17) & 0x05;
		md = (md | (md >> 1)) & 0x03;
		switch (md) {
		case 0x0:
			*mbps = 3200;
			*div = 1;
			break;
		case 0x1:
			*mbps = 2800;
			*div = 1;
			break;
		case 0x2:
			*mbps = 2400;
			*div = 1;
			break;
		case 0x3:
			*mbps = 1600;
			*div = 1;
			break;
		}
	}
	(void)brd;
}

/*******************************************************************************
 *	REFRESH TARGET DEFINITION
 *	PLEASE DEFINE _def_REFPERIOD (in ns)
 ******************************************************************************/
/* WARN: do not modify in this IPL version */
/* #define _def_REFPERIOD  1890 */

/*******************************************************************************
 *	PREDEFINED TERM CODE by chip id
 ******************************************************************************/
#define M3_SAMPLE_TT_A84        0xB866CC10, 0x3B250421
#define M3_SAMPLE_TT_A85        0xB866CC10, 0x3AA50421
#define M3_SAMPLE_TT_A86        0xB866CC10, 0x3AA48421
#define M3_SAMPLE_FF_B45        0xB866CC10, 0x3AB00C21
#define M3_SAMPLE_FF_B49        0xB866CC10, 0x39B10C21
#define M3_SAMPLE_FF_B56        0xB866CC10, 0x3AAF8C21
#define M3_SAMPLE_SS_E24        0xB866CC10, 0x3BA39421
#define M3_SAMPLE_SS_E28        0xB866CC10, 0x3C231421
#define M3_SAMPLE_SS_E32        0xB866CC10, 0x3C241421
static const uint32_t TermcodeBySample[20][3] = {
	{	M3_SAMPLE_TT_A84,		0x000158D5	},
	{	M3_SAMPLE_TT_A85,		0x00015955	},
	{	M3_SAMPLE_TT_A86,		0x00015955	},
	{	M3_SAMPLE_FF_B45,		0x00015690	},
	{	M3_SAMPLE_FF_B49,		0x00015753	},
	{	M3_SAMPLE_FF_B56,		0x00015793	},
	{	M3_SAMPLE_SS_E24,		0x00015996	},
	{	M3_SAMPLE_SS_E28,		0x000159D7	},
	{	M3_SAMPLE_SS_E32,		0x00015997	},
	{	0xFFFFFFFF,	0xFFFFFFFF,	0x0001554F	}
};
#ifdef BOARD_JUDGE_AUTO
/*******************************************************************************
 *	SAMPLE board detect function
 ******************************************************************************/
/* R/W  32      LSI Multiplexed Pin Setting Mask Register */
#define    PFC_PMMR   0xE6060000U
/* R/W  32      LSI pin pull-enable register 5 */
#define    PFC_PUEN5  0xE6060414U
/* R/W  32      LSI pin pull-enable register 6 */
#define    PFC_PUEN6  0xE6060418U
/* R/W  32      LSI pin pull-up/down control register 5 */
#define    PFC_PUD5   0xE6060454U
/* R/W  32      LSI pin pull-up/down control register 6 */
#define    PFC_PUD6   0xE6060458U
/* R    32      General input register 5 */
#define    GPIO_INDT5 0xE605500CU
/* R    32      General input register 6 */
#define    GPIO_INDT6 0xE605540CU

#define BIT25 (1U << 25)
#define BIT22 (1U << 22)
#define BIT15 (1U << 15)
#define BIT0  (1U)
#define    GPIO_GPSR6 0xE6060118U

#if (RCAR_GEN3_ULCB == 0)
static void pfc_write_and_poll(uint32_t a, uint32_t v)
{
	mmio_write_32(PFC_PMMR, ~v);
	v = ~mmio_read_32(PFC_PMMR);
	mmio_write_32(a, v);
	while (v != mmio_read_32(a))
		;
	dsb_sev();
}
#endif

#ifndef RCAR_GEN3_ULCB
#define RCAR_GEN3_ULCB		0
#endif

#if (RCAR_GEN3_ULCB == 0) /* non Starter Kit */
/*
 * Connect = return 0
 * Open    = return 1
 */
static uint32_t opencheck_SSI_WS6(void)
{
	uint32_t dataL, down, up;
	uint32_t gpsr6_bak;
	uint32_t puen5_bak;
	uint32_t pud5_bak;

	gpsr6_bak = mmio_read_32(GPIO_GPSR6);
	puen5_bak = mmio_read_32(PFC_PUEN5);
	pud5_bak = mmio_read_32(PFC_PUD5);
	dsb_sev();

	dataL = (gpsr6_bak & ~BIT15);
	pfc_write_and_poll(GPIO_GPSR6, dataL);

	/* Pull-Up/Down Enable (PUEN5[22]=1) */
	dataL = puen5_bak;
	dataL |= (BIT22);
	pfc_write_and_poll(PFC_PUEN5, dataL);

	/* Pull-Down-Enable (PUD5[22]=0, PUEN5[22]=1) */
	dataL = pud5_bak;
	dataL &= ~(BIT22);
	pfc_write_and_poll(PFC_PUD5, dataL);
	/* GPSR6[15]=SSI_WS6 */
	micro_wait(10);
	down = (mmio_read_32(GPIO_INDT6) >> 15) & 0x1;
	dsb_sev();

	/* Pull-Up-Enable (PUD5[22]=1, PUEN5[22]=1) */
	dataL = pud5_bak;
	dataL |= (BIT22);
	pfc_write_and_poll(PFC_PUD5, dataL);
	/* GPSR6[15]=SSI_WS6 */
	micro_wait(10);
	up = (mmio_read_32(GPIO_INDT6) >> 15) & 0x1;

	dsb_sev();

	pfc_write_and_poll(GPIO_GPSR6, gpsr6_bak);
	pfc_write_and_poll(PFC_PUEN5, puen5_bak);
	pfc_write_and_poll(PFC_PUD5, pud5_bak);

	/* Compare */
	if (down == up) {
		/* Same = Connect */
		return 0;
	} else {
		/* Diff = Open */
		return 1;
	}
}

#endif /* (RCAR_GEN3_ULCB == 0) */

static uint32_t _board_judge(void)
{
	uint32_t brd;

#if (RCAR_GEN3_ULCB == 1)
	/* Starter Kit */
	if (Prr_Product == PRR_PRODUCT_H3) {
		if (Prr_Cut <= PRR_PRODUCT_11) {
			/* RENESAS Starter Kit(H3 Ver.1.x/SIP) board */
			brd = 2;
		} else {
			/* RENESAS Starter Kit(H3 Ver.2.0 or later/SIP) board */
#if (RCAR_DRAM_LPDDR4_MEMCONF == 0)
			brd = 7;  /* 8Gbit/1rank */
#else
			brd = 8;  /* 8Gbit/2rank */
#endif
		}
	} else if (Prr_Product == PRR_PRODUCT_M3) {
		if (Prr_Cut >= PRR_PRODUCT_30) {
			/* RENESAS Starter Kit (M3-W Ver.3.0/SIP) */
			brd = 18;
		} else {
			/* RENESAS Starter Kit(M3-W/SIP 8Gbit 1rank) board */
			brd = 3;
		}
	} else {
		/* RENESAS Starter Kit(M3-N/SIP) board */
		brd = 11;
	}
#else /* not (RCAR_GEN3_ULCB == 1) */
	uint32_t usb2_ovc_open;

	usb2_ovc_open = opencheck_SSI_WS6();

	/* RENESAS Eva-board */
	brd = 99;
	if (Prr_Product == PRR_PRODUCT_V3H) {
		/* RENESAS Condor board */
		brd = 12;
	} else if (usb2_ovc_open) {
		if (Prr_Product == PRR_PRODUCT_M3N) {
			/* RENESAS Kriek board with M3-N */
			brd = 10;
		} else if (Prr_Product == PRR_PRODUCT_M3) {
			/* RENESAS Kriek board with M3-W */
			brd = 1;
		} else if ((Prr_Product == PRR_PRODUCT_H3) && (Prr_Cut <= PRR_PRODUCT_11)) {
			/* RENESAS Kriek board with PM3 */
			brd = 13;
		} else if ((Prr_Product == PRR_PRODUCT_H3) && (Prr_Cut > PRR_PRODUCT_20)) {
			/* RENESAS Kriek board with H3N */
			brd = 15;
		}
	} else {
		if (Prr_Product == PRR_PRODUCT_H3) {
			if (Prr_Cut <= PRR_PRODUCT_11) {
				/* RENESAS SALVATOR-X (H3 Ver.1.x/SIP) */
				brd = 2;
			} else if (Prr_Cut < PRR_PRODUCT_30) {
				/* RENESAS SALVATOR-X (H3 Ver.2.0/SIP) */
				brd = 7;  /*  8Gbit/1rank */
			} else {
				/* RENESAS SALVATOR-X (H3 Ver.3.0/SIP) */
#if (RCAR_DRAM_LPDDR4_MEMCONF == 0)
				brd = 7;  /*  8Gbit/1rank */
#else
				brd = 8;  /*  8Gbit/2rank */
			/*	brd = 14;    16Gbit/1rank */
#endif
			}
		} else if (Prr_Product == PRR_PRODUCT_M3N) {
			/* RENESAS SALVATOR-X (M3-N/SIP) */
			brd = 11;
		} else if ((Prr_Product == PRR_PRODUCT_M3) && (Prr_Cut <= PRR_PRODUCT_20)) {
			/* RENESAS SALVATOR-X (M3-W/SIP) */
			brd = 0;
		} else if ((Prr_Product == PRR_PRODUCT_M3) && (Prr_Cut < PRR_PRODUCT_30)) {
			/* RENESAS SALVATOR-X (M3-W Ver.1.x/SIP) */
			brd = 19;
		} else if ((Prr_Product == PRR_PRODUCT_M3) && (Prr_Cut >= PRR_PRODUCT_30)) {
			/* RENESAS SALVATOR-X (M3-W Ver.3.0/SIP) */
			brd = 18;
		}
	}
#endif /* (RCAR_GEN3_ULCB == 1) */

	return brd;
}
#endif
