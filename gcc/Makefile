#
# Lua plugin for the GNU Compiler Collection.
# Copyright © 2012 Peter Colberg.
# For conditions of distribution and use, see copyright notice in LICENSE.
#

CC       = gcc
LD       = $(CC)
CPPFLAGS =
CPPOPT   = -I$(shell $(CC) -print-file-name=plugin)/include
CFLAGS   = -O2 -Wall -Wformat-security
CCOPT    = -fPIC
ifneq (,$(findstring GCCLUA_USE_CXX,$(shell $(CC) -E -dM $(CPPOPT) gcclua-config.h)))
  CCOPT += -xc++
endif
LDFLAGS  = -Wl,-s -Wl,--as-needed
LDOPT    = -shared
LUAMODS  = lua lua5.2 lua5.1 luajit
LUAMOD   = $(firstword $(foreach mod,$(LUAMODS),$(shell pkg-config --exists $(mod) && echo $(mod))))
ifneq (,$(LUAMOD))
  LUACFLAGS = $(shell pkg-config --cflags $(LUAMOD))
  LUALIBS   = $(shell pkg-config --libs $(LUAMOD))
endif

all: gcclua.so

%.o: %.c
	$(CC) -c -o $@ $(CPPOPT) $(CCOPT) $(CPPFLAGS) $(CFLAGS) $(LUACFLAGS) $<

gcclua.so: gcclua.o
	$(LD) -o $@ $< $(LDOPT) $(LDFLAGS) $(LUALIBS)

clean:
	$(RM) gcclua.so gcclua.o

.PHONY: clean
