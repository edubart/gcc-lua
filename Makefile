#
# Lua plugin for the GNU Compiler Collection.
# Copyright © 2012 Peter Colberg.
# For conditions of distribution and use, see copyright notice in LICENSE.
#

PREFIX = /usr/local
GCC    = gcc

INSTALL_GCC_PLUGIN = $(shell $(GCC) -print-file-name=plugin)
INSTALL_DOC        = $(PREFIX)/share/doc/gcc-lua
INSTALL_DOC_HTML   = $(INSTALL_DOC)/html

INSTALL_D = install -d
INSTALL_F = install -m 644
INSTALL_L = install -m 644 -s

FILES_GCC_PLUGIN = gcclua.so
FILES_DOC        = INSTALL.mdwn README.mdwn index.mdwn reference.mdwn
FILES_DOC_HTML   = INSTALL.html README.html index.html reference.html pandoc.css gcc-lua.png

gcc:
	@$(MAKE) -C gcc

test:
	@$(MAKE) -C test

install: gcc doc
	$(INSTALL_D) $(DESTDIR)$(INSTALL_GCC_PLUGIN)
	cd gcc && $(INSTALL_L) $(FILES_GCC_PLUGIN) $(DESTDIR)$(INSTALL_GCC_PLUGIN)
	$(INSTALL_D) $(DESTDIR)$(INSTALL_DOC)
	cd doc && $(INSTALL_F) $(FILES_DOC) $(DESTDIR)$(INSTALL_DOC)
	$(INSTALL_D) $(DESTDIR)$(INSTALL_DOC_HTML)
	cd doc && $(INSTALL_F) $(FILES_DOC_HTML) $(DESTDIR)$(INSTALL_DOC_HTML)

doc:
	@$(MAKE) -C doc

clean:
	@$(MAKE) -C gcc clean
	@$(MAKE) -C doc clean

.PHONY: gcc test install doc clean
