/*
 * Copyright (c) 2015-2018, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bl2_lifec_register.h"
#include "bl2_axi_register.h"
#include "bl2_secure_setting.h"
#include "mmio.h"
#include "micro_wait.h"

#define LIFEC_SETTING_WAIT	(10U)	/* 10us wait */

static void lifec_security_setting(void);
static void axi_security_setting(void);

static const struct {
	uint32_t	adr;
	uint32_t	val;
} lifec_reg_setting[] = {

	/* LIFEC0 (SECURITY) settings */

	/* Security attribute setting for master ports */
	/* Bit 0: ARM realtime core (Cortex-R7) master port. */
	/*       0: Non-Secure */
	{SEC_SRC,		0x0000001EU},

	/** Security attribute setting for slave ports 0 */
	/*	{SEC_SEL0,		0xFFFFFFFFU},*/

	/** Security attribute setting for slave ports 1 */
	/*	{SEC_SEL1,		0xFFFFFFFFU},*/

	/** Security attribute setting for slave ports 2 */
	/*	{SEC_SEL2,		0xFFFFFFFFU},*/

	/** Security attribute setting for slave ports 3 */
	/* Bit19: AXI-Bus (Main Memory domain AXI) slave ports. */
	/*        0: registers can be accessed from secure resource only. */
	/* Bit 9: DBSC4 register access slave ports. */
	/*        0: registers can be accessed from secure resource only. */
#if (LIFEC_DBSC_PROTECT_ENABLE == 1)
	{SEC_SEL3,		0xFFF7FDFFU},
#else
	{SEC_SEL3,		0xFFFFFFFFU},
#endif

	/** Security attribute setting for slave ports 4 */
	/*	{SEC_SEL4,		0xFFFFFFFFU},*/

	/** Security attribute setting for slave ports 5 */
	/* Bit 6: Boot ROM slave ports. */
	/*        0: registers can be accessed from secure resource only. */
	{SEC_SEL5,		0xFFFFFFBFU},

	/** Security attribute setting for slave ports 6 */
	/* Bit13: SCEG PKA (secure APB) slave ports. */
	/*        0: registers can be accessed from secure resource only. */
	/*        1: Reserved[R-Car E3] */
	/* Bit12: SCEG PKA (public APB) slave ports. */
	/*        0: registers can be accessed from secure resource only. */
	/*        1: Reserved[R-Car E3] */
	/* Bit10: SCEG Secure Core slave ports. */
	/*        0: registers can be accessed from secure resource only. */
#if RCAR_LSI == RCAR_E3
	{SEC_SEL6,		0xFFFFFBFFU},
#else /* RCAR_LSI == RCAR_E3 */
	{SEC_SEL6,		0xFFFFCBFFU},
#endif /* RCAR_LSI == RCAR_E3 */

	/** Security attribute setting for slave ports 7 */
	/*	{SEC_SEL7,		0xFFFFFFFFU},*/

	/** Security attribute setting for slave ports 8 */
	/*	{SEC_SEL8,		0xFFFFFFFFU},*/

	/** Security attribute setting for slave ports 9 */
	/*	{SEC_SEL9,		0xFFFFFFFFU},*/

	/** Security attribute setting for slave ports 10 */
	/*	{SEC_SEL10,		0xFFFFFFFFU},*/

	/** Security attribute setting for slave ports 11 */
	/*	{SEC_SEL11,		0xFFFFFFFFU},*/

	/** Security attribute setting for slave ports 12 */
	/*	{SEC_SEL12,		0xFFFFFFFFU},*/

	/** Security attribute setting for slave ports 13 */
	/* Bit22: RPC slave ports. */
	/*        0: registers can be accessed from secure resource only. */
#if (RCAR_DISABLE_NONSECURE_RPC_ACCESS != 0)
	{SEC_SEL13,		0xFFBFFFFFU},
#endif

	/** Security attribute setting for slave ports 14 */
	/* Bit27: System Timer (SCMT) slave ports. */
	/*        0: registers can be accessed from secure resource only. */
	/* Bit26: System Watchdog Timer (SWDT) slave ports. */
	/*        0: registers can be accessed from secure resource only. */
	{SEC_SEL14,		0xF3FFFFFFU},

	/** Security attribute setting for slave ports 15 */
	/* Bit13: RST slave ports. */
	/*        0: registers can be accessed from secure resource only. */
	/* Bit 7: Life Cycle 0 slave ports. */
	/*        0: registers can be accessed from secure resource only. */
	{SEC_SEL15,		0xFFFFFF3FU},

	/** Security group 0 attribute setting for master ports 0 */
	/** Security group 1 attribute setting for master ports 0 */
	/*	{SEC_GRP0CR0,		0x00000000U},*/
	/*	{SEC_GRP1CR0,		0x00000000U},*/

	/** Security group 0 attribute setting for master ports 1 */
	/** Security group 1 attribute setting for master ports 1 */
	/*	{SEC_GRP0CR1,		0x00000000U},*/
	/*	{SEC_GRP1CR1,		0x00000000U},*/

	/** Security group 0 attribute setting for master ports 2 */
	/** Security group 1 attribute setting for master ports 2 */
	/* Bit17: SCEG Secure Core master ports. */
	/*        SecurityGroup3 */
	{SEC_GRP0CR2,		0x00020000U},
	{SEC_GRP1CR2,		0x00020000U},

	/** Security group 0 attribute setting for master ports 3 */
	/** Security group 1 attribute setting for master ports 3 */
	/*	{SEC_GRP0CR3,		0x00000000U},*/
	/*	{SEC_GRP1CR3,		0x00000000U},*/

	/** Security group 0 attribute setting for slave ports 0 */
	/** Security group 1 attribute setting for slave ports 0 */
	/*	{SEC_GRP0COND0,		0x00000000U},*/
	/*	{SEC_GRP1COND0,		0x00000000U},*/

	/** Security group 0 attribute setting for slave ports 1 */
	/** Security group 1 attribute setting for slave ports 1 */
	/*	{SEC_GRP0COND1,		0x00000000U},*/
	/*	{SEC_GRP1COND1,		0x00000000U},*/

	/** Security group 0 attribute setting for slave ports 2 */
	/** Security group 1 attribute setting for slave ports 2 */
	/*	{SEC_GRP0COND2,		0x00000000U},*/
	/*	{SEC_GRP1COND2,		0x00000000U},*/

	/** Security group 0 attribute setting for slave ports 3 */
	/** Security group 1 attribute setting for slave ports 3 */
	/* Bit19: AXI-Bus (Main Memory domain AXI) slave ports. */
	/*        SecurityGroup3 */
	/* Bit 9: DBSC4 register access slave ports. */
	/*        SecurityGroup3 */
#if (LIFEC_DBSC_PROTECT_ENABLE == 1)
	{SEC_GRP0COND3,		0x00080200U},
	{SEC_GRP1COND3,		0x00080200U},
#else
	{SEC_GRP0COND3,		0x00000000U},
	{SEC_GRP1COND3,		0x00000000U},
#endif

	/** Security group 0 attribute setting for slave ports 4 */
	/** Security group 1 attribute setting for slave ports 4 */
	/*	{SEC_GRP0COND4,		0x00000000U},*/
	/*	{SEC_GRP1COND4,		0x00000000U},*/

	/** Security group 0 attribute setting for slave ports 5 */
	/** Security group 1 attribute setting for slave ports 5 */
	/* Bit 6: Boot ROM slave ports. */
	/*        SecurityGroup3 */
	{SEC_GRP0COND5,		0x00000040U},
	{SEC_GRP1COND5,		0x00000040U},

	/** Security group 0 attribute setting for slave ports 6 */
	/** Security group 1 attribute setting for slave ports 6 */
	/* Bit13: SCEG PKA (secure APB) slave ports. */
	/*        SecurityGroup3 */
	/*        Reserved[R-Car E3] */
	/* Bit12: SCEG PKA (public APB) slave ports. */
	/*        SecurityGroup3 */
	/*        Reserved[R-Car E3] */
	/* Bit10: SCEG Secure Core slave ports. */
	/*        SecurityGroup3 */
#if RCAR_LSI == RCAR_E3
	{SEC_GRP0COND6,		0x00000400U},
	{SEC_GRP1COND6,		0x00000400U},
#else /* RCAR_LSI == RCAR_E3 */
	{SEC_GRP0COND6,		0x00003400U},
	{SEC_GRP1COND6,		0x00003400U},
#endif /* RCAR_LSI == RCAR_E3 */

	/** Security group 0 attribute setting for slave ports 7 */
	/** Security group 1 attribute setting for slave ports 7 */
	/*	{SEC_GRP0COND7,		0x00000000U},*/
	/*	{SEC_GRP1COND7,		0x00000000U},*/

	/** Security group 0 attribute setting for slave ports 8 */
	/** Security group 1 attribute setting for slave ports 8 */
	/*	{SEC_GRP0COND8,		0x00000000U},*/
	/*	{SEC_GRP1COND8,		0x00000000U},*/

	/** Security group 0 attribute setting for slave ports 9 */
	/** Security group 1 attribute setting for slave ports 9 */
	/*	{SEC_GRP0COND9,		0x00000000U},*/
	/*	{SEC_GRP1COND9,		0x00000000U},*/

	/** Security group 0 attribute setting for slave ports 10 */
	/** Security group 1 attribute setting for slave ports 10 */
	/*	{SEC_GRP0COND10,	0x00000000U},*/
	/*	{SEC_GRP1COND10,	0x00000000U},*/

	/** Security group 0 attribute setting for slave ports 11 */
	/** Security group 1 attribute setting for slave ports 11 */
	/*	{SEC_GRP0COND11,	0x00000000U},*/
	/*	{SEC_GRP1COND11,	0x00000000U},*/

	/** Security group 0 attribute setting for slave ports 12 */
	/** Security group 1 attribute setting for slave ports 12 */
	/*	{SEC_GRP0COND12,	0x00000000U},*/
	/*	{SEC_GRP1COND12,	0x00000000U},*/

	/** Security group 0 attribute setting for slave ports 13 */
	/** Security group 1 attribute setting for slave ports 13 */
	/* Bit22: RPC slave ports. */
	/*        SecurityGroup3 */
#if (RCAR_DISABLE_NONSECURE_RPC_ACCESS != 0)
	{SEC_GRP0COND13,	0x00400000U},
	{SEC_GRP1COND13,	0x00400000U},
#endif

	/** Security group 0 attribute setting for slave ports 14 */
	/** Security group 1 attribute setting for slave ports 14 */
	/* Bit26: System Timer (SCMT) slave ports. */
	/*        SecurityGroup3 */
	/* Bit27: System Watchdog Timer (SWDT) slave ports. */
	/*        SecurityGroup3 */
	{SEC_GRP0COND14,	0x0C000000U},
	{SEC_GRP1COND14,	0x0C000000U},

	/** Security group 0 attribute setting for slave ports 15 */
	/** Security group 1 attribute setting for slave ports 15 */
	/* Bit13: RST slave ports. */
	/*        SecurityGroup3 */
	/* Bit 7: Life Cycle 0 slave ports. */
	/*        SecurityGroup3 */
	/* Bit 6: TDBG (is described in Debug and Trace section) slave ports.*/
	/*        SecurityGroup3 */
	{SEC_GRP0COND15,	0x000000C0U},
	{SEC_GRP1COND15,	0x000000C0U},

	/** Security write protection attribute setting for slave ports 0 */
	/*	{SEC_READONLY0,		0x00000000U},*/

	/** Security write protection attribute setting for slave ports 1 */
	/*	{SEC_READONLY1,		0x00000000U},*/

	/** Security write protection attribute setting for slave ports 2 */
	/*	{SEC_READONLY2,		0x00000000U},*/

	/** Security write protection attribute setting for slave ports 3 */
	/*	{SEC_READONLY3,		0x00000000U},*/

	/** Security write protection attribute setting for slave ports 4 */
	/*	{SEC_READONLY4,		0x00000000U},*/

	/** Security write protection attribute setting for slave ports 5 */
	/*	{SEC_READONLY5,		0x00000000U},*/

	/** Security write protection attribute setting for slave ports 6 */
	/*	{SEC_READONLY6,		0x00000000U},*/

	/** Security write protection attribute setting for slave ports 7 */
	/*	{SEC_READONLY7,		0x00000000U},*/

	/** Security write protection attribute setting for slave ports 8 */
	/*	{SEC_READONLY8,		0x00000000U},*/

	/** Security write protection attribute setting for slave ports 9 */
	/*	{SEC_READONLY9,		0x00000000U},*/

	/** Security write protection attribute setting for slave ports 10 */
	/*	{SEC_READONLY10,	0x00000000U},*/

	/** Security write protection attribute setting for slave ports 11 */
	/*	{SEC_READONLY11,	0x00000000U},*/

	/** Security write protection attribute setting for slave ports 12 */
	/*	{SEC_READONLY12,	0x00000000U},*/

	/** Security write protection attribute setting for slave ports 13 */
	/*	{SEC_READONLY13,	0x00000000U},*/

	/** Security write protection attribute setting for slave ports 14 */
	/*	{SEC_READONLY14,	0x00000000U},*/

	/** Security write protection attribute setting for slave ports 15 */
	/*	{SEC_READONLY15,	0x00000000U}*/

};

	/* AXI settings */
static const struct {
	uint32_t	adr;
	uint32_t	val;
} axi_reg_setting[] = {

	/* DRAM protection */

	/* AXI dram protected area division */
	{AXI_DPTDIVCR0,		0x0E0403F0U},
	{AXI_DPTDIVCR1,		0x0E0407E0U},
	{AXI_DPTDIVCR2,		0x0E080000U},
	{AXI_DPTDIVCR3,		0x0E080000U},
	{AXI_DPTDIVCR4,		0x0E080000U},
	{AXI_DPTDIVCR5,		0x0E080000U},
	{AXI_DPTDIVCR6,		0x0E080000U},
	{AXI_DPTDIVCR7,		0x0E080000U},
	{AXI_DPTDIVCR8,		0x0E080000U},
	{AXI_DPTDIVCR9,		0x0E080000U},
	{AXI_DPTDIVCR10,	0x0E080000U},
	{AXI_DPTDIVCR11,	0x0E080000U},
	{AXI_DPTDIVCR12,	0x0E080000U},
	{AXI_DPTDIVCR13,	0x0E080000U},
	{AXI_DPTDIVCR14,	0x0E080000U},

	/* AXI dram protected area setting */
	{AXI_DPTCR0,		0x0E000000U},
	{AXI_DPTCR1,		0x0E000E0EU},
	{AXI_DPTCR2,		0x0E000000U},
	{AXI_DPTCR3,		0x0E000000U},
	{AXI_DPTCR4,		0x0E000000U},
	{AXI_DPTCR5,		0x0E000000U},
	{AXI_DPTCR6,		0x0E000000U},
	{AXI_DPTCR7,		0x0E000000U},
	{AXI_DPTCR8,		0x0E000000U},
	{AXI_DPTCR9,		0x0E000000U},
	{AXI_DPTCR10,		0x0E000000U},
	{AXI_DPTCR11,		0x0E000000U},
	{AXI_DPTCR12,		0x0E000000U},
	{AXI_DPTCR13,		0x0E000000U},
	{AXI_DPTCR14,		0x0E000000U},
	{AXI_DPTCR15,		0x0E000000U},


	/* SRAM ptotection */

	/* AXI sram protected area division */
	{AXI_SPTDIVCR0,		0x0E0E6304U},
	{AXI_SPTDIVCR1,		0x0E0E6360U},
	{AXI_SPTDIVCR2,		0x0E0E6360U},
	{AXI_SPTDIVCR3,		0x0E0E6360U},
	{AXI_SPTDIVCR4,		0x0E0E6360U},
	{AXI_SPTDIVCR5,		0x0E0E6360U},
	{AXI_SPTDIVCR6,		0x0E0E6360U},
	{AXI_SPTDIVCR7,		0x0E0E6360U},
	{AXI_SPTDIVCR8,		0x0E0E6360U},
	{AXI_SPTDIVCR9,		0x0E0E6360U},
	{AXI_SPTDIVCR10,	0x0E0E6360U},
	{AXI_SPTDIVCR11,	0x0E0E6360U},
	{AXI_SPTDIVCR12,	0x0E0E6360U},
	{AXI_SPTDIVCR13,	0x0E0E6360U},
	{AXI_SPTDIVCR14,	0x0E0E6360U},

	/* AXI sram protected area setting */
	{AXI_SPTCR0,		0x0E000E0EU},
	{AXI_SPTCR1,		0x0E000000U},
	{AXI_SPTCR2,		0x0E000000U},
	{AXI_SPTCR3,		0x0E000000U},
	{AXI_SPTCR4,		0x0E000000U},
	{AXI_SPTCR5,		0x0E000000U},
	{AXI_SPTCR6,		0x0E000000U},
	{AXI_SPTCR7,		0x0E000000U},
	{AXI_SPTCR8,		0x0E000000U},
	{AXI_SPTCR9,		0x0E000000U},
	{AXI_SPTCR10,		0x0E000000U},
	{AXI_SPTCR11,		0x0E000000U},
	{AXI_SPTCR12,		0x0E000000U},
	{AXI_SPTCR13,		0x0E000000U},
	{AXI_SPTCR14,		0x0E000000U},
	{AXI_SPTCR15,		0x0E000000U}

};

/* Port access security setting */
static void lifec_security_setting(void)
{
	int32_t		i, ie;

	i = 0;
	ie = (int32_t)(sizeof(lifec_reg_setting) / sizeof(lifec_reg_setting[0]));
	for (; i < ie; i++) {
		mmio_write_32((uintptr_t)lifec_reg_setting[i].adr,
			lifec_reg_setting[i].val);
	}
}

/* SRAM/DRAM protection setting */
static void axi_security_setting(void)
{
	int32_t		i, ie;

	i = 0;
	ie = (int32_t)(sizeof(axi_reg_setting) / sizeof(axi_reg_setting[0]));
	for (; i < ie; i++) {
		mmio_write_32((uintptr_t)axi_reg_setting[i].adr,
			axi_reg_setting[i].val);
	}
}

void bl2_secure_setting(void)
{
	lifec_security_setting();

	axi_security_setting();

	micro_wait(LIFEC_SETTING_WAIT);

	return;
}
