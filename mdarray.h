/* Copyright 2022. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#ifndef _MDARRAY_H
#define _MDARRAY_H	1

#include "array.h"

#ifdef TAGCOMPAT
#define mdarray(R, T) 	struct mdarray_ ## R ## _ ## T { T* data; size_t dims[R]; }
#define mdarray_decl(R, T)
#else
#define mdarray(R, T) struct mdarray_ ## R ## _ ## T
#define mdarray_decl(R, T) mdarray(R, T)	{ T* data; size_t dims[R]; }
#endif


#define mdarray_rank(X)		(array_lengthof((X).dims))
#define mdarray_eltype(X)	typeof((X).data[0])
#define mdarray_dim(X, i)	((X).dims[i])

struct _mdarray_error { char c; };
#define _mdarray_type9(T, R, dims)	choose_type(R, struct _mdarray_error, T)
#define _mdarray_type8(T, R, dims)	choose_type(R, array(_mdarray_type9(T, (R) - 1, dims), dims[(R) - 1]), T)
#define _mdarray_type7(T, R, dims)	choose_type(R, array(_mdarray_type8(T, (R) - 1, dims), dims[(R) - 1]), T)
#define _mdarray_type6(T, R, dims)	choose_type(R, array(_mdarray_type7(T, (R) - 1, dims), dims[(R) - 1]), T)
#define _mdarray_type5(T, R, dims)	choose_type(R, array(_mdarray_type6(T, (R) - 1, dims), dims[(R) - 1]), T)
#define _mdarray_type4(T, R, dims)	choose_type(R, array(_mdarray_type5(T, (R) - 1, dims), dims[(R) - 1]), T)
#define _mdarray_type3(T, R, dims)	choose_type(R, array(_mdarray_type4(T, (R) - 1, dims), dims[(R) - 1]), T)
#define _mdarray_type2(T, R, dims)	choose_type(R, array(_mdarray_type3(T, (R) - 1, dims), dims[(R) - 1]), T)
#define _mdarray_type1(T, R, dims)	choose_type(R, array(_mdarray_type2(T, (R) - 1, dims), dims[(R) - 1]), T)
#define _mdarray_type0(T, R, dims)	choose_type(R, array(_mdarray_type1(T, (R) - 1, dims), dims[(R) - 1]), T)
#define _mdarray_type(T, R, dims)	choose_type(R, array(_mdarray_type0(T, (R) - 1, dims), dims[(R) - 1]), T)

#if (GCC_VERSION >= 110300) || defined __clang__
#define mdarray_array(X)					\
(*({								\
 	auto _p = &(X);						\
	typedef mdarray_eltype(*_p) _eltype;			\
	enum { _rank = mdarray_rank(*_p) };			\
	(_mdarray_type(_eltype, _rank, _p->dims)*)_p->data;	\
}))
#else
// GCC: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=91038
#error "GCC too old"
#endif

#define _vla_eltype3(T)		choose_type(function_or_array_p(T), struct _mdarray_error, typeof(T))
#define _vla_eltype2(T)		choose_type(function_or_array_p(T), _vla_eltype3(nil(array_eltype(T))), typeof(T))
#define _vla_eltype1(T)		choose_type(function_or_array_p(T), _vla_eltype2(nil(array_eltype(T))), typeof(T))
#define _vla_eltype(T)		choose_type(function_or_array_p(T), _vla_eltype1(nil(array_eltype(T))), typeof(T))
#define vla_eltype(X)		typeof(*({ auto _p = &(X); typedef _vla_eltype((*_p)) _eltype; nil(_eltype*); }))

#define _vla_rank3(T)		choose_ice(function_or_array_p(T), 1, 1)
#define _vla_rank2(T)		choose_ice(function_or_array_p(T), 1 + _vla_rank3(nil(array_eltype(T))), 1)
#define _vla_rank1(T)		choose_ice(function_or_array_p(T), 1 + _vla_rank2(nil(array_eltype(T))), 1)
#define _vla_rank(T)		choose_ice(function_or_array_p(T), 1 + _vla_rank1(nil(array_eltype(T))), 1)
#define vla_rank(X)		(sizeof(*({ auto _p = &(X); enum { _N = _vla_rank((*_p)) }; nil(char(*)[_N]); })) - 1)

#define _vla_dims0(T)
#define _vla_dims1(T)		 sizeof(T) / sizeof(_array_element(T))
#define _vla_dims2(T)		 _vla_dims1(_array_element(T)), sizeof(T) / sizeof(_array_element(T))
#define _vla_dims3(T)		 _vla_dims2(_array_element(T)), sizeof(T) / sizeof(_array_element(T))
#define _vla_dims4(T)		 _vla_dims3(_array_element(T)), sizeof(T) / sizeof(_array_element(T))
#define _vla_dims5(T)		 _vla_dims4(_array_element(T)), sizeof(T) / sizeof(_array_element(T))
#define _vla_dims6(T)		 _vla_dims5(_array_element(T)), sizeof(T) / sizeof(_array_element(T))
#define _vla_dims7(T)		 _vla_dims6(_array_element(T)), sizeof(T) / sizeof(_array_element(T))
#define _vla_dims8(T)		 _vla_dims7(_array_element(T)), sizeof(T) / sizeof(_array_element(T))
#define _vla_dims9(T)		 _vla_dims8(_array_element(T)), sizeof(T) / sizeof(_array_element(T))
#define vla_dims(R, T)		_vla_dims ## R(T)


#define mdarray_init(R, T, X) 						\
({	auto _s = &(X);							\
 	mdarray(R, T) _v;						\
	TYPE_CHECK(T, nil(vla_eltype(*_s)));				\
	_Static_assert(mdarray_rank(_v) == vla_rank((*_s)), "");	\
	_v = (typeof(_v)){ (void*)_s, { vla_dims(R, (*_s)) } };		\
	_v;								\
})

#endif	// _MDARRAY_H


