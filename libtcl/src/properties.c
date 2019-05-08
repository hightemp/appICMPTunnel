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

#include "tcl/properties.h"

#ifdef HAS_SYS__SYSTEM_PROPERTIES_H
# include <sys/_system_properties.h>
# define SYSTEM_PROPERTIES
#endif

int property_get(const char *key, char *value, const char *default_value)
{
#ifdef SYSTEM_PROPERTIES
	int len;

    len = __system_property_get(key, value);
    if(len > 0)
        return len;
    if(default_value) {
        len = strlen(default_value);
        memcpy(value, default_value, len + 1);
    }
    return len;
#else
	int len;

	(void)key;
	len = -1;
	if (!default_value)
		return len;
	len = (int)strlen(default_value);
	if (len >= PROPERTY_VALUE_MAX)
		len = PROPERTY_VALUE_MAX - 1;
	memcpy(value, default_value, (size_t)len);
	value[len] = '\0';
	return len;
#endif
}

int property_set(const char *key, const char *value)
{
#ifdef SYSTEM_PROPERTIES
	return __system_property_set(key, value);
#else
	(void)key;
	(void)value;
	return -1;
#endif
}
