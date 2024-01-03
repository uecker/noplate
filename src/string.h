/* Copyright 2021-2022. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#ifndef __STRING_H
#define __STRING_H

#include "vec.h"
#include "view.h"

vec_decl(char);
view_decl(char);

// #define STRING_OPAQUE
#ifdef STRING_OPAQUE
struct string;
typedef vec(char) string_priv;
typedef struct string string;
#else
typedef vec(char) string;
typedef string string_priv;
#endif
typedef view(char) string_view;

#define string_check(x)	\
({ 									\
	auto __x = (x);							\
	CHECK('\0' == vec_array(char, __x)[string_length(__x)]); __x; 	\
})

#ifdef STRING_OPAQUE
#define STRING_UNWRAP(x)						\
({ 									\
	auto __y = (x);							\
	_Generic(__y,							\
		string_priv*: __y, 					\
		const string_priv*: __y, 				\
		string*: (string_priv*)__y,				\
		const string*: (const string_priv*)__y,			\
		string_view*: __y,					\
		const string_view*: __y);				\
})
#else
#define STRING_UNWRAP(x) (x)
#endif

#define string_cstr(x)		(vec_array(char, string_check(STRING_UNWRAP(x))))
#define string_view(x) 		(vec_view(char, STRING_UNWRAP(x)))
#define string_length(x)	(vec_length(STRING_UNWRAP(x)) - 1)

inline string* string_alloc(void)
{ 
	string_priv* s = vec_alloc(char);

	if (NULL == s)
		goto err;

	vec_push(char, &s, '\0');
err:
	return (string*)s;	// !
}


extern string* string_init(const char* c);

#define STRING(x) (string_init(x))

extern string* string_dup(const string_view x);
extern string* string_concat(const string_view a, const string_view b);
extern string* string_printf(const char* fmt, ...);


#endif // __STRING_H

