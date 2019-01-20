---
title: Installing
---

The Lua plugin for GCC is available from a [Git repository]:

```
git clone https://git.colberg.org/peter/gcc-lua
```

In the repository, checkout the latest release version:

```
git checkout 1.3.0
```

If you have GnuPG, verify that the tag is signed by the following key:

```
git tag -v 1.3.0
```

```
Primary key fingerprint: 2997 6EC2 2BD1 EBBC CC77  F3A6 183B D5ED 3527 8611
     Subkey fingerprint: 983B F9D0 F02A 6014 0CEC  529F 767E 12A8 C92A 31CB
```

To compile the plugin, run:

```
make
```

A distribution may ship the GCC plugin development headers in a separate
package. The Debian distribution, for example, provides the package
gcc-4.7-plugin-dev for GCC 4.7.

To compile the plugin using a specific version of the GNU compiler, override
the make variable `CC`:

```
make CC=gcc-4.8
```

To compile the plugin for use with a cross compiler of the same version:

```
make CC=gcc-4.7 CROSS=arm-linux-gnueabihf-
```

To compile the plugin for use with a cross compiler of a different version:

```
make HOST_CC=gcc-4.8 TARGET_CC=arm-linux-gnueabihf-gcc-4.7
```

By default, `pkg-config` is used to find the Lua library.

If the Lua library cannot be found, override `LUACFLAGS` and `LUALIBS`:

```
make LUACFLAGS="-I/usr/include/lua5.1" LUALIBS="-L/usr/lib -llua5.1"
```

[Git repository]: https://git.colberg.org/peter/gcc-lua
