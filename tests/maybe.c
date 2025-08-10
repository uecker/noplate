/* Copyright 2021-2025. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include <stdlib.h>
#include <stdbool.h>

#include "maybe.h"

#ifndef TAGCOMPAT
maybe_decl(int);
maybe_decl(bool);
maybe_decl(double);
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

bool one_p(double x)
{
	return (1. == x);
}

void test_maybe2()
{
	maybe(double) s = maybe_just(double, 1.);

	auto mone_p = maybe_map(bool, double, one_p);

	maybe(bool) r = mone_p(s);

	if (!r.ok || !maybe_value(bool, r))
		abort();
}


int main()
{
	test_maybe();
	test_maybe2();

	return 0;
}

