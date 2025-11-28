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
#include <lib/mmio.h>

#include "micro_delay.h"
#include "rcar_private.h"
#include "rcar_scmi_resources.h"

#define CPG_BASE U(0xE6150000)

struct scmi_reset rcar_resets[RCAR_SCMIRST_MAX] = {
	[RCAR_SCMIRST_EAVB] = {
		.rst_reg = 0x920,
		.clr_reg = 0x960,
		.bit_off = 12,
	},
	[RCAR_SCMIRST_HDMI0] = {
		.rst_reg = 0x14C,
		.clr_reg = 0x95C,
		.bit_off = 29,
	},
	[RCAR_SCMIRST_HDMI1] = {
		.rst_reg = 0x14C,
		.clr_reg = 0x95C,
		.bit_off = 28,
	},
	[RCAR_SCMIRST_I2C0] = {
		.rst_reg = 0x924,
		.clr_reg = 0x964,
		.bit_off = 31,
	},
	[RCAR_SCMIRST_I2C1] = {
		.rst_reg = 0x924,
		.clr_reg = 0x964,
		.bit_off = 30,
	},
	[RCAR_SCMIRST_I2C2] = {
		.rst_reg = 0x924,
		.clr_reg = 0x964,
		.bit_off = 29,
	},
	[RCAR_SCMIRST_I2C3] = {
		.rst_reg = 0x924,
		.clr_reg = 0x964,
		.bit_off = 28,
	},
	[RCAR_SCMIRST_I2C4] = {
		.rst_reg = 0x924,
		.clr_reg = 0x964,
		.bit_off = 27,
	},
	[RCAR_SCMIRST_XHCI0] = {
		.rst_reg = 0x0B8,
		.clr_reg = 0x94C,
		.bit_off = 28,
	},
	[RCAR_SCMIRST_USB2_01] = {
		.rst_reg = 0x1CC,
		.clr_reg = 0x95C,
		.bit_off = 3,
	},
	[RCAR_SCMIRST_USB2_02] = {
		.rst_reg = 0x1CC,
		.clr_reg = 0x95C,
		.bit_off = 4,
	},
	[RCAR_SCMIRST_USB2_1] = {
		.rst_reg = 0x1CC,
		.clr_reg = 0x95C,
		.bit_off = 2,
	},
	[RCAR_SCMIRST_USB_DMAC0] = {
		.rst_reg = 0x0B8,
		.clr_reg = 0x94C,
		.bit_off = 30,
	},
	[RCAR_SCMIRST_USB_DMAC1] = {
		.rst_reg = 0x0B8,
		.clr_reg = 0x94C,
		.bit_off = 31,
	},
	[RCAR_SCMIRST_USB_DMAC2] = {
		.rst_reg = 0x0B8,
		.clr_reg = 0x94C,
		.bit_off = 26,
	},
	[RCAR_SCMIRST_USB_DMAC3] = {
		.rst_reg = 0x0B8,
		.clr_reg = 0x94C,
		.bit_off = 29,
	},
	[RCAR_SCMIRST_SSI] = {
		.rst_reg = 0x928,
		.clr_reg = 0x968,
		.bit_off = 5,
	},
	[RCAR_SCMIRST_SSI9] = {
		.rst_reg = 0x928,
		.clr_reg = 0x968,
		.bit_off = 6,
	},
	[RCAR_SCMIRST_SSI8] = {
		.rst_reg = 0x928,
		.clr_reg = 0x968,
		.bit_off = 7,
	},
	[RCAR_SCMIRST_SSI7] = {
		.rst_reg = 0x928,
		.clr_reg = 0x968,
		.bit_off = 8,
	},
	[RCAR_SCMIRST_SSI6] = {
		.rst_reg = 0x928,
		.clr_reg = 0x968,
		.bit_off = 9,
	},
	[RCAR_SCMIRST_SSI5] = {
		.rst_reg = 0x928,
		.clr_reg = 0x968,
		.bit_off = 10,
	},
	[RCAR_SCMIRST_SSI4] = {
		.rst_reg = 0x928,
		.clr_reg = 0x968,
		.bit_off = 11,
	},
	[RCAR_SCMIRST_SSI3] = {
		.rst_reg = 0x928,
		.clr_reg = 0x968,
		.bit_off = 12,
	},
	[RCAR_SCMIRST_SSI2] = {
		.rst_reg = 0x928,
		.clr_reg = 0x968,
		.bit_off = 13,
	},
	[RCAR_SCMIRST_SSI1] = {
		.rst_reg = 0x928,
		.clr_reg = 0x968,
		.bit_off = 14,
	},
	[RCAR_SCMIRST_SSI0] = {
		.rst_reg = 0x928,
		.clr_reg = 0x968,
		.bit_off = 15,
	},
	[RCAR_SCMIRST_AUDMAC1] = {
		.rst_reg = 0x0C4,
		.clr_reg = 0x954,
		.bit_off = 1,
	},
	[RCAR_SCMIRST_AUDMAC0] = {
		.rst_reg = 0x0C4,
		.clr_reg = 0x954,
		.bit_off = 2,
	},
};

enum scmi_message_id {
	PROTOCOL_VERSION = 0x0,
	PROTOCOL_ATTRIBUTES,
	PROTOCOL_MESSAGE_ATTRIBUTES,
	RESET_DOMAIN_ATTRIBUTES,
	RESET,
	RESET_NOTIFY,
	SCMI_LAST_ID
};

static void rcar_assert_domain(uint32_t domain)
{
	assert(domain < ARRAY_SIZE(rcar_resets));
	mmio_write_32(CPG_BASE + rcar_resets[domain].rst_reg,
		      BIT(rcar_resets[domain].bit_off));
}

static void rcar_deassert_domain(uint32_t domain)
{
	assert(domain < ARRAY_SIZE(rcar_resets));
	mmio_write_32(CPG_BASE + rcar_resets[domain].clr_reg,
		      BIT(rcar_resets[domain].bit_off));
}

static void rcar_auto_domain(uint32_t domain)
{
	rcar_assert_domain(domain);
	rcar_micro_delay(35);
	rcar_deassert_domain(domain);
}

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

	*status = SCMI_SUCCESS;
	*attrs = FLD(GENMASK(31, 16), 0) |
		 FLD(GENMASK(15,  0), RCAR_SCMIRST_MAX);

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

	/* RESET_NOTIFY not supported in synchronous SCP implementation */
	if (id == RESET_NOTIFY) {
		*status = SCMI_NOT_FOUND;
	} else {
		*status = SCMI_SUCCESS;
	}

	*attrs = 0x0;

	return sizeof(*status) + sizeof(*attrs);
}

static uint32_t reset_attrs(size_t channel __unused,
			    volatile uint8_t *param,
			    size_t size)
{
	struct rst_discovery {
		int32_t status;
		uint32_t attributes;
		uint32_t latency;
		char name[16];
	} *res = (struct rst_discovery*)param;
	uint32_t domain_id = *(uint32_t*)param;

	if (size != sizeof(domain_id)) {
		res->status = SCMI_PROTOCOL_ERROR;
		return sizeof(res->status);
	}

	if (domain_id >= RCAR_SCMIRST_MAX) {
		res->status = SCMI_NOT_FOUND;
		return sizeof(res->status);
	}

	memset(res->name, 0, sizeof(res->name));

	/* synchronous SCP implementation */
	res->attributes = 0;
	res->latency = 0xFFFFFFFF;

	res->status = SCMI_SUCCESS;

	return sizeof(*res);
}

static uint32_t reset(size_t channel,
		      volatile uint8_t *payload,
		      size_t size)
{
	struct parameters {
		uint32_t id;
		uint32_t flags;
		uint32_t state;
	} param = *((struct parameters*)payload);
	int32_t status;

	if (size != sizeof(param)) {
		status = SCMI_PROTOCOL_ERROR;
		goto error;
	}

	if (param.id >= RCAR_SCMIRST_MAX) {
		status = SCMI_NOT_FOUND;
		goto error;
	}

	if (param.flags & GENMASK(31,2)) {
		/* synchronous SCP implementation */
		status = SCMI_INVALID_PARAMETERS;
		goto error;
	}

	if (!scmi_permission_granted(rcar_resets[param.id].perm, channel)) {
		status = SCMI_DENIED;
		goto error;
	}

	if (param.flags & BIT(0)) {
		rcar_auto_domain(param.id);
	} else if (param.flags & BIT(1)) {
		rcar_assert_domain(param.id);
	} else {
		rcar_deassert_domain(param.id);
	}

	status = SCMI_SUCCESS;
error:
	*(int32_t *)payload = status;
	return sizeof(status);
}

static uint32_t reset_notify(size_t channel __unused,
				   volatile uint8_t *param,
				   size_t size)
{
	*(int32_t *)param = SCMI_NOT_SUPPORTED;
	return sizeof(int32_t);
}

typedef uint32_t (*reset_handler_t)(size_t, volatile uint8_t*,size_t);

static reset_handler_t reset_handlers[SCMI_LAST_ID] = {
	[PROTOCOL_VERSION] = protocol_version,
	[PROTOCOL_ATTRIBUTES] = protocol_attrs,
	[PROTOCOL_MESSAGE_ATTRIBUTES] = protocol_msg_attrs,
	[RESET_DOMAIN_ATTRIBUTES] = reset_attrs,
	[RESET] = reset,
	[RESET_NOTIFY] = reset_notify,
};

uint32_t rcar_scmi_handle_reset(size_t channel, uint8_t cmd,
			       volatile uint8_t *param, size_t size)
{	if (cmd >= SCMI_LAST_ID) {
		*(int32_t *)param = SCMI_NOT_SUPPORTED;
		return sizeof(int32_t);
	}

	assert(reset_handlers[cmd] != NULL);

	return reset_handlers[cmd](channel, param, size);
}

void rcar_reset_reset(uint32_t domain_id)
{
	assert(domain_id < RCAR_SCMIRST_MAX);
	rcar_auto_domain(domain_id);
}

