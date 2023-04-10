/* Copyright 2021-2023. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "string.h"


extern inline string* string_alloc(void);

static string* string_init0(int len, const char c[static len])
{
	string_priv* s = vec_alloc_n(char, len + 1);

	if (NULL == s)
		goto err;

	memcpy(vec_array(s), c, len);
	vec_access(s, len) = '\0';

err:
	return (string*)s;
}

string* string_init(const char* c)
{
	return string_init0(strlen(c), c);
}


string* string_dup(const string_view x)
{
	return string_init0(string_length(&x), string_cstr(&x));
}


string* string_concat(const string_view a, const string_view b)
{
	ssize_t alen = string_length(&a);
	ssize_t blen = string_length(&b);

	string_priv* x = vec_alloc_n(char, alen + blen + 1);

	if (NULL == x)
		goto err;

	memcpy(&vec_access(x, 0), string_cstr(&a), alen);
	memcpy(&vec_access(x, alen), string_cstr(&b), blen);
	vec_access(x, alen + blen) = '\0';

err:
	return (string*)x;
}

string* string_printf(const char* fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);

	int len = vsnprintf(NULL, 0, fmt, ap);

	va_end(ap);

	string_priv* s = NULL;

	if (len < 0)
		goto err;

	s = vec_alloc_n(char, len + 1);

	if (NULL == s)
		goto err;

	va_start(ap, fmt);

	int rlen = vsnprintf(s->data, s->N, fmt, ap);

	va_end(ap);

	if (len < rlen) {

		free(s);
		s = NULL;
		goto err;
	}

err:
	return (string*)s;
}

