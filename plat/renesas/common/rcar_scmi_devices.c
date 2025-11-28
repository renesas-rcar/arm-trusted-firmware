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
		.pins = (int[]){-1},
};

const struct scmi_device rcar_devices[RCAR_SCMIDEV_MAX] = {
	[0 ... RCAR_SCMIDEV_MAX - 1] = default_dev, /* sentinel */

	[RCAR_SCMIDEV_EAVB] = {
		.rsts = (int[]){RCAR_SCMIRST_EAVB,-1},
		.clks = (int[]){RCAR_SCMICLK_EAVB,-1},
		.pins = (int[]){RCAR_SCMIPIN_EAVB,-1},
	},
	[RCAR_SCMIDEV_HDMI0] = {
		.rsts = (int[]){RCAR_SCMIRST_HDMI0,-1},
		.clks = (int[]){RCAR_SCMICLK_HDMI,RCAR_SCMICLK_HDMI0,-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_HDMI1] = {
		.rsts = (int[]){RCAR_SCMIRST_HDMI1,-1},
		.clks = (int[]){RCAR_SCMICLK_HDMI,RCAR_SCMICLK_HDMI1,-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_I2C0] = {
		.rsts = (int[]){RCAR_SCMIRST_I2C0,-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_I2C1] = {
		.rsts = (int[]){RCAR_SCMIRST_I2C1,-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_I2C2] = {
		.rsts = (int[]){RCAR_SCMIRST_I2C2,-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_I2C2, -1},
	},
	[RCAR_SCMIDEV_I2C3] = {
		.rsts = (int[]){RCAR_SCMIRST_I2C3,-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_I2C4] = {
		.rsts = (int[]){RCAR_SCMIRST_I2C4,-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_XHCI0] = {
		.rsts = (int[]){RCAR_SCMIRST_XHCI0,-1},
		.clks = (int[]){RCAR_SCMICLK_XHCI0,-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_OHCI0] = {
		.rsts = (int[]){RCAR_SCMIRST_USB2_01,RCAR_SCMIRST_USB2_02,-1},
		.clks = (int[]){RCAR_SCMICLK_EHCI0,RCAR_SCMICLK_HSUSB,-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_EHCI0] = {
		.rsts = (int[]){RCAR_SCMIRST_USB2_01,RCAR_SCMIRST_USB2_02,-1},
		.clks = (int[]){RCAR_SCMICLK_EHCI0,RCAR_SCMICLK_HSUSB,-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_OHCI1] = {
		.rsts = (int[]){RCAR_SCMIRST_USB2_1,-1},
		.clks = (int[]){RCAR_SCMICLK_EHCI1,-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_EHCI1] = {
		.rsts = (int[]){RCAR_SCMIRST_USB2_1,-1},
		.clks = (int[]){RCAR_SCMICLK_EHCI1,-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_USB_DMAC0] = {
		.rsts = (int[]){RCAR_SCMIRST_USB_DMAC0,-1},
		.clks = (int[]){RCAR_SCMICLK_USB_DMAC0,-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_USB_DMAC1] = {
		.rsts = (int[]){RCAR_SCMIRST_USB_DMAC1,-1},
		.clks = (int[]){RCAR_SCMICLK_USB_DMAC1,-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_USB_DMAC2] = {
		.rsts = (int[]){RCAR_SCMIRST_USB_DMAC2,-1},
		.clks = (int[]){RCAR_SCMICLK_USB_DMAC30,-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_USB_DMAC3] = {
		.rsts = (int[]){RCAR_SCMIRST_USB_DMAC3,-1},
		.clks = (int[]){RCAR_SCMICLK_USB_DMAC31,-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_USB2_PHY0] = {
		.rsts = (int[]){RCAR_SCMIRST_USB2_01,RCAR_SCMIRST_USB2_02,-1},
		.clks = (int[]){RCAR_SCMICLK_EHCI0,RCAR_SCMICLK_HSUSB,-1},
		.pins = (int[]){RCAR_SCMIPIN_USB2_PHY0, -1},
	},
	[RCAR_SCMIDEV_USB2_PHY1] = {
		.rsts = (int[]){RCAR_SCMIRST_USB2_1,-1},
		.clks = (int[]){RCAR_SCMICLK_EHCI1,-1},
		.pins = (int[]){RCAR_SCMIPIN_USB2_PHY1, -1},
	},
	[RCAR_SCMIDEV_HSUSB] = {
		.rsts = (int[]){RCAR_SCMIRST_USB2_01,RCAR_SCMIRST_USB2_02,-1},
		.clks = (int[]){RCAR_SCMICLK_EHCI0,RCAR_SCMICLK_HSUSB,-1},
		.pins = (int[]){-1},
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
		.pins = (int[]){RCAR_SCMIPIN_SOUND, -1},

	},
	[RCAR_SCMIDEV_AUDMA0] = {
		.rsts = (int[]){RCAR_SCMIRST_AUDMAC0,-1},
		.clks = (int[]){RCAR_SCMICLK_AUDMAC0,-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_AUDMA1] = {
		.rsts = (int[]){RCAR_SCMIRST_AUDMAC1,-1},
		.clks = (int[]){RCAR_SCMICLK_AUDMAC1,-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_GPIO0] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_GPIO0, -1},
	},
	[RCAR_SCMIDEV_GPIO1] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_GPIO1, -1},
	},
	//TODO fill gpio
	[RCAR_SCMIDEV_GPIO2] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_GPIO2, -1},
	},
	[RCAR_SCMIDEV_GPIO3] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_GPIO3, -1},
	},
	[RCAR_SCMIDEV_GPIO4] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_GPIO4, -1},
	},
	[RCAR_SCMIDEV_GPIO5] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_GPIO5, -1},
	},
	[RCAR_SCMIDEV_GPIO6] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_GPIO6, -1},
	},
	[RCAR_SCMIDEV_GPIO7] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_GPIO7, -1},
	},
	[RCAR_SCMIDEV_PINCTRL] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_PINCTRL, -1},
	},
	[RCAR_SCMIDEV_CAN0] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_CAN0, -1},
	},
	[RCAR_SCMIDEV_CAN1] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_CAN1, -1},
	},
	[RCAR_SCMIDEV_CANFD] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_CANFD, -1},
	},
	[RCAR_SCMIDEV_DRIF0] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_DRIF1] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_DRIF2] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_DRIF3] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_DU] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_DU, -1},
	},
	[RCAR_SCMIDEV_HSCIF0] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_HSCIF0, -1},
	},
	[RCAR_SCMIDEV_HSCIF1] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_HSCIF1, -1},
	},
	[RCAR_SCMIDEV_HSCIF2] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_HSCIF3] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_HSCIF4] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_I2C5] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_I2C6] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_I2C_DVFS] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_I2C_DVFS, -1},
	},
	[RCAR_SCMIDEV_MLB] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_MSIOF0] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_MSIOF1] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_MSIOF2] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_MSIOF3] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_PWM0] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_PWM1] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_PWM2] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_PWM3] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_PWM4] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_PWM5] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_PWM6] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_SATA] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_SCIF0] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_SCIF1] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_SCIF1, -1},
	},
	[RCAR_SCMIDEV_SCIF2] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_SCIF2, -1},
	},
	[RCAR_SCMIDEV_SCIF3] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_SCIF4] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_SCIF5] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_SDHI0] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_SDHI0, -1},
	},
	[RCAR_SCMIDEV_SDHI1] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_SDHI2] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_SDHI2, -1},
	},
	[RCAR_SCMIDEV_SDHI3] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_SDHI3, -1},
	},
	[RCAR_SCMIDEV_SSI] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_TMU] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_TPU] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_USB0] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_USB1] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_USB2] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_USB3] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_VIN4] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){-1},
	},
	[RCAR_SCMIDEV_VIN5] = {
		.rsts = (int[]){-1},
		.clks = (int[]){-1},
		.pins = (int[]){RCAR_SCMIPIN_VIN5, -1},
	},
};
