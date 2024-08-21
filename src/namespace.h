
#include "cppmap.h"

#define NAMESPACE_PREFIX(ns, x) ns ## _ ## x
#define NAMESPACE_ENTRY(ns, x) typeof(NAMESPACE_PREFIX(ns, x)) *x;
#define NAMESPACE_INIT(ns, x) .x = &NAMESPACE_PREFIX(ns, x),
#define NAMESPACE_MAKE(S, N) \
static const struct \
{ S(N, NAMESPACE_ENTRY) } N = { S(N, NAMESPACE_INIT) }
#define NAMESPACE_IMPORT(X, Y) const static typeof(X) Y = X



// version 2

#define NAMESPACE(S, ...) \
static const struct { MAP(NAMESPACE_ENTRY, S, __VA_ARGS__) } S = { MAP(NAMESPACE_INIT, S, __VA_ARGS__) }


#define namespace(S, ...) \
(const struct { MAP(NAMESPACE_ENTRY, S, __VA_ARGS__) }){ MAP(NAMESPACE_INIT, S, __VA_ARGS__) }



