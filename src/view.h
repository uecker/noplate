/* Copyright 2022. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#ifndef __VIEW_H
#define __VIEW_H 1

#include "core.h"

#ifdef TAGCOMPAT
#define view(T) struct CONCAT(view_, T) { ssize_t N; T* data; }
#define view_decl(T)
#else
#define view(T) struct CONCAT(view_, T)
#define view_decl(T) view(T) { ssize_t N; T* data; }
#endif


#define array_view(T, x)					\
({	auto __y = &(x);					\
	(view(T)){ array_lengthof(*__y), &(*__y)[0] };		\
})

#define vec_view(T, x)	array_view(T, vec_array(T, x))

#define vec_append(T, a, b) 						\
({									\
	vec(T) **__v = (a);						\
	view(T) __w = (b);						\
	ssize_t alen = vec_length(T, *__v);				\
	ssize_t blen = vec_length(T, &__w);				\
	vec_realloc(T, __v, alen + blen);				\
	if (NULL != *__v) {						\
		memcpy(&vec_access(T, *__v, alen), 			\
			&vec_access(T, &__w, 0), blen * sizeof(T));	\
	}								\
	*__v;								\
})


#endif

