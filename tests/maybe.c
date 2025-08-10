/* Copyright 2021-2025. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include <stdlib.h>

#include "maybe.h"

#ifndef TAGCOMPAT
maybe_decl(int);
#endif


void test_maybe()
{
	maybe(int) m = maybe_nothing(int);

	if (m.ok)
		abort();

	m = maybe_just(int, 3);

	if (!m.ok) 
		abort();

	if (3 != maybe_value(int, m))
		abort();
}


int main()
{
	test_maybe();

	return 0;
}

