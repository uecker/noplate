/* Copyright 2021. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>



#ifndef auto
#define auto __auto_type
#endif

#ifndef typeof
#define typeof(X) __typeof(X)
#endif


#define _Ptr(x) typeof(typeof(x)*)
#define _Array(x, N) typeof(typeof(x)[N])


#define TYPE_CHECK(T, x)	(1 ? (x) : (T){0})
#define containerof(x, T, member)	\
	((T*)((char*)TYPE_CHECK(typeof(&((T*)0)->member), x) - offsetof(T, member)))

#define choose_type(c, A, B)	typeof(*_Generic(&(int[1 + !!(c)]){ 0 }, int(*)[2]: (typeof(A)*)0, int(*)[1]: (typeof(B)*)0))


#if 0
// -fsanitize=bounds,null
#define CHECK(x)
#else
#define CHECK(x) if (!(x)) abort()
#endif

extern void xfree(const void*);

