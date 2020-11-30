/*
 * Copyright (c) 2016-2017, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2018-2020, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/runtime_svc.h>
#include <tools_share/uuid.h>
#include <common/debug.h>
#include <rcar_sip_svc.h>
#include "board.h"

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
	int32_t		board_ret;
	uint32_t	board_type = 0U;
	uint32_t	board_rev = 0U;
	/* Rcar SiP Service UUID */
	DEFINE_SVC_UUID2(rcar_sip_svc_uid,
			0x120f8108U, 0x8152U, 0x4154U, 0xa2U, 0xafU,
			0x7dU, 0x8aU, 0x7eU, 0x6dU, 0x90U, 0xeeU);

	switch (smc_fid) {
	case RCAR_SIP_SVC_GET_BOARD_TYPE:
		/* get Board type */
#if PMIC_ROHM_BD9571
		ret = rcar_get_board_type(&board_type, &board_rev);
		if (ret != 0) {
			board_ret = RCAR_SMC_RET_EFAILED;
		} else {
			board_ret = RCAR_SMC_RET_SUCCESS;
		}
#else
		board_ret = RCAR_SMC_RET_PMIC_DISABLE;
#endif	/* PMIC_ROHM_BD9571 */
		SMC_RET3(handle, board_ret, board_type, board_rev);

	case RCAR_SIP_SVC_CALL_COUNT:
		/* Return the number of function IDs */
		SMC_RET1(handle, RCAR_SIP_SVC_FUNCTION_NUM);

	case RCAR_SIP_SVC_UID:
		/* Return UID to the caller */
		SMC_UUID_RET(handle, rcar_sip_svc_uid);

	case RCAR_SIP_SVC_VERSION:
		/* Return the version of current implementation */
		SMC_RET2(handle, RCAR_SIP_SVC_VERSION_MAJOR,
			RCAR_SIP_SVC_VERSION_MINOR);

	default:
		WARN("Unimplemented Rcar SiP Service Call: 0x%x\n", smc_fid);
		SMC_RET1(handle, SMC_UNK);
	}

}

/* Define a runtime service descriptor for fast SMC calls */
DECLARE_RT_SVC(
	rcar_sip_svc,
	(uint8_t)OEN_SIP_START,
	(uint8_t)OEN_SIP_END,
	(uint8_t)SMC_TYPE_FAST,
	NULL,
	rcar_sip_handler
);

