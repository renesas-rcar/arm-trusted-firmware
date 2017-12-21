/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef	BL2_CPG_INIT_H__
#define	BL2_CPG_INIT_H__

void bl2_cpg_init(void);
void bl2_system_cpg_init(void);
void cpg_write(uintptr_t regadr, uint32_t regval);

#endif	/* BL2_CPG_INIT_H__ */
