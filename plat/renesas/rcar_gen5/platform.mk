#
# Copyright (c) 2025, Renesas Electronics Corporation. All rights reserved.
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
INIT_UNUSED_NS_EL2		:= 1

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

# Process VDK_ENV flag
# 0:Disable(default), 1:Enable
ifndef VDK_ENV
VDK_ENV := 0
endif
$(eval $(call add_define,VDK_ENV=$(VDK_ENV)))



ifeq (${SPD},none)
  SPD_NONE:=1
  $(eval $(call add_define,SPD_NONE))
endif

# LSI setting common define
RCAR_X5H:=10
RCAR_AUTO:=99
$(eval $(call add_define,RCAR_X5H))
$(eval $(call add_define,RCAR_AUTO))
$(eval $(call add_define,PLAT_EXTRA_LD_SCRIPT))


ifndef LSI
  $(error "Error: Unknown LSI. Please use LSI=<LSI name> to specify the LSI")
else
  ifeq (${LSI},AUTO)
    RCAR_LSI:=${RCAR_AUTO}
  else ifeq (${LSI},X5H)
    RCAR_LSI:=${RCAR_X5H}
  else
    $(error "Error: ${LSI} is not supported.")
  endif
  $(eval $(call add_define,RCAR_LSI))
endif

# Disable workarounds unnecessary for Cortex-A720AE
WORKAROUND_CVE_2017_5715	:= 0
WORKAROUND_CVE_2022_23960	:= 0



USE_COHERENT_MEM := 0
HW_ASSISTED_COHERENCY := 1


PLAT_INCLUDES	:=	-Iplat/renesas/rcar_gen5/include		\
			-Iplat/renesas/rcar_gen5			\
			-Idrivers/renesas/rcar_gen5/pwrc		\
			-Idrivers/renesas/rcar_gen5/scif

ifeq ($(VDK_ENV), 1)
    PLAT_INCLUDES += -Idrivers/renesas/rcar_gen5/tauj		\
		     -Idrivers/renesas/rcar_gen5/scif/vdk
endif


ifneq (${ENABLE_STACK_PROTECTOR},0)
BL_COMMON_SOURCES	+=	plat/renesas/rcar_gen5/rcar_stack_protector.c
endif

# R-Car Gen5 platform uses Arm GIC-Fainlight-AE,
# which is successor of GIC-700 based on GICv4.1
GIC_ENABLE_V4_EXTN	:=	1
GIC_EXT_INTID		:=	1

# GIC-600 configuration
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
			lib/cpus/aarch64/cortex_a720_ae.S			\
			plat/common/plat_psci_common.c			\
			plat/renesas/rcar_gen5/plat_topology.c		\
			plat/renesas/rcar_gen5/aarch64/plat_helpers.S	\
			plat/renesas/rcar_gen5/aarch64/platform_common.c \
			plat/renesas/rcar_gen5/bl31_plat_setup.c	\
			plat/renesas/rcar_gen5/plat_pm.c		\
			plat/renesas/rcar_gen5/plat_pm_scmi.c		\
			plat/renesas/rcar_gen5/rcar_common.c		\
			drivers/delay_timer/delay_timer.c		\
			drivers/delay_timer/generic_delay_timer.c	\
			drivers/renesas/rcar_gen5/pwrc/call_sram.S	\
			drivers/renesas/rcar_gen5/pwrc/pwrc.c		\
			drivers/renesas/rcar_gen5/tauj/tauj.c		\
			drivers/renesas/rcar_gen5/scif/scif.c		\
			drivers/renesas/rcar_gen5/scif/vdk/uart.c	\
			drivers/renesas/rcar_gen5/scif/scif_helpers.S	\
			${SCMI_DRIVER_SOURES}				\
			drivers/arm/cci/cci.c


ifeq ($(VDK_ENV), 1)
    BL31_SOURCES += drivers/renesas/rcar_gen5/tauj/tauj.c		\
		    drivers/renesas/rcar_gen5/scif/vdk/uart.c
endif

include lib/xlat_tables_v2/xlat_tables.mk
ifneq (${MBEDTLS_COMMON_MK}, 1)
include drivers/auth/mbedtls/mbedtls_crypto.mk
endif
PLAT_BL_COMMON_SOURCES	+=	${XLAT_TABLES_LIB_SRCS}

# Process RCAR_BL33_EXECUTION_EL flag
ifndef RCAR_BL33_EXECUTION_EL
RCAR_BL33_EXECUTION_EL := 0
endif
$(eval $(call add_define,RCAR_BL33_EXECUTION_EL))

# Process RCAR_BL33_ARG0 flag
ifdef RCAR_BL33_ARG0
$(eval $(call add_define,RCAR_BL33_ARG0))
endif

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
	$(Q)$($(ARCH)-oc) -O srec --srec-forceS3 ${BL31_ELF_SRC} ${SREC_PATH}/bl31.srec

