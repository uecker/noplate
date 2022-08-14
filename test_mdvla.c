/* Copyright 2022. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include <assert.h>

#include "mdvla.h"

md_vla_decl(3, int);
md_vla_decl(3, float);

int main()
{
	md_vla(3, int) x = { (int[24]){ 0 }, { 2, 3, 4, } };

	int (*y)[4][3][2] = &md_vla_array(x);

	assert(4 == array_lengthof(*y));
	assert(3 == array_lengthof((*y)[0]));
	assert(2 == array_lengthof((*y)[0][0]));

	md_vla_array(x)[0][1][1] = 3;

	int (*p)[4][3][2] = &md_vla_array(x);

	(*p)[1][0][1] = 1;



	float u3[3][2][1];
	float u2[3][3];
	float u1[2];
	extern float v;


	size_t dims[3] = { vla_dims(3, u3) };

	assert(dims[2] == 3);
	assert(dims[1] == 2);
	assert(dims[0] == 1);


#if 1
	_Static_assert(3 == vla_rank(u3), "");
	_Static_assert(2 == vla_rank(u2), "");
	_Static_assert(1 == vla_rank(u1), "");
	_Static_assert(0 == vla_rank(v), "");
#endif

	extern array_eltype(u1) v;
	extern vla_eltype(u1) v;
	extern vla_eltype(u2) v;
	extern vla_eltype(u3) v;

	u3[1][1][0] = 1.;

	auto z = md_vla_init(3, float, u3);
	md_vla(3, float)* zp = &z;

	assert(1. == md_vla_array(*zp)[1][1][0]);
}





