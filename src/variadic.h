
#include "core.h"

typedef struct variadic_id { const char *typeid; } variadic;

#ifdef TAGCOMPAT
#define variadic(T) struct val_##T { variadic super; T value; }
#define variadic_decl(T)
#else
#define variadic(T) struct val_##T
#define variadic_decl(T) variadic(T) { variadic super; T value; }
#endif

#define _variadic_id(T) STRINGIFY(CONCAT(val_, T))
#define variadic_access(T, x) 					\
(*({								\
	auto _x = (x);						\
	CHECK(0 == strcmp(_x->typeid, _variadic_id(T)));	\
	&containerof(_x, variadic(T), super)->value;		\
}))
#define variadic_make(T, x) &((variadic(T)){ { _variadic_id(T) }, (x) }.super)

