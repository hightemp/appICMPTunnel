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

#define LOG_TAG "bt_tcl_reader"

#include "tcl/log.h"
#include "tcl/io.h"
#include "tcl/reader.h"

static bool __has_byte(const eager_reader_t *reader);
static void *__inbound_read_loop(void *context);
static void __outbound_read_ready(void *context);

int eager_reader_init(eager_reader_t *ret, int fd_to_read,
	const allocator_t *allocator, unsigned buffer_size,
	unsigned max_buffer_count)
{

	assert(fd_to_read != INVALID_FD);
	assert(allocator != NULL);
	assert(buffer_size > 0);
	assert(max_buffer_count > 0);

	ret->allocator = allocator;
	ret->inbound_fd = fd_to_read;

	if (event_init(&ret->event, 0, 0)) {
		LOG_ERROR("unable to create output reading semaphore.");
		goto error;
	}

	ret->buffer_size = buffer_size;

	if (blocking_queue_init(&ret->buffers, max_buffer_count)) {
		LOG_ERROR("unable to create buffers queue.");
		goto error;
	}

	if (task_repeat(&ret->inbound_read_task, __inbound_read_loop, ret)) {
		LOG_ERROR("unable to make reading worker.");
		goto error;
	}

	ret->outbound_context = NULL;
	ret->outbound_read_ready = NULL;
	ret->outbound_registration = NULL;
	ret->current_buffer = NULL;

	return 0;

error:
	eager_reader_destroy(ret);
	return -1;
}

void eager_reader_destroy(eager_reader_t *reader)
{
	if (!reader)
		return;

	eager_reader_unregister(reader);
	event_destroy(&reader->event);
	if (reader->current_buffer)
		reader->allocator->free(reader->current_buffer);
	blocking_queue_destroy(&reader->buffers, pfree);
	task_destroy(&reader->inbound_read_task);
}

void eager_reader_register(eager_reader_t *reader, worker_t *worker,
	eager_reader_cb_t *read_cb, void *context)
{
	assert(reader != NULL);
	assert(worker != NULL);
	assert(read_cb != NULL);

	eager_reader_unregister(reader);
	reader->outbound_read_ready = read_cb;
	reader->outbound_context = context;
	reader->outbound_registration = reactor_register(&worker->reactor,
		&reader->event, reader, __outbound_read_ready, NULL);
}

void eager_reader_unregister(eager_reader_t *reader)
{
	assert(reader != NULL);

	if (reader->outbound_registration) {
		reactor_unregister(reader->outbound_registration);
		reader->outbound_registration = NULL;
	}
}

size_t eager_reader_read(eager_reader_t *reader, uint8_t *buffer,
	size_t max_size)
{
	assert(reader != NULL);
	assert(buffer != NULL);

	// Find out how many bytes we have available in our various buffers.
	event_value_t bytes_available;
	if (event_read(&reader->event, &bytes_available) == -1) {
		LOG_ERROR("unable to read semaphore for output data.");
		return 0;
	}

	if (max_size > bytes_available)
		max_size = (size_t)bytes_available;

	size_t bytes_consumed = 0;
	while (bytes_consumed < max_size) {
		if (!reader->current_buffer)
			reader->current_buffer = blocking_queue_pop(&reader->buffers);

		size_t bytes_to_copy = reader->current_buffer->length -
			reader->current_buffer->offset;
		if (bytes_to_copy > (max_size - bytes_consumed))
			bytes_to_copy = max_size - bytes_consumed;

		memcpy(&buffer[bytes_consumed],
			&reader->current_buffer->data[reader->current_buffer->offset],
			bytes_to_copy);
		bytes_consumed += bytes_to_copy;
		reader->current_buffer->offset += bytes_to_copy;

		if (reader->current_buffer->offset >= reader->current_buffer->length) {
			reader->allocator->free(reader->current_buffer);
			reader->current_buffer = NULL;
		}
	}

	bytes_available -= bytes_consumed;
	if (event_write(&reader->event, bytes_available) == -1) {
		LOG_ERROR("unable to write back bytes available for output data.");
	}

	return bytes_consumed;
}

FORCEINLINE
int eager_reader_setpriority(eager_reader_t *reader,
	int priority)
{
	return task_setpriority(&reader->inbound_read_task, priority);
}

OPTNONE
static bool __has_byte(const eager_reader_t *reader)
{
	int ret;
	fd_set read_fds;
	struct timeval timeout;

	assert(reader != NULL);

	do {
		FD_ZERO(&read_fds);
		FD_SET(reader->inbound_fd, &read_fds);

		timeout.tv_sec = 0;
		timeout.tv_usec = 0;

		ret = select(reader->inbound_fd + 1, &read_fds, NULL, NULL, &timeout);
	} while(ret == -1 && (errno == EINTR || errno == EAGAIN));

	return ret > 0 && FD_ISSET(reader->inbound_fd, &read_fds) ? true : false;
}

static void *__inbound_read_loop(void *context)
{
	eager_reader_t *reader = (eager_reader_t *)context;

	if (__has_byte(reader)) {
		data_buffer_t *buffer = (data_buffer_t *)malloc(reader->buffer_size +
			sizeof(data_buffer_t));
		if (!buffer) {
			LOG_ERROR("couldn't aquire memory for inbound data buffer.");
			return NULL;
		}

		buffer->length = 0;
		buffer->offset = 0;

		isize_t bytes_read;
		do (bytes_read = read(reader->inbound_fd, buffer->data,
			reader->buffer_size));
		while (bytes_read < 0 && errno == EINTR);
		if (bytes_read > 0) {
			// Save the data for later
			buffer->length = (size_t)bytes_read;
			blocking_queue_push(&reader->buffers, buffer);

			// Tell consumers data is available by incrementing
			// the semaphore by the number of bytes we just read
			event_write(&reader->event, (event_value_t)bytes_read);
		} else {
#ifdef TCL_LOGGING
			if (bytes_read == 0)
				LOG_WARN("fd said bytes existed, but none were found.");
			else {
				LOG_WARN("unable to read from file descriptor: %s",
					strerror(errno));
			}
#endif
			free(buffer);
		}

	}
	return NULL;
}

static void __outbound_read_ready(void *context)
{
	assert(context != NULL);

	eager_reader_t *reader = (eager_reader_t *)context;
	reader->outbound_read_ready(reader, reader->outbound_context);
}
