/* Copyright 2021-2023. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "array.h"
#include "string.h"


extern inline string* string_alloc(void);

static string* string_init0(int len, const char8_t c[static len])
{
	string_priv* s = vec_alloc_n(char8_t, len + 1);

	if (NULL == s)
		goto err;

	memcpy(vec_array(char8_t, s), c, len);
	vec_access(char8_t, s, len) = '\0';

err:
	return (string*)s;
}

string* string_init(const char* c)
{
	return string_init0(strlen(c), (const char8_t*)c);
}


string* strview_dup(const strview x)
{
	return string_init0(string_length(&x), string_utf8(&x));
}

string* string_dup(const string *x)
{
	return string_init0(string_length(x), string_utf8(x));
}

void string_append_view(string **a, const strview b)
{
	ssize_t blen = string_length(&b);

	string_priv *x = STRING_UNWRAP(*a);
	ssize_t alen = string_length(x);
	vec_realloc(char8_t, &x, alen + blen + 1);

	if (NULL == x)
		goto err;

	memcpy(&vec_access(char8_t, x, alen), string_cstr(&b), blen);
	vec_access(char8_t, x, alen + blen) = '\0';
	*a = (string*)x;
err:
}

void string_append(string **a, const string *b)
{
	string_append_view(a, string_view((string*)b));
}

string* strview_concat(const strview a, const strview b)
{
	ssize_t alen = string_length(&a);
	ssize_t blen = string_length(&b);

	string_priv *x = vec_alloc_n(char8_t, alen + blen + 1);

	if (NULL == x)
		goto err;

	memcpy(&vec_access(char8_t, x, 0), string_cstr(&a), alen);
	memcpy(&vec_access(char8_t, x, alen), string_cstr(&b), blen);
	vec_access(char8_t, x, alen + blen) = '\0';

err:
	return (string*)x;
}


string* string_concat(const string *a, const string *b)
{
	return strview_concat(string_view((string*)a), string_view((string*)b));
}


string* string_printf(const char* fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);

	int len = vsnprintf(NULL, 0, fmt, ap);

	va_end(ap);

	string_priv *s = NULL;

	if (len < 0)
		goto err;

	s = vec_alloc_n(char8_t, len + 1);

	if (NULL == s)
		goto err;

	va_start(ap, fmt);

	int rlen = vsnprintf((char*)s->data, s->N, fmt, ap);

	va_end(ap);

	if (len < rlen) {

		free(s);
		s = NULL;
		goto err;
	}

err:
	return (string*)s;
}

