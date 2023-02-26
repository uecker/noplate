# Copyright 2021. Martin Uecker
# All rights reserved. Use of this source code is governed by
# a BSD-style license which can be found in the LICENSE file.

CC ?= gcc
CFLAGS = -O2 -g -std=gnu17 -Wall -Wextra  -D_GNU_SOURCE
LDFLAGS =

ifeq ($(findstring clang,$(CC)),clang)
CFLAGS += -fsanitize=vla-bound,bounds,null -fsanitize-undefined-trap-on-error -fblocks
LDFLAGS+= -lBlocksRuntime
else
CFLAGS += -fsanitize=vla-bound,bounds-strict,null -fsanitize-undefined-trap-on-error
endif

ifeq ($(TAGCOMPAT),1)
CFLAGS += -DTAGCOMPAT -ftag-compat
endif



SRCS = vec.c list.c string.c nat.c

.INTERMEDIATE: $(SRCS:.c=.o)

(%): %
	$(AR) $(ARFLAGS) $@ $%

libnoplate.a: libnoplate.a($(SRCS:.c=.o))

test: test.c libnoplate.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
	# execstack -c test

test_mdarray: test_mdarray.c libnoplate.a
	$(CC) $(CFLAGS) $(LDFLAGS) -Wno-missing-braces -o $@ $^
	# execstack -c test

