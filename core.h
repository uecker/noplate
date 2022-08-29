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

#define nil(T) ((typeof(T)){ 0 })
#define nil2(T) ((typeof(T)){ { 0 } })

#define NODECL(T) typeof(({ nil(T); }))

//#define nil(T) (*((typeof(T)*){ 0 }))
#define has_type_p(T, x) _Generic(nil(typeof(x)*), typeof(T)*: 1, default: 0)

// seems to work on both clang and gcc
#define lvalue_convert(x) ((void)0, x)

#define function_or_array_p(T) 					\
	(   (!has_type_p(lvalue_convert(T), T)) 		\
	 && (!has_type_p(const typeof(lvalue_convert(T)), T)))

#define choose_expr(c, A, B)	(_Generic(&(int[1 + !!(c)]){ 0 }, int(*)[2]: (A), int(*)[1]: (B)))
#define choose_type(c, A, B)	typeof(*_Generic(&(int[1 + !!(c)]){ 0 }, int(*)[2]: (typeof(A)*)0, int(*)[1]: (typeof(B)*)0))
#define choose_ice(c, A, B)	sizeof(choose_type(c, nil(char[(A)]), nil(char[(B)])))

#if 0
// -fsanitize=bounds,null
#define CHECK(x)
#else
#define CHECK(x) if (!(x)) abort()
#endif

extern void xfree(const void*);

