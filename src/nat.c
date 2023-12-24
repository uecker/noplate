/* Copyright 2021-2023. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include <limits.h>

#include "string.h"

#include "nat.h"

_Static_assert((nat_base_t)-1 > (nat_base_t)0, "base type must be unsigned");

#define bitsof(t) (CHAR_BIT * sizeof(t))
#define maxof(t) ((1ULL << (bitsof(t) - 1ULL)) - 1ULL)

extern inline nat nat_alloc(void);
extern inline int net_length(const nat x);

nat nat_init(int i)
{
	if (i < 0)
		return NULL;

	nat x = vec_alloc_n(nat_base_t, 1);

	if (NULL == x)
		goto err;

	vec_access(nat_base_t, x, 0) = i;
err:
	return x;
}

nat nat_dup(const nat x)
{
	nat n = vec_alloc_n(nat_base_t, vec_length(x));

	if (NULL == n)
		goto err;

	memcpy(	vec_array(nat_base_t, n),
		vec_array(nat_base_t, x),
		vec_sizeof(nat_base_t, x));

err:
	return n;
}

extern nat nat_mul(const nat a, const nat b)
{
	int A = vec_length(a);
	int B = vec_length(b);

	nat p = vec_calloc_n(nat_base_t, A + B);

	if (NULL == p)
		goto err;

	typedef unsigned long nat_ext_t;
	_Static_assert(sizeof(nat_ext_t) == 2 * sizeof(nat_base_t), "");

	for (int i = 0; i < A; i++) {

		nat_base_t carry = 0;

		for (int j = 0; j < B; j++) {

			nat_ext_t aa = vec_access(nat_base_t, a, i);
			nat_ext_t bb = vec_access(nat_base_t, b, j);

			nat_ext_t tmp = carry + aa * bb;

			carry = tmp >> bitsof(nat_base_t);

			vec_access(nat_base_t, p, i + j) += tmp;
		}

		vec_access(nat_base_t, p, i + B) = carry;
	}
err:
	return p;	
}


extern nat nat_add(const nat a, const nat b)
{
	int A = vec_length(a);
	int B = vec_length(b);

	nat p = vec_alloc_n(nat_base_t, ((A > B) ? A : B) + 1);

	if (NULL == p)
		goto err;

	int C = vec_length(p);

	nat_base_t carry = 0;

	for (int i = 0; i < C; i++) {

		nat_base_t aa = (i < A) ? vec_access(nat_base_t, a, i) : 0;
		nat_base_t bb = (i < B) ? vec_access(nat_base_t, b, i) : 0;

		vec_access(nat_base_t, p, i) = (nat_base_t)(aa + bb + carry);	// wraps

		if (carry)
			carry = (aa >= maxof(nat_base_t) - bb);
		else
			carry = (aa > maxof(nat_base_t) - bb);
	}

	if (!carry) {

		// assert(0 == vec_access(p, C - 1));
		vec_pop(nat_base_t, &p);
	}

err:
	return p;
}


extern nat nat_div(const nat a, const nat b);
extern nat nat_sub(const nat a, const nat b);



string* nat_2string(const nat x)
{
	int X = vec_length(x);
//	int N = (X * bitsof(nat_base_t) + 2) / 3;

	char tmp[X * bitsof(nat_base_t) + 1];
	int l = 0;

	for (int i = X - 1; i >= 0; i--) {

		nat_base_t v = vec_access(nat_base_t, x, i);

		for (int j = bitsof(nat_base_t) - 1; j >= 0; j--) {

			int bit = (v >> j) & 1;

			tmp[l++] = bit ? '1' : '0';
		}
	}

	tmp[l] = '\0';

	return string_init(tmp);
}


