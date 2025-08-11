
#ifndef _NOPLATE_MAYBE_H
#define _NOPLATE_MAYBE_H	1

#include "core.h"

#ifdef TAGCOMPAT
#ifndef __clang__
#define maybe(T) struct maybe_##T { bool ok; T value [[gnu::unavailable("privat")]]; }
#else
#define maybe(T) struct maybe_##T { bool ok; T value; }
#endif
#define maybe_priv(T) struct maybe_##T { bool ok; T value; }
#define maybe_decl(T)
#else
#define maybe(T) struct maybe_##T
#define maybe_priv(T) struct maybe_##T
#define maybe_decl(T) maybe(T) { bool ok; T value; }
#endif

#define maybe_just(T, x)	(maybe(T)){ .value = (x), .ok = true }
#define maybe_nothing(T)	(maybe(T)){ .value = (T){ 0 }, .ok = false }
#define maybe_value(T, x)	(*({ maybe_priv(T) *__p = &(x); __p->ok ? &__p->value : (void*)0; }))

#define maybe_map(R, T, f)	lambda(maybe(R), (maybe(T) a), a.ok ? maybe_just(R, (f)(maybe_value(T, a))) : maybe_nothing(R))

#endif	// _NOPLATE_MAYBE_H

