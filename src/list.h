/* Copyright 2021. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */

#ifndef _NOPLATE_LIST_H
#define _NOPLATE_LIST_H 1

#include <stdlib.h>
#include <stdbool.h>

#include "core.h"

struct list_node {

	struct list_node* next;
};


struct list {

	struct list_node* head;
};


inline void list_push(struct list* h, struct list_node* n)
{
	n->next = h->head;
	h->head = n;
}

inline struct list_node* list_pop(struct list* h)
{
	struct list_node* t = h->head;

	if (NULL != t)
		h->head = t->next;

	return t;
}

inline bool list_empty(const struct list* h)
{
	return (NULL == h->head);
}

#define _list_data(T) typeof(T[1])

#ifdef TAGCOMPAT
#define list_node(T) struct list_node_##T { struct list_node node; _list_data(T) data; }
#define list(T) struct list_##T { union { struct list list; list_node(T) *dummy; }; }
#else
#define list_node(T) struct list_node_##T
#define list_node_decl(T) list_node(T) { struct list_node node; _list_data(T) data; }

#define list(T) struct list_##T
#define list_decl(T) list_node_decl(T); list(T) { union { struct list list; list_node(T) *dummy; }; }
#endif

#define list_eltype(l) typeof((__L)->dummy[0].data[0])
#define list_node_type(l) typeof((__L)->dummy[0])

#define list_empty(l)	((list_empty)(&((l)->list)))

#define list_first(l)							\
({									\
 	auto __L = (l);							\
 	typedef list_node_type(__L) __NT;				\
									\
	containerof(__L->list.head, __NT, node);			\
})

#define list_node_access(n)	((n)->data[0])
#define list_node_next(n)						\
({									\
	auto __N = (n);							\
									\
	(NULL == __N->node.next) ? NULL :				\
		containerof(__N->node.next, typeof(*__N), node);	\
})

#define list_push(l, v)							\
({									\
 	auto __L = (l);							\
 	list_eltype(__L) __v = (v);					\
 	typedef list_node_type(__L) __NT;				\
									\
	__NT* __n2 = malloc(sizeof(__NT));				\
	if (NULL == __n2)						\
		abort();						\
	__n2->data[0] = __v;						\
	(list_push)(&__L->list, &__n2->node);				\
})
 
#define list_pop(l) 							\
({									\
 	auto __L = (l);							\
 	typedef list_node_type(__L) __NT;				\
	if (list_empty(__L))						\
		abort();						\
									\
	struct list_node* __n = (list_pop)(&__L->list);			\
	__NT* __n2 = containerof(__n, __NT, node);			\
	list_eltype(__L) el = __n2->data[0];				\
 	free(__n2);							\
 	el;								\
})

#endif

