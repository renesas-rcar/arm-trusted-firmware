/*
 * Copyright (c) 2018-2019, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RCAR_SIP_SVC_H__
#define RCAR_SIP_SVC_H__

#include <utils_def.h>


/* General Service Queries */
#define RCAR_SIP_SVC_CALL_COUNT		U(0x8200ff00)
#define RCAR_SIP_SVC_UID		U(0x8200ff01)
#define RCAR_SIP_SVC_VERSION		U(0x8200ff03)

/* Function ID to get the Dramconf */
#define RCAR_SIP_SVC_GET_DRAMCONF	U(0x82000000)

/* Function ID to get the Board type */
#define RCAR_SIP_SVC_GET_BOARD_TYPE	U(0x82000001)

/* Rcar SiP Service Calls version numbers */
#define RCAR_SIP_SVC_VERSION_MAJOR	U(0x0)
#define RCAR_SIP_SVC_VERSION_MINOR	U(0x2)

/* Number of function IDs excluding general service queries */
#define RCAR_SIP_SVC_FUNCTION_NUM	U(0x2)

#define RCAR_SMC_RET_SUCCESS		0
#define RCAR_SMC_RET_EFAILED		-2
#define RCAR_SMC_RET_PMIC_DISABLE	-3


#endif /* RCAR_SIP_SVC_H__ */
