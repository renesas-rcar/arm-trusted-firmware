#
# Copyright (c) 2015-2018, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

ifneq ($(filter ${RCAR_LSI},${RCAR_E3} ${RCAR_V3M}),)
include plat/renesas/rcar/ddr/ddr_a/ddr_a.mk
else
include plat/renesas/rcar/ddr/ddr_b/ddr_b.mk
endif

BL2_SOURCES += plat/renesas/rcar/ddr/dram_sub_func.c
