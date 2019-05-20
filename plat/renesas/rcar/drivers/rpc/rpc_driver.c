/*
 * Copyright (c) 2015-2019, Renesas Electronics Corporation. All rights reserved.
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
#include "rcar_private.h"
#include "rcar_def.h"

#define MSTPSR9_RPC_BIT		(0x00020000U)
#define RPC_CMNCR_MD_BIT	(0x80000000U)
#define RPC_PHYCNT_CAL		((uint32_t)1U << 31U)
#define RPC_PHYCNT_STRTIM_M3V1	((uint32_t)0x6U << 15U)
#define RPC_PHYCNT_STRTIM	((uint32_t)0x7U << 15U)

static void enableRPC(void);
static void setupRPC(void);

static void enableRPC(void)
{
	/* Enable clock supply to RPC. */
	mstpcr_write(CPG_SMSTPCR9, CPG_MSTPSR9, MSTPSR9_RPC_BIT);
}

static void setupRPC(void)
{

	uint32_t product;
	uint32_t cut;
	uint32_t reg;
	uint32_t phy_strtim;

	/* manual mode */
	if ((mmio_read_32(RPC_CMNCR) & RPC_CMNCR_MD_BIT) != 0U) {
		/* external address space read mode */
		mmio_write_32(RPC_CMNCR,
			mmio_read_32(RPC_CMNCR) & (~RPC_CMNCR_MD_BIT));
	}

	product = mmio_read_32(RCAR_PRR) & RCAR_PRODUCT_MASK;
	cut = mmio_read_32(RCAR_PRR) & RCAR_CUT_MASK;

	if ((product ==  RCAR_PRODUCT_M3) && (cut < RCAR_CUT_VER30)) {
		phy_strtim = RPC_PHYCNT_STRTIM_M3V1;
	} else {
		phy_strtim = RPC_PHYCNT_STRTIM;
	}

	reg = mmio_read_32(RPC_PHYCNT);
	reg &= ~RPC_PHYCNT_STRTIM;
	reg |=  phy_strtim;
	mmio_write_32(RPC_PHYCNT, reg);
	reg |=  RPC_PHYCNT_CAL;
	mmio_write_32(RPC_PHYCNT, reg);

}

void initRPC(void)
{
	enableRPC();
	setupRPC();
}

