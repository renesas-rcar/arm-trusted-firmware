/*
 * Copyright (c) 2015-2018, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <debug.h>
#include <mmio.h>
#include "rcar_def.h"
#include "bl2_cpg_register.h"
#include "bl2_cpg_init.h"
#include "rcar_private.h"

static void bl2_secure_cpg_init(void);
#if (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_H3) || (RCAR_LSI == RCAR_H3N)
static void bl2_realtime_cpg_init_h3(void);
static void bl2_system_cpg_init_h3(void);
#endif /* (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_H3) || (RCAR_LSI == RCAR_H3N) */
#if (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_M3)
static void bl2_realtime_cpg_init_m3(void);
static void bl2_system_cpg_init_m3(void);
#endif /* (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_M3) */
#if (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_M3N)
static void bl2_realtime_cpg_init_m3n(void);
static void bl2_system_cpg_init_m3n(void);
#endif /* (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_M3N) */
#if (RCAR_LSI == RCAR_E3)
static void bl2_realtime_cpg_init_e3(void);
static void bl2_system_cpg_init_e3(void);
#endif  /* (RCAR_LSI == RCAR_E3) */
#if (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_V3M)
static void bl2_realtime_cpg_init_v3m(void);
static void bl2_system_cpg_init_v3m(void);
#endif /* (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_V3M) */
#if (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_V3H)
static void bl2_realtime_cpg_init_v3h(void);
static void bl2_system_cpg_init_v3h(void);
#endif /* (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_V3H) */

typedef struct {
	uintptr_t	adr;
	uint32_t	val;
} reg_setting_t;

static void bl2_secure_cpg_init(void)
{

	/* CPG (SECURITY) registers */

	/* Secure Module Stop Control Register 0 */
	cpg_write(SCMSTPCR0,	0xFFFFFFFFU);
	/* Secure Module Stop Control Register 1 */
	cpg_write(SCMSTPCR1,	0xFFFFFFFFU);

#if (RCAR_LSI == RCAR_E3)
	/* Secure Module Stop Control Register 2 */
	cpg_write(SCMSTPCR2,	0xEFFFFFFFU);
#else /* RCAR_LSI == RCAR_E3 */
	/* Secure Module Stop Control Register 2 */
	cpg_write(SCMSTPCR2,	0xEBFFFFFFU);
#endif /* RCAR_LSI == RCAR_E3 */

	/* Secure Module Stop Control Register 3 */
	cpg_write(SCMSTPCR3,	0xFFFFFFFFU);
	/* Secure Module Stop Control Register 4 */
	cpg_write(SCMSTPCR4,	0x7FFFFFFFU);
	/* Secure Module Stop Control Register 5 */
	cpg_write(SCMSTPCR5,	0xBFFFFFFFU);
	/* Secure Module Stop Control Register 6 */
	cpg_write(SCMSTPCR6,	0xFFFFFFFFU);
	/* Secure Module Stop Control Register 7 */
	cpg_write(SCMSTPCR7,	0xFFFFFFFFU);
	/* Secure Module Stop Control Register 8 */
	cpg_write(SCMSTPCR8,	0xFFFFFFFFU);
	/* Secure Module Stop Control Register 9 */
	cpg_write(SCMSTPCR9,	0xFFFDFFFFU);
	/* Secure Module Stop Control Register 10 */
	cpg_write(SCMSTPCR10,	0xFFFFFFFFU);
	/* Secure Module Stop Control Register 11 */
	cpg_write(SCMSTPCR11,	0xFFFFFFFFU);
	/* Secure Software Reset Access Enable Control Register 0 */
	cpg_write(SCSRSTECR0,	0x00000000U);
	/* Secure Software Reset Access Enable Control Register 1 */
	cpg_write(SCSRSTECR1,	0x00000000U);

#if (RCAR_LSI == RCAR_E3)
	/* Secure Software Reset Access Enable Control Register 2 */
	cpg_write(SCSRSTECR2,	0x10000000U);
#else /* RCAR_LSI == RCAR_E3 */
	/* Secure Software Reset Access Enable Control Register 2 */
	cpg_write(SCSRSTECR2,	0x14000000U);
#endif /* RCAR_LSI == RCAR_E3 */

	/* Secure Software Reset Access Enable Control Register 3 */
	cpg_write(SCSRSTECR3,	0x00000000U);
	/* Secure Software Reset Access Enable Control Register 4 */
	cpg_write(SCSRSTECR4,	0x80000003U);
	/* Secure Software Reset Access Enable Control Register 5 */
	cpg_write(SCSRSTECR5,	0x40000000U);
	/* Secure Software Reset Access Enable Control Register 6 */
	cpg_write(SCSRSTECR6,	0x00000000U);
	/* Secure Software Reset Access Enable Control Register 7 */
	cpg_write(SCSRSTECR7,	0x00000000U);
	/* Secure Software Reset Access Enable Control Register 8 */
	cpg_write(SCSRSTECR8,	0x00000000U);
	/* Secure Software Reset Access Enable Control Register 9 */
	cpg_write(SCSRSTECR9,	0x00020000U);
	/* Secure Software Reset Access Enable Control Register 10 */
	cpg_write(SCSRSTECR10,	0x00000000U);
	/* Secure Software Reset Access Enable Control Register 11 */
	cpg_write(SCSRSTECR11,	0x00000000U);
}

#if (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_H3) || (RCAR_LSI == RCAR_H3N)
static void bl2_realtime_cpg_init_h3(void)
{
	uint32_t	prr;

	prr = mmio_read_32(RCAR_PRR) & RCAR_CUT_MASK;

	/* CPG (REALTIME) registers */

	if ((prr == RCAR_CUT_VER10) || (prr == RCAR_CUT_VER11)) {
		/* Realtime Module Stop Control Register 0 */
		cpg_write(RMSTPCR0,	0x00200000U);
	} else {
		/* Realtime Module Stop Control Register 0 */
		cpg_write(RMSTPCR0,	0x00210000U);
	}
	/* Realtime Module Stop Control Register 1 */
	cpg_write(RMSTPCR1,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 2 */
	cpg_write(RMSTPCR2,	0x040E0FDCU);
	/* Realtime Module Stop Control Register 3 */
	cpg_write(RMSTPCR3,	0xFFFFFFDFU);
	/* Realtime Module Stop Control Register 4 */
	cpg_write(RMSTPCR4,	0x80000004U);
	/* Realtime Module Stop Control Register 5 */
	cpg_write(RMSTPCR5,	0xC3FFFFFFU);
	/* Realtime Module Stop Control Register 6 */
	cpg_write(RMSTPCR6,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 7 */
	cpg_write(RMSTPCR7,	0xFFFFFFFFU);
	if ((prr == RCAR_CUT_VER10) || (prr == RCAR_CUT_VER11)) {
		/* Realtime Module Stop Control Register 8 */
		cpg_write(RMSTPCR8,	0x01F1FFF4U);
	} else {
		/* Realtime Module Stop Control Register 8 */
		cpg_write(RMSTPCR8,	0x01F1FFF7U);
	}
	/* Realtime Module Stop Control Register 9 */
	cpg_write(RMSTPCR9,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 10 */
	cpg_write(RMSTPCR10,	0xFFFEFFE0U);
	/* Realtime Module Stop Control Register 11 */
	cpg_write(RMSTPCR11,	0x000000B7U);
}

static void bl2_system_cpg_init_h3(void)
{
	/* CPG (SYSTEM) registers */

	/* System Module Stop Control Register 0 */
	cpg_write(SMSTPCR0,	0x00210000U);
	/* System Module Stop Control Register 1 */
	cpg_write(SMSTPCR1,	0xFFFFFFFFU);
	/* System Module Stop Control Register 2 */
	cpg_write(SMSTPCR2,	0x040E2FDCU);
	/* System Module Stop Control Register 3 */
	cpg_write(SMSTPCR3,	0xFFFFFBDFU);
	/* System Module Stop Control Register 4 */
	cpg_write(SMSTPCR4,	0x80000004U);
	/* System Module Stop Control Register 5 */
	cpg_write(SMSTPCR5,	0xC3FFFFFFU);
	/* System Module Stop Control Register 6 */
	cpg_write(SMSTPCR6,	0xFFFFFFFFU);
	/* System Module Stop Control Register 7 */
	cpg_write(SMSTPCR7,	0xFFFFFFFFU);
	/* System Module Stop Control Register 8 */
	cpg_write(SMSTPCR8,	0x01F1FFF5U);
	/* System Module Stop Control Register 9 */
	cpg_write(SMSTPCR9,	0xFFFFFFFFU);
	/* System Module Stop Control Register 10 */
	cpg_write(SMSTPCR10,	0xFFFEFFE0U);
	/* System Module Stop Control Register 11 */
	cpg_write(SMSTPCR11,	0x000000B7U);
}
#endif /* (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_H3) || (RCAR_LSI == RCAR_H3N) */

#if (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_M3)
static void bl2_realtime_cpg_init_m3(void)
{
	/* CPG (REALTIME) registers */

	/* Realtime Module Stop Control Register 0 */
	cpg_write(RMSTPCR0,	0x00200000U);
	/* Realtime Module Stop Control Register 1 */
	cpg_write(RMSTPCR1,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 2 */
	cpg_write(RMSTPCR2,	0x040E0FDCU);
	/* Realtime Module Stop Control Register 3 */
	cpg_write(RMSTPCR3,	0xFFFFFFDFU);
	/* Realtime Module Stop Control Register 4 */
	cpg_write(RMSTPCR4,	0x80000004U);
	/* Realtime Module Stop Control Register 5 */
	cpg_write(RMSTPCR5,	0xC3FFFFFFU);
	/* Realtime Module Stop Control Register 6 */
	cpg_write(RMSTPCR6,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 7 */
	cpg_write(RMSTPCR7,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 8 */
	cpg_write(RMSTPCR8,	0x01F1FFF7U);
	/* Realtime Module Stop Control Register 9 */
	cpg_write(RMSTPCR9,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 10 */
	cpg_write(RMSTPCR10,	0xFFFEFFE0U);
	/* Realtime Module Stop Control Register 11 */
	cpg_write(RMSTPCR11,	0x000000B7U);
}

static void bl2_system_cpg_init_m3(void)
{
	/* CPG (SYSTEM) registers */

	/* System Module Stop Control Register 0 */
	cpg_write(SMSTPCR0,	0x00200000U);
	/* System Module Stop Control Register 1 */
	cpg_write(SMSTPCR1,	0xFFFFFFFFU);
	/* System Module Stop Control Register 2 */
	cpg_write(SMSTPCR2,	0x040E2FDCU);
	/* System Module Stop Control Register 3 */
	cpg_write(SMSTPCR3,	0xFFFFFBDFU);
	/* System Module Stop Control Register 4 */
	cpg_write(SMSTPCR4,	0x80000004U);
	/* System Module Stop Control Register 5 */
	cpg_write(SMSTPCR5,	0xC3FFFFFFU);
	/* System Module Stop Control Register 6 */
	cpg_write(SMSTPCR6,	0xFFFFFFFFU);
	/* System Module Stop Control Register 7 */
	cpg_write(SMSTPCR7,	0xFFFFFFFFU);
	/* System Module Stop Control Register 8 */
	cpg_write(SMSTPCR8,	0x01F1FFF7U);
	/* System Module Stop Control Register 9 */
	cpg_write(SMSTPCR9,	0xFFFFFFFFU);
	/* System Module Stop Control Register 10 */
	cpg_write(SMSTPCR10,	0xFFFEFFE0U);
	/* System Module Stop Control Register 11 */
	cpg_write(SMSTPCR11,	0x000000B7U);
}
#endif /* (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_M3) */

#if (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_M3N)
static void bl2_realtime_cpg_init_m3n(void)
{
	/* CPG (REALTIME) registers */

	/* Realtime Module Stop Control Register 0 */
	cpg_write(RMSTPCR0,	0x00210000U);
	/* Realtime Module Stop Control Register 1 */
	cpg_write(RMSTPCR1,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 2 */
	cpg_write(RMSTPCR2,	0x040E0FDCU);
	/* Realtime Module Stop Control Register 3 */
	cpg_write(RMSTPCR3,	0xFFFFFFDFU);
	/* Realtime Module Stop Control Register 4 */
	cpg_write(RMSTPCR4,	0x80000004U);
	/* Realtime Module Stop Control Register 5 */
	cpg_write(RMSTPCR5,	0xC3FFFFFFU);
	/* Realtime Module Stop Control Register 6 */
	cpg_write(RMSTPCR6,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 7 */
	cpg_write(RMSTPCR7,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 8 */
	cpg_write(RMSTPCR8,	0x00F1FFF7U);
	/* Realtime Module Stop Control Register 9 */
	cpg_write(RMSTPCR9,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 10 */
	cpg_write(RMSTPCR10,	0xFFFFFFE0U);
	/* Realtime Module Stop Control Register 11 */
	cpg_write(RMSTPCR11,	0x000000B7U);
}

static void bl2_system_cpg_init_m3n(void)
{
	/* CPG (SYSTEM) registers */

	/* System Module Stop Control Register 0 */
	cpg_write(SMSTPCR0,	0x00210000U);
	/* System Module Stop Control Register 1 */
	cpg_write(SMSTPCR1,	0xFFFFFFFFU);
	/* System Module Stop Control Register 2 */
	cpg_write(SMSTPCR2,	0x040E2FDCU);
	/* System Module Stop Control Register 3 */
	cpg_write(SMSTPCR3,	0xFFFFFBDFU);
	/* System Module Stop Control Register 4 */
	cpg_write(SMSTPCR4,	0x80000004U);
	/* System Module Stop Control Register 5 */
	cpg_write(SMSTPCR5,	0xC3FFFFFFU);
	/* System Module Stop Control Register 6 */
	cpg_write(SMSTPCR6,	0xFFFFFFFFU);
	/* System Module Stop Control Register 7 */
	cpg_write(SMSTPCR7,	0xFFFFFFFFU);
	/* System Module Stop Control Register 8 */
	cpg_write(SMSTPCR8,	0x00F1FFF7U);
	/* System Module Stop Control Register 9 */
	cpg_write(SMSTPCR9,	0xFFFFFFFFU);
	/* System Module Stop Control Register 10 */
	cpg_write(SMSTPCR10,	0xFFFFFFE0U);
	/* System Module Stop Control Register 11 */
	cpg_write(SMSTPCR11,	0x000000B7U);
}
#endif /* (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_M3N) */

#if (RCAR_LSI == RCAR_E3)
static void bl2_realtime_cpg_init_e3(void)
{
	/* CPG (REALTIME) registers */

	/* Realtime Module Stop Control Register 0 */
	cpg_write(RMSTPCR0,	0x00210000U);
	/* Realtime Module Stop Control Register 1 */
	cpg_write(RMSTPCR1,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 2 */
	cpg_write(RMSTPCR2,	0x000E0FDCU);
	/* Realtime Module Stop Control Register 3 */
	cpg_write(RMSTPCR3,	0xFFFFFFDFU);
	/* Realtime Module Stop Control Register 4 */
	cpg_write(RMSTPCR4,	0x80000004U);
	/* Realtime Module Stop Control Register 5 */
	cpg_write(RMSTPCR5,	0xC3FFFFFFU);
	/* Realtime Module Stop Control Register 6 */
	cpg_write(RMSTPCR6,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 7 */
	cpg_write(RMSTPCR7,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 8 */
	cpg_write(RMSTPCR8,	0x00F1FFF7U);
	/* Realtime Module Stop Control Register 9 */
	cpg_write(RMSTPCR9,	0xFFFFFFDFU);
	/* Realtime Module Stop Control Register 10 */
	cpg_write(RMSTPCR10,	0xFFFFFFE8U);
	/* Realtime Module Stop Control Register 11 */
	cpg_write(RMSTPCR11,	0x000000B7U);
}

static void bl2_system_cpg_init_e3(void)
{
	/* CPG (SYSTEM) registers */

	/* System Module Stop Control Register 0 */
	cpg_write(SMSTPCR0,	0x00210000U);
	/* System Module Stop Control Register 1 */
	cpg_write(SMSTPCR1,	0xFFFFFFFFU);
	/* System Module Stop Control Register 2 */
	cpg_write(SMSTPCR2,	0x000E2FDCU);
	/* System Module Stop Control Register 3 */
	cpg_write(SMSTPCR3,	0xFFFFFBDFU);
	/* System Module Stop Control Register 4 */
	cpg_write(SMSTPCR4,	0x80000004U);
	/* System Module Stop Control Register 5 */
	cpg_write(SMSTPCR5,	0xC3FFFFFFU);
	/* System Module Stop Control Register 6 */
	cpg_write(SMSTPCR6,	0xFFFFFFFFU);
	/* System Module Stop Control Register 7 */
	cpg_write(SMSTPCR7,	0xFFFFFFFFU);
	/* System Module Stop Control Register 8 */
	cpg_write(SMSTPCR8,	0x00F1FFF7U);
	/* System Module Stop Control Register 9 */
	cpg_write(SMSTPCR9,	0xFFFFFFDFU);
	/* System Module Stop Control Register 10 */
	cpg_write(SMSTPCR10,	0xFFFFFFE8U);
	/* System Module Stop Control Register 11 */
	cpg_write(SMSTPCR11,	0x000000B7U);
}
#endif /* RCAR_LSI == RCAR_E3 */

#if (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_V3M)
static void bl2_realtime_cpg_init_v3m(void)
{
	/* CPG (REALTIME) registers */

	/* Realtime Module Stop Control Register 0 */
	cpg_write(RMSTPCR0,	0x00230000U);
	/* Realtime Module Stop Control Register 1 */
	cpg_write(RMSTPCR1,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 2 */
	cpg_write(RMSTPCR2,	0x14062FD8U);
	/* Realtime Module Stop Control Register 3 */
	cpg_write(RMSTPCR3,	0xFFFFFFDFU);
	/* Realtime Module Stop Control Register 4 */
	cpg_write(RMSTPCR4,	0x80000184U);
	/* Realtime Module Stop Control Register 5 */
	cpg_write(RMSTPCR5,	0x83FFFFFFU);
	/* Realtime Module Stop Control Register 6 */
	cpg_write(RMSTPCR6,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 7 */
	cpg_write(RMSTPCR7,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 8 */
	cpg_write(RMSTPCR8,	0x7FF3FFF4U);
	/* Realtime Module Stop Control Register 9 */
	cpg_write(RMSTPCR9,	0xFFFFFFFEU);
}

static void bl2_system_cpg_init_v3m(void)
{
	/* CPG (SYSTEM) registers */

	/* System Module Stop Control Register 0 */
	cpg_write(SMSTPCR0,	0x00210000U);
	/* System Module Stop Control Register 1 */
	cpg_write(SMSTPCR1,	0xFFFFFFFFU);
	/* System Module Stop Control Register 2 */
	cpg_write(SMSTPCR2,	0x340E2FDCU);
	/* System Module Stop Control Register 3 */
	cpg_write(SMSTPCR3,	0xFFFFFBDFU);
	/* System Module Stop Control Register 4 */
	cpg_write(SMSTPCR4,	0x80000004U);
	/* System Module Stop Control Register 5 */
	cpg_write(SMSTPCR5,	0xC3FFFFFFU);
	/* System Module Stop Control Register 6 */
	cpg_write(SMSTPCR6,	0xFFFFFFFFU);
	/* System Module Stop Control Register 7 */
	cpg_write(SMSTPCR7,	0xFFFFFFFFU);
	/* System Module Stop Control Register 8 */
	cpg_write(SMSTPCR8,	0x01F1FFF5U);
	/* System Module Stop Control Register 9 */
	cpg_write(SMSTPCR9,	0xFFFFFFFEU);
}
#endif /* (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_V3M) */

#if (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_V3H)
static void bl2_realtime_cpg_init_v3h(void)
{
	/* CPG (REALTIME) registers */

	/* Realtime Module Stop Control Register 0 */
	cpg_write(RMSTPCR0,	0x00230000U);
	/* Realtime Module Stop Control Register 1 */
	cpg_write(RMSTPCR1,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 2 */
	cpg_write(RMSTPCR2,	0x14062FD8U);
	/* Realtime Module Stop Control Register 3 */
	cpg_write(RMSTPCR3,	0xFFFFFFDFU);
	/* Realtime Module Stop Control Register 4 */
	cpg_write(RMSTPCR4,	0x80000184U);
	/* Realtime Module Stop Control Register 5 */
	cpg_write(RMSTPCR5,	0x83FFFFFFU);
	/* Realtime Module Stop Control Register 6 */
	cpg_write(RMSTPCR6,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 7 */
	cpg_write(RMSTPCR7,	0xFFFFFFFFU);
	/* Realtime Module Stop Control Register 8 */
	cpg_write(RMSTPCR8,	0x7FF3FFF4U);
	/* Realtime Module Stop Control Register 9 */
	cpg_write(RMSTPCR9,	0xFFFFFFFEU);
}

static void bl2_system_cpg_init_v3h(void)
{
	/* CPG (SYSTEM) registers */

	/* System Module Stop Control Register 0 */
	cpg_write(SMSTPCR0,	0x00210000U);
	/* System Module Stop Control Register 1 */
	cpg_write(SMSTPCR1,	0xFFFFFFFFU);
	/* System Module Stop Control Register 2 */
	cpg_write(SMSTPCR2,	0x340E2FDCU);
	/* System Module Stop Control Register 3 */
	cpg_write(SMSTPCR3,	0xFFFFFBDFU);
	/* System Module Stop Control Register 4 */
	cpg_write(SMSTPCR4,	0x80000004U);
	/* System Module Stop Control Register 5 */
	cpg_write(SMSTPCR5,	0xC3FFFFFFU);
	/* System Module Stop Control Register 6 */
	cpg_write(SMSTPCR6,	0xFFFFFFFFU);
	/* System Module Stop Control Register 7 */
	cpg_write(SMSTPCR7,	0xFFFFFFFFU);
	/* System Module Stop Control Register 8 */
	cpg_write(SMSTPCR8,	0x01F1FFF5U);
	/* System Module Stop Control Register 9 */
	cpg_write(SMSTPCR9,	0xFFFFFFFEU);
}
#endif /* (RCAR_LSI == RCAR_AUTO) || (RCAR_LSI == RCAR_V3H) */

void bl2_cpg_init(void)
{
	uint32_t	modemr;
#if RCAR_LSI == RCAR_AUTO
	uint32_t	prr;
#endif /* RCAR_LSI == RCAR_AUTO */

	/* CPG (SECURITY) registers */
	bl2_secure_cpg_init();

	modemr = mmio_read_32(RCAR_MODEMR);
	modemr &= MODEMR_BOOT_CPU_MASK;

	/* CPG (REALTIME) registers */
	if((modemr == MODEMR_BOOT_CPU_CA57) ||
	   (modemr == MODEMR_BOOT_CPU_CA53)) {
#if RCAR_LSI == RCAR_AUTO
		prr = mmio_read_32(RCAR_PRR) & RCAR_PRODUCT_MASK;
		switch (prr) {
		case RCAR_PRODUCT_H3:
			bl2_realtime_cpg_init_h3();
			break;
		case RCAR_PRODUCT_M3:
			bl2_realtime_cpg_init_m3();
			break;
		case RCAR_PRODUCT_M3N:
			bl2_realtime_cpg_init_m3n();
			break;
		case RCAR_PRODUCT_V3M:
			bl2_realtime_cpg_init_v3m();
			break;
		case RCAR_PRODUCT_V3H:
			bl2_realtime_cpg_init_v3h();
			break;
		default:
			panic();
			break;
		}
#elif (RCAR_LSI == RCAR_H3) || (RCAR_LSI == RCAR_H3N)
		bl2_realtime_cpg_init_h3();
#elif RCAR_LSI == RCAR_M3
		bl2_realtime_cpg_init_m3();
#elif RCAR_LSI == RCAR_M3N
		bl2_realtime_cpg_init_m3n();
#elif RCAR_LSI == RCAR_E3
		bl2_realtime_cpg_init_e3();
#elif RCAR_LSI == RCAR_V3M
		bl2_realtime_cpg_init_v3m();
#elif RCAR_LSI == RCAR_V3H
		bl2_realtime_cpg_init_v3h();
#else /* RCAR_LSI == RCAR_XX */
#error "Don't have CPG initialize routine(unknown)."
#endif /* RCAR_LSI == RCAR_XX */
	}
}

void bl2_system_cpg_init(void)
{
#if RCAR_LSI == RCAR_AUTO
	uint32_t	prr;

	prr = mmio_read_32(RCAR_PRR) & RCAR_PRODUCT_MASK;
	switch (prr) {
	case RCAR_PRODUCT_H3:
		bl2_system_cpg_init_h3();
		break;
	case RCAR_PRODUCT_M3:
		bl2_system_cpg_init_m3();
		break;
	case RCAR_PRODUCT_M3N:
		bl2_system_cpg_init_m3n();
		break;
	case RCAR_PRODUCT_V3M:
		bl2_system_cpg_init_v3m();
		break;
	case RCAR_PRODUCT_V3H:
		bl2_system_cpg_init_v3h();
		break;
	default:
		panic();
		break;
	}
#elif (RCAR_LSI == RCAR_H3) || (RCAR_LSI == RCAR_H3N)
	bl2_system_cpg_init_h3();
#elif RCAR_LSI == RCAR_M3
	bl2_system_cpg_init_m3();
#elif RCAR_LSI == RCAR_M3N
	bl2_system_cpg_init_m3n();
#elif RCAR_LSI == RCAR_E3
	bl2_system_cpg_init_e3();
#elif RCAR_LSI == RCAR_V3M
	bl2_system_cpg_init_v3m();
#elif RCAR_LSI == RCAR_V3H
	bl2_system_cpg_init_v3h();
#else /* RCAR_LSI == RCAR_XX */
#error "Don't have CPG initialize routine(unknown)."
#endif /* RCAR_LSI == RCAR_XX */
}
