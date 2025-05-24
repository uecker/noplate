
#ifndef _NOPLATE_ALGEBRAIC_H
#define _NOPLATE_ALGEBRAIC_H	1

#include "core.h"


#define product_name(S, T) CONCAT(product_, CONCAT(CONCAT(S, _), T))
#define sum_name(S, T)	CONCAT(sum_, CONCAT(CONCAT(S, _), T))

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
	auto _x = (x);	 					\
	sum(S, T) _n;						\
	*_Generic(_x, 						\
		typeof(_n.car): (_n.sel = false, &_n.car),	\
		typeof(_n.cdr): (_n.sel = true, &_n.cdr)	\
	) = _x;							\
	_n;							\
})

#define _sum_car(id, x)	({ auto id = (x); CHECK(!id.sel); id.car; })
#define _sum_cdr(id, x)	({ auto id = (x); CHECK( id.sel); id.cdr; })

#define sum_car(x)		_sum_car(__UNIQ, x)
#define sum_cdr(x)		_sum_cdr(__UNIQ, x)

#define _sum_choice(id, x, f, g)	({ auto id = (x); (!id.sel) ? (f((sum_car(id)))) : (g((sum_cdr(id)))); })
#define sum_choice(x, f, g)		_sum_choice(__UNIQ, x, f, g)


#endif // _NOPLATE_ALGEBRAIC_H

