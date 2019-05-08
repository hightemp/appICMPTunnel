/*
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

#define LOG_TAG "tcl_fiber"

#include "tcl/log.h"
#include "tcl/fiber.h"

#ifndef FIBER_MAX
# define FIBER_MAX 256
#endif

enum {

	/*! The fiber was just created so ready */
	FIBER_ACTIVE,

	/*! TODO */
	FIBER_BLOCKING,

	/*! The fiber is terminated but still exists */
	FIBER_DONE
};

struct fiber {

	coro_t coroutine;

	uint16_t id;

	int8_t priority;

	uint8_t state;

	uint32_t __padding;

	void *arg;

	void *ret;

	struct fiber *next;
};

struct {

	struct fiber fibers[FIBER_MAX];

	struct fiber *head, *self;

	uint16_t fibers_idx;

	uint16_t __padding;

	fiber_attr_t dft_attr;

} __scope;

static void __initmain(void)
{
	if (__scope.head)
		return;
	__scope.fibers[0].coroutine = coro_self();
	__scope.head = &__scope.fibers[0];
	__scope.self = &__scope.fibers[0];
	++__scope.fibers_idx;
}

FORCEINLINE
static struct fiber *__fiber(fiber_t fiber)
{
	return __scope.fibers + coro_getdata(fiber);
}

int fiber_attr_init(fiber_attr_t *attr, int prio, uint16_t stack_size)
{
	if (prio < -20 || prio > 20) {
		errno = EINVAL;
		return -1;
	}
	attr->prio = (int8_t)prio;
	attr->stack_size = stack_size;
	return 0;
}

int fiber_create(fiber_t *fiber, const fiber_attr_t *attr,
				 routine_t *routine, void *arg)
{
	struct fiber *node, *head;

	if (__scope.fibers_idx >= FIBER_MAX) {
		errno = EINVAL;
		return -1;
	}

	if (LIKELY(!attr))
		attr = &__scope.dft_attr;

	if (coro_init(fiber, routine, attr->stack_size))
		return -1;

	__initmain();

	node = __scope.fibers + __scope.fibers_idx;
	node->id = __scope.fibers_idx++;

	node->coroutine = *fiber;
	node->state = FIBER_ACTIVE;
	node->priority = attr->prio;
	node->arg = arg;
	node->ret = NULL;
	node->next = NULL;

	coro_setdata(*fiber, (uintptr_t)node->id);

	head = __scope.head;
	while (head->next
		&& head->next->priority <= node->priority) {
		head = head->next;
	}
	node->next = head->next;
	head->next = node;

	return 0;
}

int fiber_cancel(fiber_t fiber)
{
	struct fiber *data;

	if (!fiber) {
		errno = EINVAL;
		return -1;
	}

	data = __fiber(fiber);
	if (data->state == FIBER_DONE) {
		errno = EINVAL;
		return -1;
	}

	if (fiber == fiber_self())
		fiber_exit(NULL);
	else {
		data->state = FIBER_DONE;
		coro_kill(&data->coroutine);
	}

	return 0;
}

int fiber_join(fiber_t fiber, void **retval)
{
	struct fiber *data;

	if (!fiber) {
		errno = EINVAL;
		return -1;
	}

	data = __fiber(fiber);
	if (data->state == FIBER_DONE) {
		errno = EINVAL;
		return -1;
	}

	while (data->state != FIBER_DONE)
		fiber_yield();

	if (retval)
		*retval = data->ret;

	return 0;
}

int fiber_setschedprio(fiber_t fiber, int prio)
{
	(void)fiber;
	(void)prio;
	/* TODO(tempow): update priority */
	return 0;
}

FORCEINLINE
void fiber_exit(void *retval)
{
	assert(__scope.self != __scope.head);

	__scope.self->state = FIBER_DONE;
	coro_exit(__scope.self->ret = retval);
}

FORCEINLINE
fiber_t fiber_self(void)
{
	return coro_self();
}

void fiber_lock(waitq_t *waitq)
{
	struct fiber *data;

	data =__fiber(fiber_self());
	data->state = FIBER_BLOCKING;
	waitq_push(waitq, fiber_self());
}

void fiber_unlock(waitq_t *waitq)
{
	fiber_t fiber;
	struct fiber *data;

	if ((fiber = waitq_pop(waitq))) {
		data = __fiber(fiber);
		data->state = FIBER_ACTIVE;
	}
}

void fiber_yield(void)
{
	struct fiber *begin ,*head;

	begin = __scope.self;
	head = begin;

	do {
		head = head->next ? head->next : __scope.head;
		if (head == begin)
			return;
	} while (head->state != FIBER_ACTIVE);

	__scope.self = head;
	head->ret = coro_resume(&head->coroutine, head->arg);
	if (!head->coroutine)
		head->state = FIBER_DONE;
}
