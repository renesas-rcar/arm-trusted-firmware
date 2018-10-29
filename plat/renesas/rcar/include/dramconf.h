/*
 * Copyright (c) 2018, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DRAMCONF_H__
#define DRAMCONF_H__

#include <stddef.h>
#include <mmio.h>
#include <utils_def.h>



#define RCAR_DRAMCONF_RET_SUCCESS		U(0)
#define RCAR_DRAMCONF_RET_EPARAMS		U(1)
#define RCAR_DRAMCONF_RET_EFAILED		U(2)

#define RCAR_DRAMCONF_CH_NUM			U(4)
#define RCAR_DRAMCONF_LEGACY_CH			U(0)
#define RCAR_DRAMCONF_LEGACY_ADDR		U(0x40000000)
#define RCAR_DRAMCONF_LEGACY_MAXSZ		U(0x80000000)


#define RCAR_DBSC_DBKIND			U(0xE6790020)
#define RCAR_DBSC_DBMEMCONF(ch,rank)		\
	(uint32_t)(U(0xE6790030) + (U(0x10) * (ch)) + (U(0x04) * (rank)))


#define RCAR_DBMEMCONF_DENS(data)		(((data) >> U(30)) & U(0x3))
#define RCAR_DBMEMCONF_AWRW(data)		(((data) >> U(24)) & U(0x1F))
#define RCAR_DBMEMCONF_AWBK(data)		(((data) >> U(16)) & U(0x7))
#define RCAR_DBMEMCONF_AWCL(data)		(((data) >> U(8)) & U(0xF))
#define RCAR_DBMEMCONF_DW(data)			((data) & U(0x3))

#define RCAR_DBMEMCONF_DENS_TYPE_2N		U(0)
#define RCAR_DBMEMCONF_DENS_TYPE_2N3		U(1)

#define RCAR_DBMEMCONF_AWRW_MIN			U(12)
#define RCAR_DBMEMCONF_AWRW_MAX			U(16)

#define RCAR_DBMEMCONF_AWBK_TYPE_8BANKS		U(3)

#define RCAR_DBMEMCONF_AWCL_MIN			U(9)
#define RCAR_DBMEMCONF_AWCL_MAX			U(15)

#define RCAR_DBMEMCONF_DW_TYPE_16BITS		U(1)
#define RCAR_DBMEMCONF_DW_TYPE_32BITS		U(2)


typedef struct {
	uint64_t	DramType;
	uint64_t	LegacyAddress;
	uint64_t	LegacySize;
	uint64_t	Address[RCAR_DRAMCONF_CH_NUM];
	uint64_t	Size[RCAR_DRAMCONF_CH_NUM];
} rcar_dram_conf_t;

typedef struct {
	uint64_t	address;
	uint32_t	rank0_st;
	uint32_t	rank1_st;
} rcar_dram_addr_t;


uint32_t rcar_get_dram_conf(rcar_dram_conf_t *data);

#endif /* DRAMCONF_H__ */
