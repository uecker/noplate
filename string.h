/* Copyright 2021. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#ifndef __STRING_H
#define __STRING_H

#include "vec.h"

vec_decl(char);

typedef vec(char)* string;

#define string_check(x) ({ string __x = (x); CHECK('\0' == vec_array(__x)[string_length(__x)]); __x; })

#define string_cstr(x) (vec_array(string_check(x)))

inline string string_alloc(void)
{ 
	string s = vec_alloc(char);

	if (NULL == s)
		goto err;

	vec_push(&s, '\0');
err:
	return s;
}

inline int string_length(const string x)
{
	return vec_length(x) - 1;
}

extern string string_init(const char* c);

#define STRING(x) (string_init(x))

extern string string_dup(const string x);
extern string string_concat(const string a, const string b);
extern string string_printf(const char* fmt, ...);


#endif // __STRING_H

