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
#include "io_memdrv.h"
#include "dma_driver.h"

/* As we need to be able to keep state for seek, only one file can be open
 * at a time. Make this a structure and point to the entity->info. When we
 * can malloc memory we can change this to support more open files.
 */
typedef struct {
	/* Use the 'in_use' flag as any value for base and file_pos could be
	 * valid.
	 */
	uint32_t	in_use;
	uintptr_t	base;
	ssize_t		file_pos;
} file_state_t;

static file_state_t current_file = {0};

/* memdrv device functions */
static io_type_t device_type_memdrv(void);
static int32_t memdrv_dev_open(const uintptr_t dev_spec,
				io_dev_info_t **dev_info);
static int32_t memdrv_block_open(io_dev_info_t *dev_info, const uintptr_t spec,
			     io_entity_t *entity);
static int32_t memdrv_block_seek(io_entity_t *entity, int32_t mode,
			     ssize_t offset);
static int32_t memdrv_block_read(io_entity_t *entity, uintptr_t buffer,
			     size_t length, size_t *length_read);
static int32_t memdrv_block_close(io_entity_t *entity);
static int32_t memdrv_dev_close(io_dev_info_t *dev_info);


static const io_dev_connector_t memdrv_dev_connector = {
	.dev_open = &memdrv_dev_open
};


static const io_dev_funcs_t memdrv_dev_funcs = {
	.type = &device_type_memdrv,
	.open = &memdrv_block_open,
	.seek = &memdrv_block_seek,
	.size = NULL,
	.read = &memdrv_block_read,
	.write = NULL,
	.close = &memdrv_block_close,
	.dev_init = NULL,
	.dev_close = &memdrv_dev_close,
};


/* No state associated with this device so structure can be const */
static const io_dev_info_t memdrv_dev_info = {
	.funcs = &memdrv_dev_funcs,
	.info = (uintptr_t)0
};

/* Identify the device type as memdrv */
static io_type_t device_type_memdrv(void)
{
	return IO_TYPE_MEMMAP;
}


/* Open a connection to the memdrv device */
static int32_t memdrv_dev_open(
			const uintptr_t dev_spec __attribute__((unused)),
			io_dev_info_t **dev_info)
{
	*dev_info = (io_dev_info_t *)&memdrv_dev_info; /* cast away const */

	return IO_SUCCESS;
}



/* Close a connection to the memdrv device */
static int32_t memdrv_dev_close(io_dev_info_t *dev_info)
{
	/* NOP */
	/* TODO: Consider tracking open files and cleaning them up here */
	return IO_SUCCESS;
}


/* Open a file on the memdrv device */
/* TODO: Can we do any sensible limit checks on requested memory */
static int32_t memdrv_block_open(io_dev_info_t *dev_info, const uintptr_t spec,
			     io_entity_t *entity)
{
	int32_t result;
	const io_drv_spec_t *block_spec = (io_drv_spec_t *)spec;

	/* Since we need to track open state for seek() we only allow one open
	 * spec at a time. When we have dynamic memory we can malloc and set
	 * entity->info.
	 */
	if (current_file.in_use == 0U) {

		current_file.in_use = 1U;
		current_file.base = block_spec->offset;
		/* File cursor offset for seek and incremental reads etc. */
		current_file.file_pos = 0;
		entity->info = (uintptr_t)&current_file;
		result = IO_SUCCESS;
	} else {
		/* A memdrv device is already active. Close first. */
		result = IO_RESOURCES_EXHAUSTED;
	}

	return result;
}


/* Seek to a particular file offset on the memdrv device */
static int32_t memdrv_block_seek(io_entity_t *entity, int32_t mode,
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


/* Read data from a file on the memdrv device */
static int32_t memdrv_block_read(io_entity_t *entity, uintptr_t buffer,
			     size_t length, size_t *length_read)
{
	file_state_t *fp;

	fp = (file_state_t *)entity->info;

	NOTICE("BL2: dst=0x%lx src=0x%lx len=%ld(0x%lx)\n",
		buffer,
		fp->base + fp->file_pos,
		length, length);

	/* DMA driver */
	execDMA(buffer, (uint32_t)(fp->base + fp->file_pos),
		(uint32_t)length);

	*length_read = length;
	/* advance the file 'cursor' for incremental reads */
	fp->file_pos += (ssize_t)length;

	return IO_SUCCESS;
}


/* Close a file on the memdrv device */
static int32_t memdrv_block_close(io_entity_t *entity)
{
	entity->info = 0U;

	/* This would be a mem free() if we had malloc.*/
	(void)memset((void *)&current_file, 0, sizeof(current_file));

	return IO_SUCCESS;
}


/* Exported functions */

/* Register the memdrv driver with the IO abstraction */
int32_t register_io_dev_memdrv(const io_dev_connector_t **dev_con)
{
	int32_t result;

	result = io_register_device(&memdrv_dev_info);
	if (result == IO_SUCCESS) {
		*dev_con = &memdrv_dev_connector;
	}

	return result;
}
