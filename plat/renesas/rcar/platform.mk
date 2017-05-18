#
# Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
# Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# Neither the name of ARM nor the names of its contributors may be used
# to endorse or promote products derived from this software without specific
# prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#


# Process flags

PLAT_INCLUDES		:=	-Iinclude/common/tbbr				\
				-Iinclude/plat/arm/common			\
				-Iplat/renesas/rcar/drivers/iic_dvfs/		\
				-Iplat/renesas/rcar/drivers/board/		\
				-Iplat/renesas/rcar/include			\
				-Iplat/renesas/rcar

PLAT_BL_COMMON_SOURCES	:=	lib/xlat_tables/xlat_tables_common.c		\
				lib/xlat_tables/aarch64/xlat_tables.c		\
				plat/common/aarch64/plat_common.c

RCAR_GIC_SOURCES	:=	drivers/arm/gic/common/gic_common.c	\
				drivers/arm/gic/v2/gicv2_main.c		\
				drivers/arm/gic/v2/gicv2_helpers.c	\
				plat/common/plat_gicv2.c

BL2_SOURCES		+=	${RCAR_GIC_SOURCES}				\
				plat/common/${ARCH}/platform_up_stack.S		\
				plat/renesas/rcar/drivers/timer/bl2_swdt.c	\
				plat/renesas/rcar/drivers/error/bl2_int_error.c	\
				plat/renesas/rcar/aarch64/rcar_helpers.S	\
				plat/renesas/rcar/bl2_rcar_setup.c		\
				plat/renesas/rcar/aarch64/rcar_common.c		\
				plat/renesas/rcar/drivers/io/io_rcar.c		\
				plat/renesas/rcar/drivers/io/io_memdrv.c	\
				plat/renesas/rcar/drivers/io/io_emmcdrv.c	\
				plat/renesas/rcar/drivers/scif/scif.S		\
				plat/renesas/rcar/drivers/memdrv/rcar_printf.c	\
				plat/renesas/rcar/drivers/auth/rcarboot.c	\
				plat/renesas/rcar/rcar_io_storage.c		\
				drivers/io/io_storage.c				\
				plat/renesas/rcar/drivers/rpc/rpc_driver.c	\
				plat/renesas/rcar/drivers/dma/dma_driver.c	\
				plat/renesas/rcar/drivers/avs/avs_driver.c	\
				plat/renesas/rcar/drivers/wait/micro_wait.S	\
				plat/renesas/rcar/drivers/emmc/emmc_utility.c	\
				plat/renesas/rcar/drivers/emmc/emmc_interrupt.c	\
				plat/renesas/rcar/drivers/emmc/emmc_cmd.c	\
				plat/renesas/rcar/drivers/emmc/emmc_init.c	\
				plat/renesas/rcar/drivers/emmc/emmc_mount.c	\
				plat/renesas/rcar/drivers/emmc/emmc_read.c	\
				plat/renesas/rcar/drivers/rom/rom_api.c		\
				plat/renesas/rcar/drivers/board/board.c		\
				plat/renesas/rcar/bl2_secure_setting.c		\
				plat/renesas/rcar/bl2_cpg_init.c		\
				plat/renesas/rcar/aarch64/bl2_reset.S

BL31_SOURCES		+=	${RCAR_GIC_SOURCES}				\
				drivers/arm/cci/cci.c				\
				lib/cpus/aarch64/aem_generic.S			\
				lib/cpus/aarch64/cortex_a53.S			\
				lib/cpus/aarch64/cortex_a57.S			\
				plat/common/aarch64/platform_mp_stack.S		\
				plat/renesas/rcar/bl31_rcar_setup.c		\
				plat/renesas/rcar/rcar_pm.c			\
				plat/renesas/rcar/drivers/memdrv/rcar_console.S	\
				plat/renesas/rcar/drivers/memdrv/rcar_printf.c	\
				plat/renesas/rcar/rcar_topology.c		\
				plat/renesas/rcar/aarch64/rcar_helpers.S	\
				plat/renesas/rcar/aarch64/rcar_common.c		\
				plat/renesas/rcar/drivers/pwrc/rcar_call_sram.S	\
				plat/renesas/rcar/drivers/pwrc/rcar_pwrc.c	\
				plat/renesas/rcar/drivers/cpld/ulcb_cpld.c

# compile option setting
ARM_CCI_PRODUCT_ID	:= 500
TRUSTED_BOARD_BOOT	:= 1
ERROR_DEPRECATED	:= 1

ifeq (${SPD},none)
  SPD_NONE:=1
  $(eval $(call add_define,SPD_NONE))
endif

# LSI setting common define
RCAR_H3:=0
RCAR_M3:=1
RCAR_AUTO:=99
$(eval $(call add_define,RCAR_H3))
$(eval $(call add_define,RCAR_M3))
$(eval $(call add_define,RCAR_AUTO))
RCAR_CUT_10:=0
RCAR_CUT_11:=1
RCAR_CUT_20:=10
$(eval $(call add_define,RCAR_CUT_10))
$(eval $(call add_define,RCAR_CUT_11))
$(eval $(call add_define,RCAR_CUT_20))

ifndef LSI
  $(error "Error: Unknown LSI. Please use LSI=<LSI name> to specify the LSI")
else
  ifeq (${LSI},AUTO)
    RCAR_LSI:=${RCAR_AUTO}
  else ifeq (${LSI},H3)
    RCAR_LSI:=${RCAR_H3}
    ifndef LSI_CUT
      # enable compatible function.
      RCAR_LSI_CUT_COMPAT := 1
      $(eval $(call add_define,RCAR_LSI_CUT_COMPAT))
    else
      # disable compatible function.
      ifeq (${LSI_CUT},10)
        RCAR_LSI_CUT:=0
      endif
      ifeq (${LSI_CUT},11)
        RCAR_LSI_CUT:=1
      endif
      ifeq (${LSI_CUT},20)
        RCAR_LSI_CUT:=10
      endif
      $(eval $(call add_define,RCAR_LSI_CUT))
    endif
  else ifeq (${LSI},M3)
    RCAR_LSI:=${RCAR_M3}
    ifndef LSI_CUT
      # enable compatible function.
      RCAR_LSI_CUT_COMPAT := 1
      $(eval $(call add_define,RCAR_LSI_CUT_COMPAT))
    else
      # disable compatible function.
      ifeq (${LSI_CUT},10)
        RCAR_LSI_CUT:=0
      endif
      ifeq (${LSI_CUT},11)
        RCAR_LSI_CUT:=1
      endif
      $(eval $(call add_define,RCAR_LSI_CUT))
    endif
  else
    $(error "Error: ${LSI} is not supported.")
  endif
  $(eval $(call add_define,RCAR_LSI))
endif

# Process RCAR_SECURE_BOOT flag
ifndef RCAR_SECURE_BOOT
RCAR_SECURE_BOOT := 1
endif
$(eval $(call add_define,RCAR_SECURE_BOOT))

# Process RCAR_QOS_TYPE flag
ifndef RCAR_QOS_TYPE
RCAR_QOS_TYPE := 0
endif
$(eval $(call add_define,RCAR_QOS_TYPE))

# Process RCAR_DRAM_SPLIT flag
ifndef RCAR_DRAM_SPLIT
RCAR_DRAM_SPLIT := 0
endif
$(eval $(call add_define,RCAR_DRAM_SPLIT))

# Process RCAR_BL33_EXECUTION_EL flag
ifndef RCAR_BL33_EXECUTION_EL
RCAR_BL33_EXECUTION_EL := 0
endif
$(eval $(call add_define,RCAR_BL33_EXECUTION_EL))

# Process PSCI_DISABLE_BIGLITTLE_IN_CA57BOOT flag
ifndef PSCI_DISABLE_BIGLITTLE_IN_CA57BOOT
PSCI_DISABLE_BIGLITTLE_IN_CA57BOOT := 1
endif
$(eval $(call add_define,PSCI_DISABLE_BIGLITTLE_IN_CA57BOOT))

# Process RCAR_AVS_SETTING_ENABLE flag
ifeq (${RCAR_AVS_SETTING_ENABLE},0)
AVS_SETTING_ENABLE := 0
else
AVS_SETTING_ENABLE := 1
endif
$(eval $(call add_define,AVS_SETTING_ENABLE))

# Process RCAR_LOSSY_ENABLE flag
ifndef RCAR_LOSSY_ENABLE
RCAR_LOSSY_ENABLE := 0
endif
$(eval $(call add_define,RCAR_LOSSY_ENABLE))

# Process LIFEC_DBSC_PROTECT_ENABLE flag
ifndef LIFEC_DBSC_PROTECT_ENABLE
LIFEC_DBSC_PROTECT_ENABLE := 1
endif
$(eval $(call add_define,LIFEC_DBSC_PROTECT_ENABLE))

# Process PMIC_ON_BOARD flag
ifndef PMIC_ON_BOARD
PMIC_ON_BOARD := 1
endif
ifeq (${PMIC_ON_BOARD},1)
BL2_SOURCES		+=	plat/renesas/rcar/drivers/iic_dvfs/iic_dvfs.c
BL31_SOURCES		+=	plat/renesas/rcar/drivers/iic_dvfs/iic_dvfs.c
endif
$(eval $(call add_define,PMIC_ON_BOARD))

# Process PMIC_LEVEL_MODE flag
ifndef PMIC_LEVEL_MODE
PMIC_LEVEL_MODE := 1
endif
$(eval $(call add_define,PMIC_LEVEL_MODE))

# Process RCAR_GEN3_ULCB flag
ifndef RCAR_GEN3_ULCB
RCAR_GEN3_ULCB := 0
endif
ifeq (${RCAR_GEN3_ULCB},1)
 BOARD_ID := 0x10
 $(eval $(call add_define,BOARD_ID))
endif
$(eval $(call add_define,RCAR_GEN3_ULCB))

include plat/renesas/rcar/ddr/ddr.mk
include plat/renesas/rcar/qos/qos.mk
include plat/renesas/rcar/pfc/pfc.mk
