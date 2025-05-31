/* Copyright 2025. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include <stdlib.h>

#include "variadic.h"

variadic_decl(int);

void test_variadic()
{
	variadic *v = variadic_make(int, 3);
	
	if (3 != variadic_access(int, v))
		abort();
}


int main()
{
	test_variadic();

	return 0;
}

