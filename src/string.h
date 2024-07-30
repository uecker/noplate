/* Copyright 2021-2024. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#ifndef __STRING_H
#define __STRING_H

#include "vec.h"
#include "view.h"

typedef unsigned char char8_t;
vec_decl(char8_t);
view_decl(char8_t);
view_decl(char);

// #define STRING_OPAQUE
#ifdef STRING_OPAQUE
struct string;
typedef vec(char8_t) string_priv;
typedef struct string string;
#else
typedef vec(char8_t) string;
typedef string string_priv;
#endif
typedef view(char8_t) strview;

#define string_check(x)	\
({ 									\
	auto __s = (x);							\
	CHECK('\0' == vec_array(char8_t, __s)[string_length(__s)]); __s; \
})

#ifdef STRING_OPAQUE
#define STRING_UNWRAP(x)						\
({									\
	auto __y = (x);							\
	_Generic(__y,							\
		string_priv*: __y, 					\
		const string_priv*: __y, 				\
		string*: (string_priv*)__y,				\
		const string*: (const string_priv*)__y,			\
		strview*: __y,						\
		const strview*: __y);					\
})
#else
#define STRING_UNWRAP(x) (x)
#endif

#define string_utf8(x)		(vec_array(char8_t, string_check(STRING_UNWRAP(x))))
#define string_cstr(x)		(array_cast(char, string_utf8(x)))
#define string_view(x) 		(vec_view(char8_t, STRING_UNWRAP(x)))
#define string_length(x)	(vec_length(STRING_UNWRAP(x)) - 1)


inline string *string_alloc(void)
{ 
	string_priv *s = vec_alloc(char8_t);

	if (NULL == s)
		goto err;

	vec_push(char8_t, &s, '\0');
err:
	return (string*)s;	// !
}


extern string* string_init(const char* c);

#define STRING(x) (string_init(x))

extern string *string_dup(const string *s);
extern string *string_concat(const string *a, const string *b);
extern void string_append(string * restrict *a, const string *b);
extern string *string_printf(const char* fmt, ...);
extern void string_puts(string **a, const char *cstr);

extern void string_append_view(string * restrict *a, const strview b);
extern string *strview_dup(const strview v);
extern string *strview_concat(const strview a, const strview b);

#endif // __STRING_H

