/*
 * Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef UART_H_
#define UART_H_

/* Headers */
#include <arch_features.h>

/* Prototype */
void uart_init();
void uart_console_putc(uint8_t outchar);
uint8_t uart_console_getc();
uint8_t uart_console_scan_rd();

#endif /* UART_H_ */

