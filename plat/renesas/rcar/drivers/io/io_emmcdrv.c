/*
 * Copyright (c) 2014, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2015-2017, Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <debug.h>
#include <io_driver.h>
#include <io_storage.h>
#include <string.h>
#include "io_common.h"
#include "io_private.h"
#include "io_emmcdrv.h"
#include "emmc_config.h"
#include "emmc_hal.h"
#include "emmc_std.h"
#include "emmc_def.h"

/* As we need to be able to keep state for seek, only one file can be open
 * at a time. Make this a structure and point to the entity->info. When we
 * can malloc memory we can change this to support more open files.
 */
typedef struct
{
	/* Use the 'in_use' flag as any value for base and file_pos could be
	 * valid.
	 */
	uint32_t in_use;
	uintptr_t base;
	ssize_t file_pos;
	EMMC_PARTITION_ID partition;
} file_state_t;

static file_state_t current_file = {0U};
/* Initialize to defualt partition */
static EMMC_PARTITION_ID emmcdrv_bootpartition = PARTITION_ID_USER;

/* emmcdrv device functions */
static io_type_t device_type_emmcdrv(void);
static int32_t emmcdrv_dev_open(const uintptr_t dev_spec,
				io_dev_info_t **dev_info);
static int32_t emmcdrv_block_open(io_dev_info_t *dev_info, const uintptr_t spec,
			     io_entity_t *entity);
static int32_t emmcdrv_block_seek(io_entity_t *entity, int32_t mode,
			     ssize_t offset);
static int32_t emmcdrv_block_read(io_entity_t *entity, uintptr_t buffer,
			     size_t length, size_t *length_read);
static int32_t emmcdrv_block_close(io_entity_t *entity);
static int32_t emmcdrv_dev_close(io_dev_info_t *dev_info);

static const io_dev_connector_t emmcdrv_dev_connector = {
	&emmcdrv_dev_open
};

static const io_dev_funcs_t emmcdrv_dev_funcs = {
	&device_type_emmcdrv,
	&emmcdrv_block_open,
	&emmcdrv_block_seek,
	NULL,
	&emmcdrv_block_read,
	NULL,
	&emmcdrv_block_close,
	NULL,
	&emmcdrv_dev_close
};

/* No state associated with this device so structure can be const */
static const io_dev_info_t emmcdrv_dev_info = {
	&emmcdrv_dev_funcs,
	(uintptr_t)0
};

/* Identify the device type as emmcdrv */
static io_type_t device_type_emmcdrv(void)
{
	return IO_TYPE_MEMMAP;
}

/* Open a connection to the emmcdrv device */
static int32_t emmcdrv_dev_open(
			const uintptr_t dev_spec __attribute__((unused)),
			io_dev_info_t **dev_info)
{
	*dev_info = (io_dev_info_t *)&emmcdrv_dev_info;

	return IO_SUCCESS;
}

/* Close a connection to the emmcdrv device */
static int32_t emmcdrv_dev_close(io_dev_info_t *dev_info)
{
	/* NOP */
	/* TODO: Consider tracking open files and cleaning them up here */
	return IO_SUCCESS;
}

/* Open a file on the emmcdrv device */
/* TODO: Can we do any sensible limit checks on requested memory */
static int32_t emmcdrv_block_open(io_dev_info_t *dev_info, const uintptr_t spec,
			     io_entity_t *entity)
{
	int32_t result = IO_SUCCESS;
	const io_drv_spec_t *block_spec = (io_drv_spec_t *)spec;

	/* Since we need to track open state for seek() we only allow one open
	 * spec at a time. When we have dynamic memory we can malloc and set
	 * entity->info.
	 */
	if (current_file.in_use == 0U) {

		current_file.in_use = 1U;
		/* File cursor offset for seek and incremental reads etc. */
		current_file.file_pos = 0;

		if (emmcdrv_bootpartition == PARTITION_ID_USER) {
			emmcdrv_bootpartition = mmc_drv_obj.boot_partition_en;
			/* Partition check */
			if ((PARTITION_ID_BOOT_1==emmcdrv_bootpartition) ||
			    (PARTITION_ID_BOOT_2==emmcdrv_bootpartition)) {
				current_file.partition = emmcdrv_bootpartition;
				NOTICE("BL2: eMMC boot from partition %d\n",
					emmcdrv_bootpartition);
			} else {
				/* eMMC boot partition error */
				result = IO_FAIL;
			}
		} else {
			if ((PARTITION_ID_USER
				== (EMMC_PARTITION_ID)
					(block_spec->partition))
				|| (PARTITION_ID_BOOT_1
					== (EMMC_PARTITION_ID)
						(block_spec->partition))
				|| (PARTITION_ID_BOOT_2
					== (EMMC_PARTITION_ID)
						(block_spec->partition))) {
				current_file.partition =
					(EMMC_PARTITION_ID)
						(block_spec->partition);
			} else {
				/* Set boot partition */
				current_file.partition = emmcdrv_bootpartition;
			}
		}
		if (IO_SUCCESS == result) {
			if (emmc_select_partition(current_file.partition)
					!= EMMC_SUCCESS) {
				result = IO_FAIL;
			}
			entity->info = (uintptr_t)&current_file;
		}
	} else {
		/* A emmcdrv device is already active. Close first. */
		result = IO_RESOURCES_EXHAUSTED;
	}

	return result;
}

/* Seek to a particular file offset on the emmcdrv device */
static int32_t emmcdrv_block_seek(io_entity_t *entity, int32_t mode,
				ssize_t offset)
{
	int32_t result;

	/* We only support IO_SEEK_SET for the moment. */
	if ((io_seek_mode_t)mode == IO_SEEK_SET) {

		/* TODO: can we do some basic limit checks on seek? */
		((file_state_t *)entity->info)->file_pos = offset;
		result = IO_SUCCESS;
	} else {
		result = IO_FAIL;
	}

	return result;
}

/* Read data from a file on the emmcdrv device */
static int32_t emmcdrv_block_read(io_entity_t *entity, uintptr_t buffer,
			     size_t length, size_t *length_read)
{
	file_state_t *fp;
	uint32_t sector_add;
	uint32_t sector_num;
	uint32_t emmc_dma = 0U;
	int32_t result = IO_SUCCESS;

	fp = (file_state_t *)entity->info;

	NOTICE("BL2: Load dst=0x%lx src=(p:%d)0x%lx(%d) len=0x%lx(%d)\n",
		buffer,
		(uint32_t)(current_file.partition),
		(current_file.file_pos),
		(uint32_t)(current_file.file_pos) >> EMMC_SECTOR_SIZE_SHIFT,
		length, 
		(uint32_t)((length + (EMMC_SECTOR_SIZE - 1U)) 
						>> EMMC_SECTOR_SIZE_SHIFT));

	sector_add = (uint32_t)(current_file.file_pos)
						>> EMMC_SECTOR_SIZE_SHIFT;
	sector_num = (uint32_t)((length + (EMMC_SECTOR_SIZE - 1U)) 
						>> EMMC_SECTOR_SIZE_SHIFT);
	if ((uint64_t)(buffer + length) <= UINT32_MAX ) {
		emmc_dma = LOADIMAGE_FLAGS_DMA_ENABLE;
	}
	
	if (emmc_read_sector((uint32_t *)buffer, sector_add, sector_num,
			emmc_dma) != EMMC_SUCCESS) {
		result = IO_FAIL;
	}

	*length_read = length;
	/* advance the file 'cursor' for incremental reads */
	fp->file_pos += (ssize_t)length;

	return result;
}

/* Close a file on the emmcdrv device */
static int32_t emmcdrv_block_close(io_entity_t *entity)
{
	entity->info = 0U;

	/* This would be a mem free() if we had malloc.*/
	(void)memset((void *)&current_file, 0, sizeof(current_file));

	return IO_SUCCESS;
}

/* Exported functions */

/* Register the emmcdrv driver with the IO abstraction */
int32_t register_io_dev_emmcdrv(const io_dev_connector_t **dev_con)
{
	int32_t result;

	result = io_register_device(&emmcdrv_dev_info);
	if (result == IO_SUCCESS) {
		*dev_con = &emmcdrv_dev_connector;
	}

	return result;
}
