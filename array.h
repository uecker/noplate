/* Copyright 2022. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */


#include "core.h"

#define array(T, N) typeof(typeof(T)[N])
#define array_lengthof(x) 					\
(sizeof *({ 							\
	auto __x = &(x);					\
	(void)TYPE_CHECK(typeof((*__x)[0])(*)[], __x);		\
	char (*__tmp)[(sizeof(*__x) / sizeof((*__x)[0]))];	\
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


#define array_view(T, x)					\
(*({	auto __y = &(x);					\
	&(view(T)){ array_lengthof(*__y), &(*__y)[0] };		\
}))


// note, this can not fail for non-arrays
#define _array_element(x)									\
(*({												\
	auto _q = &(x);										\
	&choose_expr(function_or_array_p(*_q), (*_q), nil(int[1]))[0];				\
}))


// FIXME: should fail if not an error
#define array_eltype(x) typeof_(array_element(x))


