---
title: Releases
---

This document lists the changes between stable releases.

The version numbers follow [semantic versioning].

# gcc‑lua 1.3.0 (2015-04-23) {#v1.3.0}

  * Support compilation of the plugin for GCC 5.

  * Run test suite with default make target.

  * Add `node:mode()` for types.

# gcc‑lua 1.2.1 (2014-04-01) {#v1.2.1}

  * Support compilation of the plugin for GCC 4.9.

  * Fix test suite on x86 architecture.

# gcc‑lua 1.2.0 (2013-12-12) {#v1.2.0}

  * The `Makefile` detects whether the GCC plugin ABI is C or C++.

    The `gcc` frontend is used for both cases, and the flag `-xc++` is included
    when needed.

  * Fix missing format strings for the output of plugin errors.

  * The make variables `CPPFLAGS`, `CFLAGS`, and `LDFLAGS` may be overriden.

    This is useful for setting distribution-specific compiler flags.

  * Add `node:stub_decl()` for types.

    This can be used to get a declaration node for struct, union, or enum
    types.

  * Return column number for `node:location()` with GCC 4.8 or later.

  * Add `node:uid()` for declarations.

    The unique identifier is useful for ordering declarations by definition.

  * Ensure matching versions of compiler and plugin.

  * Support compilation of the plugin for a cross compiler.

  * Add `gcc.GCC_VERSION`.

  * Support compilation of the plugin for GCC 4.5.

Thanks to Dennis Schridde for contributing to this release.

# gcc‑lua 1.1.0 (2013-10-06) {#v1.1.0}

  * The `Makefile` uses `pkg-config` to find the Lua headers and library.

    For most operating systems, compilation should no longer require any flags.

  * Add `node:operand()` and `node:type()` for nodes of class **expression**.

    These were provided only for **addr_expr** and **modify_expr** nodes before.

# gcc‑lua 1.0.0 (2013-09-29) {#v1.0.0}

  * Initial release.

[semantic versioning]: https://semver.org/spec/v2.0.0.html
