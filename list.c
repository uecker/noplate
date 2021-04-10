/* Copyright 2021. Martin Uecker
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 * */


#include "list.h"

extern inline bool (list_empty)(const struct list* h);
extern inline void (list_push)(struct list* h, struct list_node* n);
extern inline struct list_node* (list_pop)(struct list* h);


