/*
 * Copyright (c) 2015-2021, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MSSR_H
#define MSSR_H

void rcar_mssr_clock(int n, uint32_t data, bool on, bool force);
void rcar_mssr_soft_reset(int n, uint32_t data, bool assert, bool force);
void rcar_mssr_setup(void);

#endif /* MSSR_H */
