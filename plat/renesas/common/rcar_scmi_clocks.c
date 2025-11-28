/*
 * Copyright (c) 2021 EPAM Systems Inc. All rights reserved.
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

#include <lib/mmio.h>
#include <lib/spinlock.h>

#include "rcar_def.h"
#include "rcar_private.h"
#include "rcar_scmi_resources.h"

#define CPG_BASE U(0xE6150000)
#define SMSTPCR3  0x13C
#define SMSTPCR5  0x144
#define SMSTPCR7  0x14C
#define SMSTPCR8  0x990
#define SMSTPCR10 0x998

#define MSTPSR3  0x048
#define MSTPSR5  0x03C
#define MSTPSR7  0x1C4
#define MSTPSR8  0x9A0
#define MSTPSR10 0x9A8

enum scmi_message_id {
	PROTOCOL_VERSION = 0x0,
	PROTOCOL_ATTRIBUTES,
	PROTOCOL_MESSAGE_ATTRIBUTES,
	CLOCK_ATTRIBUTES,
	CLOCK_DESCRIBE_RATES,
	CLOCK_RATE_SET,
	CLOCK_RATE_GET,
	CLOCK_CONFIG_SET,
	SCMI_LAST_ID
};

enum rcar_clk_type {
	CLK_TYPE_FIXED,
	CLK_TYPE_EXTAL,
	CLK_TYPE_DIV6,
	CLK_TYPE_SD,
	CLK_TYPE_MSSR,
};

enum rcar_mssr_state {
	MSSR_ON	= 0x0,
	MSSR_OFF,
};

struct scmi_clk_ops {
	int (*enable)(struct scmi_clk*);
	void (*disable)(struct scmi_clk*);
	uint64_t (*get_rate)(struct scmi_clk*);
	int (*set_rate)(struct scmi_clk*,uint64_t);
	bool (*is_enabled)(struct scmi_clk*);
};

static spinlock_t clk_lock;

struct scmi_clk rcar_clocks[RCAR_CLK_MAX] = {
	[RCAR_SCMICLK_EAVB] = {
		.name = "_etheravb",
		.parent = RCAR_CLK_S0D6,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR8,
			.st = MSTPSR8,
			.bit = 12,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_XHCI0] = {
		.name = "_usb3-if0",
		.parent = RCAR_CLK_S3D1,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR3,
			.st = MSTPSR3,
			.bit = 28,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_EHCI0] = {
		.name = "_ehci0",
		.parent = RCAR_CLK_S3D2,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR7,
			.st = MSTPSR7,
			.bit = 3,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_HSUSB] = {
		.name = "_hsusb",
		.parent = RCAR_CLK_S3D2,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR7,
			.st = MSTPSR7,
			.bit = 4,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_EHCI1] = {
		.name = "_ehci1",
		.parent = RCAR_CLK_S3D2,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR7,
			.st = MSTPSR7,
			.bit = 2,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_USB_DMAC0] = {
		.name = "_usb-dmac0",
		.parent = RCAR_CLK_S3D1,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR3,
			.st = MSTPSR3,
			.bit = 30,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_USB_DMAC1] = {
		.name = "_usb-dmac1",
		.parent = RCAR_CLK_S3D1,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR3,
			.st = MSTPSR3,
			.bit = 31,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_USB_DMAC30] = {
		.name = "_usb-dmac30",
		.parent = RCAR_CLK_S3D1,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR3,
			.st = MSTPSR3,
			.bit = 26,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_USB_DMAC31] = {
		.name = "_usb-dmac31",
		.parent = RCAR_CLK_S3D1,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR3,
			.st = MSTPSR3,
			.bit = 29,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SSI_ALL] = {
		.name = "_ssi-all",
		.parent = RCAR_CLK_S3D4,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 5,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SSI9] = {
		.name = "_ssi9",
		.parent = RCAR_SCMICLK_SSI_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 6,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SSI8] = {
		.name = "_ssi8",
		.parent = RCAR_SCMICLK_SSI_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 7,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SSI7] = {
		.name = "_ssi7",
		.parent = RCAR_SCMICLK_SSI_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 8,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SSI6] = {
		.name = "_ssi6",
		.parent = RCAR_SCMICLK_SSI_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 9,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SSI5] = {
		.name = "_ssi5",
		.parent = RCAR_SCMICLK_SSI_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 10,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SSI4] = {
		.name = "_ssi4",
		.parent = RCAR_SCMICLK_SSI_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 11,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SSI3] = {
		.name = "_ssi3",
		.parent = RCAR_SCMICLK_SSI_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 12,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SSI2] = {
		.name = "_ssi2",
		.parent = RCAR_SCMICLK_SSI_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 13,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SSI1] = {
		.name = "_ssi1",
		.parent = RCAR_SCMICLK_SSI_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 14,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SSI0] = {
		.name = "_ssi0",
		.parent = RCAR_SCMICLK_SSI_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 15,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SCU_ALL] = {
		.name = "_scu-all",
		.parent = RCAR_CLK_S3D4,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 17,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SCU_DVC1] = {
		.name = "_scu-dvc1",
		.parent = RCAR_SCMICLK_SCU_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 18,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SCU_DVC0] = {
		.name = "_scu-dvc0",
		.parent = RCAR_SCMICLK_SCU_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 19,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SCU_MIX1] = {
		.name = "_scu-ctu1-mix1",
		.parent = RCAR_SCMICLK_SCU_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 20,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SCU_MIX0] = {
		.name = "_scu-ctu0-mix0",
		.parent = RCAR_SCMICLK_SCU_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 21,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SCU_SRC9] = {
		.name = "_scu-src9",
		.parent = RCAR_SCMICLK_SCU_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 22,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SCU_SRC8] = {
		.name = "_scu-src8",
		.parent = RCAR_SCMICLK_SCU_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 23,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SCU_SRC7] = {
		.name = "_scu-src7",
		.parent = RCAR_SCMICLK_SCU_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 24,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SCU_SRC6] = {
		.name = "_scu-src6",
		.parent = RCAR_SCMICLK_SCU_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 25,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SCU_SRC5] = {
		.name = "_scu-src5",
		.parent = RCAR_SCMICLK_SCU_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 26,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SCU_SRC4] = {
		.name = "_scu-src4",
		.parent = RCAR_SCMICLK_SCU_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 27,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SCU_SRC3] = {
		.name = "_scu-src3",
		.parent = RCAR_SCMICLK_SCU_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 28,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SCU_SRC2] = {
		.name = "_scu-src2",
		.parent = RCAR_SCMICLK_SCU_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 29,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SCU_SRC1] = {
		.name = "_scu-src1",
		.parent = RCAR_SCMICLK_SCU_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 30,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_SCU_SRC0] = {
		.name = "_scu-src0",
		.parent = RCAR_SCMICLK_SCU_ALL,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR10,
			.st = MSTPSR10,
			.bit = 31,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_AUDMAC1] = {
		.name = "_audmac1",
		.parent = RCAR_CLK_S1D2,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR5,
			.st = MSTPSR5,
			.bit = 1,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_AUDMAC0] = {
		.name = "_audmac0",
		.parent = RCAR_CLK_S1D2,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR5,
			.st = MSTPSR5,
			.bit = 2,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_HDMI] = {
		.name = "_hdmi",
		.parent = -1, /*TODO: div6 clocks support */
		.type = CLK_TYPE_DIV6,
		.clk.div6 = {
			.cr = 0x250,
		}
	},
	[RCAR_SCMICLK_HDMI0] = {
		.name = "_hdmi0",
		.parent = RCAR_SCMICLK_HDMI,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR7,
			.st = MSTPSR7,
			.bit = 29,
			.init = MSSR_OFF,
		}
	},
	[RCAR_SCMICLK_HDMI1] = {
		.name = "_hdmi1",
		.parent = RCAR_SCMICLK_HDMI,
		.type = CLK_TYPE_MSSR,
		.clk.mssr = {
			.cr = SMSTPCR7,
			.st = MSTPSR7,
			.bit = 28,
			.init = MSSR_OFF,
		}
	},
	[RCAR_CLK_EXTAL] = {
		.name = "extal",
		.parent = -1,
		.type = CLK_TYPE_EXTAL,
		.clk.extal = {
			.rate = 16666666,
		}
	},
	[RCAR_CLK_MAIN] = {
		.name = ".main",
		.parent = RCAR_CLK_EXTAL,
		.type = CLK_TYPE_FIXED,
		/*TODO: should be set according to mode pins */
		.clk.fixed = {
			.div = 1,
			.mult = 1,
		}
	},
	[RCAR_CLK_PLL1] = {
		.name = ".pll1",
		.parent = RCAR_CLK_MAIN,
		.type = CLK_TYPE_FIXED,
		/*TODO: should be set according to mode pins */
		.clk.fixed = {
			.div = 1,
			.mult = 192,
		}
	},
	[RCAR_CLK_PLL1D2] = {
		.name = ".pll1_div2",
		.parent = RCAR_CLK_PLL1,
		.type = CLK_TYPE_FIXED,
		.clk.fixed = {
			.div = 2,
			.mult = 1,
		}
	},
	[RCAR_CLK_S0] = {
		.name = ".s0",
		.parent = RCAR_CLK_PLL1D2,
		.type = CLK_TYPE_FIXED,
		.clk.fixed = {
			.div = 2,
			.mult = 1,
		}
	},
	[RCAR_CLK_S1] = {
		.name = ".s1",
		.parent = RCAR_CLK_PLL1D2,
		.type = CLK_TYPE_FIXED,
		.clk.fixed = {
			.div = 3,
			.mult = 1,
		}
	},
	[RCAR_CLK_S3] = {
		.name = ".s3",
		.parent = RCAR_CLK_PLL1D2,
		.type = CLK_TYPE_FIXED,
		.clk.fixed = {
			.div = 6,
			.mult = 1,
		}
	},
	[RCAR_CLK_S0D6] = {
		.name = "s0d6",
		.parent = RCAR_CLK_S0,
		.type = CLK_TYPE_FIXED,
		.clk.fixed = {
			.div = 6,
			.mult = 1,
		}
	},
	[RCAR_CLK_S1D2] = {
		.name = "s1d2",
		.parent = RCAR_CLK_S1,
		.type = CLK_TYPE_FIXED,
		.clk.fixed = {
			.div = 2,
			.mult = 1,
		}
	},
	[RCAR_CLK_S3D1] = {
		.name = "s3d1",
		.parent = RCAR_CLK_S3,
		.type = CLK_TYPE_FIXED,
		.clk.fixed = {
			.div = 1,
			.mult = 1,
		}
	},
	[RCAR_CLK_S3D2] = {
		.name = "s3d2",
		.parent = RCAR_CLK_S3,
		.type = CLK_TYPE_FIXED,
		.clk.fixed = {
			.div = 2,
			.mult = 1,
		}
	},
	[RCAR_CLK_S3D4] = {
		.name = "s3d4",
		.parent = RCAR_CLK_S3,
		.type = CLK_TYPE_FIXED,
		.clk.fixed = {
			.div = 4,
			.mult = 1,
		}
	},
};

static uint64_t __clk_get_rate_locked(uint32_t);

static uint64_t extal_clk_get_rate(struct scmi_clk *sclk)
{
	assert(sclk);
	return sclk->clk.extal.rate;
}

static bool extal_clk_is_enabled(struct scmi_clk *sclk)
{
	return true;
}

static const struct scmi_clk_ops extal_clk_ops = {
	.get_rate = extal_clk_get_rate,
	.is_enabled = extal_clk_is_enabled,
};

static uint64_t fixed_clk_get_rate(struct scmi_clk *sclk)
{
	uint64_t rate;

	assert(sclk);
	assert(sclk->parent >= 0);

	rate = __clk_get_rate_locked(sclk->parent);
	rate *= sclk->clk.fixed.mult;
	rate /= sclk->clk.fixed.div;
	return rate;
}

static bool fixed_clk_is_enabled(struct scmi_clk *sclk)
{
	return true;
}

static const struct scmi_clk_ops fixed_clk_ops = {
	.get_rate = fixed_clk_get_rate,
	.is_enabled = fixed_clk_is_enabled,
};

static uint64_t mssr_clk_get_rate(struct scmi_clk *sclk)
{
	assert(sclk);
	assert(sclk->parent >= 0);
	return __clk_get_rate_locked(sclk->parent);
}

static int mssr_clk_enable(struct scmi_clk *sclk)
{
	uint32_t value;
	uintptr_t ctl_reg = CPG_BASE + sclk->clk.mssr.cr;
	uintptr_t status_reg = CPG_BASE + sclk->clk.mssr.st;
	uint32_t mask = BIT(sclk->clk.mssr.bit);

	value = mmio_read_32(ctl_reg);
	value &= ~mask;
	mmio_write_32(ctl_reg, value);

	for (int i = 0; i < 1000; i++) {
		if (!(mmio_read_32(status_reg) & mask)) {
			return SCMI_SUCCESS;
		}
	}

	ERROR("failed to enable clock %s\n", sclk->name);
	return SCMI_HARDWARE_ERROR;
}

static void mssr_clk_disable(struct scmi_clk *sclk)
{
	uint32_t value;
	uintptr_t ctl_reg = CPG_BASE + sclk->clk.mssr.cr;
	uint32_t mask = BIT(sclk->clk.mssr.bit);

	value = mmio_read_32(ctl_reg);
	value |= mask;
	mmio_write_32(ctl_reg, value);
}

static bool mssr_clk_is_enabled(struct scmi_clk *sclk)
{
	uintptr_t status_reg = CPG_BASE + sclk->clk.mssr.st;
	uint32_t value = mmio_read_32(status_reg);

	return !(value & BIT(sclk->clk.mssr.bit));
}

static const struct scmi_clk_ops mssr_clk_ops = {
	.enable = mssr_clk_enable,
	.disable = mssr_clk_disable,
	.get_rate = mssr_clk_get_rate,
	.is_enabled = mssr_clk_is_enabled,
};

static inline struct scmi_clk * __clk_get(uint32_t id)
{
	assert(id < ARRAY_SIZE(rcar_clocks));
	return &rcar_clocks[id];
}

static inline const char * __clk_get_name(uint32_t id)
{
	struct scmi_clk * sclk = __clk_get(id);
	return sclk->name;
}

static const struct scmi_clk_ops * __clk_get_ops(struct scmi_clk *sclk)
{
	assert(sclk);

	switch (sclk->type) {
	case CLK_TYPE_FIXED:
		return &fixed_clk_ops;
	case CLK_TYPE_EXTAL:
		return &extal_clk_ops;
	case CLK_TYPE_MSSR:
		return &mssr_clk_ops;
	case CLK_TYPE_DIV6:
	case CLK_TYPE_SD:
	default:
		ERROR("unsupported clock type 0x%x\n", sclk->type);
		assert(false);
	}

	/* should not reach here */
}

static uint64_t __clk_get_rate_locked(uint32_t id)
{
	struct scmi_clk *sclk = __clk_get(id);
	const struct scmi_clk_ops *ops = __clk_get_ops(sclk);

	assert(ops->get_rate);
	return ops->get_rate(sclk);
}

static bool __clk_enabled(uint32_t id)
{
	struct scmi_clk *sclk = __clk_get(id);
	const struct scmi_clk_ops *ops = __clk_get_ops(sclk);

	assert(ops->is_enabled);

	return ops->is_enabled(sclk);
}

static void __clk_disable_locked(uint32_t id, uint32_t channel)
{
	struct scmi_clk *sclk = __clk_get(id);
	const struct scmi_clk_ops *ops = __clk_get_ops(sclk);

	sclk->usage &= ~(1 << channel);

	if (sclk->usage == 0 && ops->disable) {
		ops->disable(sclk);
	}

	if (sclk->parent >= 0) {
		__clk_disable_locked(sclk->parent, channel);
	}
}

static int __clk_enable_locked(uint32_t id, uint32_t channel)
{
	struct scmi_clk *sclk = __clk_get(id);
	const struct scmi_clk_ops *ops = __clk_get_ops(sclk);

	if (sclk->parent >= 0) {
		int res = __clk_enable_locked(sclk->parent, channel);
		if (res) {
			return res;
		}
	}

	if (sclk->usage == 0 && ops->enable) {
		int res = ops->enable(sclk);
		if (res) {
			__clk_disable_locked(sclk->parent, channel);
			return res;
		}
	}

	sclk->usage |= 1 << channel;
	return SCMI_SUCCESS;
}

static uint32_t protocol_version(size_t channel __unused,
				 volatile uint8_t *param,
				 size_t size __unused)
{
	int32_t *status = (int32_t*)param;
	uint32_t *version = (uint32_t*)(param + sizeof(*status));
	*status = SCMI_SUCCESS;
	*version = 0x10000; /* DEN0056C, 4.6.2.1 */
	return sizeof(*status) + sizeof(*version);
}

static uint32_t protocol_attrs(size_t channel __unused,
			       volatile uint8_t *param,
			       size_t size __unused)
{
	int32_t *status = (int32_t*)param;
	uint32_t *attrs = (uint32_t*)(param + sizeof(*status));

	*status = SCMI_SUCCESS;
	*attrs = FLD(GENMASK(31, 16), 0) |
		 FLD(GENMASK(15,  0), RCAR_SCMICLK_MAX);

	return sizeof(*status) + sizeof(*attrs);
}

static uint32_t protocol_msg_attrs(size_t channel __unused,
				   volatile uint8_t *param,
				   size_t size)
{
	uint32_t id = *(uint32_t*)param;
	int32_t *status = (int32_t*)param;
	uint32_t *attrs = (uint32_t*)(param + sizeof(*status));

	if (size != sizeof(id)) {
		*status = SCMI_PROTOCOL_ERROR;
		return sizeof(*status);
	}

	/* all commands are mandatory */
	*status = SCMI_SUCCESS;
	*attrs = 0x0;

	return sizeof(*status) + sizeof(*attrs);
}

static uint32_t clk_attrs(size_t channel, volatile uint8_t *param, size_t size)
{
	struct clk_discovery {
		int32_t status;
		uint32_t attributes;
		char name[16];
	} *res = (struct clk_discovery*)param;
	uint32_t clock_id = *(uint32_t*)param;

	if (size != sizeof(clock_id)) {
		res->status = SCMI_PROTOCOL_ERROR;
		return sizeof(res->status);
	}

	if (clock_id >= RCAR_SCMICLK_MAX) {
		res->status = SCMI_NOT_FOUND;
		return sizeof(res->status);
	}

	if (!scmi_permission_granted(rcar_clocks[clock_id].perm, channel)) {
		res->status = SCMI_DENIED;
		return sizeof(res->status);
	}

	res->attributes = __clk_enabled(clock_id) ? BIT(0) : 0;

	assert(__clk_get_name(clock_id) != NULL);
	strlcpy(res->name, __clk_get_name(clock_id), sizeof(res->name));

	res->status = SCMI_SUCCESS;

	return sizeof(*res);
}

static uint32_t set_rate(size_t channel __unused,
			      volatile uint8_t *param,
			      size_t size __unused)
{
	*(int32_t *)param = SCMI_NOT_SUPPORTED;
	return sizeof(int32_t);
}

static uint64_t clk_get_rate(uint32_t clock_id)
{
	int ret;
	spin_lock(&clk_lock);
	ret = __clk_get_rate_locked(clock_id);
	spin_unlock(&clk_lock);
	return ret;
}

/*TODO: not conforming to SCMI spec */
static uint32_t get_rates(size_t channel,
			  volatile uint8_t *payload,
			  size_t size)
{
	struct parameters {
		uint32_t id;
		uint32_t rate_index;
	} param = *((struct parameters*)payload);
	struct clk_rates {
		int32_t status;
		uint32_t num_rates_flags;
		uint32_t rates[2];
	} *res = (struct clk_rates*)payload;
	uint64_t rate;

	if (size != sizeof(param)) {
		res->status = SCMI_PROTOCOL_ERROR;
		return sizeof(res->status);
	}

	if (param.id >= RCAR_SCMICLK_MAX) {
		res->status = SCMI_NOT_FOUND;
		return sizeof(res->status);
	}

	if (!scmi_permission_granted(rcar_clocks[param.id].perm, channel)) {
		res->status = SCMI_DENIED;
		return sizeof(res->status);
	}

	rate = clk_get_rate(param.id);
	/*TODO: only constant rate clocks supported for now */
	res->num_rates_flags = FLD(GENMASK(31,12), 0) | FLD(GENMASK(11,0), 1);
	res->rates[0] = (rate >>  0) & GENMASK(31,0); /* lower word */
	res->rates[1] = (rate >> 32) & GENMASK(31,0); /* upper word */
	res->status = SCMI_SUCCESS;

	return sizeof(*res);
}

static uint32_t get_rate(size_t channel, volatile uint8_t *param, size_t size)
{
	struct clk_rate {
		int32_t status;
		uint32_t rate[2];
	} *res = (struct clk_rate*)param;
	uint32_t clock_id = *(uint32_t*)param;
	uint64_t rate;

	if (size != sizeof(clock_id)) {
		res->status = SCMI_PROTOCOL_ERROR;
		return sizeof(res->status);
	}

	if (clock_id >= RCAR_SCMICLK_MAX) {
		res->status = SCMI_NOT_FOUND;
		return sizeof(res->status);
	}

	if (!scmi_permission_granted(rcar_clocks[clock_id].perm, channel)) {
		res->status = SCMI_DENIED;
		return sizeof(res->status);
	}

	rate = clk_get_rate(clock_id);
	res->rate[0] = (rate >>  0) & GENMASK(31,0); /* lower word */
	res->rate[1] = (rate >> 32) & GENMASK(31,0); /* upper word */
	res->status = SCMI_SUCCESS;

	return sizeof(*res);
}

static int clk_enable(uint32_t id, uint32_t channel)
{
	int ret;
	spin_lock(&clk_lock);
	ret = __clk_enable_locked(id, channel);
	spin_unlock(&clk_lock);
	return ret;
}

static void clk_disable(uint32_t id, uint32_t channel)
{
	spin_lock(&clk_lock);
	__clk_disable_locked(id, channel);
	spin_unlock(&clk_lock);
}

static uint32_t set_cfg(size_t channel, volatile uint8_t *payload, size_t size)
{

	struct parameters {
		uint32_t id;
		uint32_t attributes;
	} param = *((struct parameters*)payload);
	int32_t status = SCMI_SUCCESS;

	if (size != sizeof(param)) {
		status = SCMI_PROTOCOL_ERROR;
		goto error;
	}

	if (param.id >= RCAR_SCMICLK_MAX) {
		status = SCMI_NOT_FOUND;
		goto error;
	}

	if (!scmi_permission_granted(rcar_clocks[param.id].perm, channel)) {
		status = SCMI_DENIED;
		goto error;
	}

	if (param.attributes & ~BIT(0)) {
		status = SCMI_INVALID_PARAMETERS;
		goto error;
	}

	if (param.attributes & BIT(0)) {
		status = clk_enable(param.id, channel);
	} else {
		clk_disable(param.id, channel);
	}

error:
	*(int32_t *)payload = status;
	return sizeof(status);
}

typedef uint32_t (*clock_handler_t)(size_t, volatile uint8_t*,size_t);

static clock_handler_t clock_handlers[SCMI_LAST_ID] = {
	[PROTOCOL_VERSION] = protocol_version,
	[PROTOCOL_ATTRIBUTES] = protocol_attrs,
	[PROTOCOL_MESSAGE_ATTRIBUTES] = protocol_msg_attrs,
	[CLOCK_ATTRIBUTES] = clk_attrs,
	[CLOCK_DESCRIBE_RATES] = get_rates,
	[CLOCK_RATE_SET] = set_rate,
	[CLOCK_RATE_GET] = get_rate,
	[CLOCK_CONFIG_SET] = set_cfg,
};

uint32_t rcar_scmi_handle_clock(size_t channel, uint8_t cmd,
			       volatile uint8_t *param, size_t size)
{
	if (cmd >= SCMI_LAST_ID) {
		*(int32_t *)param = SCMI_NOT_SUPPORTED;
		return sizeof(int32_t);
	}

	assert(clock_handlers[cmd] != NULL);

	return clock_handlers[cmd](channel, param, size);
}

void rcar_reset_clock(uint32_t id, uint32_t agent_id)
{
	struct scmi_clk *sclk = __clk_get(id);
	switch (sclk->type) {
	case CLK_TYPE_MSSR:
		if (sclk->clk.mssr.init == MSSR_OFF) {
			clk_disable(id, agent_to_channel(agent_id));
		} else {
			clk_enable(id, agent_to_channel(agent_id));
		}
		break;
	case CLK_TYPE_DIV6:
		WARN("div6 clocks not supported yet\n");
		break;

	case CLK_TYPE_FIXED:
	case CLK_TYPE_EXTAL:
	case CLK_TYPE_SD:
		ERROR("reset of core clock requested");
		break;
	default:
		ERROR("requested reset of unknown clock type %d\n", sclk->type);
		assert(false);
	}
}

int rcar_cpg_init(void)
{
	return 0;
}

