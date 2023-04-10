
#ifndef __MAYBE_H
#define __MAYBE_H	1

#include "core.h"

#ifdef TAGCOMPAT
#define maybe(T) struct maybe_ ## T { bool ok; T value; }
#define maybe_decl(T)
#else
#define maybe(T) struct maybe_ ## T
#define maybe_decl(T) maybe(T) { bool ok; T value; }
#endif



#define maybe_ok(T, x)	(maybe(T)){ .value = (x), .ok = true }
#define maybe_fail(T)	(maybe(T)){ .value = (T){ 0 }, .ok = false }
#define maybe_use(x)	({ auto __t = (x); CHECK(__t.ok); __t.value; })


#endif	// __MAYBE_H

