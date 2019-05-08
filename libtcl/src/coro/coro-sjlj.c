/*
 * Copyright (C) 2018 Tempow
 *
 * Written by Tony Finch <dot@dotat.at>
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
#define LOG_TAG "tcl_coro_sjlj"

#include "tcl/log.h"
#include "coro/internal.h"

#include <setjmp.h>

static struct coro {
	struct coro *next;
	uint8_t terminated;
	uintptr_t data;
	jmp_buf state;
} __main, *__self = &__main, *__idle;

static volatile size_t __last_stack;

static void __push(coro_t *list, coro_t c)
{
	c->next = *list;
	*list = c;
}

static coro_t __pop(coro_t *list)
{
	coro_t c = *list;
	
	*list = c->next;
	c->next = NULL;
	return(c);
}

OPTNONE
static void *__pass(coro_t me, void *arg)
{
	static void *saved;
	
	saved = arg;
	if (!setjmp(me->state))
		longjmp(__self->state, 1);
	return saved;
}

static void *__resume(coro_t c, void *arg)
{
	void *ret;

	__push(&__self, c);
	ret = __pass(c->next, arg);
	return ret;
}

static void __start(size_t stack_size);

OPTNONE
static void __root(void *ret)
{
	void *(*fun)(void *);
	struct coro me;
	void *y;

	__push(&__idle, &me);
	fun = __pass(&me, ret);
	if(!setjmp(__self->state))
		__start(__last_stack);
	for(;;) {
		y = coro_yield(&me);
		ret = fun(y);
		__self->terminated = 1;
		__push(&__idle, __pop(&__self));
		fun = __pass(&me, ret);
	}
}

static void __start(size_t stack_size)
{
#ifdef CC_MSVC
	char *stack = alloca(stack_size + 1);
#else
	char stack[stack_size + 1] ALIGNED(sizeof(int));
#endif

	stack[stack_size] = 1;
	__root(stack);
}

OPTNONE
int coro_init(coro_t *coro, routine_t *fun, size_t stack_size)
{
	if (stack_size < 4096)
		stack_size = 4096;
 	__last_stack = stack_size;
	if(__idle == NULL && !setjmp(__self->state))
		__start(__last_stack);
	*coro = (__resume(__pop(&__idle), fun));
	return 0;
}

void coro_kill(coro_t *coro)
{
	assert(*coro);
	assert(*coro != __self);

	*coro = NULL;
}

FORCEINLINE
void coro_setdata(coro_t coro, uintptr_t data)
{
	coro->data = data;
}

FORCEINLINE
uintptr_t coro_getdata(coro_t coro)
{
	return coro->data;
}

void *coro_resume(coro_t *c, void *arg)
{
	void *ret;

	if (__self->next == *c)
		return coro_yield(arg);
	ret = __resume(*c, arg);
	if ((*c)->terminated)
		*c = NULL;
	return ret;
}

void *coro_yield(void *arg)
{
	return __pass(__pop(&__self), arg);
}

void coro_exit(void *retval)
{
	__self->terminated = 1;
	coro_yield(retval);
	abort();
}

FORCEINLINE
coro_t coro_self(void)
{
	return __self;
}
