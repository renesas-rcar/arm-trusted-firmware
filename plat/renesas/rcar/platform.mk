#
# Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
# Copyright (c) 2015-2019, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#


# Process flags

PLAT_INCLUDES		:=	-Iinclude/common/tbbr				\
				-Iplat/renesas/rcar/drivers/iic_dvfs/		\
				-Iplat/renesas/rcar/drivers/board/		\
				-Iplat/renesas/rcar/drivers/memdrv/		\
				-Iplat/renesas/rcar/drivers/cpld/		\
				-Iplat/renesas/rcar/include			\
				-Iplat/renesas/rcar				\
				-Iplat/renesas/rcar/ddr				\
				-Iplat/renesas/rcar/qos

include lib/xlat_tables_v2/xlat_tables.mk

PLAT_BL_COMMON_SOURCES	:=	plat/renesas/rcar/drivers/iic_dvfs/iic_dvfs.c	\
				${XLAT_TABLES_LIB_SRCS}

RCAR_GIC_SOURCES	:=	drivers/arm/gic/common/gic_common.c		\
				drivers/arm/gic/v2/gicv2_main.c			\
				drivers/arm/gic/v2/gicv2_helpers.c		\
				plat/common/plat_gicv2.c

BL2_SOURCES		+=	${RCAR_GIC_SOURCES}				\
				plat/renesas/rcar/drivers/timer/bl2_swdt.c	\
				plat/renesas/rcar/drivers/error/bl2_int_error.c	\
				plat/renesas/rcar/aarch64/rcar_helpers.S	\
				plat/renesas/rcar/bl2_rcar_setup.c		\
				plat/renesas/rcar/aarch64/rcar_common.c		\
				plat/renesas/rcar/aarch64/rcar_drivers_common.c	\
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
				plat/renesas/rcar/drivers/wait/micro_wait.c	\
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
				lib/cpus/aarch64/cortex_a53.S			\
				lib/cpus/aarch64/cortex_a57.S

BL31_SOURCES		+=	${RCAR_GIC_SOURCES}				\
				drivers/arm/cci/cci.c				\
				lib/cpus/aarch64/aem_generic.S			\
				lib/cpus/aarch64/cortex_a53.S			\
				lib/cpus/aarch64/cortex_a57.S			\
				plat/renesas/rcar/bl31_rcar_setup.c		\
				plat/renesas/rcar/rcar_pm.c			\
				plat/renesas/rcar/rcar_sip_svc.c		\
				plat/renesas/rcar/drivers/board/board.c		\
				plat/renesas/rcar/drivers/dramconf/dramconf.c	\
				plat/renesas/rcar/drivers/memdrv/rcar_console.S	\
				plat/renesas/rcar/drivers/memdrv/rcar_printf.c	\
				plat/renesas/rcar/rcar_topology.c		\
				plat/renesas/rcar/aarch64/rcar_helpers.S	\
				plat/renesas/rcar/aarch64/rcar_common.c		\
				plat/renesas/rcar/aarch64/rcar_drivers_common.c	\
				plat/renesas/rcar/drivers/pwrc/rcar_call_sram.S	\
				plat/renesas/rcar/drivers/pwrc/rcar_pwrc.c	\
				plat/renesas/rcar/drivers/cpld/ulcb_cpld.c	\
				plat/renesas/rcar/drivers/wait/micro_wait.c

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
RCAR_M3N:=2
RCAR_E3:=3
RCAR_H3N:=4
RCAR_AUTO:=99
$(eval $(call add_define,RCAR_H3))
$(eval $(call add_define,RCAR_M3))
$(eval $(call add_define,RCAR_M3N))
$(eval $(call add_define,RCAR_E3))
$(eval $(call add_define,RCAR_H3N))
$(eval $(call add_define,RCAR_AUTO))
RCAR_CUT_10:=0
RCAR_CUT_11:=1
RCAR_CUT_13:=3
RCAR_CUT_20:=10
RCAR_CUT_30:=20
$(eval $(call add_define,RCAR_CUT_10))
$(eval $(call add_define,RCAR_CUT_11))
$(eval $(call add_define,RCAR_CUT_13))
$(eval $(call add_define,RCAR_CUT_20))
$(eval $(call add_define,RCAR_CUT_30))

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
      else ifeq (${LSI_CUT},11)
        RCAR_LSI_CUT:=1
      else ifeq (${LSI_CUT},20)
        RCAR_LSI_CUT:=10
      else ifeq (${LSI_CUT},30)
        RCAR_LSI_CUT:=20
      else
        $(error "Error: ${LSI_CUT} is not supported.")
      endif
      $(eval $(call add_define,RCAR_LSI_CUT))
    endif
  else ifeq (${LSI},H3N)
    RCAR_LSI:=${RCAR_H3N}
    ifndef LSI_CUT
      # enable compatible function.
      RCAR_LSI_CUT_COMPAT := 1
      $(eval $(call add_define,RCAR_LSI_CUT_COMPAT))
    else
      # disable compatible function.
      ifeq (${LSI_CUT},30)
        RCAR_LSI_CUT:=20
      else
        $(error "Error: ${LSI_CUT} is not supported.")
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
      else ifeq (${LSI_CUT},11)
        RCAR_LSI_CUT:=1
      else ifeq (${LSI_CUT},13)
        RCAR_LSI_CUT:=3
      else ifeq (${LSI_CUT},30)
        RCAR_LSI_CUT:=20
      else
        $(error "Error: ${LSI_CUT} is not supported.")
      endif
      $(eval $(call add_define,RCAR_LSI_CUT))
    endif
  else ifeq (${LSI},M3N)
    RCAR_LSI:=${RCAR_M3N}
    ifndef LSI_CUT
      # enable compatible function.
      RCAR_LSI_CUT_COMPAT := 1
      $(eval $(call add_define,RCAR_LSI_CUT_COMPAT))
    else
      # disable compatible function.
      ifeq (${LSI_CUT},10)
        RCAR_LSI_CUT:=0
      else ifeq (${LSI_CUT},11)
        RCAR_LSI_CUT:=1
      else
        $(error "Error: ${LSI_CUT} is not supported.")
      endif
      $(eval $(call add_define,RCAR_LSI_CUT))
    endif
  else ifeq (${LSI},E3)
    RCAR_LSI:=${RCAR_E3}
    ifndef LSI_CUT
      # enable compatible function.
      RCAR_LSI_CUT_COMPAT := 1
      $(eval $(call add_define,RCAR_LSI_CUT_COMPAT))
    else
      # disable compatible function.
      ifeq (${LSI_CUT},10)
        RCAR_LSI_CUT:=0
      else ifeq (${LSI_CUT},11)
        RCAR_LSI_CUT:=1
      else
        $(error "Error: ${LSI_CUT} is not supported.")
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

# Process PMIC_ROHM_BD9571 flag
ifndef PMIC_ROHM_BD9571
PMIC_ROHM_BD9571 := 1
endif
$(eval $(call add_define,PMIC_ROHM_BD9571))

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
 BOARD_DEFAULT := 0x10
 $(eval $(call add_define,BOARD_DEFAULT))
endif
$(eval $(call add_define,RCAR_GEN3_ULCB))

# Process RCAR_REF_INT flag
ifndef RCAR_REF_INT
RCAR_REF_INT :=0
endif
$(eval $(call add_define,RCAR_REF_INT))

# Process RCAR_REWT_TRAINING flag
ifndef RCAR_REWT_TRAINING
RCAR_REWT_TRAINING := 1
endif
$(eval $(call add_define,RCAR_REWT_TRAINING))

# Process RCAR_SYSTEM_SUSPEND flag
ifndef RCAR_SYSTEM_SUSPEND
RCAR_SYSTEM_SUSPEND := 1
endif
$(eval $(call add_define,RCAR_SYSTEM_SUSPEND))

# SYSTEM_SUSPEND requires power control of PMIC etc.
# When executing SYSTEM_SUSPEND other than Salvator-X, Salvator-XS and Ebisu,
# processing equivalent to that implemented in PMIC_ROHM_BD9571 is necessary.
ifeq (${RCAR_SYSTEM_SUSPEND},1)
  ifeq (${PMIC_ROHM_BD9571},0)
    $(error "Error: When you want RCAR_SYSTEM_SUSPEND to be enable, please also set PMIC_ROHM_BD9571 to enable.")
  endif
endif

# Process RCAR_DRAM_LPDDR4_MEMCONF flag
ifndef RCAR_DRAM_LPDDR4_MEMCONF
RCAR_DRAM_LPDDR4_MEMCONF :=1
endif
$(eval $(call add_define,RCAR_DRAM_LPDDR4_MEMCONF))

# Process RCAR_DRAM_DDR3L_MEMCONF flag
ifndef RCAR_DRAM_DDR3L_MEMCONF
RCAR_DRAM_DDR3L_MEMCONF :=1
endif
$(eval $(call add_define,RCAR_DRAM_DDR3L_MEMCONF))

# Process RCAR_DRAM_DDR3L_MEMDUAL flag
ifndef RCAR_DRAM_DDR3L_MEMDUAL
RCAR_DRAM_DDR3L_MEMDUAL :=1
endif
$(eval $(call add_define,RCAR_DRAM_DDR3L_MEMDUAL))

# Process RCAR_BL33_ARG0 flag
ifdef RCAR_BL33_ARG0
$(eval $(call add_define,RCAR_BL33_ARG0))
endif

#Process RCAR_BL2_DCACHE flag
ifndef RCAR_BL2_DCACHE
RCAR_BL2_DCACHE := 0
endif
$(eval $(call add_define,RCAR_BL2_DCACHE))

# Process RCAR_DRAM_CHANNEL flag
ifndef RCAR_DRAM_CHANNEL
RCAR_DRAM_CHANNEL :=15
endif
$(eval $(call add_define,RCAR_DRAM_CHANNEL))

#Process RCAR_SYSTEM_RESET_KEEPON_DDR flag
ifndef RCAR_SYSTEM_RESET_KEEPON_DDR
RCAR_SYSTEM_RESET_KEEPON_DDR := 0
endif
$(eval $(call add_define,RCAR_SYSTEM_RESET_KEEPON_DDR))

# RCAR_SYSTEM_RESET_KEEPON_DDR requires power control of PMIC etc.
# When executing SYSTEM_SUSPEND other than Salvator-X, Salvator-XS and Ebisu,
# processing equivalent to that implemented in PMIC_ROHM_BD9571 is necessary.
# Also, it is necessary to enable RCAR_SYSTEM_SUSPEND.
ifeq (${RCAR_SYSTEM_RESET_KEEPON_DDR},1)
  ifeq (${PMIC_ROHM_BD9571},0)
    $(error "Error: When you want RCAR_SYSTEM_RESET_KEEPON_DDR to be enable, please also set PMIC_ROHM_BD9571 to enable.")
  endif
  ifeq (${RCAR_SYSTEM_SUSPEND},0)
    $(error "Error: When you want RCAR_SYSTEM_RESET_KEEPON_DDR to be enable, please also set RCAR_SYSTEM_SUSPEND to enable.")
  endif
endif

# Enable dynamic memory mapping
PLAT_XLAT_TABLES_DYNAMIC := 1
$(eval $(call add_define,PLAT_XLAT_TABLES_DYNAMIC))

# Process RCAR_SMC_GET_DRAMCONF flag
ifndef RCAR_SMC_GET_DRAMCONF
RCAR_SMC_GET_DRAMCONF := 1
endif
$(eval $(call add_define,RCAR_SMC_GET_DRAMCONF))

# Enable workarounds for selected Cortex-A53 erratas.
ERRATA_A53_835769  := 1
ERRATA_A53_843419  := 1
ERRATA_A53_855873  := 1

# Enable workarounds for selected Cortex-A57 erratas.
ERRATA_A57_859972  := 1

# Do not enable SVE
ENABLE_SVE_FOR_NS  := 0

# Non-TF Boot ROM
#if IMAGE_BL2
BL2_AT_EL3	:=	1
PROGRAMMABLE_RESET_ADDRESS	:=	1
COLD_BOOT_SINGLE_CPU		:=	1
#endif /* IMAGE_BL2 */

include plat/renesas/rcar/ddr/ddr.mk
include plat/renesas/rcar/qos/qos.mk
include plat/renesas/rcar/pfc/pfc.mk
