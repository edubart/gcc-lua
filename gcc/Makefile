#
# Lua plugin for the GNU Compiler Collection.
# Copyright © 2012–2015 Peter Colberg.
# Distributed under the MIT license. (See accompanying file LICENSE.)
#

CC        = gcc
HOST_CC   = $(CC)
TARGET_CC = $(CROSS)$(CC)
CPPFLAGS  =
CPPOPT    = -I$(shell $(TARGET_CC) -print-file-name=plugin)/include
CFLAGS    = -O2 -Wall -Wformat-security
CCOPT     = -fPIC
ifneq (,$(findstring GCCLUA_USE_CXX,$(shell $(HOST_CC) -E -dM $(CPPOPT) -xc++ gcclua-config.h)))
  CCOPT  += -xc++
endif
LDFLAGS   =

HOST_SYS := $(shell uname -s)
ifeq (Darwin,$(HOST_SYS))
  LDOPT   = -dynamiclib -Wl,-undefined,dynamic_lookup
else
  LDOPT   = -shared
endif

LUAMODS   = lua lua-5.3 lua5.3 lua53 lua-5.2 lua5.2 lua52 lua-5.1 lua5.1 lua51 luajit
LUAMOD    = $(firstword $(foreach mod,$(LUAMODS),$(shell pkg-config --exists $(mod) && echo $(mod))))
ifneq (,$(LUAMOD))
  LUACFLAGS = $(shell pkg-config --cflags $(LUAMOD))
  LUALIBS   = $(shell pkg-config --libs $(LUAMOD))
endif

all: gcclua.so

%.o: %.c
	$(HOST_CC) -c -o $@ $(CPPOPT) $(CCOPT) $(CPPFLAGS) $(CFLAGS) $(LUACFLAGS) $<

gcclua.so: gcclua.o
	$(HOST_CC) -o $@ $< $(LDOPT) $(LDFLAGS) $(LUALIBS)

clean:
	$(RM) gcclua.so gcclua.o

.PHONY: clean
