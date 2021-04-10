/* Copyright 2021. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#ifndef __VEC_H
#define __VEC_H 1

#include "core.h"


// I am happy with int at the moment.
typedef int vec_size_t;

#define vec(T) struct vec_ ## T
#define vec_decl(T) vec(T) { vec_size_t N; T data[]; }

#define vec_eltype(T) typeof((T)->data[0])
#define vec_length(T) ((T)->N)

#if 0
// -fsanitize=bounds,null
#define CHECK(x)
#else
#define CHECK(x) if (!(x)) abort()
#endif

#define NULL_CHECK(x) ({ auto __x = (x); if (!__x) abort(); __x; })

#define vec_sizeof(T) 							\
({									\
 	auto __T = (T);							\
	sizeof(__T) + __T->N * sizeof(vec_eltype(__T));			\
})

#define vec_realloc(T, M)						\
({									\
	auto __Ta = (T);						\
	(*__Ta)->N = (M);						\
	*__Ta = realloc(*__Ta, vec_sizeof(*__Ta));			\
	*__Ta;								\
})

#define vec_alloc_n(T, M) \
({									\
	vec_size_t __Na = (M);						\
 	vec(T)* __t = malloc(sizeof(vec(T)) + __Na * sizeof(T));	\
	if (__t) __t->N = __Na;						\
	__t;								\
})

#define vec_alloc(T) (vec_alloc_n(T, 0))

#if 0
#define vec_array(T) \
({									\
 	auto __T = (T);							\
	(vec_eltype(__T)(*)[vec_length(__T)])((T)->data);		\
})
#else
// work around a compiler bug
// GCC: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=91038
extern _Thread_local struct vec_a { int N; void* data; } vec_array_tmp;
#define vec_array(T)							\
(*(({									\
 	auto __T = (T);							\
	vec_array_tmp.N = __T->N;					\
	vec_array_tmp.data = __T->data;					\
}), (vec_eltype(T)(*)[vec_array_tmp.N])(vec_array_tmp.data)))
#endif

#define vec_access(T4, _i)						\
(*({									\
 	auto __T4 = (T4);						\
	vec_size_t __i = (_i);						\
	CHECK((0 <= __i) && (__i < vec_length(__T4)));			\
	&(vec_array(__T4)[__i]);					\
}))

#define vec_push(v2, x2) 						\
({ 									\
 	auto __T3 = (v2);						\
	vec_size_t __N = vec_length(*__T3);				\
	NULL_CHECK(vec_realloc(__T3, __N + 1));				\
	vec_access(*__T3, __N) = (x2);					\
})

#define vec_pop(v2) 							\
({ 									\
 	auto __T2 = (v2);						\
	vec_size_t __N = vec_length(*__T2);				\
	CHECK(0 < __N);							\
	auto __r = vec_access(*__T2, __N - 1);				\
	NULL_CHECK(vec_realloc(__T2, __N - 1));				\
 	__r;								\
})

#define vec_sort(v2, cmp)						\
({									\
 	auto __T1 = (v2);						\
	typedef vec_eltype(__T1) __ET;					\
	struct { int (*xcmp)(const __ET*, const __ET*); }		\
		__d = { (cmp) };					\
	int __cmp(const void* a, const void* b, void* _d)		\
	{								\
		typeof(__d)* d = _d;					\
		return d->xcmp((const __ET*)a, (const __ET*)b);		\
	}								\
	qsort_r(__T1->data, __T1->N, sizeof(__ET), __cmp, &__d);	\
})

#endif

