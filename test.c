/* Copyright 2021-2023. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "string.h"
#include "vec.h"
#include "view.h"
#include "list.h"
#include "nat.h"
#include "array.h"
#include "mem.h"
#include "maybe.h"
#include "algebraic.h"
#include "rbtree.h"

typedef string* string_ptr;

#ifndef TAGCOMPAT
vec_decl(int);
vec_decl(string_ptr);
view_decl(int);
view_decl(float);
list_decl(int);
list_decl(string_ptr);
maybe_decl(int);
typedef product_decl(int, float) product_name(int, float);
sum_decl(product_name(int, float), string_ptr);
typedef product_decl(int, int) product_name(int, int);
rbtree_decl(product_name(int, int));
#else
typedef product(int, float) product_name(int, float);
typedef product(int, int) product_name(int, int);
#endif


int main()
{
	list(int) l = { 0 };
	list(string_ptr) r = { 0 };

	for (int i = 0; i < 10; i++)
		list_push(&l, i);

	list_node(int)* n = list_first(&l);

	for (int i = 0; i < 5; i++)
		n = list_node_next(n);

	assert(4 == list_node_access(n));

	while (!list_empty(&l))
		printf("%d\n", list_pop(&l));

	list_push(&r, STRING("Dir?"));
	list_push(&r, STRING("es"));
	list_push(&r, STRING("geht"));
	list_push(&r, STRING("Wie"));
	list_push(&r, STRING("Hallo!"));

	while (!list_empty(&r)) {

		string* s2 = list_pop(&r);

		printf("%s\n", string_cstr(s2));

		free(s2);
	}


	vec(int)* v = NULL_CHECK(vec_alloc(int));

	vec_push(int, &v, 1);
	vec_push(int, &v, 3);
	vec_push(int, &v, 8);

	vec(string_ptr)* s = NULL_CHECK(vec_alloc(string_ptr));

	vec_push(string_ptr, &s, string_init(" Du!"));
	vec_push(string_ptr, &s, string_init("Hallo"));

	vec_access(int, v, 1)++;


	float bb[4];
	view(float) vf = array_view(float, bb);

	vec_access(float, &vf, 1) = 1.;

	view(int) vi = vec_view(int, v);

	vec_access(int, &vi, 1)++;

	int (*t)[3] = &vec_array(int, &vi);

	assert(5 == vec_array(int, &vi)[1]);
	assert((*t)[2] == vec_array(int, &vi)[2]);

#if 0
	// these should all fail
	vec_push(vi, 3);
	vec_push(&vi, 3);
	vec_push((view(int)*){ &vi }, 3);
	vec_push(&(view(int)*){ &vi }, 3);
#endif

	NESTED(int, cmp, (const int* a, const int* b))
	{ 
		return *a - *b;
	};

	vec_sort(int, v, cmp);

	NESTED(int, cmp2, (const string_ptr* a, const string_ptr* b))
	{ 
		return strcmp(string_cstr(*a), string_cstr(*b));
	};

	vec_sort(string_ptr, s, cmp2);

	string* ss = NULL_CHECK(string_alloc());
    
	for (int i = 0; i < (int)vec_length(s); i++)
		string_append(&ss, vec_access(string_ptr, s, vec_length(s) - 1 - i));

	const string* s2 = ss;
	const char* x = string_cstr(s2);

	printf("%s %d\n", x, vec_access(int, v, 0));

	assert(vec_length(s) == array_lengthof(vec_array(string_ptr, s)));

	free(v);
	xfree(ss);

	while (0 < vec_length(s))
		free(vec_pop(string_ptr, &s));

	free(s);

	string* s3 = string_init("hallo");

	// this assignment is not checked by compilers
	// char (*slice)[3]
	auto slice = &array_slice(string_cstr(s3), 1, 1 + 3);

	(*slice)[0] = 'A';
	(*slice)[1] = 'L';
	(*slice)[2] = 'L';

	printf("%s\n", string_cstr(s3));

	strview v3 = string_view(s3);

	printf("%s\n", string_cstr(&v3));

	assert(vec_length(&v3) == string_length(s3) + 1);

	free(s3);

	char buf[100];

	poke(int, &array_slice(buf, 11, 11 + sizeof(int)), 3);
	int i = peek(int, &array_slice(buf, 11, 11 + sizeof(int)));

	assert(3 == i);


	maybe(int) m = maybe_fail(int);

	if (m.ok)
		printf("not ok: %d\n", maybe_use(m));

	m = maybe_ok(int, 3);

	if (m.ok)
		printf("ok: %d\n", maybe_use(m));

	auto ifl = product_init(int, float, (3, 0.1));
	auto si = sum_init(product_name(int, float), string_ptr, ifl);

	NESTED(int, add1, (product(int, float) ifl)) { return product_car(ifl) + 1; };
	NESTED(int, slen, (string_ptr x)) { return (int)string_length(x); };

	int si2 = sum_choice(si, add1, slen);

	assert(4 == si2);

	nat a = NAT(3000);
	nat b = NAT(1 << 20);

	nat c = nat_mul(a, b);
	nat d = nat_mul(c, b);
	nat e = nat_add(d, a);

	printf("%s\n", string_cstr(nat_2string(e)));

	free(a);
	free(b);
	free(c);
	free(d);
	free(e);

	rbtree(product_name(int, int)) rb = { };

	NESTED(int, cmp3, (const product(int, int)* a, const product(int, int)* b))
	{
		return cmp(&product_car(*a), &product_car(*b));
	};

	rbtree_set_compare(&rb, cmp3);

	unsigned int m31 = (1u << 31) - 1u;
	assert(m31 == 0x7FFFFFFFU);

	unsigned int b2 = 3;
	for (int i = 0; i != 100000; i++, b2 = (b2 * 3u) % m31)
		rbtree_insert(&rb, product_init(int, int, (b2, i)));

	int prev = 0;
	for (auto n = rbtree_first(&rb); NULL != n; n = rbtree_node_next(n)) {

		product(int, int) x = rbtree_node_access(n);
		assert(prev < product_car(x));
		prev = product_car(x);
	}

	for (rbtree_node_type(&rb)* last; (last = rbtree_last(&rb)); )
		rbtree_delete(&rb, rbtree_node_access(last));
}





