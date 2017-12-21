/*
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef BL2_INT_ERROR_H__
#define BL2_INT_ERROR_H__

extern void bl2_swdt_release(void);

/* int_id : IAR.Interrupt ID */
void bl2_interrupt_error_id(uint32_t int_id);

/* ex_type : vector index */
void bl2_interrupt_error_type(uint32_t ex_type);

#endif /* BL2_INT_ERROR_H__ */
