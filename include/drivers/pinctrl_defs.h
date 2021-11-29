/*
 * Copyright (c) 2021 EPAM Systems
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PINCTRL_DEFS_H
#define PINCTRL_DEFS_H

struct pinctrl_pin {
	uint16_t pin;
	uint16_t enum_id;
	const char *name;
	unsigned int configs;
};

#endif /* PINCTRL_DEFS_H */
