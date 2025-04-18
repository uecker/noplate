/* Copyright 2021-2025. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include <stdlib.h>
#include <string.h>

#include "vec.h"
#include "span.h"
#include "array.h"

#ifndef TAGCOMPAT
vec_decl(int);
span_decl(int);
#endif

void test_vec()
{
	vec(int)* v = NULL_CHECK(vec_alloc(int));

	vec_push(int, &v, 1);
	vec_push(int, &v, 3);
	vec_push(int, &v, 8);

	vec(int)* w = NULL_CHECK(vec_alloc(int));

	vec_push(int, &w, 1);
	vec_push(int, &w, 3);
	vec_push(int, &w, 8);

	vec_append(int, &v, vec2span(int, w));

	free(w);

	vec_access(int, v, 1)++;

	int good[] = { 1, 4, 8, 1, 3, 8 };

	for (int i = 0; i < (int)vec_length(int, v); i++)
		if (good[i] != vec_access(int, v, i))
			abort();

	NESTED(int, cmp, (const int* a, const int* b))
	{ 
		return *a - *b;
	};

	vec_sort(int, v, cmp);

	int good2[] = { 1, 1, 3, 4, 8, 8 };

	for (int i = 0; i < (int)vec_length(int, v); i++)
		if (good2[i] != vec_access(int, v, i))
			abort();

	free(v);
}

int main()
{
	test_vec();

	return 0;
}

