#
# Lua plugin for the GNU Compiler Collection.
# Copyright © 2012 Peter Colberg.
# For conditions of distribution and use, see copyright notice in LICENSE.
#

CC       =
CXX      = g++
ifdef CC
LD       = $(CC)
CPPFLAGS = -I`$(CC) -print-file-name=plugin`/include
CFLAGS   = -fPIC -O2 -Wall -pedantic -std=c99
else
LD       = gcc
CPPFLAGS = -I`$(CXX) -print-file-name=plugin`/include
CXXFLAGS = -fPIC -O2 -Wall -pedantic -fno-exceptions -std=c++98 -Wno-long-long
endif
LDFLAGS  = -shared -Wl,-s
LUALIBS  = -llua

all: gcclua.so

%.o: %.c
ifdef CC
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
else
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
endif

gcclua.so: gcclua.o
	$(LD) -o $@ $< $(LDFLAGS) $(LUALIBS)

clean:
	$(RM) gcclua.so gcclua.o

.PHONY: clean
