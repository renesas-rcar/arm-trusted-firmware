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

#include "rcar_private.h"
#include "rcar_scmi_resources.h"

static const struct scmi_device default_dev = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
};

const struct scmi_device rcar_devices[RCAR_SCMIDEV_MAX] = {
	[0 ... RCAR_SCMIDEV_MAX - 1] = default_dev, /* sentinel */

	[RCAR_SCMIDEV_EAVB] = {
		.rsts = (int[]){RCAR_SCMIRST_EAVB,-1},
		.clks = (int[]){RCAR_SCMICLK_EAVB,-1},
	},
	[RCAR_SCMIDEV_HDMI0] = {
		.rsts = (int[]){RCAR_SCMIRST_HDMI0,-1},
		.clks = (int[]){RCAR_SCMICLK_HDMI,RCAR_SCMICLK_HDMI0,-1},
	},
	[RCAR_SCMIDEV_HDMI1] = {
		.rsts = (int[]){RCAR_SCMIRST_HDMI1,-1},
		.clks = (int[]){RCAR_SCMICLK_HDMI,RCAR_SCMICLK_HDMI1,-1},
	},
	[RCAR_SCMIDEV_I2C0] = {
		.rsts = (int[]){RCAR_SCMIRST_I2C0,-1},
		.clks = (int[]){-1},
	},
	[RCAR_SCMIDEV_I2C1] = {
		.rsts = (int[]){RCAR_SCMIRST_I2C1,-1},
		.clks = (int[]){-1},
	},
	[RCAR_SCMIDEV_I2C2] = {
		.rsts = (int[]){RCAR_SCMIRST_I2C2,-1},
		.clks = (int[]){-1},
	},
	[RCAR_SCMIDEV_I2C3] = {
		.rsts = (int[]){RCAR_SCMIRST_I2C3,-1},
		.clks = (int[]){-1},
	},
	[RCAR_SCMIDEV_I2C4] = {
		.rsts = (int[]){RCAR_SCMIRST_I2C4,-1},
		.clks = (int[]){-1},
	},
	[RCAR_SCMIDEV_XHCI0] = {
		.rsts = (int[]){RCAR_SCMIRST_XHCI0,-1},
		.clks = (int[]){RCAR_SCMICLK_XHCI0,-1},
	},
	[RCAR_SCMIDEV_OHCI0] = {
		.rsts = (int[]){RCAR_SCMIRST_USB2_01,RCAR_SCMIRST_USB2_02,-1},
		.clks = (int[]){RCAR_SCMICLK_EHCI0,RCAR_SCMICLK_HSUSB,-1},
	},
	[RCAR_SCMIDEV_EHCI0] = {
		.rsts = (int[]){RCAR_SCMIRST_USB2_01,RCAR_SCMIRST_USB2_02,-1},
		.clks = (int[]){RCAR_SCMICLK_EHCI0,RCAR_SCMICLK_HSUSB,-1},
	},
	[RCAR_SCMIDEV_OHCI1] = {
		.rsts = (int[]){RCAR_SCMIRST_USB2_1,-1},
		.clks = (int[]){RCAR_SCMICLK_EHCI1,-1},
	},
	[RCAR_SCMIDEV_EHCI1] = {
		.rsts = (int[]){RCAR_SCMIRST_USB2_1,-1},
		.clks = (int[]){RCAR_SCMICLK_EHCI1,-1},
	},
	[RCAR_SCMIDEV_USB_DMAC0] = {
		.rsts = (int[]){RCAR_SCMIRST_USB_DMAC0,-1},
		.clks = (int[]){RCAR_SCMICLK_USB_DMAC0,-1},
	},
	[RCAR_SCMIDEV_USB_DMAC1] = {
		.rsts = (int[]){RCAR_SCMIRST_USB_DMAC1,-1},
		.clks = (int[]){RCAR_SCMICLK_USB_DMAC1,-1},
	},
	[RCAR_SCMIDEV_USB_DMAC2] = {
		.rsts = (int[]){RCAR_SCMIRST_USB_DMAC2,-1},
		.clks = (int[]){RCAR_SCMICLK_USB_DMAC30,-1},
	},
	[RCAR_SCMIDEV_USB_DMAC3] = {
		.rsts = (int[]){RCAR_SCMIRST_USB_DMAC3,-1},
		.clks = (int[]){RCAR_SCMICLK_USB_DMAC31,-1},
	},
	[RCAR_SCMIDEV_USB2_PHY0] = {
		.rsts = (int[]){RCAR_SCMIRST_USB2_01,RCAR_SCMIRST_USB2_02,-1},
		.clks = (int[]){RCAR_SCMICLK_EHCI0,RCAR_SCMICLK_HSUSB,-1},
	},
	[RCAR_SCMIDEV_USB2_PHY1] = {
		.rsts = (int[]){RCAR_SCMIRST_USB2_1,-1},
		.clks = (int[]){RCAR_SCMICLK_EHCI1,-1},
	},
	[RCAR_SCMIDEV_HSUSB] = {
		.rsts = (int[]){RCAR_SCMIRST_USB2_01,RCAR_SCMIRST_USB2_02,-1},
		.clks = (int[]){RCAR_SCMICLK_EHCI0,RCAR_SCMICLK_HSUSB,-1},
	},
	[RCAR_SCMIDEV_SOUND] = {
		.rsts = (int[]){RCAR_SCMIRST_SSI,RCAR_SCMIRST_SSI9,
				RCAR_SCMIRST_SSI8,RCAR_SCMIRST_SSI7,
				RCAR_SCMIRST_SSI6,RCAR_SCMIRST_SSI5,
				RCAR_SCMIRST_SSI4,RCAR_SCMIRST_SSI3,
				RCAR_SCMIRST_SSI2,RCAR_SCMIRST_SSI1,
				RCAR_SCMIRST_SSI0,-1},
		.clks = (int[]){RCAR_SCMICLK_SSI_ALL,RCAR_SCMICLK_SSI9,
				RCAR_SCMICLK_SSI8,RCAR_SCMICLK_SSI7,
				RCAR_SCMICLK_SSI6,RCAR_SCMICLK_SSI5,
				RCAR_SCMICLK_SSI4,RCAR_SCMICLK_SSI3,
				RCAR_SCMICLK_SSI2,RCAR_SCMICLK_SSI1,
				RCAR_SCMICLK_SSI0,
				RCAR_SCMICLK_SCU_SRC9,RCAR_SCMICLK_SCU_SRC8,
				RCAR_SCMICLK_SCU_SRC7,RCAR_SCMICLK_SCU_SRC6,
				RCAR_SCMICLK_SCU_SRC5,RCAR_SCMICLK_SCU_SRC4,
				RCAR_SCMICLK_SCU_SRC3,RCAR_SCMICLK_SCU_SRC2,
				RCAR_SCMICLK_SCU_SRC1,RCAR_SCMICLK_SCU_SRC0,
				RCAR_SCMICLK_SCU_MIX1,RCAR_SCMICLK_SCU_MIX0,
				RCAR_SCMICLK_SCU_DVC1,RCAR_SCMICLK_SCU_DVC0,-1},
	},
	[RCAR_SCMIDEV_AUDMA0] = {
		.rsts = (int[]){RCAR_SCMIRST_AUDMAC0,-1},
		.clks = (int[]){RCAR_SCMICLK_AUDMAC0,-1},
	},
	[RCAR_SCMIDEV_AUDMA1] = {
		.rsts = (int[]){RCAR_SCMIRST_AUDMAC1,-1},
		.clks = (int[]){RCAR_SCMICLK_AUDMAC1,-1},
	},
};
