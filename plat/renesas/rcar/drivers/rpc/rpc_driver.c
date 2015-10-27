/*
 * Copyright (c) 2015-2016, Renesas Electronics Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   - Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *   - Neither the name of Renesas nor the names of its contributors may be
 *     used to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <string.h>
#include "bl2_cpg_register.h"
#include "bl2_cpg_write.h"
#include "bl2_rpc_register.h"
#include "rpc_driver.h"
#include "debug.h"

static void enableRPC(void);
static void setupRPC(void);
#if DEBUG
static void disableRPC(void);
#endif

static void enableRPC(void)
{
	while((mmio_read_32(CPG_MSTPSR9) & 0x00020000U) != 0U) {
		cpg_write(CPG_SMSTPCR9,
			mmio_read_32(CPG_SMSTPCR9) & (~0x00020000U));
	}
}

static void setupRPC(void)
{
	/* manual mode */
	if ((mmio_read_32(RPC_CMNCR) & 0x80000000U) != 0U) {
		/* external address space read mode */
		mmio_write_32(RPC_CMNCR,
			mmio_read_32(RPC_CMNCR) & (~0x80000000U));
	}
}

#if DEBUG
static void disableRPC(void)
{
	cpg_write(CPG_SMSTPCR9,
		mmio_read_32(CPG_SMSTPCR9) | 0x00020000U);
}
#endif

void initRPC(void)
{
	enableRPC();
	setupRPC();
}

#if DEBUG
void termRPC(void)
{
	disableRPC();
}
#endif
