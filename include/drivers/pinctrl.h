/*
 * Copyright (c) 2021 EPAM Systems
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PINCTRL_H
#define PINCTRL_H

#include <stdint.h>
#include <drivers/pinctrl_defs.h>

typedef struct pinctrl_ops {
	uint16_t (*get_groups_count) ();
	const char *(*get_group_name) (unsigned selector);
	int (*get_group_pins) (unsigned selector, const unsigned **pins,
						   unsigned *num_pins);
	int (*get_pins) (const struct pinctrl_pin **pins, unsigned *num_pins);
} pinctrl_ops_t;

typedef struct pinmux_ops {
	int (*request) (unsigned offset);
	int (*free) (unsigned offset);
	uint16_t (*get_functions_count) ();
	const char *(*get_function_name) (unsigned selector);
	int (*get_function_groups) (unsigned selector, const int **groups,
				  unsigned *num_groups);
	int (*set_mux) (unsigned func_selector,
			unsigned group_selector);
} pinmux_ops_t;

typedef struct pinconf_ops {
	int (*pin_config_get) (unsigned pin, unsigned long *config);
	int (*pin_config_set) (unsigned pin, unsigned long config);
	int (*pin_config_group_get) (unsigned selector, unsigned long *config);
	int (*pin_config_group_set) (unsigned selector, unsigned long config);
} pinconf_ops_t;

void pinctrl_init(const pinctrl_ops_t *ctrl_ops, const pinmux_ops_t *mux_ops,
				  const pinconf_ops_t *conf_ops);

/* pinctrl functions */
uint16_t pinctrl_get_groups_count(void);
int pinctrl_get_group_pins(unsigned selector, const unsigned **pins,
					 unsigned *num_pins);
int pinctrl_get_pins(const struct pinctrl_pin **pins, unsigned *num_pins);

/* pinmux functions */
int pinctrl_request(unsigned offset);
int pinctrl_free(unsigned offset);
uint16_t pinctrl_get_functions_count(void);
int pinctrl_get_function_groups(unsigned selector, const int **groups,
			  unsigned *num_groups);
int pinctrl_set_mux(unsigned func_selector, unsigned group_selector);

/* pinconf funcitons */
int pinctrl_config_get(unsigned pin, unsigned long *config);
int pinctrl_config_set(unsigned pin, unsigned long config);
int pinctrl_config_group_get(unsigned selector, unsigned long *config);
int pinctrl_config_group_set(unsigned selector, unsigned long config);
#endif /* PINCTRL_H */
