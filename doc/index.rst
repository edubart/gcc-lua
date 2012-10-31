Lua plugin for the GNU Compiler Collection
==========================================

gcc‑lua extends the `GNU Compiler Collection`_ with the ability to run Lua
scripts. The plugin provides an interface to register callback functions for
plugin events, and inspect the abstract syntax tree of a translation unit.
The plugin is useful for static C code analysis. gcc‑lua supports GCC 4.6 or
4.7, and, `Lua`_ 5.1 or 5.2, or `LuaJIT`_.

.. _GNU Compiler Collection: http://gcc.gnu.org/
.. _Lua: http://www.lua.org/
.. _LuaJIT: http://luajit.org/

Reference
---------

.. toctree::
   :maxdepth: 2

   reference/index

Installation
------------

gcc‑lua is available from a `git repository`_::

   git clone http://git.colberg.org/gcc-lua.git

.. _git repository: http://git.colberg.org/gcc-lua.git

To compile the plugin, run::

   make

.. note::

   A distribution may ship the GCC plugin development headers in a separate
   package, e.g., Debian wheezy contains a package ``gcc-4.7-plugin-dev``
   for GCC 4.7.

The plugin is accompanied by a test suite::

   make test

.. note::

   While the plugin is written in C, by default it is compiled with ``g++``.
   C++ will be the default from GCC 4.8 onwards, and many distributions
   already ship a version of GCC 4.7 compiled with ``g++``. If the plugin fails
   to load due to unresolved symbols, try compiling with ``gcc`` instead.

To compile the plugin using the GNU C compiler, set the make variable ``CC``::

   make CC=gcc

To compile the plugin using a non‑default GNU C++ compiler, override ``CXX``::

   make CXX=g++-4.7

The make variables ``CC`` or ``CXX`` apply to the test suite as well.

By default, the plugin links against the ``lua`` library.

To link against a differently named library, override ``LUALIBS``::

   make LUALIBS=-llua5.1

If the Lua headers are not in a default path, set the environment variable ``CPATH``::

   CPATH=/usr/include/lua5.1 make

To compile and link against LuaJIT 2.0::

   CPATH=/usr/include/luajit-2.0 make LUALIBS=-lluajit-5.1

Usage
-----

Consider the following C source file ``hello.c``:

.. code-block:: c

   static const char hello[] = "Hello, World!";

We retrieve the string constant using a Lua script ``hello.lua``:

.. code-block:: lua

   local gcc = require("gcc")

   -- send assembler output to /dev/null
   gcc.set_asm_file_name(gcc.HOST_BIT_BUCKET)

   -- invoke Lua function after translation unit has been parsed
   gcc.register_callback(gcc.PLUGIN_FINISH_UNIT, function()
     -- get global variables in reverse order of declaration
     local vars = gcc.get_variables()
     for i = #vars, 1, -1 do
       -- initial value is a string constant
       print(vars[i]:initial():value())
     end
   end)

The plugin is loaded into GCC by passing its path to ``-fplugin``::

   gcc -S hello.c -fplugin=./gcc/gcclua.so -fplugin-arg-gcclua-script=hello.lua

For detailed examples, please refer to the source code of the test suite.

Resources
---------

The `Plugins <http://gcc.gnu.org/onlinedocs/gccint/Plugins.html>`_ section of
the `GCC Internals <http://gcc.gnu.org/onlinedocs/gccint/>`_ documentation
explains writing, building and loading plugins.
The `GENERIC <http://gcc.gnu.org/onlinedocs/gccint/GENERIC.html>`_ section
describes the format of abstract syntax trees.

Relevant GCC source files are ``gcc/tree.h``, ``gcc/tree.def`` and
``gcc/tree-dump.c`` for the GENERIC format, and ``gcc/cgraph.h`` for
entry points to global variable, function and type declarations.

The `GCC Wiki <http://gcc.gnu.org/wiki/plugins>`_ links to documentation and
existing plugins.

Boris Kolpackov has written a series on Parsing C++ with GCC plugins:
`[1] <http://www.codesynthesis.com/~boris/blog/2010/05/03/parsing-cxx-with-gcc-plugin-part-1/>`_,
`[2] <http://www.codesynthesis.com/~boris/blog/2010/05/10/parsing-cxx-with-gcc-plugin-part-2/>`_,
`[3] <http://www.codesynthesis.com/~boris/blog/2010/05/17/parsing-cxx-with-gcc-plugin-part-3/>`_.

Acknowledgements
----------------

I would like to thank David Malcolm, author of the `GCC Python plugin
<http://gcc-python-plugin.readthedocs.org>`_, whose code and documentation was
of invaluable aid in comprehending the GCC internal API.
