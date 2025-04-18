/* Copyright 2021-2025. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"

typedef char* string_ptr;

#ifndef TAGCOMPAT
list_decl(int);
list_decl(string_ptr);
#endif


void list_int()
{
	list(int) l = { 0 };

	for (int i = 0; i < 10; i++)
		list_push(&l, i);

	list_node(int)* n = list_first(&l);

	for (int i = 0; i < 5; i++)
		n = list_node_next(n);

	assert(4 == list_node_access(n));

	int c = 9;

	while (!list_empty(&l))
		if (c-- != list_pop(&l))
			abort();
}

void list_string()
{
	list(string_ptr) r = { 0 };
	list_push(&r, "Dir?");
	list_push(&r, "es");
	list_push(&r, "geht");
	list_push(&r, "Wie");
	list_push(&r, "Hallo!");

	if (0 != strcmp("Hallo!", list_node_access(list_first(&r))))
		abort();

	while (!list_empty(&r))
		list_pop(&r);
}

int main()
{
	list_int();
	list_string();

	return 0;
}





