/* Copyright 2021-2022. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#ifndef __NAT_H
#define __NAT_H

#include "vec.h"

typedef unsigned int nat_base_t;
vec_decl(nat_base_t);

typedef vec(nat_base_t)* nat;


inline nat nat_alloc(void)
{ 
	return vec_alloc(nat_base_t);
}

inline int net_length(const nat x)
{
	return vec_length(x);
}

extern nat nat_init(int x);

#define NAT(x) (nat_init(x))

extern nat nat_dup(const nat x);

extern nat nat_mul(const nat a, const nat b);
extern nat nat_div(const nat a, const nat b);
extern nat nat_add(const nat a, const nat b);
extern nat nat_sub(const nat a, const nat b);

#ifndef STRING
vec_decl(char);
typedef vec(char)* string;
#endif

extern string* nat_2string(const nat x);


#endif // __NAT_H

