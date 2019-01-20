---
title: Reference
toc: 2
---

# Plugin Events

`gcc.register_callback(event, f)`

  : Registers the function `f` for the given plugin event.

    The following plugin events are supported:

    `gcc.PLUGIN_START_UNIT`

      : Event triggered before a translation unit is parsed.

        The callback function receives no arguments.

    `gcc.PLUGIN_PRE_GENERICIZE`

      : Event triggered after a function definition has been parsed.

        The callback function receives a **function_decl** node as its first
        argument.

    `gcc.PLUGIN_FINISH_DECL`

      : Event triggered after a declaration has been parsed.

        The callback function receives a node of the class **declaration** as
        its first argument.

        *This event is available with GCC 4.7 or later.*

    `gcc.PLUGIN_FINISH_TYPE`

      : Event triggered after a struct or union type has been parsed.

        The callback function receives a node of the class **type** as its
        first argument.

    `gcc.PLUGIN_FINISH_UNIT`

      : Event triggered after a translation unit has been parsed.

        The callback function receives no arguments.

# Globals

`gcc.get_asm_file_name()`

  : Returns assembly output filename.

`gcc.set_asm_file_name(filename)`

  : Sets assembly output filename.

    `gcc.HOST_BIT_BUCKET`

      : Filename of a device that discards data written to it, for example
        **"/dev/null"**.

`gcc.get_main_input_basename()`

  : Returns main input filename with any leading directory components removed.

    *This function is available with GCC 4.6 or later.*

`gcc.get_main_input_filename()`

  : Returns main input filename.

`gcc.get_translation_units()`

  : Returns translation units as a sequence of **translation_unit_decl** nodes.

    *This function is available with GCC 4.6 or later.*

`gcc.get_functions()`

  : Returns function declarations in translation unit as a *reverse* sequence
    of **function_decl** nodes.

`gcc.get_variables()`

  : Returns variable declarations in current translation unit as a *reverse*
    sequence of **var_decl** nodes.

`gcc.get_identifier(s)`

  : Returns identifier for given string as an **identifier_node** node.

`gcc.define_macro(s)`

  : Defines preprocessor macro.

    ~~~ {.lua}
    gcc.define_macro("TURTLES")
    gcc.define_macro("PI=3")
    gcc.define_macro("SIN(x)=x")
    ~~~

`gcc.GCC_VERSION`

  : A number corresponding to the GCC version the plugin was built for,
    e.g., **4008** for GCC 4.8.

# Tree Nodes

Any [tree node] provides the following methods:

`node:class()`

  : Returns tree code class as a string.

`node:code()`

  : Returns tree code as a string.

## Declarations

This tree code class is used for declarations.

`declaration`

  : The class provides the following methods:

    `node:align()`

      : Returns alignment in bits.

    `node:align_unit()`

      : Returns alignment in bytes.

    `node:assembler_name()`

      : Returns tree node of assembler name.

    `node:artificial()`

      : Returns **true** if declaration is a compiler-generated entity, or
        **false** otherwise.

    `node:attributes()`

      : Returns tree node of attributes.

    `node:chain()`

      : Returns tree node of next declaration.

    `node:name()`

      : Returns name as an **identifier_node** node.

    `node:location()`

      : Returns source filename and line number.

        With GCC 4.8 or later, further returns column number.

    `node:size()`

      : Returns size in bits.

    `node:size_unit()`

      : Returns size in bytes.

    `node:type()`

      : Returns tree node of type of declaration.

    `node:uid()`

      : Returns unique number.

    `node:user_align()`

      : Returns **true** if declaration has user-defined alignment, or **false** otherwise.

    The following tree codes provide further methods:

    `const_decl`

      : C enum value.

        `node:initial()`

          : Returns tree node of constant value.

    `field_decl`

      : C struct or union field.

        `node:bit_field()`

          : Returns **true** if field is a bitfield, or **false** otherwise.

        `node:bit_field_type()`

          : Returns tree node of type specified for a bitfield.

        `node:initial()`

          : Returns initial value.

    `function_decl`

      : Function declaration.

        `node:args()`

          : Returns tree node of argument declarations.

        `node:body()`

          : Returns tree node of body expression.

        `node:external()`

          : Returns **true** if function refers to definition elsewhere, or **false** otherwise.

        `node:public()`

          : Returns **true** if function is accessible outside translation unit, or **false** otherwise.

    `translation_unit_decl`

      : Translation unit.

        `node:block()`

          : Returns tree node of block.

        `node:language()`

          : Returns language of translation unit.

            *This function is available with GCC 4.6 or later.*

    `type_decl`

      : Type declaration.

        `node:public()`

          : Returns **true** if type is accessible outside translation unit, or **false** otherwise.

    `var_decl`

      : Variable declaration.

        `node:external()`

          : Returns **true** if variable refers to definition elsewhere, or **false** otherwise.

        `node:initial()`

          : Returns tree node of initial value.

        `node:public()`

          : Returns **true** if variable is accessible outside translation unit, or **false** otherwise.

## Types

This tree code class is used for type objects.

`type`

  : The class provides the following methods:

    `node:align()`

      : Returns alignment in bits.

    `node:align_unit()`

      : Returns alignment in bytes.

    `node:attributes()`

      : Returns tree node of attributes.

    `node:canonical()`

      : Returns tree node of canonical type.

        The canonical type includes qualifiers.

    `node:const()`

      : Returns **true** if type has `const` qualifier, or **false** otherwise.

    `node:main_variant()`

      : Returns tree node of main variant of type.

        The main variant excludes qualifiers.

    `node:mode()`

      : Returns [machine mode] as a string.

    `node:name()`

      : Returns tree node of type name.

    `node:packed()`

      : Returns **true** if type has attribute `packed`.

    `node:restrict()`

      : Returns **true** if type has `restrict` qualifier, or **false** otherwise.

    `node:size()`

      : Returns size of type in bits.

    `node:size_unit()`

      : Returns size of type in bytes.

    `node:stub_decl()`

      : Returns stub declaration as a **type_decl** node.

    `node:user_align()`

      : Returns **true** if type has user-defined alignment, or **false** otherwise.

    `node:volatile()`

      : Returns **true** if type has `volatile` qualifier, or **false** otherwise.

    The following tree codes provide further methods:

    `array_type`

      : Array type.

        `node:domain()`

          : Returns tree node of lower and upper array bounds.

        `node:type()`

          : Returns type of array elements.

    `enumeral_type`

      : C enum.

        `node:values()`

          : Returns tree node of enum values.

        The following methods are available with the C++ frontend.

        `node:anonymous()`

          : Returns **true** if enum is unnamed, or **false** otherwise.

    `function_type`

      : Function type.

        `node:args()`

          : Returns tree node of argument types.

        `node:type()`

          : Returns tree node of return type.

    `integer_type`

      : Integer type.

        `node:max()`

          : Returns tree node of maximum value.

        `node:min()`

          : Returns tree node of minimum value.

        `node:unsigned()`

          : Returns **true** if type is unsigned, or **false** otherwise.

    `pointer_type`

      : Pointer type.

        `node:type()`

          : Returns tree node of pointed type.

        `node:unsigned()`

          : Returns **true** if type is unsigned, or **false** otherwise.

    `record_type`

      : C struct.

        `node:fields()`

          : Returns tree node of struct fields.

        The following methods are available with the C++ frontend.

        `node:anonymous()`

          : Returns **true** if struct is unnamed, or **false** otherwise.

    `reference_type`

      : Reference type.

        `node:type()`

          : Returns tree node of referenced type.

    `union_type`

      : C union.

        `node:fields()`

          : Returns tree node of union fields.

        The following methods are available with the C++ frontend.

        `node:anonymous()`

          : Returns **true** if union is unnamed, or **false** otherwise.

    `vector_type`

      : GCC vector type.

        `node:type()`

          : Returns tree node of vector element type.

        `node:units()`

          : Returns number of vector elements.

## Constants

This tree code class is used for constant values.

`constant`

  : The class provides the following methods:

    `node:type()`

      : Returns tree node of type of constant.

    The following tree codes provide further methods:

    `integer_cst`

      : Integer constant.

        `node:value()`

          : Returns value as a number.

    `real_cst`

      : Floating-point constant.

        `node:value()`

          : Returns value as a number.

    `string_cst`

      : String constant.

        `node:value()`

          : Returns value as a string.

## Unary expressions

This tree code class is used for unary arithmetic expressions.

`unary`

  : The class provides the following methods:

    `node:operand()`

      : Returns tree node of operand.

    `node:type()`

      : Returns tree node of type of expression.

## Binary expressions

This tree code class is used for binary arithmetic expressions.

`binary`

  : The class provides the following methods:

    `node:operand()`

      : Returns tree nodes of operands.

    `node:type()`

      : Returns tree node of type of expression.

## Statement expressions

This tree code class is used for statement expressions.

`statement`

  : The following tree codes provide further methods:

    `return_expr`

      : Return statement.

        `node:operand()`

          : Returns tree node of return value.

## Variable-length expressions

This tree code class is used for function calls, and expressions with a
variable number of operands.

`vl_exp`

  : The class provides the following methods:

    `node:type()`

      : Returns tree node of type of expression.

    The following tree codes provide further methods:

    `call_expr`

      : Call expression.

        `node:function()`

          : Returns tree node of function declaration.

        `node:args()`

          : Returns tree nodes of arguments.

## Other expressions

This tree code class is used for other expressions.

`expression`

  : The class provides the following methods:

    `node:operand()`

      : Returns tree nodes of operands of expression.

    `node:type()`

      : Returns tree node of type of expression.

    The following tree codes provide further methods:

    `bind_expr`

      : Bind expression of local variables.

        `node:block()`

          : Returns tree node of block corresponding to bind expression.

        `node:body()`

          : Returns tree node of expression computed using the variables.

        `node:vars()`

          : Returns tree node of chain of declarations.

## Exceptional codes

This tree code class is used for codes that fit no other category.

`exceptional`

  : The following tree codes provide further methods:

    `block`

      : Symbol-binding block.

        `node:vars()`

          : Returns tree node of chain of declarations.

    `constructor`

      : Array or struct initializer list.

        `node:elements()`

          : Returns sequence of tree nodes of constructor elements.

    `identifier_node`

      : Identifier.

        `node:value()`

          : Returns identifier string.

    `statement_list`

      : Statement list.

        `node:statements()`

          : Returns sequence of tree nodes of statements.

    `tree_list`

      : Singly-linked list of key-value pairs.

        `node:chain()`

          : Returns tree node of next element in list.

        `node:purpose()`

          : Returns tree node of key.

        `node:value()`

          : Returns tree node of value.

[machine mode]: https://gcc.gnu.org/onlinedocs/gccint/Machine-Modes.html
[tree node]: https://gcc.gnu.org/onlinedocs/gccint/GENERIC.html
