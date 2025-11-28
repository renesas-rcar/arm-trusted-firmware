/* SPDX-License-Identifier: GPL-2.0
 *
 * SuperH Pin Function Controller support.
 *
 * Copyright (C) 2012  Renesas Solutions Corp.
 */
#ifndef __SH_PFC_CORE_H__
#define __SH_PFC_CORE_H__

#include "sh_pfc.h"

uint32_t sh_pfc_read_raw_reg(uint32_t mapped_reg, unsigned int reg_width);
void sh_pfc_write_raw_reg(uint32_t mapped_reg, unsigned int reg_width,
			  uint32_t data);
uint32_t sh_pfc_read(struct sh_pfc *pfc, uint32_t reg);
void sh_pfc_write(struct sh_pfc *pfc, uint32_t reg, uint32_t data);
int sh_pfc_get_pin_index(struct sh_pfc *pfc, unsigned int pin);
int sh_pfc_config_mux(struct sh_pfc *pfc, unsigned mark, int pinmux_type);
const struct pinmux_bias_reg *
sh_pfc_pin_to_bias_reg(const struct sh_pfc *pfc, unsigned int pin,
		       unsigned int *bit);
int sh_pfc_init(struct sh_pfc *pfc);

#endif /* __SH_PFC_CORE_H__ */
