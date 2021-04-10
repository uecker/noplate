/* Copyright 2021. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>



#ifndef auto
#define auto __auto_type
#endif

#ifndef typeof
#define typeof(X) __typeof(X)
#endif


#define TYPE_CHECK(T, x)	(1 ? (x) : (T)0)
#define container_of(x, T, member)	\
	((T*)((char*)TYPE_CHECK(typeof(&((T*)0)->member), x) - offsetof(T, member)))


extern void xfree(const void*);

