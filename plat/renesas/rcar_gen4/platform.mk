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


ifeq (${SPD},none)
  SPD_NONE:=1
  $(eval $(call add_define,SPD_NONE))
endif

# LSI setting common define
RCAR_S4:=9
RCAR_AUTO:=99
$(eval $(call add_define,RCAR_S4))
$(eval $(call add_define,RCAR_AUTO))


ifndef LSI
  $(error "Error: Unknown LSI. Please use LSI=<LSI name> to specify the LSI")
else
  ifeq (${LSI},AUTO)
    RCAR_LSI:=${RCAR_AUTO}
  else ifeq (${LSI},S4)
    RCAR_LSI:=${RCAR_S4}
  else
    $(error "Error: ${LSI} is not supported.")
  endif
  $(eval $(call add_define,RCAR_LSI))
endif


# Enable workarounds for selected Cortex-A55 errata.
ERRATA_A55_1530923 := 1

USE_COHERENT_MEM := 0
HW_ASSISTED_COHERENCY := 1


PLAT_INCLUDES	:=	-Iplat/renesas/rcar_gen4/include		\
			-Iplat/renesas/rcar_gen4			\
			-Idrivers/renesas/rcar_gen4/pwrc

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

BL31_SOURCES	+=	${RCAR_GIC_SOURCES}				\
			lib/cpus/aarch64/cortex_a55.S			\
			plat/common/plat_psci_common.c			\
			plat/renesas/rcar_gen4/plat_topology.c		\
			plat/renesas/rcar_gen4/aarch64/plat_helpers.S	\
			plat/renesas/rcar_gen4/aarch64/platform_common.c \
			plat/renesas/rcar_gen4/bl31_plat_setup.c	\
			plat/renesas/rcar_gen4/plat_pm.c		\
			plat/renesas/rcar_gen4/rcar_common.c		\
			drivers/renesas/rcar_gen4/pwrc/pwrc.c		\
			drivers/renesas/rcar_gen4/scif/scif.S		\
			drivers/arm/cci/cci.c

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

