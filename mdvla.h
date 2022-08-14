/* Copyright 2022. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#ifndef _MD_VLA_H
#define _MD_VLA_H	1

#include "array.h"

#ifdef TAGCOMPAT
#define md_vla(R, T) 	struct md_ ## R ## _ ## T { T* data; size_t dims[R]; }
#define md_vla_decl(R, T)
#else
#define md_vla(R, T) struct md_ ## R ## _ ## T
#define md_vla_decl(R, T) md_vla(R, T)	{ T* data; size_t dims[R]; }
#endif


#define md_vla_rank(X)		(array_lengthof((X).dims))
#define md_vla_eltype(X)	typeof((X).data[0])
#define md_vla_dim(X, i)	((X).dims[i])

struct _md_vla_error { char c; };
#define _md_cstr_array9(T, R, dims)	choose_type(R, struct _md_vla_error, T)
#define _md_cstr_array8(T, R, dims)	choose_type(R, array(_md_cstr_array9(T, (R) - 1, dims), dims[(R) - 1]), T)
#define _md_cstr_array7(T, R, dims)	choose_type(R, array(_md_cstr_array8(T, (R) - 1, dims), dims[(R) - 1]), T)
#define _md_cstr_array6(T, R, dims)	choose_type(R, array(_md_cstr_array7(T, (R) - 1, dims), dims[(R) - 1]), T)
#define _md_cstr_array5(T, R, dims)	choose_type(R, array(_md_cstr_array6(T, (R) - 1, dims), dims[(R) - 1]), T)
#define _md_cstr_array4(T, R, dims)	choose_type(R, array(_md_cstr_array5(T, (R) - 1, dims), dims[(R) - 1]), T)
#define _md_cstr_array3(T, R, dims)	choose_type(R, array(_md_cstr_array4(T, (R) - 1, dims), dims[(R) - 1]), T)
#define _md_cstr_array2(T, R, dims)	choose_type(R, array(_md_cstr_array3(T, (R) - 1, dims), dims[(R) - 1]), T)
#define _md_cstr_array1(T, R, dims)	choose_type(R, array(_md_cstr_array2(T, (R) - 1, dims), dims[(R) - 1]), T)
#define _md_cstr_array0(T, R, dims)	choose_type(R, array(_md_cstr_array1(T, (R) - 1, dims), dims[(R) - 1]), T)
#define _md_cstr_array(T, R, dims)	choose_type(R, array(_md_cstr_array0(T, (R) - 1, dims), dims[(R) - 1]), T)

#if (GCC_VERSION >= 110300) || defined __clang__
#define md_vla_array(X)					\
(*({								\
 	auto _p = &(X);						\
	typedef md_vla_eltype(*_p) _eltype;			\
	enum { _rank = md_vla_rank(*_p) };			\
	(_md_cstr_array(_eltype, _rank, _p->dims)*)_p->data;	\
}))
#else
// GCC: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=91038
#error "GCC too old"
#endif

#define _vla_eltype3(T)		choose_type(function_or_array_p(T), struct _md_vla_error, typeof(T))
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
#define _vla_dims1(T)		 sizeof(T) / sizeof(array_element(T))
#define _vla_dims2(T)		 _vla_dims1(array_element(T)), sizeof(T) / sizeof(array_element(T))
#define _vla_dims3(T)		 _vla_dims2(array_element(T)), sizeof(T) / sizeof(array_element(T))
#define _vla_dims4(T)		 _vla_dims3(array_element(T)), sizeof(T) / sizeof(array_element(T))
#define _vla_dims5(T)		 _vla_dims4(array_element(T)), sizeof(T) / sizeof(array_element(T))
#define _vla_dims6(T)		 _vla_dims5(array_element(T)), sizeof(T) / sizeof(array_element(T))
#define _vla_dims7(T)		 _vla_dims6(array_element(T)), sizeof(T) / sizeof(array_element(T))
#define _vla_dims8(T)		 _vla_dims7(array_element(T)), sizeof(T) / sizeof(array_element(T))
#define _vla_dims9(T)		 _vla_dims8(array_element(T)), sizeof(T) / sizeof(array_element(T))
#define vla_dims(R, T)		_vla_dims ## R(T)


#define md_vla_init(R, T, X) 					\
({	auto _s = &(X);						\
 	md_vla(R, T) _v;					\
	TYPE_CHECK(T, nil(vla_eltype(*_s)));			\
	_Static_assert(md_vla_rank(_v) == vla_rank((*_s)), "");	\
	_v = (typeof(_v)){ (void*)_s, { vla_dims(R, (*_s)) } };	\
	_v;							\
})

#endif	// _MD_VLA_H


