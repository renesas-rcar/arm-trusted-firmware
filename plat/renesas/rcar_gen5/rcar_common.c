/*
 * Copyright (c) 2019-2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <drivers/console.h>
#include "rcar_private.h"
#include "scif.h"

/* RAS functions common to AArch64 ARM platforms */
void plat_ea_handler(unsigned int ea_reason, uint64_t syndrome, void *cookie,
		void *handle, uint64_t flags)
{
}


/*
 * With RCAR_CONSOLE_UART=0 the serial console is still registered, because
 * console_rcar_init() also initializes MFIS, but only in the crash scope,
 * which console_putc() never uses. The crash console calls the driver
 * directly.
 */
#if RCAR_CONSOLE_UART
#define RCAR_CONSOLE_BOOT_SCOPE		CONSOLE_FLAG_BOOT
#define RCAR_CONSOLE_RUNTIME_SCOPE	(CONSOLE_FLAG_BOOT |		\
					 CONSOLE_FLAG_RUNTIME |		\
					 CONSOLE_FLAG_CRASH)
#else
#define RCAR_CONSOLE_BOOT_SCOPE		CONSOLE_FLAG_CRASH
#define RCAR_CONSOLE_RUNTIME_SCOPE	CONSOLE_FLAG_CRASH
#endif

void rcar_console_boot_init(void)
{
	static console_t rcar_boot_console = {0};
	int ret;

	ret = console_rcar_register(0, 0, 0, &rcar_boot_console);
	if (ret == 0) {
		panic();
	}

	console_set_scope(&rcar_boot_console, RCAR_CONSOLE_BOOT_SCOPE);
}

void rcar_console_runtime_init(void)
{
	static console_t rcar_runtime_console = {0};
	int ret;

	ret = console_rcar_register(1, 0, 0, &rcar_runtime_console);
	if (ret == 0) {
		panic();
	}

	console_set_scope(&rcar_runtime_console, RCAR_CONSOLE_RUNTIME_SCOPE);
}
