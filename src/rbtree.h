
#include "rb3ptr.h"

#include "core.h"

#ifndef TAGCOMPAT
#define rbtree_node(T) struct CONCAT(rbtree_node_, T)
#define rbtree_node_decl(T) rbtree_node(T) { struct rb3_head head; T data; }

#define rbtree(T) struct CONCAT(rbtree_, T)
#define rbtree_decl(T) rbtree_node_decl(T); rbtree(T) { struct rb3_tree tree; rb3_cmp* compare; rbtree_node(T) type[]; }
#else
#define rbtree_node(T) struct CONCAT(rbtree_node_, T) { struct rb3_head head; T data; }
#define rbtree_node_decl(T) 

#define rbtree_decl(T) 
#define rbtree(T) struct CONCAT(rbtree_, T) { struct rb3_tree tree; rb3_cmp* compare; rbtree_node(T) type[]; }
#endif

#define rbtree_eltype(l) typeof((l)->type[0].data)
#define rbtree_node_type(l) typeof((l)->type[0])

#if 0
struct __rbtree_cmp {

	void* ptr;
	size_t size;
	off_t offset;
};

inline int __rbtree_cmp_default(struct rb3_head* a, void* ptr)
{
	struct __rbtree_cmp* data = ptr;
	return memcmp(a + data->offset, data->ptr, data->size);
}
#endif

#define __rbtree_cmp_default(NT, ET)							\
	int __compare(struct rb3_head* a, void* ptr) { 					\
 		return memcmp(&containerof(a, NT, head)->data, ptr, sizeof(ET)); 	\
 	};								\
	
#define rbtree_set_compare(l, cmp)					\
({									\
 	auto __L = (l);							\
 	typedef rbtree_eltype(__L) __ET;				\
 	typedef rbtree_node_type(__L) __NT;				\
	 								\
	int __compare(struct rb3_head* a, void* ptr) { 			\
 		return cmp(&containerof(a, __NT, head)->data, (__ET*)ptr); 	\
 	};								\
 	__L->compare = __compare;					\
})

#define rbtree_first(l)							\
({									\
 	auto __L = (l);							\
 	typedef rbtree_node_type(__L) __NT;				\
	auto __N2 = rb3_get_min(&__L->tree);				\
	(NULL == __N2) ? NULL : containerof(__N2, __NT, head);		\
})

#define rbtree_last(l)							\
({									\
 	auto __L = (l);							\
 	typedef rbtree_node_type(__L) __NT;				\
	auto __N2 = rb3_get_max(&__L->tree);				\
	(NULL == __N2) ? NULL : containerof(__N2, __NT, head);		\
})



#define rbtree_node_access(n)	((n)->data)

#define rbtree_node_next(n)						\
({									\
	auto __N = (n);							\
	auto __N2 = rb3_get_next(&__N->head);				\
	(NULL == __N2) ? NULL :						\
		containerof(__N2, typeof(*__N), head);			\
})

#define rbtree_node_prev(n)						\
({									\
	auto __N = (n);							\
	auto __N2 = rb3_get_prev(&__N->head);				\
	(NULL == __N2) ? NULL :						\
		containerof(__N2, typeof(*__N), head);			\
})


#define rbtree_insert(l, v)						\
({									\
 	auto __L = (l);							\
 	typedef rbtree_eltype(__L) __ET;				\
 	typedef rbtree_node_type(__L) __NT;				\
 	__ET __v = (v);							\
									\
	__NT* __n2 = malloc(sizeof(__NT));				\
	if (NULL == __n2)						\
		abort();						\
	__n2->data = __v;						\
	__rbtree_cmp_default(__NT, __ET);				\
	rb3_insert(&__L->tree, &__n2->head, __L->compare ? __L->compare : __compare, &__n2->data);	\
})


#define rbtree_delete(l, v)						\
({									\
 	auto __L = (l);							\
 	typedef rbtree_eltype(__L) __ET;				\
 	typedef rbtree_node_type(__L) __NT;				\
									\
	__rbtree_cmp_default(__NT, __ET);				\
	struct rb3_head* nt = rb3_delete(&__L->tree, __L->compare ? __L->compare : __compare, &compound_literal(__ET, v)); \
	free(containerof(nt, __NT, head));				\
})


#define rbtree_find(l, v)						\
({									\
 	auto __L = (l);							\
 	typedef rbtree_eltype(__L) __ET;				\
 	typedef rbtree_node_type(__L) __NT;				\
									\
	__rbtree_cmp_default(__NT, __ET);				\
	struct rb3_head* nt = rb3_find(&__L->tree, __L->compare ? __L->compare : __compare, &(__ET){ v }); \
	containerof(nt, __NT, head);					\
})


