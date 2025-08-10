/* Copyright 2021-2025. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#ifndef _NOPLATE_VEC_H
#define _NOPLATE_VEC_H 1

#include "core.h"


#ifdef TAGCOMPAT
#define vec(T) struct vec_##T { ssize_t N; [[COUNTED_BY(N)]] T data[]; }
#define vec_decl(T)
#else
#define vec(T) struct vec_##T
#define vec_decl(T) vec(T) { ssize_t N; [[COUNTED_BY(N)]] T data[]; }
#endif

#define vec_eltype(T, x) typeof((x)->data[0])
#define vec_length(T, x) ((size_t)((x)->N))
//#define vec_length(T, x) ((size_t)(TYPE_CHECK(typeof(vec(T)*), x)->N))

#define NULL_CHECK(x) ({ auto __x = (x); if (!__x) abort(); __x; })

#define _vec_sizeof(T, N) ((ssize_t)sizeof(vec(T)) + (N) * (ssize_t)sizeof(T))
#define vec_sizeof(T, x) _vec_sizeof(T, (x)->N)


inline ssize_t vec_capacity_auto(ssize_t x)
{
	ssize_t dw = x & 0xFFFF;
	ssize_t up = x - dw;

	if (!dw)
		return x;
#if 1
	// round up to power of two, 16 bit
	// fibonacci is said to work better
	dw--;
	for (int i = 0; i < 4; i++)
		dw |= dw >> (1 << i);
	dw++;
#else
	dw = 1u << (_INT_WIDTH_ - _builtin_clz(dw));
#endif
	return up + dw;
}


#define vec_realloc_cap(T, capacity, x, M, C)				\
({									\
	vec(T) **_Ta = (x);						\
	(void)TYPE_CHECK(typeof(typeof(vec_eltype(T, *_Ta)[])*), 	\
			&(*_Ta)->data);					\
	ssize_t *_c = (capacity);					\
	(*_Ta)->N = (M);						\
	if ((*_Ta)->N > *_c) {						\
		*_c = (C);						\
		*_Ta = realloc(*_Ta, _vec_sizeof(T, *_c));		\
	}								\
	*_Ta;								\
})

#define vec_push_cap(T, capacity, v2, x2) 				\
({ 									\
	vec(T) **_T3 = (v2);						\
	ssize_t _N = vec_length(T, *_T3);				\
	ssize_t _C = vec_capacity_auto(_N + 1);				\
	NULL_CHECK(vec_realloc_cap(T, capacity, _T3, _N + 1, _C));	\
	vec_access(T, *_T3, _N) = (x2);					\
})

#define vec_push_auto(T, v2, x2) 					\
({ 									\
	vec(T) **_T4 = (v2);						\
	ssize_t _N = vec_length(T, *_T4);				\
	ssize_t _cap = vec_capacity_auto(_N);				\
	vec_push_cap(T, &_cap, _T4, x2);				\
})

#define vec_fit(T, v2)							\
({ 									\
	vec(T) **_T3 = (v2);						\
	NULL_CHECK(vec_realloc(T, _T3, vec_length(*_T3)));		\
})


#define vec_realloc(T, x, M)						\
({									\
	vec(T) **_Ta = (x);						\
	(void)TYPE_CHECK(typeof(typeof(vec_eltype(T, *_Ta)[])*), 	\
			&(*_Ta)->data);					\
	(*_Ta)->N = (M);						\
	*_Ta = realloc(*_Ta, vec_sizeof(T, *_Ta));			\
	*_Ta;								\
})

#define vec_calloc_n(T, M) 						\
({									\
	ssize_t _Na = (M);						\
	vec(T)* _t = calloc(1, _vec_sizeof(T, _Na));			\
	if (_t) _t->N = _Na;						\
	_t;								\
})

#define vec_alloc_n(T, M) \
({									\
	ssize_t _Na = (M);						\
	vec(T)* _t = malloc(_vec_sizeof(T, _Na));			\
	if (_t) _t->N = _Na;						\
	_t;								\
})

#define vec_alloc(T) (vec_alloc_n(T, 0))


#if (GCC_VERSION >= 110300) || defined __clang__
#define vec2array(T, x) \
(*({									\
	auto _x = (x);							\
	_Static_assert(same_type_unq_p(T, vec_eltype(T, _x)), "");	\
	(vec_eltype(T, _x)(*)[vec_length(T, _x)])((x)->data);		\
}))
#else
// work around a compiler bug
// GCC: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=91038
extern _Thread_local struct vec_a { ssize_t N; const void* data; } vec_array_tmp;
#define vec2array(T, x)							\
(*(({									\
	auto _x = (x);							\
	_Static_assert(same_type_unq_p(T, vec_eltype(T, _x)), "");	\
	vec_array_tmp.N = _x->N;					\
	vec_array_tmp.data = _x->data;					\
}), (vec_eltype(T, x)(*)[vec_array_tmp.N])(vec_array_tmp.data)))
#endif

#define vec_access(T, x4, i)						\
(*({									\
	auto _x4 = (x4);						\
	ssize_t _i = (i);						\
	CHECK((0 <= _i) && (_i < (ssize_t)vec_length(T, _x4)));		\
	&(vec2array(T, _x4)[_i]);					\
}))

#define vec_push(T, v2, x2) 						\
({ 									\
	vec(T) **_T3 = (v2);						\
	ssize_t _N = vec_length(T, *_T3);				\
	NULL_CHECK(vec_realloc(T, _T3, _N + 1));			\
	vec_access(T, *_T3, _N) = (x2);					\
})

#define vec_pop(T, v2) 							\
({ 									\
	vec(T) **_T2 = (v2);						\
	ssize_t _N = vec_length(T, *_T2);				\
	CHECK(0 < _N);							\
	auto _r = vec_access(T, *_T2, _N - 1);				\
	NULL_CHECK(vec_realloc(T, _T2, _N - 1));			\
	_r;								\
})

#define vec_pop_cap(T, capacity, v2)					\
({ 									\
	vec(T) **_T2 = (v2);						\
	ssize_t _N = vec_length(T, *_T2);				\
	CHECK(0 < _N);							\
	ssize_t *_c = (capacity);					\
	ssize_t _C = vec_capacity_auto(_N - 1);				\
	auto _r = vec_access(T, *_T2, _N - 1);				\
	if (_C < *_c) {							\
		NULL_CHECK(vec_realloc(T, _T2, _N - 1));		\
		*_c = _C;						\
	}								\
	_r;								\
})

#define vec_pop_auto(T, v2) 						\
({ 									\
	vec(T) **_T3 = (v2);						\
	ssize_t _N = vec_length(T, *_T3);				\
	ssize_t _cap = vec_capacity_auto(_N);				\
	vec_pop_cap(T, &_cap, _T3);					\
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
	auto _T1 = (v2);						\
	typedef vec_eltype(T, _T1) _ET;					\
	_Static_assert(same_type_unq_p(T, _ET), "");			\
	struct { int CLOSURE_TYPE(xcmp)(const _ET*, const _ET*); }	\
		_d2 = { (cmp) };					\
	NESTED(int, _cmp, (const void* a, const void* b, void* _d))	\
	{								\
		typeof(_d2)* d = _d;					\
		return d->xcmp(a, b);					\
	};								\
	noplate_qsort(_T1->data, _T1->N, sizeof(_ET), _cmp, &_d2);	\
})

#endif

