
#include "core.h"

#define array(T, N) typeof(typeof(T)[N])
#define array_lengthof(x) 					\
(sizeof *({ 							\
	auto __x = &(x);					\
	(void)TYPE_CHECK(typeof((*__x)[0])(*)[], __x);		\
	char (*__tmp)[(sizeof(*__x) / sizeof((*__x)[0]))];	\
	 __tmp;							\
}))
	

#define array_slice(x, start, end)				\
(*({								\
	auto __x = &(x);					\
   	ssize_t __start = (start);				\
	ssize_t __end = (end);					\
	CHECK(__start >= 0);					\
	CHECK(__end >= __start);				\
	CHECK(__end <= array_lengthof(*__x));			\
	(array(typeof((*__x)[0]), __end - __start)*)&(*__x)[__start];	\
}))


