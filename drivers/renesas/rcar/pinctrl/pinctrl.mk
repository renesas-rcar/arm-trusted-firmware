#
# Copyright (c) 2021 EPAM Systems
#
# SPDX-License-Identifier: BSD-3-Clause
#

ifeq (${RCAR_LSI},${RCAR_AUTO})
    BL31_SOURCES += drivers/renesas/rcar/pinctrl/pfc-r8a77951.c
else
  ifeq (${RCAR_LSI},${RCAR_H3})
    BL31_SOURCES += drivers/renesas/rcar/pinctrl/pfc-r8a77951.c
  endif
endif
BL31_SOURCES += drivers/renesas/rcar/pinctrl/core.c
BL31_SOURCES += drivers/renesas/rcar/pinctrl/pinctrl_init.c
