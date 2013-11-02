#
# Lua plugin for the GNU Compiler Collection.
# Copyright © 2012 Peter Colberg.
# For conditions of distribution and use, see copyright notice in LICENSE.
#

CC       = gcc
CFLAGS   = -std=c99 -Wall -pedantic
CXXFLAGS = -std=c++98 -Wall -pedantic
GCCLUA   = $(CURDIR)/../gcc/gcclua.so

all: event globals tree tree-cp

%: %.c %.lua
	$(CC) -S $(CFLAGS) -fplugin=$(GCCLUA) -fplugin-arg-gcclua-script=$@.lua ./$<

%: %.cxx %.lua
	$(CC) -S $(CXXFLAGS) -fplugin=$(GCCLUA) -fplugin-arg-gcclua-script=$@.lua ./$<
