/*
 * Copyright (c) 2016,2017 ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2017,2021 EPAM Systems Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
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

#include <assert.h>
#include <common/debug.h>
#include <lib/mmio.h>
#include <lib/smccc.h>

#include "rcar_def.h"
#include "rcar_private.h"

#define RCAR_SCMI_SHMEM_BASE	DRAM2_NS_SCMI_BASE /* va == pa */

enum scmi_protocol_id {
	SCMI_BASE_PROTO  = 0x10,
	SCMI_POWER_PROTO = 0x11,
	SCMI_CLOCK_PROTO = 0x14,
	SCMI_RESET_PROTO = 0x16,
	SCMI_LAST_PROTO
};

typedef volatile struct scmi_shared_mem {
	 uint32_t reserved;
	 uint32_t channel_status;
	 uint32_t reserved1[2];
	 uint32_t flags;
	 uint32_t length;
	 uint32_t msg_header;
	 uint8_t msg_payload[];
} scmi_shmem_t;

static uint32_t scmi_handle_default(size_t channel __unused,
				    uint8_t cmd __unused,
				    volatile uint8_t *param,
				    size_t size __unused)
{
	*(int32_t *)param = SCMI_NOT_SUPPORTED;
	return sizeof(int32_t);
}

typedef uint32_t (*proto_handler_t)(size_t,uint8_t,volatile uint8_t*,size_t);

static proto_handler_t proto_handlers[SCMI_LAST_PROTO] = {
	[0 ... SCMI_LAST_PROTO - 1] = scmi_handle_default,
	[SCMI_BASE_PROTO]  = rcar_scmi_handle_base,
	[SCMI_POWER_PROTO]  = rcar_scmi_handle_power,
	[SCMI_CLOCK_PROTO] = rcar_scmi_handle_clock,
	[SCMI_RESET_PROTO] = rcar_scmi_handle_reset,
};

static uint32_t scmi_handle_cmd(size_t ch, uint8_t protocol, uint8_t cmd,
				volatile uint8_t *payload,
				size_t payload_size)
{
	if (protocol < ARRAY_SIZE(proto_handlers)) {
		return proto_handlers[protocol](ch, cmd, payload, payload_size);
	}

	*(int32_t *)payload = SCMI_NOT_SUPPORTED;
	return sizeof(int32_t);
}

uint32_t scmi_count_protocols(void)
{
	uint32_t count = 0;
	for (int i = 0; i < ARRAY_SIZE(proto_handlers); i++) {
		if (proto_handlers[i] != scmi_handle_default) {
			count++;
		}
	}
	return count;
}

uint8_t scmi_get_first_protocol(uint32_t skip)
{
	uint32_t skipped = 0;
	if (skip >= ARRAY_SIZE(proto_handlers)) {
		return 0;
	}

	for (int i = 0; i < ARRAY_SIZE(proto_handlers); i++) {
		if (proto_handlers[i] != scmi_handle_default) {
			if (skipped == skip) {
				return i;
			}
			skipped++;
		}
	}

	return 0;
}

uint8_t scmi_get_next_protocol(uint8_t protocol_id)
{
	if (protocol_id >= ARRAY_SIZE(proto_handlers)) {
		return 0;
	}

	for (int i = protocol_id + 1; i < ARRAY_SIZE(proto_handlers); i++) {
		if (proto_handlers[i] != scmi_handle_default) {
			return i;
		}
	}

	return 0;
}

int rcar_setup_scmi(void)
{
	return rcar_cpg_init();
}

uint32_t rcar_trigger_scmi(size_t chan_id)
{
	scmi_shmem_t *shmem;
	size_t payload_size;
	uint8_t protocol_id, message_id;

	if (chan_id >= RCAR_SCMI_CHAN_COUNT) {
		return SMC_ARCH_CALL_INVAL_PARAM;
	}

	shmem = (struct scmi_shared_mem *)
		(RCAR_SCMI_SHMEM_BASE + chan_id * PAGE_SIZE);
	payload_size = MAX(sizeof(shmem->msg_header),(size_t)shmem->length);
	payload_size -= sizeof(shmem->msg_header);
	protocol_id = FLD_GET(GENMASK(17,10), shmem->msg_header);
	message_id = FLD_GET(GENMASK(7,0), shmem->msg_header);

	VERBOSE("BL31-SCMI: channel: 0x%lx, protocol: 0x%x, command: 0x%x\n",
		chan_id, protocol_id, message_id);

	shmem->length = scmi_handle_cmd(chan_id, protocol_id, message_id,
					shmem->msg_payload, payload_size);

	assert(shmem->length + sizeof(*shmem) < PAGE_SIZE);
	shmem->length += sizeof(shmem->msg_header);
	shmem->channel_status = BIT(0); /* mark free */
	flush_dcache_range((uintptr_t)shmem, sizeof(*shmem) + shmem->length);

	return SMC_ARCH_CALL_SUCCESS;
}
