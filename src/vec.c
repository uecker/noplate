/* Copyright 2021-2025. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include "vec.h"

#if (GCC_VERSION < 110300) && !defined __clang__
_Thread_local struct vec_a vec_array_tmp = { 0 };
#endif

extern inline ssize_t vec_capacity_auto(ssize_t x);


#ifdef __clang__
struct qsort_frw_data { noplate_qsort_cmp_func_t fun; void* data; };
static int qsort_block_forward(const void* a, const void* b, void* _d)
{
	struct qsort_frw_data* data = _d;
	return data->fun(a, b, data->data);
}
extern void noplate_qsort(void* ptr, size_t N, size_t si, noplate_qsort_cmp_func_t cmp, void* data)
{
	qsort_r(ptr, N, si, qsort_block_forward, &(struct qsort_frw_data){ cmp, data });
}
#endif

