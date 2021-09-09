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

#include "rcar_private.h"
#include "rcar_scmi_resources.h"

enum scmi_message_id {
	PROTOCOL_VERSION = 0x0,
	PROTOCOL_ATTRIBUTES,
	PROTOCOL_MESSAGE_ATTRIBUTES,
	POWER_DOMAIN_ATTRIBUTES,
	POWER_STATE_SET,
	POWER_STATE_GET,
	POWER_STATE_NOTIFY,
	POWER_STATE_CHANGE_REQUESTED_NOTIFY,
	SCMI_LAST_ID
};

static uint32_t global_power_state = 0;

static uint32_t protocol_version(size_t channel __unused,
				 volatile uint8_t *param,
				 size_t size __unused)
{
	int32_t *status = (int32_t*)param;
	uint32_t *version = (uint32_t*)(param + sizeof(*status));
	*status = SCMI_SUCCESS;
	*version = 0x21000; /* DEN0056C, 4.3.2.1 */
	return sizeof(*status) + sizeof(*version);
}

static uint32_t protocol_attrs(size_t channel __unused,
			       volatile uint8_t *param,
			       size_t size __unused)
{
	struct proto_attrs {
		int32_t status;
		uint32_t attributes;
		uint32_t statistic_addr[2];
		uint32_t statistic_len;
	} *res = (struct proto_attrs*)param;

	res->status = SCMI_SUCCESS;
	/*TODO: single stub domain for now */
	res->attributes = FLD(GENMASK(31, 16), 0) |
			  FLD(GENMASK(15,  0), 1);
	res->statistic_len = 0;

	return sizeof(*res);
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

	/* notifications not supported in synchronous SCP implementation */
	if (id == POWER_STATE_NOTIFY ||
	    id == POWER_STATE_CHANGE_REQUESTED_NOTIFY)
	{
		*status = SCMI_NOT_FOUND;
	} else {
		*status = SCMI_SUCCESS;
	}

	*attrs = 0x0;

	return sizeof(*status) + sizeof(*attrs);
}

static uint32_t power_attrs(size_t channel __unused,
			    volatile uint8_t *param,
			    size_t size __unused)
{
	struct power_discovery {
		int32_t status;
		uint32_t attributes;
		char name[16];
	} *res = (struct power_discovery*)param;

	/*TODO: stub implementation, single same domain only */
	res->status = SCMI_SUCCESS;
	res->attributes = 0;
	strlcpy(res->name, "scmi_core", sizeof(res->name));
	return sizeof(*res);
}

static uint32_t power_state_set(size_t channel __unused,
				 volatile uint8_t *payload,
				 size_t size)
{
	struct parameters {
		uint32_t flags;
		uint32_t domain_id;
		uint32_t state;
	} param = *((struct parameters*)payload);
	int32_t status = SCMI_SUCCESS;

	if (size != sizeof(param)) {
		status = SCMI_PROTOCOL_ERROR;
		goto error;
	}

	global_power_state = param.state;
error:
	*(int32_t *)payload = status;
	return sizeof(status);
}

static uint32_t power_state_get(size_t channel __unused,
				 volatile uint8_t *param,
				 size_t size)
{
	struct power_state {
		int32_t status;
		uint32_t state;
	} *res = (struct power_state*)param;
	uint32_t domain_id = *(uint32_t*)param;

	if (size != sizeof(domain_id)) {
		res->status = SCMI_PROTOCOL_ERROR;
		return sizeof(res->status);
	}

	res->state = global_power_state;
	res->status = SCMI_SUCCESS;

	return sizeof(*res);
}

static uint32_t not_supported(size_t channel __unused,
			      volatile uint8_t *param,
			      size_t size)
{
	*(int32_t *)param = SCMI_NOT_SUPPORTED;
	return sizeof(int32_t);
}

typedef uint32_t (*power_handler_t)(size_t, volatile uint8_t*,size_t);

static power_handler_t power_handlers[SCMI_LAST_ID] = {
	[PROTOCOL_VERSION] = protocol_version,
	[PROTOCOL_ATTRIBUTES] = protocol_attrs,
	[PROTOCOL_MESSAGE_ATTRIBUTES] = protocol_msg_attrs,
	[POWER_DOMAIN_ATTRIBUTES] = power_attrs,
	[POWER_STATE_SET] = power_state_set,
	[POWER_STATE_GET] = power_state_get,
	[POWER_STATE_NOTIFY] = not_supported,
	[POWER_STATE_CHANGE_REQUESTED_NOTIFY] = not_supported,
};

uint32_t rcar_scmi_handle_power(size_t channel, uint8_t cmd,
			       volatile uint8_t *param, size_t size)
{	if (cmd >= SCMI_LAST_ID) {
		*(int32_t *)param = SCMI_NOT_SUPPORTED;
		return sizeof(int32_t);
	}

	assert(power_handlers[cmd] != NULL);

	return power_handlers[cmd](channel, param, size);
}
