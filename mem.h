
#ifndef _LDST_H
#define _LDST_H	1

#include "core.h"

#define _LDST_MEMCPY	\
	extern void *memcpy(void *dest, const void *src, typeof(sizeof(0)) n)

#define peek(T, ptr) 				\
(*({ 						\
	typedef T _T;				\
	enum { _N = sizeof(_T) };		\
	char (*_ptr)[_N] = (ptr);		\
	_LDST_MEMCPY;				\
       	(_T*)memcpy(&(_T){ 0 }, _ptr, _N);	\
}))

#define poke(T, ptr, x)				\
({						\
 	T _x = (x);				\
	enum { _N = sizeof(_x) };		\
	char (*_ptr)[_N] = (ptr);		\
	_LDST_MEMCPY;				\
 	memcpy(_ptr, &_x, _N);			\
	_x;					\
})


#endif // _LDST_H

