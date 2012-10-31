#
# Lua plugin for the GNU Compiler Collection.
# Copyright © 2012 Peter Colberg.
# For conditions of distribution and use, see copyright notice in LICENSE.
#

gcc:
	@$(MAKE) -C gcc

test: gcc
	@$(MAKE) -C test

clean:
	@$(MAKE) -C gcc clean
	@$(MAKE) -C doc clean

.PHONY: gcc test clean
