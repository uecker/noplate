/* Copyright 2021. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include "vec.h"

#if (GCC_VERSION < 110300) && !defined __clang__
_Thread_local struct vec_a vec_array_tmp = { 0 };
#endif

#ifdef __clang__
extern inline int qsort_block_forward(const void* a, const void* b, void* _d);
#endif

