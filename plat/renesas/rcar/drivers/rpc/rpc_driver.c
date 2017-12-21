/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <string.h>
#include <mmio.h>
#include "bl2_cpg_register.h"
#include "bl2_cpg_init.h"
#include "bl2_rpc_register.h"
#include "rpc_driver.h"
#include "debug.h"

#define MSTPSR9_RPC_BIT		(0x00020000U)
#define RPC_CMNCR_MD_BIT	(0x80000000U)

static void enableRPC(void);
static void setupRPC(void);

static void enableRPC(void)
{
	while((mmio_read_32(CPG_MSTPSR9) & MSTPSR9_RPC_BIT) != 0U) {
		cpg_write(CPG_SMSTPCR9,
			mmio_read_32(CPG_SMSTPCR9) & (~MSTPSR9_RPC_BIT));
	}
}

static void setupRPC(void)
{
	/* manual mode */
	if ((mmio_read_32(RPC_CMNCR) & RPC_CMNCR_MD_BIT) != 0U) {
		/* external address space read mode */
		mmio_write_32(RPC_CMNCR,
			mmio_read_32(RPC_CMNCR) & (~RPC_CMNCR_MD_BIT));
	}
}

void initRPC(void)
{
	enableRPC();
	setupRPC();
}

