/* Copyright 2021-2024. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

#if __STDC_VERSION__ >= 202000
#if (GCC_VERSION >= 140000) && !defined __clang__
#define TAGCOMPAT
#endif
#endif

#if (GCC_VERSION >= 150000) && !defined __clang__
#define COUNTED_BY(N) gnu::counted_by(N)
#else
#define COUNTED_BY(N)
#endif


#define _CONCAT1(A, B) A ## B
#define CONCAT(A, B) _CONCAT1(A, B)

#ifndef auto
#define auto __auto_type
#endif

#ifndef typeof
#define typeof(X) __typeof(X)
#endif

#define compound_literal(T, x)  (struct { T data; }){ x }.data

#define _Ptr(x) typeof(typeof(x)*)
#define _Array(x, N) typeof(typeof(x)[N])



#define TYPE_CHECK(T, x)	(1 ? (x) : (T){0})
#define containerof(x, T, member)	\
	((T*)((char*)TYPE_CHECK(typeof(&((T*)0)->member), x) - offsetof(T, member)))

#define nil(T) ((typeof(T)){ 0 })
#define nil2(T) ((typeof(T)){ { 0 } })

#define CONST(x) (struct { const typeof(x) data; }){ x }.data

#define NODECL(T) typeof(({ nil(T); }))

//#define nil(T) (*((typeof(T)*){ 0 }))
#define same_type_p(T, x) _Generic(nil(typeof(x)*), typeof(T)*: 1, default: 0)
#define has_type_p(T, x) _Generic(nil(typeof(x)*), typeof(T)*: 1, default: 0)

// seems to work on both clang and gcc
#define lvalue_convert(x) ((void)0, x)

#define function_or_array_p(T) 					\
	(   (!has_type_p(lvalue_convert(T), T)) 		\
	 && (!has_type_p(const typeof(lvalue_convert(T)), T)))

#define choose_expr(c, A, B)	(_Generic(&(int[1 + !!(c)]){ 0 }, int(*)[2]: (A), int(*)[1]: (B)))
#define choose_type(c, A, B)	typeof(*_Generic(&(int[1 + !!(c)]){ 0 }, int(*)[2]: (typeof(A)*)0, int(*)[1]: (typeof(B)*)0))
#define choose_ice(c, A, B)	sizeof(choose_type(c, nil(char[(A)]), nil(char[(B)])))

#if defined(__clang__) && !defined(__CUDACC__)
#if __has_extension(blocks)
#define NESTED(RET, NAME, ARGS) \
	RET (^NAME)ARGS = ^ARGS
#define CLOSURE_TYPE(x) (^x)
#endif
#else
#define NESTED(RET, NAME, ARGS) \
	RET NAME ARGS
#define CLOSURE_TYPE(x) (*x)
#define __block
#endif

#define __CONCAT0(x, y) x ## y
#define __CONCAT1(x, y) __CONCAT0(x, y)
#define __UNIQ CONCAT(__x,  __COUNTER__)

#if 0
// -fsanitize=bounds,null
#define CHECK(x)
#else
#define CHECK(x) if (!(x)) abort()
#endif

extern void xfree(const void*);

