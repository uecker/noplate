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


#define mdarray_init(R, T, X) 						\
({	auto _s = &(X);							\
 	mdarray(R, T) _v;						\
	TYPE_CHECK(T, nil(array_nested_eltype(*_s)));			\
	_Static_assert(mdarray_rank(_v) == array_rank((*_s)), "");	\
	_v = (typeof(_v)){ (void*)_s, { array_dims(R, (*_s)) } };	\
	_v;								\
})

#endif	// _MDARRAY_H


