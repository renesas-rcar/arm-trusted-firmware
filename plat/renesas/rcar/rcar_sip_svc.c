/*
 * Copyright (c) 2016-2017, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2018-2019, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <runtime_svc.h>
#include <smccc_helpers.h>
#include <uuid.h>
#include <debug.h>
#include <xlat_tables_v2.h>
#include <string.h>
#include <arch_helpers.h>
#include "rcar_private.h"
#include "rcar_sip_svc.h"
#include "dramconf.h"
#include "board.h"

#define RCAR_MMAP_DYNAMIC_ADDR_MASK	0xfffff000U
#define RCAR_MMAP_DYNAMIC_SIZE		0x2000U

#define RCAR_DRAMCONF_SET_AREA_MIN	0x48000000U
#define RCAR_DRAMCONF_SET_AREA_MAX	0x80000000U

#if RCAR_SMC_GET_DRAMCONF
static int32_t rcar_sip_get_dramconf(u_register_t addr);
#endif

/* Rcar SiP Service UUID */
DEFINE_SVC_UUID(rcar_sip_svc_uid,
		0x120f8108U, 0x8152U, 0x4154U, 0xa2U, 0xafU,
		0x7dU, 0x8aU, 0x7eU, 0x6dU, 0x90U, 0xeeU);


/*
 * This function handles Rcar defined SiP Calls
 */
static uintptr_t rcar_sip_handler(unsigned int smc_fid,
			u_register_t x1,
			u_register_t x2,
			u_register_t x3,
			u_register_t x4,
			void *cookie,
			void *handle,
			u_register_t flags)
{
	int32_t		ret;
	int32_t		board_ret = RCAR_SMC_RET_SUCCESS;
	uint32_t	board_type = 0U;
	uint32_t	board_rev = 0U;

	switch (smc_fid) {
	case RCAR_SIP_SVC_GET_DRAMCONF:
		/* get Dramconf */
#if RCAR_SMC_GET_DRAMCONF
		ret = rcar_sip_get_dramconf(x1);
#else
		ret = SMC_UNK;
#endif
		SMC_RET1(handle, ret);

	case RCAR_SIP_SVC_GET_BOARD_TYPE:
		/* get Board type */
#if PMIC_ROHM_BD9571
		ret = get_board_type(&board_type, &board_rev);
		if (0 != ret) {
			board_ret = RCAR_SMC_RET_EFAILED;
		}
#else	/* PMIC_ROHM_BD9571 */
		board_ret = RCAR_SMC_RET_PMIC_DISABLE;
#endif	/* PMIC_ROHM_BD9571 */
		SMC_RET3(handle, board_ret, board_type, board_rev);

	case RCAR_SIP_SVC_CALL_COUNT:
		/* Return the number of function IDs */
#if RCAR_SMC_GET_DRAMCONF
		SMC_RET1(handle, RCAR_SIP_SVC_FUNCTION_NUM);
#else
		SMC_RET1(handle, RCAR_SIP_SVC_FUNCTION_NUM - 1);
#endif

	case RCAR_SIP_SVC_UID:
		/* Return UID to the caller */
		SMC_UUID_RET(handle, rcar_sip_svc_uid);

	case RCAR_SIP_SVC_VERSION:
		/* Return the version of current implementation */
		SMC_RET2(handle, RCAR_SIP_SVC_VERSION_MAJOR,
			RCAR_SIP_SVC_VERSION_MINOR);

	default:
		WARN("Unimplemented Rcar SiP Service Call: 0x%x \n", smc_fid);
		SMC_RET1(handle, SMC_UNK);
	}

}

#if RCAR_SMC_GET_DRAMCONF
static int32_t rcar_sip_get_dramconf(u_register_t addr)
{
	int32_t			ret = RCAR_SMC_RET_SUCCESS;
	uint32_t		dram_ret;
	rcar_dram_conf_t	dram_conf;
	int32_t			mmap_ret;
	uint32_t		mmu_addr;

	if((addr < RCAR_DRAMCONF_SET_AREA_MIN) || 
	   (RCAR_DRAMCONF_SET_AREA_MAX <= addr)) {
		ERROR("RCAR set dramconf value outside the area=0x%lx.\n",addr);
		ret = RCAR_SMC_RET_EFAILED;
	}

	if(RCAR_SMC_RET_SUCCESS == ret) {
		/* get Dramconf */
		dram_ret = rcar_get_dram_conf(&dram_conf);
		if (RCAR_DRAMCONF_RET_SUCCESS != dram_ret) {
			ERROR("RCAR dramconf get_dram_conf err ret=%d.\n",
								dram_ret);
			ret = RCAR_SMC_RET_EFAILED;
		}
	}

	if(RCAR_SMC_RET_SUCCESS == ret) {
		mmu_addr = ((uint32_t)addr & RCAR_MMAP_DYNAMIC_ADDR_MASK);

		mmap_ret = mmap_add_dynamic_region(mmu_addr, mmu_addr,
			RCAR_MMAP_DYNAMIC_SIZE,
			(MT_MEMORY | MT_RW | MT_SECURE | MT_EXECUTE_NEVER));
		if(0 != mmap_ret) {
			ERROR("RCAR dramconf add_dynamic_mmap err ret=%d.\n",
								mmap_ret);
			ret = RCAR_SMC_RET_EFAILED;
		}
	}

	if(RCAR_SMC_RET_SUCCESS == ret) {
		(void)memcpy((void *)addr,&dram_conf,sizeof(rcar_dram_conf_t));

		flush_dcache_range((uint64_t)addr,sizeof(rcar_dram_conf_t));

		mmap_ret = mmap_remove_dynamic_region(mmu_addr,
							RCAR_MMAP_DYNAMIC_SIZE);
		if(0 != mmap_ret) {
			ERROR("RCAR dramconf remove_dynamic_mmap err ret=%d.\n",
								mmap_ret);
			ret = RCAR_SMC_RET_EFAILED;
		}
	}

	return ret;
}
#endif

/* Define a runtime service descriptor for fast SMC calls */
DECLARE_RT_SVC(
	rcar_sip_svc,
	OEN_SIP_START,
	OEN_SIP_END,
	SMC_TYPE_FAST,
	NULL,
	rcar_sip_handler
);

