
#ifndef __ALGEBRAIC_H
#define __ALGEBRAIC_H	1

#include "core.h"


#define product_name(S, T) product_ ## S ## _ ## T
#define sum_name(S, T)	sum_ ## S ## _ ## T

#ifdef TAGCOMPAT
#define product(S, T) struct product_name(S, T) { S car; T cdr; }
#define product_decl(S, T)
#else
#define product(S, T) struct product_name(S, T)
#define product_decl(S, T) product(S, T) { S car; T cdr; }
#endif

#define remove_bracket(...)	__VA_ARGS__
#define product_init(S, T, x)	((product(S, T)){ remove_bracket x })
#define product_car(x)		((x).car)
#define product_cdr(x)		((x).cdr)


#ifdef TAGCOMPAT
#define sum(S, T) struct sum_name(S, T) { bool sel; union { S car; T cdr; }; }
#define sum_decl(S, T)
#else
#define sum(S, T) struct sum_name(S, T)
#define sum_decl(S, T) sum(S, T) { bool sel; union { S car; T cdr; }; }
#endif

#define sum_init(S, T, x)					\
({ 								\
	auto __x = (x); 					\
	sum(S, T) __n;						\
	*_Generic(__x, 						\
		typeof(__n.car): (__n.sel = false, &__n.car),	\
		typeof(__n.cdr): (__n.sel = true, &__n.cdr)	\
	) = __x;						\
	__n;							\
})


#define sum_car(x)	({ auto __x = (x); CHECK(!__x.sel); __x.car; })
#define sum_cdr(x)	({ auto __x = (x); CHECK( __x.sel); __x.cdr; })

#define sum_choice(x, f, g)	({ auto __x = (x); (!__x.sel) ? (f((sum_car(__x)))) : (g((sum_cdr(__x)))); })



#endif // __ALGEBRAIC_H

