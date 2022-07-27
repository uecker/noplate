/* Copyright 2021-2022. Martin Uecker
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


#ifndef TAGCOMPAT
vec_decl(int);
typedef string* string_ptr;
vec_decl(string_ptr);
view_decl(int);
view_decl(float);
list_decl(int);
list_decl(string_ptr);
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

	vec_push(&v, 1);
	vec_push(&v, 3);
	vec_push(&v, 8);

	vec(string_ptr)* s = NULL_CHECK(vec_alloc(string_ptr));

	vec_push(&s, string_init(" Du!"));
	vec_push(&s, string_init("Hallo"));

	vec_access(v, 1)++;


	float bb[4];
	view(float) vf = array_view(float, bb);

	vec_access(&vf, 1) = 1.;

	view(int) vi = vec_view(int, v);

	vec_access(&vi, 1)++;

	assert(5 == vec_array(&vi)[1]);

#if 0
	// these should all fail
	vec_push(vi, 3);
	vec_push(&vi, 3);
	vec_push((view(int)*){ &vi }, 3);
	vec_push(&(view(int)*){ &vi }, 3);
#endif

#ifndef __clang__
	int cmp(const int* a, const int* b)
	{ 
		return *b - *a;
	}

	vec_sort(v, cmp);

	int cmp2(const string_ptr* a, const string_ptr* b)
	{ 
		return strcmp(string_cstr(*a), string_cstr(*b));
	}

	vec_sort(s, cmp2);
#endif
	string* ss = NULL_CHECK(string_alloc());
    
	for (int i = 0; i < (int)vec_length(s); i++) {

		string* t = string_concat(ss, vec_access(s, vec_length(s) - 1 - i));
		free(ss);
		ss = t;
	}

	const string* s2 = ss;
	const char* x = string_cstr(s2);

	printf("%s %d\n", x, vec_access(v, 0));

	assert(vec_length(s) == array_lengthof(vec_array(s)));

	free(v);
	free(ss);

	while (0 < vec_length(s))
		free(vec_pop(&s));

	free(s);

	/*const*/ string* s3 = string_init("hallo");

	// this assignment is not checked by compilers
	// char (*slice)[3]
	auto slice = &array_slice(string_cstr(s3), 1, 1 + 3);

	(*slice)[0] = 'A';
	(*slice)[1] = 'L';
	(*slice)[2] = 'L';

	printf("%s\n", string_cstr(s3));


	char buf[100];

	poke(int, &array_slice(buf, 11, 11 + sizeof(int)), 3);
	int i = peek(int, &array_slice(buf, 11, 11 + sizeof(int)));

	assert(3 == i);


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
}





