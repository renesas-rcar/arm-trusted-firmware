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
ENABLE_SVE_FOR_NS		:= 1
ENABLE_SVE_FOR_SWD		:= 1
MULTI_CONSOLE_API		:= 1
INIT_UNUSED_NS_EL2		:= 1

ENABLE_FEAT_AMU			:= 1
ENABLE_AMU_AUXILIARY_COUNTERS	:= 1

ENABLE_PAUTH			:= 1
CTX_INCLUDE_PAUTH_REGS		:= 1
# Build for Cortex-A720 instead of generic ARMv8A, this makes additional
# instructions, like 'xpaci' used by Pointer Authentication, available.
MARCH_DIRECTIVE			:= -march=armv9.2-a

ENABLE_SYS_REG_TRACE_FOR_NS	:= 1

CRASH_REPORTING			:= 1
HANDLE_EA_EL3_FIRST_NS		:= 1
ENABLE_STACK_PROTECTOR	:= strong

CTX_INCLUDE_AARCH32_REGS	:= 0

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

# Serial console (SCIF or HSCIF) output. With 0 the console is kept for the
# crash path only, which removes the UART cost from normal logging.
RCAR_CONSOLE_UART ?= 1

# LDR (Logging Data Recorder) console: record BL31 console output in DRAM.
RCAR_CONSOLE_LDR ?= 1

$(eval $(call assert_booleans,RCAR_CONSOLE_UART RCAR_CONSOLE_LDR))
$(eval $(call add_define,RCAR_CONSOLE_UART))
$(eval $(call add_define,RCAR_CONSOLE_LDR))

ifeq (${RCAR_CONSOLE_UART},0)
$(info UART console output DISABLED: serial console is registered for its rcar_mfis_init() side effect only. Crash output is unaffected.)
endif

ifeq (${RCAR_CONSOLE_LDR},1)
LDR_LIB_DIR := drivers/renesas/rcar_gen5/ldr/lib

LDR_SOURCES := drivers/renesas/rcar_gen5/ldr/ldr.c			\
	       drivers/renesas/rcar_gen5/ldr/ldr_console.c		\
	       ${LDR_LIB_DIR}/ldr_dlt_protocol/ldr_dlt_protocol.c	\
	       ${LDR_LIB_DIR}/ldr_ring_buffer/ldr_ring_buffer.c

BL31_SOURCES += ${LDR_SOURCES}
endif

# Disable workarounds unnecessary for Cortex-A720AE
WORKAROUND_CVE_2017_5715	:= 0
WORKAROUND_CVE_2022_23960	:= 0



USE_COHERENT_MEM := 0
HW_ASSISTED_COHERENCY := 1


PLAT_INCLUDES	:=	-Iplat/renesas/rcar_gen5/include		\
			-Iplat/renesas/rcar_gen5			\
			-Idrivers/renesas/rcar_gen5/pwrc		\
			-Idrivers/renesas/rcar_gen5/ldr			\
			-Idrivers/renesas/rcar_gen5/ldr/lib		\
			-Idrivers/renesas/rcar_gen5/scif		\
			-Idrivers/renesas/rcar_gen5/mfis



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
			plat/renesas/rcar_gen5/aarch64/plat_helpers_secondary_reset.S	\
			plat/renesas/rcar_gen5/aarch64/platform_common.c \
			plat/renesas/rcar_gen5/bl31_plat_setup.c	\
			plat/renesas/rcar_gen5/plat_pm.c		\
			plat/renesas/rcar_gen5/plat_pm_scmi.c		\
			plat/renesas/rcar_gen5/rcar_common.c		\
			drivers/delay_timer/delay_timer.c		\
			drivers/delay_timer/generic_delay_timer.c	\
			drivers/renesas/rcar_gen5/pwrc/call_sram.S	\
			drivers/renesas/rcar_gen5/pwrc/pwrc.c		\
			drivers/renesas/rcar_gen5/scif/scif.c		\
			drivers/renesas/rcar_gen5/scif/scif_helpers.S	\
			drivers/renesas/rcar_gen5/mfis/mfis.c	\
			${SCMI_DRIVER_SOURES}				\
			drivers/arm/cci/cci.c



include lib/xlat_tables_v2/xlat_tables.mk
ifneq (${MBEDTLS_COMMON_MK}, 1)
include drivers/auth/mbedtls/mbedtls_crypto.mk
endif
PLAT_BL_COMMON_SOURCES	+=	${XLAT_TABLES_LIB_SRCS}

# Pointer Authentication sources
ifeq (${ENABLE_PAUTH}, 1)
PLAT_BL_COMMON_SOURCES	+=	plat/arm/common/aarch64/arm_pauth.c \
		lib/extensions/pauth/pauth_helpers.S
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

# LDR region for the build log; ldr.c checks the values against rcar_def.h.
ifeq (${RCAR_CONSOLE_LDR},1)
RCAR_LDR_MK_BASE	:= 0x8C108000
RCAR_LDR_MK_SEND_SIZE	:= 0x0003F000
RCAR_LDR_MK_RECV_SIZE	:= 0x00001000
$(eval $(call add_define_val,RCAR_LDR_MK_BASE,$(RCAR_LDR_MK_BASE)))
$(eval $(call add_define_val,RCAR_LDR_MK_SEND_SIZE,$(RCAR_LDR_MK_SEND_SIZE)))
$(eval $(call add_define_val,RCAR_LDR_MK_RECV_SIZE,$(RCAR_LDR_MK_RECV_SIZE)))
RCAR_LDR_MK_RECV_BASE	:= $(shell printf '0x%08X' $$(( $(RCAR_LDR_MK_BASE) + $(RCAR_LDR_MK_SEND_SIZE) )))
RCAR_LDR_MK_END		:= $(shell printf '0x%08X' $$(( $(RCAR_LDR_MK_RECV_BASE) + $(RCAR_LDR_MK_RECV_SIZE) )))
RCAR_LDR_MK_SEND_KB	:= $(shell echo $$(( $(RCAR_LDR_MK_SEND_SIZE) / 1024 )))
RCAR_LDR_MK_RECV_KB	:= $(shell echo $$(( $(RCAR_LDR_MK_RECV_SIZE) / 1024 )))
$(info LDR enabled: BL31 send ring BASE=$(RCAR_LDR_MK_BASE) SIZE=$(RCAR_LDR_MK_SEND_SIZE) ($(RCAR_LDR_MK_SEND_KB) KB); recv reserved BASE=$(RCAR_LDR_MK_RECV_BASE) SIZE=$(RCAR_LDR_MK_RECV_SIZE) ($(RCAR_LDR_MK_RECV_KB) KB, not used by BL31); region ends $(RCAR_LDR_MK_END))
endif

# LDR_ENABLE was renamed: fail rather than silently ignore an old command line.
ifdef LDR_ENABLE
$(error LDR_ENABLE has been renamed to RCAR_CONSOLE_LDR)
endif
