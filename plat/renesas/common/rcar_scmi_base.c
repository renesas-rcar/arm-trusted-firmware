/*
 * Copyright (c) 2021 EPAM Systems Inc. All rights reserved.
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

#include "rcar_def.h"
#include "rcar_private.h"
#include "rcar_scmi_resources.h"

enum scmi_message_id {
	PROTOCOL_VERSION = 0x0,
	PROTOCOL_ATTRIBUTES,
	PROTOCOL_MESSAGE_ATTRIBUTES,
	BASE_DISCOVER_VENDOR,
	BASE_DISCOVER_SUB_VENDOR,
	BASE_DISCOVER_IMPLEMENTATION_VERSION,
	BASE_DISCOVER_LIST_PROTOCOLS,
	BASE_DISCOVER_AGENT,
	BASE_NOTIFY_ERRORS,
	BASE_SET_DEVICE_PERMISSIONS,
	BASE_SET_PROTOCOL_PERMISSIONS,
	BASE_RESET_AGENT_CONFIGURATION,
	SCMI_LAST_ID
};

static uint32_t protocol_version(size_t channel __unused,
				 volatile uint8_t *param,
				 size_t size __unused)
{
	int32_t *status = (int32_t*)param;
	uint32_t *version = (uint32_t*)(param + sizeof(*status));
	*status = SCMI_SUCCESS;
	*version = SCMI_PROTOCOL_VERSION;
	return sizeof(*status) + sizeof(*version);
}

static uint32_t protocol_attrs(size_t channel __unused,
			       volatile uint8_t *param,
			       size_t size __unused)
{
	int32_t *status = (int32_t*)param;
	uint32_t *attrs = (uint32_t*)(param + sizeof(*status));
	uint32_t num_proto = scmi_count_protocols();

	*status = SCMI_SUCCESS;
	*attrs = FLD(GENMASK(31, 16), 0) |
		 FLD(GENMASK(15, 8), RCAR_SCMI_CHAN_COUNT) |
		 FLD(GENMASK(7,  0), num_proto);

	return sizeof(*status) + sizeof(*attrs);
}

static uint32_t protocol_msg_attrs(size_t channel __unused,
				   volatile uint8_t *param,
				   size_t size)
{
	uint32_t id = *(uint32_t*)param;
	int32_t *status = (int32_t*)param;
	uint32_t *attrs = (uint32_t*)(param + sizeof(*status));

	if (size != sizeof(id)) {
		*status = SCMI_PROTOCOL_ERROR;
		return sizeof(*status);
	}

	/* BASE_NOTIFY_ERRORS not supported in synchronous SCP implementation
	 * BASE_SET_PROTOCOL_PERMISSIONS not implemented yet
	*/
	if (id == BASE_NOTIFY_ERRORS || id == BASE_SET_PROTOCOL_PERMISSIONS) {
		*status = SCMI_NOT_FOUND;
	} else {
		*status = SCMI_SUCCESS;
	}

	*attrs = 0x0;

	return sizeof(*status) + sizeof(*attrs);
}

static uint32_t __copy_vendor_str(volatile uint8_t *param,
				  const char src[], size_t len)
{
	struct vendor {
		int32_t status;
		char name[16];
	} *res = (struct vendor*)param;

	assert(len <= sizeof(res->name));

	strlcpy(res->name, src, len);
	res->status = SCMI_SUCCESS;

	return sizeof(*res);
}

static uint32_t get_vendor(size_t channel __unused,
			   volatile uint8_t *param,
			   size_t size __unused)
{
	return __copy_vendor_str(param, "ATF\0", 4);
}

static uint32_t get_sub_vendor(size_t channel __unused,
			       volatile uint8_t *param,
			       size_t size __unused)
{
	return __copy_vendor_str(param, "R-Car\0", 6);
}

static uint32_t impl_version(size_t channel __unused,
			     volatile uint8_t *param,
			     size_t size __unused)
{
	int32_t *status = (int32_t*)param;
	uint32_t *version = (uint32_t*)(param + sizeof(*status));
	*status = SCMI_SUCCESS;
	*version = 0x1001;
	return sizeof(*status) + sizeof(*version);
}

static uint32_t get_protocols(size_t channel __unused,
			      volatile uint8_t *param,
			      size_t size)
{
	struct protocols_response {
		int32_t status;
		uint32_t num_protocols;
		uint8_t protocols[];
	} *res = (struct protocols_response*)param;
	uint32_t skip = *(uint32_t*)param;
	uint32_t max_payload_size = SCMI_MAX_PAYLOAD - sizeof(*res);
	uint8_t protocol_id;
	int counter = 0;

	if (size != sizeof(skip)) {
		res->status = SCMI_PROTOCOL_ERROR;
		return sizeof(res->status);
	}

	if (skip > scmi_count_protocols()) {
		res->status = SCMI_INVALID_PARAMETERS;
		return sizeof(res->status);
	}

	protocol_id = scmi_get_first_protocol(skip);
	if (!protocol_id) {
		res->status = SCMI_SUCCESS;
		res->num_protocols = 0;
		return sizeof(*res);
	}

	while (protocol_id) {
		if (counter * sizeof(protocol_id) >= max_payload_size) {
			break;
		}

		res->protocols[counter] = protocol_id;
		protocol_id = scmi_get_next_protocol(protocol_id);
		counter++;
	}

	res->num_protocols = counter;
	res->status = SCMI_SUCCESS;

	return sizeof(*res) +
		ALIGN_NEXT(counter * sizeof(protocol_id), sizeof(uint32_t));
}

static uint32_t get_agent(size_t channel, volatile uint8_t *param, size_t size)
{
	struct agent_discovery {
		int32_t status;
		uint32_t agent_id;
		char name[16];
	} *res = (struct agent_discovery*)param;
	uint32_t agent_id = *(uint32_t*)param;

	if (size != sizeof(agent_id)) {
		res->status = SCMI_PROTOCOL_ERROR;
		return sizeof(res->status);
	}

	if (agent_id == 0xFFFFFFFF) {
		agent_id = channel_to_agent(channel);
	}

	switch (agent_id) {
	case 0:
		strlcpy(res->name, "platform ATF", sizeof(res->name));
		break;
	case 1:
		strlcpy(res->name, "HYP", sizeof(res->name));
		break;
	case 2 ... RCAR_SCMI_CHAN_COUNT:
		snprintf(res->name, sizeof(res->name), "VM%u", agent_id - 2);
		break;
	default:
		res->status = SCMI_NOT_FOUND;
		return sizeof(res->status);
	}

	res->agent_id = agent_id;
	res->status = SCMI_SUCCESS;

	return sizeof(*res);
}

static uint32_t notify_errors(size_t channel __unused,
			      volatile uint8_t *param,
			      size_t size __unused)
{
	*(int32_t *)param = SCMI_NOT_SUPPORTED;
	return sizeof(int32_t);
}

static inline void __set_perm(scmi_perm_t *perm, uint32_t agent_id, bool permit)
{
	scmi_perm_t mask = 1 << agent_to_channel(agent_id);
	if (permit) {
		*perm |= mask;
	} else {
		*perm &= ~mask;
	}
}

static inline void __scmi_permit(scmi_perm_t *perm, uint32_t agent_id)
{
	__set_perm(perm, agent_id, true);
}

static inline void __scmi_deny(scmi_perm_t *perm, uint32_t agent_id)
{
	__set_perm(perm, agent_id, false);
}

static inline void __mangle_dev_perm(uint32_t agent_id, uint32_t dev_id,
				  void (*fn)(scmi_perm_t*, uint32_t))
{
#define MANGLE(attr, devs) {\
	int *index = rcar_devices[dev_id].attr; \
	while (index && *index >= 0) { \
		fn(&devs[*index++].perm, agent_id); \
	} \
}
	assert(dev_id < RCAR_SCMIDEV_MAX);
	assert(agent_id <= RCAR_SCMI_CHAN_COUNT);
	MANGLE(rsts, rcar_resets);
	MANGLE(clks, rcar_clocks);
	MANGLE(pins, rcar_pinctrl);
#undef MANGLE
}

static inline void scmi_permit(uint32_t agent_id, uint32_t dev_id)
{
	__mangle_dev_perm(agent_id, dev_id, __scmi_permit);
}

static inline void scmi_deny(uint32_t agent_id, uint32_t dev_id)
{
	__mangle_dev_perm(agent_id, dev_id, __scmi_deny);
}

static inline void scmi_drop_permissions(uint32_t agent_id)
{
	assert(agent_id <= RCAR_SCMI_CHAN_COUNT);
	for (uint32_t dev_id = 0; dev_id < RCAR_SCMIDEV_MAX; dev_id++) {
		scmi_deny(agent_id, dev_id);
	}
}

static uint32_t set_dev_perm(size_t channel,
			     volatile uint8_t *payload,
			     size_t size)
{
	struct parameters {
		uint32_t agent_id;
		uint32_t device_id;
		uint32_t flags;
	} param = *((struct parameters*)payload);
	int32_t status;

	if (channel != 0) {
		status = SCMI_DENIED;
		goto error;
	}

	if (size != sizeof(param)) {
		status = SCMI_PROTOCOL_ERROR;
		goto error;
	}

	if (param.agent_id > RCAR_SCMI_CHAN_COUNT ||
	    param.device_id >= RCAR_SCMIDEV_MAX)
	{
		status = SCMI_NOT_FOUND;
		goto error;
	}

	if (param.flags & ~0x1) {
		status = SCMI_INVALID_PARAMETERS;
		goto error;
	}


	if (param.flags & 0x1) {
		scmi_permit(param.agent_id, param.device_id);
	} else {
		scmi_deny(param.agent_id, param.device_id);
	}

	status = SCMI_SUCCESS;
error:
	*(int32_t *)payload = status;
	return sizeof(status);
}

/* Not implemented yet */
static uint32_t set_protocol_perm(size_t channel __unused,
			      volatile uint8_t *param,
			      size_t size __unused)
{
	*(int32_t *)param = SCMI_NOT_SUPPORTED;
	return sizeof(int32_t);
}

static void reset_agent_resources(uint32_t agent_id)
{
	uint32_t channel = agent_to_channel(agent_id);
	for (uint32_t id = 0; id < RCAR_SCMICLK_MAX; id++) {
		struct scmi_clk *sclk = &rcar_clocks[id];
		if (!scmi_permission_granted(sclk->perm, channel)) {
			continue;
		}
		rcar_reset_clock(id, agent_id);
	}

	for (uint32_t id = 0; id < RCAR_SCMIRST_MAX; id++) {
		struct scmi_reset *rst = &rcar_resets[id];
		if (!scmi_permission_granted(rst->perm, channel)) {
			continue;
		}
		rcar_reset_reset(id);
	}
}

static uint32_t reset_agent(size_t channel,
			     volatile uint8_t *param,
			     size_t size)
{
	uint32_t agent_id = *(uint32_t*)param;
	uint32_t flags = *(uint32_t*)(param + sizeof(agent_id));
	int32_t status;

	if (size != sizeof(agent_id) + sizeof(flags)) {
		status = SCMI_PROTOCOL_ERROR;
		goto error;
	}

	if (agent_id > RCAR_SCMI_CHAN_COUNT) {
		status = SCMI_NOT_FOUND;
		goto error;
	}

	if (agent_id != channel_to_agent(channel) && channel != 0) {
		status = SCMI_DENIED;
		goto error;

	}

	if (flags & ~0x1) {
		status = SCMI_INVALID_PARAMETERS;
		goto error;
	}

	reset_agent_resources(agent_id);

	/* check if need to reset access permissions */
	if (flags & 0x1) {
		scmi_drop_permissions(agent_id);
	}

	status = SCMI_SUCCESS;
error:
	*(int32_t *)param = status;
	return sizeof(status);
}

typedef uint32_t (*base_handler_t)(size_t, volatile uint8_t*,size_t);

static base_handler_t base_handlers[SCMI_LAST_ID] = {
	[PROTOCOL_VERSION] = protocol_version,
	[PROTOCOL_ATTRIBUTES] = protocol_attrs,
	[PROTOCOL_MESSAGE_ATTRIBUTES] = protocol_msg_attrs,
	[BASE_DISCOVER_VENDOR] = get_vendor,
	[BASE_DISCOVER_SUB_VENDOR] = get_sub_vendor,
	[BASE_DISCOVER_IMPLEMENTATION_VERSION] = impl_version,
	[BASE_DISCOVER_LIST_PROTOCOLS] = get_protocols,
	[BASE_DISCOVER_AGENT] = get_agent,
	[BASE_NOTIFY_ERRORS] = notify_errors,
	[BASE_SET_DEVICE_PERMISSIONS] = set_dev_perm,
	[BASE_SET_PROTOCOL_PERMISSIONS] = set_protocol_perm,
	[BASE_RESET_AGENT_CONFIGURATION] = reset_agent,
};

uint32_t rcar_scmi_handle_base(size_t channel, uint8_t cmd,
			       volatile uint8_t *param, size_t size)
{
	if (cmd >= SCMI_LAST_ID) {
		*(int32_t *)param = SCMI_NOT_SUPPORTED;
		return sizeof(int32_t);
	}

	assert(base_handlers[cmd] != NULL);

	return base_handlers[cmd](channel, param, size);
}
