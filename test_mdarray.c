/* Copyright 2022-2023. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include <assert.h>

#include "mdarray.h"

mdarray_decl(3, int);
mdarray_decl(3, float);


void fill(mdarray(3, float) arg, float val)
{
	auto x = &mdarray_array(arg);

	for (size_t i = 0; i < array_lengthof((*x)); i++)
		for (size_t j = 0; j < array_lengthof((*x)[0]); j++)
			for (size_t k = 0; k < array_lengthof((*x)[0][0]); k++)
				(*x)[i][j][k] = val;
}

int main()
{
	mdarray(3, int) x = mdarray_init(3, int, (int[4][3][2]){ 0 });

	// mdarray_array provides an lvalue with a conventional array type

	int (*y)[4][3][2] = &mdarray_array(x);

	assert(4 == array_lengthof(*y));
	assert(3 == array_lengthof((*y)[0]));
	assert(2 == array_lengthof((*y)[0][0]));

	mdarray_array(x)[0][1][1] = 3;

	(*y)[1][0][1] = 1;


	// array_rank retuns the rank of a multi-dimensional array

	float u3[3][2][1];
	float u2[3][3];
	float u1[2];
	extern float v;

	_Static_assert(3 == array_rank(u3), "");
	_Static_assert(2 == array_rank(u2), "");
	_Static_assert(1 == array_rank(u1), "");
	_Static_assert(0 == array_rank(v), "");


	// array_dims retuns the dimensions as a comma-separated list

	size_t dims[3] = { array_dims(3, u3) };

	assert(dims[2] == 3);
	assert(dims[1] == 2);
	assert(dims[0] == 1);


	extern array_eltype(u1) v;
	extern array_nested_eltype(u1) v;
	extern array_nested_eltype(u2) v;
	extern array_nested_eltype(u3) v;

	u3[1][1][0] = 1.;

	auto z = mdarray_init(3, float, u3);
	mdarray(3, float)* zp = &z;

	assert(1. == mdarray_array(*zp)[1][1][0]);

	fill(z, 2.);

	assert(2. == mdarray_array(*zp)[1][1][0]);
}





