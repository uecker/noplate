/* Copyright 2021-2023. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#include "vec.h"

#if (GCC_VERSION < 110300) && !defined __clang__
_Thread_local struct vec_a vec_array_tmp = { 0 };
#endif

#ifdef __clang__
typedef int CLOSURE_TYPE(qsort_cmp_func_t)(const void*, const void*, void*);
struct qsort_frw_data { qsort_cmp_func_t fun; void* data; };
static int qsort_block_forward(const void* a, const void* b, void* _d)
{
	struct qsort_frw_data* data = _d;
	return data->fun(a, b, data->data);
}
extern void noplate_qsort_blocks(void* ptr, size_t N, size_t si, cmp, void* data)
{
	qsort_r(ptr, N, si, qsort_block_forward, &(struct qsort_frw_data){ cmp, data });
}
#endif

