# Copyright 2021. Martin Uecker
# All rights reserved. Use of this source code is governed by
# a BSD-style license which can be found in the LICENSE file.

CFLAGS = -O2 -g -std=gnu17 -Wall -Wextra -fsanitize=bounds,null -fsanitize-undefined-trap-on-error -D_GNU_SOURCE

ifeq ($(TAGCOMPAT),1)
CFLAGS += -DTAGCOMPAT -ftag-compat
endif

CC ?= gcc
SRCS = vec.c list.c string.c nat.c

.INTERMEDIATE: $(SRCS:.c=.o)

(%): %
	$(AR) $(ARFLAGS) $@ $%

libnoplate.a: libnoplate.a($(SRCS:.c=.o))

test: test.c libnoplate.a
	$(CC) $(CFLAGS) -o $@ $^
	# execstack -c test

test_mdarray: test_mdarray.c libnoplate.a
	$(CC) $(CFLAGS) -o $@ $^
	# execstack -c test

