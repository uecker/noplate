/* Copyright 2021-2025. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include <stdlib.h>
#include <string.h>

#include "vec.h"
#include "array.h"
#include "span.h"


#ifndef TAGCOMPAT
vec_decl(int);
span_decl(int);
span_decl(float);
#endif


void test_span()
{
	float bb[4];
	span(float) vf = array2span(float, bb);

	span_access(float, &vf, 1) = 1.;

	vec(int) *v = NULL_CHECK(vec_alloc(int));

	vec_push(int, &v, 1);
	vec_push(int, &v, 3);
	vec_push(int, &v, 8);

	span(int) vi = vec2span(int, v);

	span_access(int, &vi, 1)++;

	int (*t)[3] = &span2array(int, &vi);

	if (4 != span2array(int, &vi)[1])
		abort();

	if ((*t)[2] != span2array(int, &vi)[2])
		abort();
}

int main()
{
	test_span();
	return 0;
}

