#
# Lua plugin for the GNU Compiler Collection.
# Copyright © 2012 Peter Colberg.
# For conditions of distribution and use, see copyright notice in LICENSE.
#

CC       = gcc
LD       = $(CC)
CPPFLAGS = -I$(shell $(CC) -print-file-name=plugin)/include
CFLAGS   = -fPIC -O2 -Wall
ifneq (,$(findstring GCCLUA_USE_CXX,$(shell $(CC) -E -dM $(CPPFLAGS) gcclua-config.h)))
CFLAGS  += -xc++
endif
LDFLAGS  = -shared -Wl,-s

LUAMODS   = lua lua5.2 lua5.1 luajit
LUAMOD    = $(firstword $(foreach mod,$(LUAMODS),$(shell pkg-config --exists $(mod) && echo $(mod))))
ifneq (,$(LUAMOD))
LUACFLAGS = $(shell pkg-config --cflags $(LUAMOD))
LUALIBS   = $(shell pkg-config --libs $(LUAMOD))
endif

all: gcclua.so

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LUACFLAGS) -c -o $@ $<

gcclua.so: gcclua.o
	$(LD) -o $@ $< $(LDFLAGS) $(LUALIBS)

clean:
	$(RM) gcclua.so gcclua.o

.PHONY: clean
