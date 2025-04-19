
#ifndef _NOPLATE_MAYBE_H
#define _NOPLATE_MAYBE_H	1

#include "core.h"

#ifdef TAGCOMPAT
#define maybe(T) struct CONCAT(maybe_, T) { bool ok; T value; }
#define maybe_decl(T)
#else
#define maybe(T) struct CONCAT(maybe_, T)
#define maybe_decl(T) maybe(T) { bool ok; T value; }
#endif

#define maybe_ok(T, x)		(maybe(T)){ .value = (x), .ok = true }
#define maybe_fail(T)		(maybe(T)){ .value = (T){ 0 }, .ok = false }
#define maybe_just(T, x)	(*({ maybe(T) *__p = &(x); __p->ok ? &__p->value : (void*)0; }))

#endif	// _NOPLATE_MAYBE_H

