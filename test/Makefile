#
# Lua plugin for the GNU Compiler Collection.
# Copyright © 2012 Peter Colberg.
# For conditions of distribution and use, see copyright notice in LICENSE.
#

CC       = gcc
CFLAGS   = -Wall -pedantic
CCOPT    = -std=c99
CXXFLAGS = -Wall -pedantic
CXXOPT   = -std=c++98
GCCLUA   = $(CURDIR)/../gcc/gcclua.so

all: event globals tree tree-cp

%: %.c %.lua
	$(CC) -S $(CCOPT) $(CFLAGS) -fplugin=$(GCCLUA) -fplugin-arg-gcclua-script=$@.lua ./$<

%: %.cxx %.lua
	$(CC) -S $(CXXOPT) $(CXXFLAGS) -fplugin=$(GCCLUA) -fplugin-arg-gcclua-script=$@.lua ./$<
