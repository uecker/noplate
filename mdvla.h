/* Copyright 2022. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#ifndef _MD_VLA_H
#define _MD_VLA_H	1

#include "array.h"

#define md_vla(R, T) 			\
struct md_ ## R ## _ ## T {		\
	T* data;			\
	size_t dims[R];			\
}


#define md_vla_rank(X)		array_lengthof((X).dims)
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


#endif	// _MD_VLA_H

