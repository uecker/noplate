/* Copyright 2022-2025. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#ifndef _NOPLATE_SPAN_H
#define _NOPLATE_SPAN_H 1

#include "core.h"

#ifdef TAGCOMPAT
#define span(T) struct CONCAT(span_, T) { ssize_t N; T* data; }
#define span_decl(T)
#else
#define span(T) struct CONCAT(span_, T)
#define span_decl(T) span(T) { ssize_t N; T* data; }
#endif


#define vec2span(T, x)	array2span(T, vec2array(T, x))

#define span_length(T, x) vec_length(T, TYPE_CHECK(span(T)*, x))
#define span2array(T, x) vec2array(T, TYPE_CHECK(span(T)*, x))
#define span_access(T, x, i) vec_access(T, TYPE_CHECK(span(T)*, x), i)

#define vec_append(T, a, b) 						\
({									\
	vec(T) **__v = (a);						\
	span(T) __w = (b);						\
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

