/*
 * Copyright (C) 2014 Google, Inc.
 * Copyright (C) 2018 Tempow
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

/*!@file tcl/reader.h
 * @author uael
 *
 * @addtogroup tcl.reader @{
 */
#ifndef __TCL_READER_H
# define __TCL_READER_H

#include "tcl/allocator.h"
#include "tcl/blocking_queue.h"
#include "tcl/task.h"
#include "tcl/worker.h"

/*!@public
 *
 * @brief
 * The eager reader structure declaration.
 */
typedef struct eager_reader eager_reader_t;

/*!@private
 *
 * @brief
 * The data buffer structure declaration.
 */
typedef struct data_buffer data_buffer_t;

/*!@public
 *
 * @brief
 * The eager reader callback declaration.
 */
typedef void (eager_reader_cb_t)(eager_reader_t *reader, void *context);

/*!@private
 *
 * @brief
 * The data buffer structure definition.
 */
struct data_buffer {

	/*! TODO. */
	size_t length;

	/*! TODO. */
	size_t offset;

	/*! TODO. */
	uint8_t data[1];
};

/*!@public
 *
 * @brief
 * The reader structure definition.
 */
struct eager_reader {

	/*! TODO. */
	event_t event;

	/*! TODO. */
	int inbound_fd;

	/*! TODO. */
	const allocator_t *allocator;

	/*! TODO. */
	size_t buffer_size;

	/*! TODO. */
	blocking_queue_t buffers;

	/*! TODO. */
	data_buffer_t *current_buffer;

	/*! TODO. */
	task_t inbound_read_task;

	/*! TODO. */
	reactor_object_t *outbound_registration;

	/*! TODO. */
	eager_reader_cb_t *outbound_read_ready;

	/*! TODO. */
	void *outbound_context;
};

/*!@public
 * 
 * @brief
 * Creates a new eager reader object, which pulls data from |fd_to_read| into
 * buffers of size |buffer_size| allocated using |allocator|, and has an
 * internal read worker named |worker_name|. The returned object must be freed
 * using |eager_reader_free|. |fd_to_read| must be valid, |buffer_size| and
 * |max_buffer_count| must be greater than zero. |allocator| and |worker_name|
 * may not be NULL.
 * 
 * @param reader 
 * @param fd_to_read 
 * @param allocator 
 * @param buffer_size 
 * @param max_buffer_count
 * @return
 */
__api int eager_reader_init(eager_reader_t *reader, int fd_to_read,
	const allocator_t *allocator, unsigned buffer_size,
	unsigned max_buffer_count);

/*!@public
 * 
 * @brief
 * Frees an eager reader object, and associated internal resources.
 * |reader| may be NULL.
 * 
 * @param reader 
 */
__api void eager_reader_destroy(eager_reader_t *reader);

/*!@public
 * 
 * @brief
 * Registers |reader| with the |reactor|. When the reader has data
 * |read_cb| will be called. The |context| parameter is passed, untouched,
 * to |read_cb|. Neither |reader|, nor |reactor|, nor |read_cb| may be NULL.
 * |context| may be NULL.
 * 
 * @param reader 
 * @param worker
 * @param read_cb 
 * @param context 
 */
__api void eager_reader_register(eager_reader_t *reader, worker_t *worker,
	eager_reader_cb_t *read_cb, void *context);

/*!@public
 * 
 * @brief
 * Unregisters |reader| from whichever reactor it is registered with, if any.
 * This function is idempotent.
 * 
 * @param reader 
 */
__api void eager_reader_unregister(eager_reader_t *reader);


/*!@public
 * 
 * @brief
 * Reads up to |max_size| bytes into |buffer| from currently available bytes.
 * NOT SAFE FOR READING FROM MULTIPLE THREADS
 * but you should probably only be reading from one worker anyway,
 * otherwise the byte stream probably doesn't make sense.
 * 
 * @param reader 
 * @param buffer 
 * @param max_size 
 * @return 
 */
__api size_t eager_reader_read(eager_reader_t *reader, uint8_t *buffer,
	size_t max_size);

/*!@public
 * 
 * @brief
 * Set the priority of the reading worker on which the reader is executed.
 * 
 * @param reader   The reader to set the priority for.
 * @param priority The priority to set.
 * @return         0 on success, non-zero otherwise.
 */
__api int eager_reader_setpriority(eager_reader_t *reader, int priority);

#endif /* __TCL_READER_H */
/*!@} */
