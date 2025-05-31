
#include "core.h"

typedef struct variadic_id { } *variadic;

#ifdef TAGCOMPAT
#define variadic(T) struct CONCAT(val_, T) { variadic super; T value; }
#define variadic_decl(T)
#else
#define variadic(T) struct CONCAT(val_, T)
#define variadic_decl(T) variadic(T) { variadic super; T value; }
#endif

#define _variadic_id(T) ({ extern struct variadic_id CONCAT(variadic_id_, T); &CONCAT(variadic_id_, T); })
#define variadic_access(T, x) 				\
(*({							\
	auto _x = (x);					\
	CHECK(*_x == _variadic_id(T));			\
	&containerof(_x, variadic(T), super)->value;	\
}))
#define variadic_make(T, x) &((variadic(T)){ _variadic_id(T), (x) }.super)

