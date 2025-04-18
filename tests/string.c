/* Copyright 2021-2025. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "string.h"

typedef string* string_ptr;

#ifndef TAGCOMPAT
vec_decl(string_ptr);
#endif

void test_string()
{
	vec(string_ptr)* s = NULL_CHECK(vec_alloc(string_ptr));

	vec_push(string_ptr, &s, string_init(" Du!"));
	vec_push(string_ptr, &s, string_init("Hallo"));

	string* ss = NULL_CHECK(string_alloc());
    
	for (int i = 0; i < (int)vec_length(int, s); i++)
		string_append(&ss, vec_access(string_ptr, s, vec_length(int, s) - 1 - i));

	if (vec_length(string_ptr, s) != array_lengthof(vec2array(string_ptr, s)))
		abort();

	free(ss);

	while (0 < vec_length(string_ptr, s))
		free(vec_pop(string_ptr, &s));

	free(s);
}

void test_string_array()
{
	string* s3 = string_init("hallo");

	// this assignment is not checked by compilers
	// char (*slice)[3]
	auto slice = &array_slice(string_cstr(s3), 1, 1 + 3);

	(*slice)[0] = 'A';
	(*slice)[1] = 'L';
	(*slice)[2] = 'L';

	strview v3 = string_view(s3);

	if (0 != strcmp("hALLo", string_cstr(&v3)))
		abort();

	if (vec_length(char8_t, &v3) != string_length(s3) + 1)
		abort();

	free(s3);
}

int main()
{
	test_string();
	test_string_array();

	return 0;
}

