#
# Lua plugin for the GNU Compiler Collection.
# Copyright © 2012 Peter Colberg.
# Distributed under the MIT license. (See accompanying file LICENSE.)
#

PREFIX = /usr/local
CC     = gcc

INSTALL_GCC_PLUGIN = $(shell $(CC) -print-file-name=plugin)
INSTALL_DOC        = $(PREFIX)/share/doc/gcc-lua

INSTALL_D = install -d
INSTALL_F = install -m 644
INSTALL_L = install -m 644 -s

FILES_GCC_PLUGIN = gcclua.so
FILES_DOC        = CHANGES.mdwn INSTALL.mdwn README.mdwn index.mdwn reference.mdwn
FILES_DOC_HTML   = CHANGES.html INSTALL.html README.html index.html reference.html pandoc.css gcc-lua.png

all: gcc test
test: gcc

install:
	$(INSTALL_D) $(DESTDIR)$(INSTALL_GCC_PLUGIN)
	cd gcc && $(INSTALL_L) $(FILES_GCC_PLUGIN) $(DESTDIR)$(INSTALL_GCC_PLUGIN)
	$(INSTALL_D) $(DESTDIR)$(INSTALL_DOC)
	cd doc && $(INSTALL_F) $(FILES_DOC) $(FILES_DOC_HTML) $(DESTDIR)$(INSTALL_DOC)

clean:
	@$(MAKE) -C gcc clean

SUBDIRS = gcc test doc

.PHONY: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@
