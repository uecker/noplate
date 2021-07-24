/* Copyright 2021. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "string.h"


extern inline string string_alloc(void);
extern inline int string_length(const string x);

static string string_init0(int len, const char c[static len])
{
	string s = vec_alloc_n(char, len + 1);

	if (NULL == s)
		goto err;

	memcpy(vec_array(s), c, len);
	vec_access(s, len) = '\0';

err:
	return s;
}

string string_init(const char* c)
{
	return string_init0(strlen(c), c);
}


string string_dup(const string x)
{
	return string_init0(string_length(x), string_cstr(x));
}


string string_concat(const string a, const string b)
{
	int alen = string_length(a);
	int blen = string_length(b);

	string x = vec_alloc_n(char, alen + blen + 1);

	if (NULL == x)
		goto err;

	memcpy(string_cstr(x), string_cstr(a), alen);
	memcpy(string_cstr(x) + alen, string_cstr(b), blen);
	vec_access(x, alen + blen) = '\0';

err:
	return x;
}

string string_printf(const char* fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);

	int len = vsnprintf(NULL, 0, fmt, ap);

	va_end(ap);

	string s = NULL;

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
	return s;
}

