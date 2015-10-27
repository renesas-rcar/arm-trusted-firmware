#
# Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
# Copyright (c) 2015-2016, Renesas Electronics Corporation. All rights reserved.
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

PLAT_INCLUDES		:=	-Iplat/renesas/rcar/include/

PLAT_BL_COMMON_SOURCES	:=	lib/aarch64/xlat_tables.c			\
				plat/common/aarch64/plat_common.c

BL2_SOURCES		+=	plat/common/aarch64/platform_up_stack.S		\
				plat/renesas/rcar/bl2_rcar_setup.c		\
				plat/renesas/rcar/aarch64/rcar_common.c	\
				plat/renesas/rcar/drivers/io/io_rcar.c	\
				plat/renesas/rcar/drivers/io/io_memdrv.c	\
				plat/renesas/rcar/drivers/scif/scif.S		\
				plat/renesas/rcar/rcar_io_storage.c		\
				drivers/io/io_storage.c				\
				plat/renesas/rcar/bl2_pfc_init.c		\
				plat/renesas/rcar/ddr/boot_init_dram.c		\
				plat/renesas/rcar/bl2_phy_init.c		\
				plat/renesas/rcar/drivers/rpc/rpc_driver.c	\
				plat/renesas/rcar/drivers/dma/dma_driver.c	\
				plat/renesas/rcar/bl2_secure_setting.c		\
				plat/renesas/rcar/bl2_cpg_init.c		\
				plat/renesas/rcar/bl2_qos_init.c		\
				plat/renesas/rcar/aarch64/bl2_reset.S
#				plat/renesas/rcar/ddr/boot_init_dram.c		\
#				plat/renesas/rcar/bl2_sdram_init.c		\
#				plat/renesas/rcar/bl2_ddr3_init.S		\

BL31_SOURCES		+=	drivers/arm/cci/cci.c				\
				drivers/arm/gic/arm_gic.c			\
				drivers/arm/gic/gic_v2.c			\
				drivers/arm/gic/gic_v3.c			\
				lib/cpus/aarch64/aem_generic.S			\
				lib/cpus/aarch64/cortex_a53.S			\
				lib/cpus/aarch64/cortex_a57.S			\
				plat/common/plat_gic.c				\
				plat/common/aarch64/platform_mp_stack.S		\
				plat/renesas/rcar/bl31_rcar_setup.c		\
				plat/renesas/rcar/rcar_pm.c			\
				plat/renesas/rcar/drivers/memdrv/rcar_console.S	\
				plat/renesas/rcar/drivers/memdrv/rcar_printf.c	\
				plat/renesas/rcar/rcar_topology.c		\
				plat/renesas/rcar/aarch64/rcar_helpers.S	\
				plat/renesas/rcar/aarch64/rcar_common.c		\
				plat/renesas/rcar/drivers/pwrc/rcar_pwrc.c

# compile option setting
SPD			:= opteed
ARM_CCI_PRODUCT_ID	:= 500
TRUSTED_BOARD_BOOT	:= 1
AUTH_MOD		:= rcar

# Process RCAR_SECURE_BOOT flag
ifndef RCAR_SECURE_BOOT
RCAR_SECURE_BOOT := 0
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

# Process MASTER_BOOT_CPU flag
ifndef MASTER_BOOT_CPU
MASTER_BOOT_CPU := 0
endif
$(eval $(call add_define,MASTER_BOOT_CPU))

# Process RCAR_BL33_EXECUTION_EL flag
ifndef RCAR_BL33_EXECUTION_EL
RCAR_BL33_EXECUTION_EL := 0
endif
$(eval $(call add_define,RCAR_BL33_EXECUTION_EL))
