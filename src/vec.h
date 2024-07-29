/* Copyright 2021-2024. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#ifndef __VEC_H
#define __VEC_H 1

#include "core.h"


#ifdef TAGCOMPAT
#define vec(T) CONCAT(struct vec_, T) { ssize_t N; [[COUNTED_BY(N)]] T data[]; }
#define vec_decl(T)
#else
#define vec(T) CONCAT(struct vec_, T)
#define vec_decl(T) vec(T) { ssize_t N; [[COUNTED_BY(N)]] T data[]; }
#endif

#define vec_eltype(T) typeof((T)->data[0])
#define vec_length(T) ((size_t)((T)->N))


#define NULL_CHECK(x) ({ auto __x = (x); if (!__x) abort(); __x; })

#define vec_sizeof(T, x) (sizeof(vec(T)) + (x)->N * sizeof(T))

#define vec_realloc(T, x, M)						\
({									\
	vec(T) **__Ta = (x);						\
	(void)TYPE_CHECK(typeof(typeof(vec_eltype(*__Ta)[])*), 		\
			&(*__Ta)->data);				\
	(*__Ta)->N = (M);						\
	*__Ta = realloc(*__Ta, vec_sizeof(T, *__Ta));			\
	*__Ta;								\
})

#define vec_calloc_n(T, M) \
({									\
	ssize_t __Na = (M);						\
	vec(T)* __t = calloc(1, sizeof(vec(T)) + __Na * sizeof(T));	\
	if (__t) __t->N = __Na;						\
	__t;								\
})

#define vec_alloc_n(T, M) \
({									\
	ssize_t __Na = (M);						\
 	vec(T)* __t = malloc(sizeof(vec(T)) + __Na * sizeof(T));	\
	if (__t) __t->N = __Na;						\
	__t;								\
})

#define vec_alloc(T) (vec_alloc_n(T, 0))


#if (GCC_VERSION >= 110300) || defined __clang__
#define vec_array(T, x) \
(*({									\
	auto __x = (x);							\
	_Static_assert(same_type_unq_p(T, vec_eltype(__x)), "");	\
	(vec_eltype(__x)(*)[vec_length(__x)])((__x)->data);		\
}))
#else
// work around a compiler bug
// GCC: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=91038
extern _Thread_local struct vec_a { ssize_t N; const void* data; } vec_array_tmp;
#define vec_array(T, x)							\
(*(({									\
	auto __x = (x);							\
	_Static_assert(same_type_unq_p(T, (vec_eltype(__x)), "");	\
	vec_array_tmp.N = __x->N;					\
	vec_array_tmp.data = __x->data;					\
}), (vec_eltype(x)(*)[vec_array_tmp.N])(vec_array_tmp.data)))
#endif

#define vec_access(T, x4, i)						\
(*({									\
 	auto __x4 = (x4);						\
	ssize_t __i = (i);						\
	CHECK((0 <= __i) && (__i < (ssize_t)vec_length(__x4)));		\
	&(vec_array(T, __x4)[__i]);					\
}))

#define vec_push(T, v2, x2) 						\
({ 									\
	vec(T) **__T3 = (v2);						\
	ssize_t __N = vec_length(*__T3);				\
	NULL_CHECK(vec_realloc(T, __T3, __N + 1));			\
	vec_access(T, *__T3, __N) = (x2);				\
})

#define vec_pop(T, v2) 							\
({ 									\
	vec(T) **__T2 = (v2);						\
	ssize_t __N = vec_length(*__T2);				\
	CHECK(0 < __N);							\
	auto __r = vec_access(T, *__T2, __N - 1);			\
	NULL_CHECK(vec_realloc(T, __T2, __N - 1));			\
 	__r;								\
})

#ifndef __clang__
#define noplate_qsort(ptr, N, si, cmp, data) qsort_r(ptr, N, si, cmp, data)
#else
#ifdef CLOSURE_TYPE
typedef int CLOSURE_TYPE(noplate_qsort_cmp_func_t)(const void*, const void*, void*);
extern void noplate_qsort(void* ptr, size_t N, size_t si, noplate_qsort_cmp_func_t cmp, void* data);
#endif
#endif

#define vec_sort(T, v2, cmp)						\
({									\
 	auto __T1 = (v2);						\
	typedef vec_eltype(__T1) __ET;					\
	_Static_assert(same_type_unq_p(T, __ET), "");			\
	struct { int CLOSURE_TYPE(xcmp)(const __ET*, const __ET*); }	\
		__d = { (cmp) };					\
	NESTED(int, __cmp, (const void* a, const void* b, void* _d))	\
	{								\
		typeof(__d)* d = _d;					\
		return d->xcmp((const __ET*)a, (const __ET*)b);		\
	};								\
	noplate_qsort(__T1->data, __T1->N, sizeof(__ET), __cmp, &__d);	\
})

#endif

