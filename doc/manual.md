---
title: User's Guide
---

Consider the following C source file `hello.c`:

```c
static const char hello[] = "Hello, World!";
```

We retrieve the string constant using a Lua script `hello.lua`:

```lua
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
```

The plugin is loaded into GCC by passing its path to `-fplugin`:

```
gcc -S hello.c -fplugin=./gcc/gcclua.so -fplugin-arg-gcclua-script=hello.lua
```

For detailed examples, please refer to the source code of the [test suite].

# Resources

The [Plugins] section of the [GCC Internals] explains writing, building and
loading plugins. The [GENERIC] section describes the format of abstract syntax
trees. Relevant GCC source files are `gcc/tree.h`, `gcc/tree.def` and
`gcc/tree-dump.c` for the GENERIC format, and `gcc/cgraph.h` for entry points
to global variable, function and type declarations.

The [GCC Wiki] links to documentation and existing plugins.

Boris Kolpackov has written a three-part series on [Parsing C++ with GCC
plugins].

[GCC Internals]: https://gcc.gnu.org/onlinedocs/gccint/
[GCC Wiki]: https://gcc.gnu.org/wiki/plugins
[GENERIC]: https://gcc.gnu.org/onlinedocs/gccint/GENERIC.html
[Git repository]: https://git.colberg.org/peter/gcc-lua
[Parsing C++ with GCC plugins]: https://www.codesynthesis.com/~boris/blog/2010/05/03/parsing-cxx-with-gcc-plugin-part-1/
[Plugins]: https://gcc.gnu.org/onlinedocs/gccint/Plugins.html
[test suite]: https://git.colberg.org/peter/gcc-lua/src/branch/master/test
