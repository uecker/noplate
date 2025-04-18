/* Copyright 2022-2025. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */


#include "core.h"

#define array(T, N) typeof(typeof(T)[N])
#define array_lengthof(x) 					\
(sizeof *({ 							\
	auto __x = &(x);					\
	(void)TYPE_CHECK(typeof((*__x)[0])(*)[], __x);		\
	char (*__tmp)[(sizeof(*__x) / sizeof((*__x)[0]))] = 0;	\
	__tmp;							\
}))
	

#define array_slice(x, start, end)				\
(*({								\
	auto __y = &(x);					\
   	ssize_t __start = (start);				\
	ssize_t __end = (end);					\
	CHECK(__start >= 0);					\
	CHECK(__end >= __start);				\
	CHECK((size_t)__end <= array_lengthof(*__y));		\
	(array(typeof((*__y)[0]), __end - __start)*)&(*__y)[__start];	\
}))


#define array2span(T, x)					\
({	auto __y = &(x);					\
	(span(T)){ array_lengthof(*__y), &(*__y)[0] };		\
})

#define array_cast(S, x)  					\
*({								\
	auto __y = &(x);					\
	typedef typeof(S) __S;					\
	CHECK(sizeof(__S) == sizeof((*__y)[0]));		\
	(__S(*)[array_lengthof(*__y)])__y;			\
})

#define ptr2array(N, p)						\
*({								\
	auto __p = (p);						\
	auto __N = (N);						\
	size_t s = __builtin_dynamic_object_size(__p, 0);	\
	typedef typeof(*__p) eltype;				\
	CHECK(s != (size_t)-1);					\
	CHECK(sizeof(eltype[__N]) <= s);			\
	(eltype (*)[__N])__p;					\
})


// note, this can not fail for non-arrays
#define _array_element(x)									\
(*({												\
	auto _q = &(x);										\
	&choose_expr(function_or_array_p(*_q), (*_q), nil(int[1]))[0];				\
}))


// FIXME: should fail if not an error
#define array_eltype(x) typeof(_array_element(x))



#define _array_eltype9(T)		choose_type(function_or_array_p(T), struct _mdarray_error, typeof(T))
#define _array_eltype8(T)		choose_type(function_or_array_p(T), _array_eltype9(nil(array_eltype(T))), typeof(T))
#define _array_eltype7(T)		choose_type(function_or_array_p(T), _array_eltype8(nil(array_eltype(T))), typeof(T))
#define _array_eltype6(T)		choose_type(function_or_array_p(T), _array_eltype7(nil(array_eltype(T))), typeof(T))
#define _array_eltype5(T)		choose_type(function_or_array_p(T), _array_eltype6(nil(array_eltype(T))), typeof(T))
#define _array_eltype4(T)		choose_type(function_or_array_p(T), _array_eltype5(nil(array_eltype(T))), typeof(T))
#define _array_eltype3(T)		choose_type(function_or_array_p(T), _array_eltype4(nil(array_eltype(T))), typeof(T))
#define _array_eltype2(T)		choose_type(function_or_array_p(T), _array_eltype3(nil(array_eltype(T))), typeof(T))
#define _array_eltype1(T)		choose_type(function_or_array_p(T), _array_eltype2(nil(array_eltype(T))), typeof(T))
#define _array_eltype0(T)		choose_type(function_or_array_p(T), _array_eltype1(nil(array_eltype(T))), typeof(T))
#define array_nested_eltype(X)		typeof(*({ auto _p = &(X); typedef _array_eltype0((*_p)) _eltype; nil(_eltype*); }))

#define _array_rank9(T)		choose_ice(function_or_array_p(T), 1, 1)
#define _array_rank8(T)		choose_ice(function_or_array_p(T), 1 + _array_rank9(nil(array_eltype(T))), 1)
#define _array_rank7(T)		choose_ice(function_or_array_p(T), 1 + _array_rank8(nil(array_eltype(T))), 1)
#define _array_rank6(T)		choose_ice(function_or_array_p(T), 1 + _array_rank7(nil(array_eltype(T))), 1)
#define _array_rank5(T)		choose_ice(function_or_array_p(T), 1 + _array_rank6(nil(array_eltype(T))), 1)
#define _array_rank4(T)		choose_ice(function_or_array_p(T), 1 + _array_rank5(nil(array_eltype(T))), 1)
#define _array_rank3(T)		choose_ice(function_or_array_p(T), 1 + _array_rank4(nil(array_eltype(T))), 1)
#define _array_rank2(T)		choose_ice(function_or_array_p(T), 1 + _array_rank3(nil(array_eltype(T))), 1)
#define _array_rank1(T)		choose_ice(function_or_array_p(T), 1 + _array_rank2(nil(array_eltype(T))), 1)
#define _array_rank0(T)		choose_ice(function_or_array_p(T), 1 + _array_rank1(nil(array_eltype(T))), 1)
#define array_rank(X)		(sizeof(*({ auto _p = &(X); enum { _N = _array_rank0((*_p)) }; nil(char(*)[_N]); })) - 1)

#define _array_dims0(T)
#define _array_dims1(T)		 sizeof(T) / sizeof(_array_element(T))
#define _array_dims2(T)		 _array_dims1(_array_element(T)), sizeof(T) / sizeof(_array_element(T))
#define _array_dims2(T)		 _array_dims1(_array_element(T)), sizeof(T) / sizeof(_array_element(T))
#define _array_dims3(T)		 _array_dims2(_array_element(T)), sizeof(T) / sizeof(_array_element(T))
#define _array_dims4(T)		 _array_dims3(_array_element(T)), sizeof(T) / sizeof(_array_element(T))
#define _array_dims5(T)		 _array_dims4(_array_element(T)), sizeof(T) / sizeof(_array_element(T))
#define _array_dims6(T)		 _array_dims5(_array_element(T)), sizeof(T) / sizeof(_array_element(T))
#define _array_dims7(T)		 _array_dims6(_array_element(T)), sizeof(T) / sizeof(_array_element(T))
#define _array_dims8(T)		 _array_dims7(_array_element(T)), sizeof(T) / sizeof(_array_element(T))
#define _array_dims9(T)		 _array_dims8(_array_element(T)), sizeof(T) / sizeof(_array_element(T))
#define array_dims(R, T)		_array_dims ## R(T)



