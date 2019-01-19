#
# Lua plugin for the GNU Compiler Collection.
# Copyright © 2012–2015 Peter Colberg.
# Distributed under the MIT license. (See accompanying file LICENSE.)
#

PREFIX = /usr/local
CC     = gcc

INSTALL_GCC_PLUGIN = $(shell $(CC) -print-file-name=plugin)

INSTALL_D = install -d
INSTALL_L = install -m 644 -s

FILES_GCC_PLUGIN = gcclua.so

all: gcc test
test: gcc

install:
	$(INSTALL_D) $(DESTDIR)$(INSTALL_GCC_PLUGIN)
	cd gcc && $(INSTALL_L) $(FILES_GCC_PLUGIN) $(DESTDIR)$(INSTALL_GCC_PLUGIN)

clean:
	@$(MAKE) -C gcc clean

SUBDIRS = gcc test

.PHONY: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@
