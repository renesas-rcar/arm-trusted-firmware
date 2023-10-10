// SPDX-License-Identifier: GPL-2.0
/*
 * Pin Control and GPIO driver for SuperH Pin Function Controller.
 *
 * Authors: Magnus Damm, Paul Mundt, Laurent Pinchart
 *
 * Copyright (C) 2008 Magnus Damm
 * Copyright (C) 2009 - 2012 Paul Mundt
 */

#include <common/debug.h>
#include <lib/libc/errno.h>
#include <lib/mmio.h>
#include <string.h>
#include <stddef.h>
#include "core.h"

int sh_pfc_get_pin_index(struct sh_pfc *pfc, unsigned int pin)
{
	unsigned int offset;
	unsigned int i;

	for (i = 0, offset = 0; i < pfc->nr_ranges; ++i) {
		const struct sh_pfc_pin_range *range = &pfc->ranges[i];

		if (pin <= range->end)
			return pin >= range->start
			     ? offset + pin - range->start : -1;

		offset += range->end - range->start + 1;
	}

	return -EINVAL;
}

static int sh_pfc_enum_in_range(uint16_t enum_id, const struct pinmux_range *r)
{
	if (enum_id < r->begin)
		return 0;

	if (enum_id > r->end)
		return 0;

	return 1;
}

uint32_t sh_pfc_read_raw_reg(uint32_t mapped_reg, unsigned int reg_width)
{
	switch (reg_width) {
	case 8:
		return mmio_read_8(mapped_reg);
	case 16:
		return mmio_read_16(mapped_reg);
	case 32:
		return mmio_read_32(mapped_reg);
	}

	ERROR("error");
	return 0;
}

void sh_pfc_write_raw_reg(uint32_t mapped_reg, unsigned int reg_width,
			  uint32_t data)
{
	switch (reg_width) {
	case 8:
		mmio_write_8(mapped_reg, data);
		return;
	case 16:
		mmio_write_16(mapped_reg, data);
		return;
	case 32:
		mmio_write_32(mapped_reg, data);
		return;
	}

	ERROR("error");
}

uint32_t sh_pfc_read(struct sh_pfc *pfc, uint32_t reg)
{
	return sh_pfc_read_raw_reg(reg, 32);
}

static uint32_t sh_pfc_get_unlock_reg(struct sh_pfc *pfc, uint32_t reg)
{
	uint32_t unlock_reg;
	unsigned int i;

	if (pfc->info->unlock_reg)
		return pfc->info->unlock_reg;

	for (i = 0; i < pfc->info->unlock_regs_size; i++) {
		unlock_reg = *(pfc->info->unlock_regs_table + i);

		if (reg < unlock_reg)
			continue;
		if (reg > unlock_reg + pfc->info->writable_area)
			continue;
		return unlock_reg;
	}

	ERROR("error");
	return 0;
}

void sh_pfc_write(struct sh_pfc *pfc, uint32_t reg, uint32_t data)
{
	if ((pfc->info->unlock_reg != 0) ||
	    (pfc->info->unlock_regs_table != NULL))
		sh_pfc_write_raw_reg(
				sh_pfc_get_unlock_reg(pfc, reg), 32,
			~data);

	sh_pfc_write_raw_reg(reg, 32, data);
}

static void sh_pfc_config_reg_helper(struct sh_pfc *pfc,
				     const struct pinmux_cfg_reg *crp,
				     unsigned int in_pos,
				     uint32_t *mapped_reg, uint32_t *maskp,
				     unsigned int *posp)
{
	unsigned int k;

	*mapped_reg = crp->reg;

	if (crp->field_width) {
		*maskp = (1 << crp->field_width) - 1;
		*posp = crp->reg_width - ((in_pos + 1) * crp->field_width);
	} else {
		*maskp = (1 << crp->var_field_width[in_pos]) - 1;
		*posp = crp->reg_width;
		for (k = 0; k <= in_pos; k++)
			*posp -= crp->var_field_width[k];
	}
}

static void sh_pfc_write_config_reg(struct sh_pfc *pfc,
				    const struct pinmux_cfg_reg *crp,
				    unsigned int field, uint32_t value)
{
	uint32_t mapped_reg;
	unsigned int pos;
	uint32_t mask, data;

	sh_pfc_config_reg_helper(pfc, crp, field, &mapped_reg, &mask, &pos);

	VERBOSE("write_reg addr = %x, value = 0x%x, field = %u, "
		"r_width = %u, f_width = %u\n",
		crp->reg, value, field, crp->reg_width, mask);

	mask = ~(mask << pos);
	value = value << pos;

	data = sh_pfc_read_raw_reg(mapped_reg, crp->reg_width);
	data &= mask;
	data |= value;

	if ((pfc->info->unlock_reg != 0) ||
	    (pfc->info->unlock_regs_table != NULL))
		sh_pfc_write_raw_reg(
				sh_pfc_get_unlock_reg(pfc, crp->reg), 32,
			~data);

	sh_pfc_write_raw_reg(crp->reg, crp->reg_width, data);
}

static int sh_pfc_get_config_reg(struct sh_pfc *pfc, uint16_t enum_id,
				 const struct pinmux_cfg_reg **crp,
				 unsigned int *fieldp, uint32_t *valuep)
{
	unsigned int k = 0;

	while (1) {
		const struct pinmux_cfg_reg *config_reg =
			pfc->info->cfg_regs + k;
		unsigned int r_width = config_reg->reg_width;
		unsigned int f_width = config_reg->field_width;
		unsigned int curr_width;
		unsigned int bit_pos;
		unsigned int pos = 0;
		unsigned int m = 0;

		if (!r_width)
			break;

		for (bit_pos = 0; bit_pos < r_width; bit_pos += curr_width) {
			uint32_t ncomb;
			uint32_t n;

			if (f_width)
				curr_width = f_width;
			else
				curr_width = config_reg->var_field_width[m];

			ncomb = 1 << curr_width;
			for (n = 0; n < ncomb; n++) {
				if (config_reg->enum_ids[pos + n] == enum_id) {
					*crp = config_reg;
					*fieldp = m;
					*valuep = n;
					return 0;
				}
			}
			pos += ncomb;
			m++;
		}
		k++;
	}

	return -EINVAL;
}

static int sh_pfc_mark_to_enum(struct sh_pfc *pfc, uint16_t mark, int pos,
			      uint16_t *enum_idp)
{
	const uint16_t *data = pfc->info->pinmux_data;
	unsigned int k;

	if (pos) {
		*enum_idp = data[pos + 1];
		return pos + 1;
	}

	for (k = 0; k < pfc->info->pinmux_data_size; k++) {
		if (data[k] == mark) {
			*enum_idp = data[k + 1];
			return k + 1;
		}
	}

	WARN("cannot locate data/mark enum_id for mark %d\n",
		mark);
	return -EINVAL;
}

int sh_pfc_config_mux(struct sh_pfc *pfc, unsigned mark, int pinmux_type)
{
	const struct pinmux_range *range;
	int pos = 0;

	switch (pinmux_type) {
	case PINMUX_TYPE_GPIO:
	case PINMUX_TYPE_FUNCTION:
		range = NULL;
		break;

	case PINMUX_TYPE_OUTPUT:
		range = &pfc->info->output;
		break;

	case PINMUX_TYPE_INPUT:
		range = &pfc->info->input;
		break;

	default:
		return -EINVAL;
	}

	/* Iterate over all the configuration fields we need to update. */
	while (1) {
		const struct pinmux_cfg_reg *cr;
		unsigned int field;
		uint16_t enum_id;
		uint32_t value;
		int in_range;
		int ret;

		pos = sh_pfc_mark_to_enum(pfc, mark, pos, &enum_id);
		if (pos < 0)
			return pos;

		if (!enum_id)
			break;

		/* Check if the configuration field selects a function. If it
		 * doesn't, skip the field if it's not applicable to the
		 * requested pinmux type.
		 */
		in_range = sh_pfc_enum_in_range(enum_id, &pfc->info->function);
		if (!in_range) {
			if (pinmux_type == PINMUX_TYPE_FUNCTION) {
				/* Functions are allowed to modify all
				 * fields.
				 */
				in_range = 1;
			} else if (pinmux_type != PINMUX_TYPE_GPIO) {
				/* Input/output types can only modify fields
				 * that correspond to their respective ranges.
				 */
				in_range = sh_pfc_enum_in_range(enum_id, range);

				/*
				 * special case pass through for fixed
				 * input-only or output-only pins without
				 * function enum register association.
				 */
				if (in_range && enum_id == range->force)
					continue;
			}
			/* GPIOs are only allowed to modify function fields. */
		}

		if (!in_range)
			continue;

		ret = sh_pfc_get_config_reg(pfc, enum_id, &cr, &field, &value);
		if (ret < 0)
			return ret;

		sh_pfc_write_config_reg(pfc, cr, field, value);
	}

	return 0;
}

const struct pinmux_bias_reg *
sh_pfc_pin_to_bias_reg(const struct sh_pfc *pfc, unsigned int pin,
		       unsigned int *bit)
{
	unsigned int i, j;

	for (i = 0; pfc->info->bias_regs[i].puen; i++) {
		for (j = 0; j < ARRAY_SIZE(pfc->info->bias_regs[i].pins); j++) {
			if (pfc->info->bias_regs[i].pins[j] == pin) {
				*bit = j;
				return &pfc->info->bias_regs[i];
			}
		}
	}

	WARN("Pin %u is not in bias info list\n", pin);

	return NULL;
}

static int sh_pfc_init_ranges(struct sh_pfc *pfc)
{
	struct sh_pfc_pin_range *range;
	unsigned int nr_ranges;
	unsigned int i;

	if (pfc->info->pins[0].pin == (uint16_t)-1) {
		/* Pin number -1 denotes that the SoC doesn't report pin numbers
		 * in its pin arrays yet. Consider the pin numbers range as
		 * continuous and allocate a single range.
		 */
		pfc->nr_ranges = 1;
		pfc->ranges->start = 0;
		pfc->ranges->end = pfc->info->nr_pins - 1;
		pfc->nr_gpio_pins = pfc->info->nr_pins;

		return 0;
	}

	range = pfc->ranges;
	range->start = pfc->info->pins[0].pin;

	for (i = 1, nr_ranges = 1; i < pfc->info->nr_pins; ++i) {
		if (pfc->info->pins[i-1].pin == pfc->info->pins[i].pin - 1)
			continue;

		nr_ranges++;

		range->end = pfc->info->pins[i-1].pin;
		if (!(pfc->info->pins[i-1].configs & SH_PFC_PIN_CFG_NO_GPIO))
			pfc->nr_gpio_pins = range->end + 1;

		range++;
		range->start = pfc->info->pins[i].pin;
	}

	pfc->nr_ranges = nr_ranges;
	range->end = pfc->info->pins[i-1].pin;
	if (!(pfc->info->pins[i-1].configs & SH_PFC_PIN_CFG_NO_GPIO))
		pfc->nr_gpio_pins = range->end + 1;

	return 0;
}

int sh_pfc_init(struct sh_pfc *pfc)
{
	const struct sh_pfc_soc_info *info;
	int ret;

	info = &plat_soc_info;
	pfc->info = info;

	if (info->ops && info->ops->init) {
		ret = info->ops->init(pfc);
		if (ret < 0)
			return ret;

		/* .init() may have overridden pfc->info */
		info = pfc->info;
	}

	ret = sh_pfc_init_ranges(pfc);
	if (ret < 0)
		return ret;

	INFO("%s support registered\n", info->name);

	return 0;
}
