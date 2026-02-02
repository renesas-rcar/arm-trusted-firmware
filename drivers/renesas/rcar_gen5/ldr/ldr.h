/*
 * Copyright (c) 2026, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LDR_H
#define LDR_H

#include <stdbool.h>
#include <stddef.h>

void rcar_ldr_init(void);
bool rcar_ldr_is_ready(void);

/*
 * Record one line of text. 'level' is an LdrLogLevel; 'text' need not be
 * NUL-terminated.
 */
void rcar_ldr_record_text(int level, const char *text, size_t len);

/* Register the LDR console (ldr_console.c). Call after rcar_ldr_init(). */
void console_rcar_ldr_register(void);

#endif /* LDR_H */
