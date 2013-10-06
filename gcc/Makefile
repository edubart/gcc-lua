#
# Lua plugin for the GNU Compiler Collection.
# Copyright © 2012 Peter Colberg.
# For conditions of distribution and use, see copyright notice in LICENSE.
#

CC       =
CXX      = g++
ifdef CC
LD       = $(CC)
CPPFLAGS = -I$(shell $(CC) -print-file-name=plugin)/include
CFLAGS   = -fPIC -O2 -Wall -pedantic -std=c99
else
LD       = gcc
CPPFLAGS = -I$(shell $(CXX) -print-file-name=plugin)/include
CXXFLAGS = -fPIC -O2 -Wall -pedantic -fno-exceptions -std=c++98 -Wno-long-long
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
ifdef CC
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LUACFLAGS) -c -o $@ $<
else
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LUACFLAGS) -c -o $@ $<
endif

gcclua.so: gcclua.o
	$(LD) -o $@ $< $(LDFLAGS) $(LUALIBS)

clean:
	$(RM) gcclua.so gcclua.o

.PHONY: clean
