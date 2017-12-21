#
# Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

ifeq (${RCAR_LSI},${RCAR_AUTO})
    BL2_SOURCES += plat/renesas/rcar/pfc/H3/pfc_init_h3_v1.c
    BL2_SOURCES += plat/renesas/rcar/pfc/H3/pfc_init_h3_v2.c
    BL2_SOURCES += plat/renesas/rcar/pfc/M3/pfc_init_m3.c
    BL2_SOURCES += plat/renesas/rcar/pfc/M3N/pfc_init_m3n.c

else ifdef RCAR_LSI_CUT_COMPAT
  ifeq (${RCAR_LSI},${RCAR_H3})
    BL2_SOURCES += plat/renesas/rcar/pfc/H3/pfc_init_h3_v1.c
    BL2_SOURCES += plat/renesas/rcar/pfc/H3/pfc_init_h3_v2.c
  endif
  ifeq (${RCAR_LSI},${RCAR_M3})
    BL2_SOURCES += plat/renesas/rcar/pfc/M3/pfc_init_m3.c
  endif
  ifeq (${RCAR_LSI},${RCAR_M3N})
    BL2_SOURCES += plat/renesas/rcar/pfc/M3N/pfc_init_m3n.c
  endif
else
  ifeq (${RCAR_LSI},${RCAR_H3})
    ifeq (${LSI_CUT},10)
      BL2_SOURCES += plat/renesas/rcar/pfc/H3/pfc_init_h3_v1.c
    endif
    ifeq (${LSI_CUT},11)
      BL2_SOURCES += plat/renesas/rcar/pfc/H3/pfc_init_h3_v1.c
    endif
    ifeq (${LSI_CUT},20)
      BL2_SOURCES += plat/renesas/rcar/pfc/H3/pfc_init_h3_v2.c
    endif
  endif
  ifeq (${RCAR_LSI},${RCAR_M3})
    BL2_SOURCES += plat/renesas/rcar/pfc/M3/pfc_init_m3.c
  endif
  ifeq (${RCAR_LSI},${RCAR_M3N})
    BL2_SOURCES += plat/renesas/rcar/pfc/M3N/pfc_init_m3n.c
  endif
endif

BL2_SOURCES += plat/renesas/rcar/pfc/pfc_init.c
