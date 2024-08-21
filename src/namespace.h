

#define NAMESPACE_PREFIX(ns, x) ns ## _ ## x
#define NAMESPACE_ENTRY(ns, x) typeof(NAMESPACE_PREFIX(ns, x)) *x;
#define NAMESPACE_INIT(ns, x) .x = &NAMESPACE_PREFIX(ns, x),
#define NAMESPACE_MAKE(S, N) \
static const struct \
{ S(N, NAMESPACE_ENTRY) } N = { S(N, NAMESPACE_INIT) }
#define NAMESPACE_IMPORT(X, Y) const static typeof(X) Y = X

#define NAMESPACE(S) (const struct { S(S, NAMESPACE_ENTRY) }){ S(S, NAMESPACE_INIT) }

