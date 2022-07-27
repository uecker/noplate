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

typedef vec(char) string;
typedef view(char) string_view;

#define string_check(x)	\
({ 									\
	auto __x = (x);							\
	(void)_Generic(__x, string*: 1,	const string*: 1,		\
			string_view*: 1, const string_view*: 1); 	\
	CHECK('\0' == vec_array(__x)[string_length(__x)]); __x; 	\
})

#define string_cstr(x) (vec_array(string_check(x)))
#define string_view(x) (vec_view(char, x))
#define string_length(x)	(vec_length(x) - 1)

inline string* string_alloc(void)
{ 
	string* s = vec_alloc(char);

	if (NULL == s)
		goto err;

	vec_push(&s, '\0');
err:
	return s;
}


extern string* string_init(const char* c);

#define STRING(x) (string_init(x))

extern string* string_dup(const string_view x);
extern string* string_concat(const string_view a, const string_view b);
extern string* string_printf(const char* fmt, ...);


#endif // __STRING_H

