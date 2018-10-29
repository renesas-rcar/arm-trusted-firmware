/*
 * Copyright (c) 2018, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "dramconf.h"
#include "rcar_def.h"

static uint64_t rcar_get_dram_size(uint32_t ch_type, uint32_t rank_type);

uint32_t rcar_get_dram_conf(rcar_dram_conf_t *data)
{
	uint32_t		ret = RCAR_DRAMCONF_RET_SUCCESS;
	uint32_t		ch_cnt;
	uint64_t		tmp_size;
	uint32_t		lsi_product;
	rcar_dram_addr_t*	tmp_dram_addr;

	/* address, rank0_st, rank1_st */
	rcar_dram_addr_t h3_ch_addr[RCAR_DRAMCONF_CH_NUM] = {
		{0x400000000U,	1U,	1U},
		{0x500000000U,	1U,	1U},
		{0x600000000U,	1U,	1U},
		{0x700000000U,	1U,	1U}
	};

	/* address, rank0_st, rank1_st */
	rcar_dram_addr_t m3_ch_addr[RCAR_DRAMCONF_CH_NUM] = {
		{0x400000000U,	1U,	1U},
		{0x600000000U,	1U,	1U},
		{0x000000000U,	0U,	0U},
		{0x000000000U,	0U,	0U}
	};

	/* address, rank0_st, rank1_st */
	rcar_dram_addr_t m3n_ch_addr[RCAR_DRAMCONF_CH_NUM] = {
		{0x400000000U,	1U,	1U},
		{0x000000000U,	0U,	0U},
		{0x000000000U,	0U,	0U},
		{0x000000000U,	0U,	0U}
	};

	/* address, rank0_st, rank1_st */
	rcar_dram_addr_t e3_ch_addr[RCAR_DRAMCONF_CH_NUM] = {
		{0x400000000U,	1U,	0U},
		{0x000000000U,	0U,	0U},
		{0x000000000U,	0U,	0U},
		{0x000000000U,	0U,	0U}
	};

	/* NULL check */
	if (NULL == data) {
		ret = RCAR_DRAMCONF_RET_EPARAMS;
	} else {
		/* get dram_addr table */
		lsi_product = mmio_read_32(RCAR_PRR) & RCAR_PRODUCT_MASK;
		switch (lsi_product) {
		case RCAR_PRODUCT_H3:
			tmp_dram_addr = h3_ch_addr;
			break;
		case RCAR_PRODUCT_M3:
			tmp_dram_addr = m3_ch_addr;
			break;
		case RCAR_PRODUCT_M3N:
			tmp_dram_addr = m3n_ch_addr;
			break;
		case RCAR_PRODUCT_E3:
			tmp_dram_addr = e3_ch_addr;
			break;
		default:
			ret = RCAR_DRAMCONF_RET_EFAILED;
			break;
		}
	}

	if (RCAR_DRAMCONF_RET_SUCCESS == ret) {
		data->DramType = (uint64_t)mmio_read_32(RCAR_DBSC_DBKIND);
		data->LegacyAddress = RCAR_DRAMCONF_LEGACY_ADDR;

		/* set address and size */
		for (ch_cnt = 0U ; ch_cnt < RCAR_DRAMCONF_CH_NUM ; ch_cnt++) {

			tmp_size = 0U;
			if (1U == tmp_dram_addr[ch_cnt].rank0_st) {
				tmp_size = rcar_get_dram_size(ch_cnt, 0U);
			}
			if (1U == tmp_dram_addr[ch_cnt].rank1_st) {
				tmp_size += rcar_get_dram_size(ch_cnt, 1U);
			}
			data->Size[ch_cnt] = tmp_size;

			if (0U == tmp_size) {
				data->Address[ch_cnt] = 0U;
			} else {
				data->Address[ch_cnt] = tmp_dram_addr[ch_cnt].address;
			}

			/* set Legacy size */
			if (RCAR_DRAMCONF_LEGACY_CH == ch_cnt) {
				if (RCAR_DRAMCONF_LEGACY_MAXSZ < tmp_size) {
					tmp_size = RCAR_DRAMCONF_LEGACY_MAXSZ;
				}
				data->LegacySize = tmp_size;
			}
		}
	}

	return ret;
}


static uint64_t rcar_get_dram_size(uint32_t ch_type, uint32_t rank_type)
{
	uint64_t ret_size = 0U;
	uint32_t dbmemconf_val;
	uint32_t chk_flg = 0U;

	uint32_t dens_val;
	uint32_t awrw_val;
	uint32_t awbk_val;
	uint32_t awcl_val;
	uint32_t dw_val;

	/* Read SDRAM Configuration Setting Register */
	dbmemconf_val = mmio_read_32(RCAR_DBSC_DBMEMCONF(ch_type,rank_type));

	/* Memory Density Type */
	dens_val = RCAR_DBMEMCONF_DENS(dbmemconf_val);
	if ((RCAR_DBMEMCONF_DENS_TYPE_2N != dens_val) &&
	     (RCAR_DBMEMCONF_DENS_TYPE_2N3 != dens_val)) {
		chk_flg = 1U;
	}

	/* Row Address Bit Width */
	awrw_val = RCAR_DBMEMCONF_AWRW(dbmemconf_val);
	if ((awrw_val < RCAR_DBMEMCONF_AWRW_MIN) ||
	    (RCAR_DBMEMCONF_AWRW_MAX < awrw_val)) {
		chk_flg = 1U;
	}

	/* Number of Banks */
	awbk_val = RCAR_DBMEMCONF_AWBK(dbmemconf_val);
	if (RCAR_DBMEMCONF_AWBK_TYPE_8BANKS != awbk_val) {
		chk_flg = 1U;
	}

	/* Column Address Bit Width */
	awcl_val = RCAR_DBMEMCONF_AWCL(dbmemconf_val);
	if (awcl_val < RCAR_DBMEMCONF_AWCL_MIN) {
		chk_flg = 1U;
	}

	/* External Data Bus Width */
	dw_val = RCAR_DBMEMCONF_DW(dbmemconf_val);
	if ((RCAR_DBMEMCONF_DW_TYPE_16BITS != dw_val) &&
	     (RCAR_DBMEMCONF_DW_TYPE_32BITS != dw_val)) {
		chk_flg = 1U;
	}

	if (0U == chk_flg) {
		/* Calculate size */
		ret_size = ((uint64_t)1U << 
				(awrw_val + awbk_val + awcl_val + dw_val));
		if(RCAR_DBMEMCONF_DENS_TYPE_2N3 == dens_val) {
			ret_size = (ret_size / 4U) * 3U;
		}
	}

	return ret_size;
}
