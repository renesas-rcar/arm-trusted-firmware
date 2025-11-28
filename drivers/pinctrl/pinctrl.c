/*
 * Copyright (c) 2021 EPAM Systems
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Pin control subsystem
 *
 * Defines a simple and generic interface to access pins.
 *
 */

#include <assert.h>
#include <errno.h>

#include <drivers/pinctrl.h>

static const pinctrl_ops_t *ctrl_ops_g;
static const pinconf_ops_t *conf_ops_g;
static const pinmux_ops_t *mux_ops_g;

uint16_t pinctrl_get_groups_count(void)
{
	assert(ctrl_ops_g);
	assert(ctrl_ops_g->get_groups_count != 0);

	return ctrl_ops_g->get_groups_count();
}

int pinctrl_get_group_pins(unsigned selector, const unsigned **pins,
					 unsigned *num_pins)
{
	assert(ctrl_ops_g);
	assert(ctrl_ops_g->get_group_pins != 0);

	return ctrl_ops_g->get_group_pins(selector, pins, num_pins);
}

int pinctrl_get_pins(const struct pinctrl_pin **pins, unsigned *num_pins)
{
	assert(ctrl_ops_g);
	assert(ctrl_ops_g->get_pins != 0);

	return ctrl_ops_g->get_pins(pins, num_pins);
}

uint16_t pinctrl_get_functions_count(void)
{
	assert(mux_ops_g);
	assert(mux_ops_g->get_functions_count != 0);

	return mux_ops_g->get_functions_count();
}

int pinctrl_get_function_groups(unsigned selector, const int **groups,
			  unsigned *num_groups)
{
	assert(mux_ops_g);
	assert(mux_ops_g->get_function_groups != 0);

	return mux_ops_g->get_function_groups(selector, groups, num_groups);
}

int pinctrl_config_get(unsigned pin, unsigned long *config)
{
	assert(conf_ops_g);
	assert(conf_ops_g->pin_config_get);

	return conf_ops_g->pin_config_get(pin, config);
}

int pinctrl_config_set(unsigned pin, unsigned long config)
{
	assert(conf_ops_g);
	assert(conf_ops_g->pin_config_set);

	return conf_ops_g->pin_config_set(pin, config);
}

int pinctrl_config_group_get(unsigned selector, unsigned long *config)
{
	assert(conf_ops_g);
	assert(conf_ops_g->pin_config_group_get);

	return conf_ops_g->pin_config_group_get(selector, config);
}

int pinctrl_config_group_set(unsigned selector, unsigned long config)
{
	assert(conf_ops_g);
	assert(conf_ops_g->pin_config_group_set);

	return conf_ops_g->pin_config_group_set(selector, config);
}

int pinctrl_set_mux(unsigned func_selector, unsigned group_selector)
{
	assert(mux_ops_g);
	assert(mux_ops_g->set_mux != 0);

	return mux_ops_g->set_mux(func_selector, group_selector);
}

int pinctrl_request(unsigned offset)
{
	assert(mux_ops_g);
	assert(mux_ops_g->request != 0);

	return mux_ops_g->request(offset);
}

int pinctrl_free(unsigned offset)
{
	assert(mux_ops_g);
	assert(mux_ops_g->free != 0);
	return mux_ops_g->free(offset);
}

void pinctrl_init(const pinctrl_ops_t *ctrl_ops, const pinmux_ops_t *mux_ops,
				  const pinconf_ops_t *conf_ops)
{
	assert(ctrl_ops != 0 && mux_ops != 0 && conf_ops != 0);

	ctrl_ops_g = ctrl_ops;
	mux_ops_g = mux_ops;
	conf_ops_g = conf_ops;
}
