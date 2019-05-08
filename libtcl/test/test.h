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

#pragma once

#ifndef __TCL_TEST_H
# define __TCL_TEST_H

#include "tcl/conf.h"
#include "tcl/string.h"

#include <assert.h>
#include <stdio.h>

#ifndef ASSERT_F
# define ASSERT_F(...) "%s:%d: `%s'\n", __FILE__, __LINE__, #__VA_ARGS__
#endif

#ifndef TEARDOWN
#define ASSERT(cond) do if(!(cond))exit(printf(ASSERT_F(cond))>0);while(0)
#else
#define ASSERT(cond) do if(!(cond)) { \
		TEARDOWN(); exit(printf(ASSERT_F(cond))>0); \
	} while(0)
#endif

#define ASSERT_EQ(a, b) ASSERT((a) == (b))
#define ASSERT_GE(a, b) ASSERT((a) >= (b))
#define ASSERT_LE(a, b) ASSERT((a) <= (b))
#define ASSERT_NEQ(a, b) ASSERT((a) != (b))
#define ASSERT_TRUE(a) ASSERT_EQ(a, true)
#define ASSERT_FALSE(a) ASSERT_EQ(a, false)
#define ASSERT_NULL(a) ASSERT_EQ(a, NULL)
#define ASSERT_STREQ(a, b) ASSERT_EQ(0, strcmp(a, b))

#ifdef TEST
int main(void)
{
#ifdef SETUP
	SETUP();
#endif
	TEST();
#ifdef TEARDOWN
	TEARDOWN();
#endif
	return 0;
}
#endif /* TEST */

#endif /* !__TCL_TEST_H */
