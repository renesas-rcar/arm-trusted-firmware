/*
 * Copyright (c) 2015-2019, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/debug.h>
#include <drivers/pinctrl.h>
#include <lib/mmio.h>
#include <stdint.h>
#include <lib/libc/errno.h>
#include <stdbool.h>

#include "rcar_def.h"
#include "core.h"
#include "pinctrl-generic.h"

struct sh_pfc_pin_config {
	unsigned int mux_mark;
	bool mux_set;
};

struct sh_pfc_pinctrl {
	struct pinctrl_dev *pctl;
	struct sh_pfc pfc;

	struct pinctrl_pin_desc pins[SH_PFC_PINS_MAX];
	struct sh_pfc_pin_config configs[SH_PFC_CONFIGS_MAX];

	const char *func_prop_name;
	const char *groups_prop_name;
	const char *pins_prop_name;
};

static struct sh_pfc_pinctrl pmx;

static int sh_pfc_get_group_pins (unsigned selector,
										const unsigned **pins,
										unsigned *num_pins)
{
	*pins = pmx.pfc.info->groups[selector].pins;
	*num_pins = pmx.pfc.info->groups[selector].nr_pins;

	return 0;
}

static int sh_pfc_get_function_groups (unsigned selector,
										const int **groups,
										unsigned *num_groups)
{
	*groups = pmx.pfc.info->functions[selector].groups;
	*num_groups = pmx.pfc.info->functions[selector].nr_groups;

	return 0;
}

int sh_pfc_func_set_mux(unsigned selector,
			       unsigned group)
{
	struct sh_pfc *pfc = &pmx.pfc;
	const struct sh_pfc_pin_group *grp = &pfc->info->groups[group];
	unsigned int i;
	int ret = 0;

	VERBOSE("Configuring pin group %s\n", grp->name);

	spin_lock(&pfc->lock);

	for (i = 0; i < grp->nr_pins; ++i) {
		ret = sh_pfc_config_mux(pfc, grp->mux[i], PINMUX_TYPE_FUNCTION);
		if (ret < 0)
			goto done;
	}

	/* All group pins are configured, mark the pins as mux_set */
	for (i = 0; i < grp->nr_pins; ++i) {
		int idx = sh_pfc_get_pin_index(pfc, grp->pins[i]);
		struct sh_pfc_pin_config *cfg = &pmx.configs[idx];

		cfg->mux_set = true;
		cfg->mux_mark = grp->mux[i];
	}

done:
	spin_unlock(&pfc->lock);
	return ret;
}

static uint32_t sh_pfc_pinconf_find_drive_strength_reg(struct sh_pfc *pfc,
		unsigned int pin, unsigned int *offset, unsigned int *size)
{
	const struct pinmux_drive_reg_field *field;
	const struct pinmux_drive_reg *reg;
	unsigned int i;

	for (reg = pfc->info->drive_regs; reg->reg; ++reg) {
		for (i = 0; i < ARRAY_SIZE(reg->fields); ++i) {
			field = &reg->fields[i];

			if (field->size && field->pin == pin) {
				*offset = field->offset;
				*size = field->size;

				return reg->reg;
			}
		}
	}

	return 0;
}

static int sh_pfc_pinconf_get_drive_strength(struct sh_pfc *pfc,
					     unsigned int pin)
{
	unsigned int offset;
	unsigned int size;
	uint32_t reg;
	uint32_t val;

	reg = sh_pfc_pinconf_find_drive_strength_reg(pfc, pin, &offset, &size);
	if (!reg)
		return -EINVAL;

	spin_lock(&pfc->lock);
	val = sh_pfc_read(pfc, reg);
	spin_unlock(&pfc->lock);

	val = (val >> offset) & GENMASK(size - 1, 0);

	/* Convert the value to mA based on a full drive strength value of 24mA.
	 * We can make the full value configurable later if needed.
	 */
	return (val + 1) * (size == 2 ? 6 : 3);
}

static int sh_pfc_pinconf_set_drive_strength(struct sh_pfc *pfc,
					     unsigned int pin, uint16_t strength)
{
	unsigned int offset;
	unsigned int size;
	unsigned int step;
	uint32_t reg;
	uint32_t val;

	reg = sh_pfc_pinconf_find_drive_strength_reg(pfc, pin, &offset, &size);
	if (!reg)
		return -EINVAL;

	step = size == 2 ? 6 : 3;

	if (strength < step || strength > 24)
		return -EINVAL;

	/* Convert the value from mA based on a full drive strength value of
	 * 24mA. We can make the full value configurable later if needed.
	 */
	strength = strength / step - 1;

	spin_lock(&pfc->lock);

	val = sh_pfc_read(pfc, reg);
	val &= ~GENMASK(offset + size - 1, offset);
	val |= strength << offset;

	sh_pfc_write(pfc, reg, val);

	spin_unlock(&pfc->lock);

	return 0;
}

/* Check whether the requested parameter is supported for a pin. */
static bool sh_pfc_pinconf_validate(struct sh_pfc *pfc, unsigned int _pin,
				    enum pin_config_param param)
{
	int idx = sh_pfc_get_pin_index(pfc, _pin);
	const struct pinctrl_pin *pin = &pfc->info->pins[idx];

	switch (param) {
	case PIN_CONFIG_BIAS_DISABLE:
		return pin->configs & SH_PFC_PIN_CFG_PULL_UP_DOWN;

	case PIN_CONFIG_BIAS_PULL_UP:
		return pin->configs & SH_PFC_PIN_CFG_PULL_UP;

	case PIN_CONFIG_BIAS_PULL_DOWN:
		return pin->configs & SH_PFC_PIN_CFG_PULL_DOWN;

	case PIN_CONFIG_DRIVE_STRENGTH:
		return pin->configs & SH_PFC_PIN_CFG_DRIVE_STRENGTH;

	case PIN_CONFIG_POWER_SOURCE:
		return pin->configs & SH_PFC_PIN_CFG_IO_VOLTAGE;

	default:
		return false;
	}
}

static int sh_pfc_pinconf_get(unsigned _pin, unsigned long *config)
{
	struct sh_pfc *pfc = &pmx.pfc;
	enum pin_config_param param = pinconf_to_config_param(*config);
	unsigned int arg;

	if (!sh_pfc_pinconf_validate(pfc, _pin, param))
		return -ENOTSUP;

	switch (param) {
	case PIN_CONFIG_BIAS_DISABLE:
	case PIN_CONFIG_BIAS_PULL_UP:
	case PIN_CONFIG_BIAS_PULL_DOWN: {
		unsigned int bias;

		if (!pfc->info->ops || !pfc->info->ops->get_bias)
			return -ENOTSUP;

		spin_lock(&pfc->lock);
		bias = pfc->info->ops->get_bias(pfc, _pin);
		spin_unlock(&pfc->lock);

		if (bias != param)
			return -EINVAL;

		arg = 0;
		break;
	}

	case PIN_CONFIG_DRIVE_STRENGTH: {
		int ret;

		ret = sh_pfc_pinconf_get_drive_strength(pfc, _pin);
		if (ret < 0)
			return ret;

		arg = ret;
		break;
	}

	case PIN_CONFIG_POWER_SOURCE: {
		uint32_t pocctrl, val;
		int bit;

		if (!pfc->info->ops || !pfc->info->ops->pin_to_pocctrl)
			return -ENOTSUP;

		bit = pfc->info->ops->pin_to_pocctrl(pfc, _pin, &pocctrl);
		if (bit < 0) {
			WARN("invalid pin %#x", _pin);
			return bit;
		}

		spin_lock(&pfc->lock);
		val = sh_pfc_read(pfc, pocctrl);
		spin_unlock(&pfc->lock);

		arg = (val & BIT(bit)) ? 3300 : 1800;
		break;
	}

	default:
		return -ENOTSUP;
	}

	*config = pinconf_to_config_packed(param, arg);
	return 0;
}

static int sh_pfc_pinconf_set(unsigned _pin, unsigned long config)
{
	struct sh_pfc *pfc = &pmx.pfc;
	enum pin_config_param param;

	param = pinconf_to_config_param(config);

	if (!sh_pfc_pinconf_validate(pfc, _pin, param))
		return -ENOTSUP;

	switch (param) {
	case PIN_CONFIG_BIAS_PULL_UP:
	case PIN_CONFIG_BIAS_PULL_DOWN:
	case PIN_CONFIG_BIAS_DISABLE:
		if (!pfc->info->ops || !pfc->info->ops->set_bias)
			return -ENOTSUP;

		spin_lock(&pfc->lock);
		pfc->info->ops->set_bias(pfc, _pin, param);
		spin_unlock(&pfc->lock);

		break;

	case PIN_CONFIG_DRIVE_STRENGTH: {
		unsigned int arg =
			pinconf_to_config_argument(config);
		int ret;

		ret = sh_pfc_pinconf_set_drive_strength(pfc, _pin, arg);
		if (ret < 0)
			return ret;

		break;
	}

	case PIN_CONFIG_POWER_SOURCE: {
		unsigned int mV = pinconf_to_config_argument(config);
		uint32_t pocctrl, val;
		int bit;

		if (!pfc->info->ops || !pfc->info->ops->pin_to_pocctrl)
			return -ENOTSUP;

		bit = pfc->info->ops->pin_to_pocctrl(pfc, _pin, &pocctrl);
		if (bit < 0) {
			WARN("invalid pin %#x", _pin);
			return bit;
		}

		if (mV != 1800 && mV != 3300)
			return -EINVAL;

		spin_lock(&pfc->lock);
		val = sh_pfc_read(pfc, pocctrl);
		if (mV == 3300)
			val |= BIT(bit);
		else
			val &= ~BIT(bit);
		sh_pfc_write(pfc, pocctrl, val);
		spin_unlock(&pfc->lock);

		break;
	}

	default:
		return -ENOTSUP;
	}

	return 0;
}

static int sh_pfc_pinconf_group_set(unsigned group, unsigned long config)
{
	const unsigned int *pins;
	unsigned int num_pins;
	unsigned int i, ret;

	pins = pmx.pfc.info->groups[group].pins;
	num_pins = pmx.pfc.info->groups[group].nr_pins;

	for (i = 0; i < num_pins; ++i) {
		ret = sh_pfc_pinconf_set(pins[i], config);
		if (ret)
			return ret;
	}

	return 0;
}

/* PFC ranges -> pinctrl pin descs */
static int sh_pfc_map_pins(struct sh_pfc *pfc, struct sh_pfc_pinctrl *lpmx)
{
	unsigned int i;

	VERBOSE("pinctl: Mapping pins: info = %d name = %s\n",
			pfc->info->pins[0].pin, pfc->info->pins[0].name);
	for (i = 0; i < pfc->info->nr_pins; ++i) {
		const struct pinctrl_pin *info = &pfc->info->pins[i];
		struct pinctrl_pin_desc *pin = &lpmx->pins[i];

		/* If the pin number is equal to -1 all pins are considered */
		pin->number = info->pin != (uint16_t)-1 ? info->pin : i;
		pin->name = info->name;
	}

	return 0;
}

static int sh_pfc_get_pins(const struct pinctrl_pin **pins,
							unsigned *num_pins)
{
	//DONE tested
	*pins = pmx.pfc.info->pins;
	*num_pins = pmx.pfc.info->nr_pins;

	return 0;
}

static uint16_t sh_pfc_get_groups_count (void);
static uint16_t sh_pfc_get_functions_count (void);

static const pinctrl_ops_t ctrl_ops = {
	.get_groups_count = sh_pfc_get_groups_count,
	.get_group_pins = sh_pfc_get_group_pins,
	.get_pins = sh_pfc_get_pins,
};

static const pinconf_ops_t conf_ops = {
	.pin_config_get = sh_pfc_pinconf_get,
	.pin_config_set = sh_pfc_pinconf_set,
	.pin_config_group_set = sh_pfc_pinconf_group_set,
};

static const pinmux_ops_t mux_ops = {
	.get_functions_count = sh_pfc_get_functions_count,
	.get_function_groups = sh_pfc_get_function_groups,
	.set_mux = sh_pfc_func_set_mux,
};

static uint16_t sh_pfc_get_groups_count (void)
{
	return pmx.pfc.info->nr_groups;
}

static uint16_t sh_pfc_get_functions_count (void)
{
	return pmx.pfc.info->nr_functions;
}

void rcar_pinctrl_init(void)
{
	int ret;

	ret = sh_pfc_init(&pmx.pfc);
	if (ret < 0) {
		ERROR("pinctrl: Unable to init pinctrl\n");
		return;
	}

	ret = sh_pfc_map_pins(&pmx.pfc, &pmx);
	if (ret < 0) {
		ERROR("pinctrl: Unable to map pins\n");
		return;
	}

	pinctrl_init(&ctrl_ops, &mux_ops, &conf_ops);
}
