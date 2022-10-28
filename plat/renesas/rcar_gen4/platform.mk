#
# Copyright (c) 2018-2021, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

PROGRAMMABLE_RESET_ADDRESS	:= 0
COLD_BOOT_SINGLE_CPU		:= 1
ARM_CCI_PRODUCT_ID		:= 500
TRUSTED_BOARD_BOOT		:= 1
RESET_TO_BL31			:= 1
GENERATE_COT			:= 1
ENABLE_SVE_FOR_NS		:= 0
MULTI_CONSOLE_API		:= 1

CRASH_REPORTING			:= 1
HANDLE_EA_EL3_FIRST		:= 1
ENABLE_STACK_PROTECTOR	:= strong

# Process SET_SCMI_PARAM flag
# 0:Disable(default), 1:Enable
ifndef SET_SCMI_PARAM
    SET_SCMI_PARAM := 0
    $(eval $(call add_define,SET_SCMI_PARAM))
else
    ifeq (${SET_SCMI_PARAM}, 0)
        $(eval $(call add_define,SET_SCMI_PARAM))
    else ifeq (${SET_SCMI_PARAM},1)
        $(eval $(call add_define,SET_SCMI_PARAM))
    else
        $(error "Error:SET_SCMI_PARAM=${SET_SCMI_PARAM} is not supported.")
    endif
endif

ifeq (${SPD},none)
  SPD_NONE:=1
  $(eval $(call add_define,SPD_NONE))
endif

# LSI setting common define
RCAR_S4:=9
RCAR_V3U:=10
RCAR_V4H:=11
RCAR_AUTO:=99
$(eval $(call add_define,RCAR_S4))
$(eval $(call add_define,RCAR_V3U))
$(eval $(call add_define,RCAR_V4H))
$(eval $(call add_define,RCAR_AUTO))
$(eval $(call add_define,PLAT_EXTRA_LD_SCRIPT))


ifndef LSI
  $(error "Error: Unknown LSI. Please use LSI=<LSI name> to specify the LSI")
else
  ifeq (${LSI},AUTO)
    RCAR_LSI:=${RCAR_AUTO}
  else ifeq (${LSI},S4)
    RCAR_LSI:=${RCAR_S4}
  else ifeq (${LSI},V3U)
    RCAR_LSI:=${RCAR_V3U}
  else ifeq (${LSI},V4H)
    RCAR_LSI:=${RCAR_V4H}
  else
    $(error "Error: ${LSI} is not supported.")
  endif
  $(eval $(call add_define,RCAR_LSI))
endif

ifeq (${RCAR_LSI},${RCAR_S4})
# Enable workarounds for selected Cortex-A55 errata.
ERRATA_A55_1530923 := 1
BL31_SOURCES += lib/cpus/aarch64/cortex_a55.S
else
# Enable workarounds for selected Cortex-A76 errata.
DYNAMIC_WORKAROUND_CVE_2018_3639 := 1
ERRATA_A76_1073348  :=1
ERRATA_A76_1130799  :=1
ERRATA_A76_1220197  :=1
ERRATA_A76_1257314  :=1
ERRATA_A76_1262606  :=1
ERRATA_A76_1262888  :=1
ERRATA_A76_1275112  :=1
ERRATA_A76_1286807  :=1

BL31_SOURCES +=	lib/cpus/aarch64/cortex_a76.S
endif

USE_COHERENT_MEM := 0
HW_ASSISTED_COHERENCY := 1


PLAT_INCLUDES	:=	-Iplat/renesas/rcar_gen4/include		\
			-Iplat/renesas/rcar_gen4			\
			-Idrivers/renesas/rcar_gen4/pwrc		\
			-Idrivers/renesas/rcar_gen4/scif

ifneq (${ENABLE_STACK_PROTECTOR},0)
BL_COMMON_SOURCES	+=	plat/renesas/rcar_gen4/rcar_stack_protector.c
endif


# GIC-600 configuration
#GICV3_IMPL := GIC600
GICV3_SUPPORT_GIC600 := 1
# Include GICv3 driver files
include drivers/arm/gic/v3/gicv3.mk
RCAR_GIC_SOURCES	:=	${GICV3_SOURCES}	\
				plat/common/plat_gicv3.c
#

# SCMI driver configuration
SCMI_DRIVER_SOURES :=	drivers/arm/css/scmi/scmi_common.c		\
			drivers/arm/css/scmi/scmi_pwr_dmn_proto.c	\
			drivers/arm/css/scmi/scmi_sys_pwr_proto.c

PLAT_INCLUDES	+=	-Idrivers/arm/css/scmi				\
			-Iinclude/drivers
#

BL31_SOURCES	+=	${RCAR_GIC_SOURCES}				\
			plat/common/plat_psci_common.c			\
			plat/renesas/rcar_gen4/plat_topology.c		\
			plat/renesas/rcar_gen4/aarch64/plat_helpers.S	\
			plat/renesas/rcar_gen4/aarch64/platform_common.c \
			plat/renesas/rcar_gen4/bl31_plat_setup.c	\
			plat/renesas/rcar_gen4/plat_pm.c		\
			plat/renesas/rcar_gen4/rcar_common.c		\
			drivers/renesas/rcar_gen4/pwrc/call_sram.S	\
			drivers/renesas/rcar_gen4/pwrc/pwrc.c		\
			drivers/renesas/rcar_gen4/scif/scif.c		\
			drivers/renesas/rcar_gen4/scif/scif_helpers.S	\
			drivers/arm/cci/cci.c

ifeq (${SET_SCMI_PARAM},1)
BL31_SOURCES	+=	${SCMI_DRIVER_SOURES}				\
			plat/renesas/rcar_gen4/plat_pm_scmi.c
endif

include lib/xlat_tables_v2/xlat_tables.mk
ifneq (${MBEDTLS_COMMON_MK}, 1)
include drivers/auth/mbedtls/mbedtls_crypto.mk
endif
PLAT_BL_COMMON_SOURCES	+=	${XLAT_TABLES_LIB_SRCS}

# build the layout images for the bootrom and the necessary srecords
rcar: rcar_srecord
distclean realclean clean: clean_srecord

# srecords
SREC_PATH	= ${BUILD_PLAT}
BL31_ELF_SRC	= ${SREC_PATH}/bl31/bl31.elf

clean_srecord:
	@echo "clean bl31 srecs"
	rm -f ${SREC_PATH}/bl31.srec

.PHONY: rcar_srecord
rcar_srecord: $(BL31_ELF_SRC)
	@echo "generating srec: ${SREC_PATH}/bl31.srec"
	$(Q)$(OC) -O srec --srec-forceS3 ${BL31_ELF_SRC} ${SREC_PATH}/bl31.srec

