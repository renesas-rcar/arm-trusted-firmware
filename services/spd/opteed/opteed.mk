#
# Copyright (c) 2013-2014, ARM Limited and Contributors. All rights reserved.
# Copyright (c) 2015-2016, Renesas Electronics Corporation. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

OPTEED_DIR		:=	services/spd/opteed
SPD_INCLUDES		:=

SPD_SOURCES		:=	services/spd/opteed/opteed_common.c	\
				services/spd/opteed/opteed_helpers.S	\
				services/spd/opteed/opteed_main.c	\
				services/spd/opteed/opteed_pm.c

NEED_BL32		:=	no
