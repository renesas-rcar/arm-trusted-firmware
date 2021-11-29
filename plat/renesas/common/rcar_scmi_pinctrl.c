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
#include <lib/mmio.h>
#include <drivers/pinctrl.h>
#include "rcar_private.h"
#include "rcar_scmi_resources.h"

enum scmi_message_id {
	PROTOCOL_VERSION = 0x0,
	PROTOCOL_ATTRIBUTES,
	PROTOCOL_MESSAGE_ATTRIBUTES,
	PINCTRL_GET_GROUP_PINS = 0x3,
	PINCTRL_GET_FUNCTION_GROUPS = 0x4,
	PINCTRL_SET_MUX = 0x5,
	PINCTRL_GET_PINS = 0x6,
	PINCTRL_GET_CONFIG = 0x7,
	PINCTRL_SET_CONFIG = 0x8,
	PINCTRL_GET_CONFIG_GROUP = 0x9,
	PINCTRL_SET_CONFIG_GROUP = 0xa,
	SCMI_LAST_ID
};

static const struct scmi_pinctrl default_device = {
	.groups = (int[]){-1},
	.pins = (int[]){-1},
};

struct scmi_pinctrl rcar_pinctrl[RCAR_PINCTRL_MAX] = {
	[0 ... RCAR_PINCTRL_MAX -1 ] = default_device,
};

static uint32_t protocol_version(size_t channel __unused,
				 volatile uint8_t *param,
				 size_t size __unused)
{
	int32_t *status = (int32_t*)param;
	uint32_t *version = (uint32_t*)(param + sizeof(*status));
	*status = SCMI_SUCCESS;
	*version = 0x10000;
	VERBOSE("scmi: Process pinctrl protocol_version\n");
	return sizeof(*status) + sizeof(*version);
}

static uint32_t protocol_attrs(size_t channel __unused,
			       volatile uint8_t *param,
			       size_t size __unused)
{
	int32_t *status = (int32_t*)param;
	uint32_t *attrs = (uint32_t*)(param + sizeof(*status));
	uint16_t nr_groups = pinctrl_get_groups_count();
	uint16_t nr_functions = pinctrl_get_functions_count();

	*status = SCMI_SUCCESS;
	*attrs = FLD(GENMASK(31, 16), nr_groups) |
		 FLD(GENMASK(15,  0), nr_functions);

	VERBOSE("scmi: Process pinctrl protocol_attrs group_max = %d, func_max = %d\n",
			nr_groups, nr_functions);
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

	/* all commands are mandatory */
	*status = SCMI_SUCCESS;
	*attrs = 0x0;

	VERBOSE("scmi: Process pinctrl protocol_msg_attrs\n");

	return sizeof(*status) + sizeof(*attrs);
}

static uint32_t get_group_pins(size_t channel __unused,
			       volatile uint8_t *param,
			       size_t size __unused)
{
	struct pins_tx {
		uint16_t selector;
		uint16_t skip;
	} tx = *(struct pins_tx *)param;
	struct pins_rx {
		int32_t status;
		uint32_t nr_pins;
		uint16_t pins[];
	} *rx = (struct pins_rx *)param;
	uint32_t max_payload_size = SCMI_MAX_PAYLOAD - sizeof(*rx);
	int counter = 0, lcounter = 0;

	const unsigned *pins;
	unsigned nr_pins;
	int ret;

	VERBOSE("scmi: pinctrl get_group_pins sel = %d skip = %d\n", tx.selector, tx.skip);
	if (size != sizeof(tx)) {
		rx->status = SCMI_PROTOCOL_ERROR;
		return sizeof(rx->status);
	}

	ret = pinctrl_get_group_pins(tx.selector, &pins, &nr_pins);
	if (ret) {
		ERROR("scmi: pinctrl_get_group_pins failed with ret = %d\n", ret);
		rx->status = SCMI_GENERIC_ERROR;
		return sizeof(rx->status);
	}

	for (counter = tx.skip; counter < nr_pins; counter++, lcounter++) {
		if (lcounter * sizeof(uint16_t) >= max_payload_size) {
			break;
		}

		rx->pins[lcounter] = pins[counter];
		VERBOSE("group pins [%d] = %d\n", lcounter, pins[counter]);
	}

	rx->nr_pins = lcounter;
	VERBOSE("scmi: pinctrl get nr_pins= %d\n", rx->nr_pins);
	rx->status = SCMI_SUCCESS;

	return sizeof(*rx) +
		ALIGN_NEXT(lcounter * sizeof(uint16_t), sizeof(uint32_t));
}

static uint32_t get_pins(size_t channel __unused,
			       volatile uint8_t *param,
			       size_t size __unused)
{
	uint32_t skip = *(uint32_t *)param;
	struct pins_rx {
		int32_t status;
		uint32_t nr_pins;
		uint16_t pins[];
	} *rx = (struct pins_rx *)param;
	uint32_t max_payload_size = SCMI_MAX_PAYLOAD - sizeof(*rx);
	int counter = 0, lcounter = 0;

	const struct pinctrl_pin *pins;
	unsigned nr_pins;
	int ret;

	VERBOSE("scmi: pinctrl get_pins skip = %d\n", skip);
	if (size != sizeof(skip)) {
		rx->status = SCMI_PROTOCOL_ERROR;
		return sizeof(rx->status);
	}

	ret = pinctrl_get_pins(&pins, &nr_pins);
	if (ret) {
		ERROR("scmi: pinctrl_get_pins failed with ret = %d\n", ret);
		rx->status = SCMI_GENERIC_ERROR;
		return sizeof(rx->status);
	};

	VERBOSE("max_payload_size = %d\n", max_payload_size);

	for (counter = skip; counter < nr_pins; counter++, lcounter++) {
		const struct pinctrl_pin *info = &pins[counter];
		if (lcounter * sizeof(uint16_t) >= max_payload_size) {
			break;
		}

		rx->pins[lcounter] = info->pin != (uint16_t)-1 ? info->pin : counter;
	}

	rx->nr_pins = lcounter;
	VERBOSE("scmi: pinctrl get nr_pins = %d\n", rx->nr_pins);
	rx->status = SCMI_SUCCESS;

	return sizeof(*rx) +
		ALIGN_NEXT(lcounter * sizeof(uint16_t), sizeof(uint32_t));
}

static uint32_t get_function_groups(size_t channel __unused,
			       volatile uint8_t *param,
			       size_t size __unused)
{
	struct groups_tx {
		uint16_t selector;
		uint16_t skip;
	} tx = *(struct groups_tx *)param;
	struct groups_rx {
		int32_t status;
		uint32_t nr_groups;
		uint16_t groups[];
	} *rx = (struct groups_rx *)param;
	uint32_t max_payload_size = SCMI_MAX_PAYLOAD - sizeof(*rx);
	int counter = 0, lcounter = 0;

	const int *groups;
	unsigned nr_groups;
	int ret;

	VERBOSE("scmi: pinctrl get_func_groups sel = %d skip = %d\n", tx.selector, tx.skip);
	if (size != sizeof(tx)) {
		rx->status = SCMI_PROTOCOL_ERROR;
		return sizeof(rx->status);
	}

	ret = pinctrl_get_function_groups(tx.selector, &groups, &nr_groups);
	if (ret) {
		ERROR("scmi: pinctrl_get_fucntion_groups failed with ret = %d\n", ret);
		rx->status = SCMI_GENERIC_ERROR;
		return sizeof(rx->status);
	}

	for (counter = tx.skip; counter < nr_groups; counter++, lcounter++) {
		if (lcounter * sizeof(uint16_t) >= max_payload_size) {
			break;
		}

		rx->groups[lcounter] = groups[counter];
		VERBOSE("func groups [%d] = %d\n", lcounter, groups[counter]);
	}

	rx->nr_groups = lcounter;
	VERBOSE("scmi: pinctrl get nr_groups= %d\n", rx->nr_groups);
	rx->status = SCMI_SUCCESS;

	return sizeof(*rx) +
		ALIGN_NEXT(lcounter * sizeof(uint16_t), sizeof(uint32_t));

};

static uint32_t set_mux(size_t channel __unused,
			       volatile uint8_t *param,
			       size_t size __unused)
{
	struct mux_tx {
		uint16_t function;
		uint16_t group;
	} tx = *(struct mux_tx *)param;
	int32_t *status = (int32_t *)param;
	int ret;

	VERBOSE("scmi:pinctrl set mux func = %d, group = %d\n", tx.function,
		 tx.group);

	if (size != sizeof(tx)) {
		*status = SCMI_PROTOCOL_ERROR;
		return sizeof(*status);
	}

	ret = pinctrl_set_mux(tx.function, tx.group);
	if (ret) {
		ERROR("scmi: pinctrl_set_mux failed with ret = %d\n", ret);
		*status = SCMI_GENERIC_ERROR;
		return sizeof(*status);
	}

	*status = SCMI_SUCCESS;
	return sizeof(*status);
}

static uint32_t get_config(size_t channel __unused,
			       volatile uint8_t *param,
			       size_t size __unused)
{
	struct conf_tx {
		uint32_t pin;
		uint32_t config;
	} tx = *(struct conf_tx *) param;
	struct conf_rx {
		int32_t status;
		uint32_t config;
	} *rx = (struct conf_rx *)param;
	int ret;
	unsigned long config_param;

	if (size != sizeof(tx)) {
		rx->status = SCMI_PROTOCOL_ERROR;
		return sizeof(rx->status);
	}

	config_param = tx.config;

	ret = pinctrl_config_get(tx.pin, &config_param);
	if (ret) {
		ERROR("scmi: pinctrl_config_get failed with ret = %d\n", ret);
		rx->status = SCMI_GENERIC_ERROR;
		return sizeof(rx->status);
	}

	rx->config = config_param;
	rx->status = SCMI_SUCCESS;
	return sizeof(*rx);
}

static uint32_t set_config(size_t channel __unused,
			       volatile uint8_t *param,
			       size_t size __unused)
{
	struct conf_tx {
		uint32_t pin;
		uint32_t config;
	} tx = *(struct conf_tx *) param;
	int32_t *status = (int32_t *)param;
	int ret;

	if (size != sizeof(tx)) {
		*status = SCMI_PROTOCOL_ERROR;
		return sizeof(*status);
	}

	ret = pinctrl_config_set(tx.pin, tx.config);
	if (ret) {
		ERROR("scmi: pinctrl_config_set failed with ret = %d\n", ret);
		*status = SCMI_GENERIC_ERROR;
		return sizeof(*status);
	}

	*status = SCMI_SUCCESS;
	return sizeof(*status);
}

static uint32_t get_config_group(size_t channel __unused,
			       volatile uint8_t *param,
			       size_t size __unused)
{
	struct conf_tx {
		uint32_t selector;
		uint32_t config;
	} tx = *(struct conf_tx *) param;
	struct conf_rx {
		int32_t status;
		uint32_t config;
	} *rx = (struct conf_rx *)param;
	int ret;
	unsigned long config_param;

	if (size != sizeof(tx)) {
		rx->status = SCMI_PROTOCOL_ERROR;
		return sizeof(rx->status);
	}

	config_param = tx.config;

	ret = pinctrl_config_group_get(tx.selector, &config_param);
	if (ret) {
		ERROR("scmi: pinctrl_config_group_get failed with ret = %d\n", ret);
		rx->status = SCMI_GENERIC_ERROR;
		return sizeof(rx->status);
	}

	rx->config = config_param;
	rx->status = SCMI_SUCCESS;
	return sizeof(*rx);
}

static uint32_t set_config_group(size_t channel __unused,
			       volatile uint8_t *param,
			       size_t size __unused)
{
	struct conf_tx {
		uint32_t selector;
		uint32_t config;
	} tx = *(struct conf_tx *) param;
	int32_t *status = (int32_t *)param;
	int ret;

	if (size != sizeof(tx)) {
		*status = SCMI_PROTOCOL_ERROR;
		return sizeof(*status);
	}

	ret = pinctrl_config_group_set(tx.selector, tx.config);
	if (ret) {
		ERROR("scmi: pinctrl_config_group_set failed with ret = %d\n", ret);
		*status = SCMI_GENERIC_ERROR;
		return sizeof(*status);
	}

	*status = SCMI_SUCCESS;
	return sizeof(*status);
}

typedef uint32_t (*pinctrl_handler_t)(size_t, volatile uint8_t*,size_t);

static pinctrl_handler_t pinctrl_handlers[SCMI_LAST_ID] = {
	[PROTOCOL_VERSION] = protocol_version,
	[PROTOCOL_ATTRIBUTES] = protocol_attrs,
	[PROTOCOL_MESSAGE_ATTRIBUTES] = protocol_msg_attrs,
	[PINCTRL_GET_GROUP_PINS] = get_group_pins,
	[PINCTRL_GET_FUNCTION_GROUPS] = get_function_groups,
	[PINCTRL_SET_MUX] = set_mux,
	[PINCTRL_GET_PINS] = get_pins,
	[PINCTRL_GET_CONFIG] = get_config,
	[PINCTRL_SET_CONFIG] = set_config,
	[PINCTRL_GET_CONFIG_GROUP] = get_config_group,
	[PINCTRL_SET_CONFIG_GROUP] = set_config_group,
};


uint32_t rcar_scmi_handle_pinctrl(size_t channel, uint8_t cmd,
					volatile uint8_t *param, size_t size)
{
	if (cmd >= SCMI_LAST_ID) {
		*(int32_t *)param = SCMI_NOT_SUPPORTED;
		return sizeof(int32_t);
	}

	assert(pinctrl_handlers[cmd] != NULL);

	return pinctrl_handlers[cmd](channel, param, size);
}
