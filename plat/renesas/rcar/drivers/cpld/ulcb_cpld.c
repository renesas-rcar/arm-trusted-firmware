/*
 * Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2017, Renesas Electronics Corporation. All rights reserved.
 * Copyright (C) 2017, Cogent Embedded, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <mmio.h>

#define SCLK			8 /* GP_6_8 */
#define SSTBZ			3 /* GP_2_3 */
#define MOSI			7 /* GP_6_7 */

#define CPLD_ADDR_RESET		0x80 /* RW */

/* LSI Multiplexed Pin Setting Mask Register */
#define PFC_PMMR		0xE6060000

/* General output registers */
#define GPIO_OUTDT2		0xE6052008
#define GPIO_OUTDT6		0xE6055408

/* General input/output switching registers */
#define GPIO_INOUTSEL2		0xE6052004
#define GPIO_INOUTSEL6		0xE6055404

/* GPIO/perihperal function select */
#define PFC_GPSR2		0xE6060108
#define PFC_GPSR6		0xE6060118

static void gpio_set_value(uint32_t addr, uint8_t gpio, uint32_t val)
{
	uint32_t reg;

	reg = mmio_read_32(addr);
	if (val)
		reg |= (1 << gpio);
	else
		reg &= ~(1 << gpio);
	mmio_write_32(addr, reg);
}

static void gpio_direction_output(uint32_t addr, uint8_t gpio)
{
	uint32_t reg;

	reg = mmio_read_32(addr);
	reg |= (1 << gpio);
	mmio_write_32(addr, reg);
}

static void gpio_pfc(uint32_t addr, uint8_t gpio)
{
	uint32_t reg;

	reg = mmio_read_32(addr);
	reg &= ~(1 << gpio);
	mmio_write_32(PFC_PMMR, ~reg);
	mmio_write_32(addr, reg);
}

static void cpld_write(uint8_t addr, uint32_t data)
{
	int i;

	for (i = 0; i < 32; i++) {
		gpio_set_value(GPIO_OUTDT6, MOSI, data & (1 << 31)); /* MSB first */
		gpio_set_value(GPIO_OUTDT6, SCLK, 1);
		data <<= 1;
		gpio_set_value(GPIO_OUTDT6, SCLK, 0);
	}

	for (i = 0; i < 8; i++) {
		gpio_set_value(GPIO_OUTDT6, MOSI, addr & 0x80); /* MSB first */
		gpio_set_value(GPIO_OUTDT6, SCLK, 1);
		addr <<= 1;
		gpio_set_value(GPIO_OUTDT6, SCLK, 0);
	}

	gpio_set_value(GPIO_OUTDT6, MOSI, 1); /* WRITE */
	gpio_set_value(GPIO_OUTDT2, SSTBZ, 0);
	gpio_set_value(GPIO_OUTDT6, SCLK, 1);
	gpio_set_value(GPIO_OUTDT6, SCLK, 0);
	gpio_set_value(GPIO_OUTDT2, SSTBZ, 1);
}

static void cpld_init(void)
{
	gpio_pfc(PFC_GPSR6, SCLK);
	gpio_pfc(PFC_GPSR2, SSTBZ);
	gpio_pfc(PFC_GPSR6, MOSI);

	gpio_set_value(GPIO_OUTDT6, SCLK, 0);
	gpio_set_value(GPIO_OUTDT2, SSTBZ, 1);
	gpio_set_value(GPIO_OUTDT6, MOSI, 0);

	gpio_direction_output(GPIO_INOUTSEL6, SCLK);
	gpio_direction_output(GPIO_INOUTSEL2, SSTBZ);
	gpio_direction_output(GPIO_INOUTSEL6, MOSI);
}

void cpld_reset_cpu(void)
{
	cpld_init();

	cpld_write(CPLD_ADDR_RESET, 1);
}
