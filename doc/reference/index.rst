GCC Module
==========

.. module:: gcc

Plugin Events
-------------

.. function:: register_callback(event, func)

   Register callback function for the given plugin event.

The following plugin events are supported:

.. data:: PLUGIN_START_UNIT

   Event triggered before a translation unit is parsed.

   The callback function receives no arguments.

.. data:: PLUGIN_PRE_GENERICIZE

   Event triggered after a function definition has been parsed.

   The callback receives a node of code :class:`function_decl <FunctionDecl>` as its first argument.

.. data:: PLUGIN_FINISH_DECL

   *This event is available with GCC 4.7 or greater.*

   Event triggered after a declaration has been parsed.

   The callback receives a node of code class :class:`declaration <Declaration>` as its first argument.

.. data:: PLUGIN_FINISH_TYPE

   Event triggered after a struct or union type has been parsed.

   The callback receives a node of code class :class:`type <Type>` as its first argument.

.. data:: PLUGIN_FINISH_UNIT

   Event triggered after a translation unit has been parsed.

   The callback function receives no arguments.

Globals
-------

.. function:: get_asm_file_name()

   Returns assembly output filename.

.. function:: set_asm_file_name(filename)

   Set assembly output filename.

.. data:: HOST_BIT_BUCKET

   Filename of a device that discards data written to it, e.g., ``/dev/null``.

.. function:: get_translation_units()

   :returns: sequence of tree nodes of code :class:`translation_unit_decl <TranslationUnitDecl>`

   Returns translation units.

.. function:: get_functions()

   :returns: sequence of tree nodes of code :class:`function_decl <FunctionDecl>`

   Returns function declarations in current translation unit, in *reverse* order.

.. function:: get_variables()

   :returns: sequence of tree nodes of code :class:`var_decl <VarDecl>`

   Returns variable declarations in current translation unit, in *reverse* order.

Tree Node
---------

All `tree nodes <http://gcc.gnu.org/onlinedocs/gccint/GENERIC.html>`_ have
the following common attributes:

.. class:: Tree

   .. method:: class()

      Returns tree code class.

      All tree class codes are defined as constants in the :mod:`gcc` module.

      This function returns, e.g., :data:`gcc.DECLARATION` for a node of code
      class :class:`declaration <Declaration>`.

   .. method:: class_name()

      Returns tree code class name.

   .. method:: code()

      Returns tree code.

      All tree codes are defined as constants in the :mod:`gcc` module.

      This function returns, e.g., :data:`gcc.VAR_DECL` for a node of code
      :class:`var_decl <VarDecl>`.

   .. method:: code_name()

      Returns tree code name.

Tree Code Classes
-----------------

All tree nodes have an associated code :meth:`class <gcc.Tree.class>`, which
inherits from :class:`gcc.Tree`.

.. note::

   A tree node of a code class not listed below has the same attributes as
   :class:`gcc.Tree`.

.. class:: Binary

   Binary-operator expression.

   .. method:: operand([op])

      Returns tree node of given operand ``op`` in integer range [0, 2).

      If ``op`` is absent, returns number of operands.

   .. method:: type()

      Returns tree node of type of expression.

.. class:: Constant

   .. method:: type()

      Returns tree node of type of constant.

.. class:: Declaration

   .. method:: assembler_name()

      Returns tree node of assembler name.

   .. method:: chain()

      Returns tree node of next declaration.

   .. method:: name()

      Returns :class:`identifier node <IdentifierNode>` of name.

   .. method:: location()

      Returns filename and line number.

   .. method:: type()

      Returns tree node of type of declaration.

.. class:: Type

   .. method:: align()

      Returns alignment in bits.

   .. method:: align_unit()

      Returns alignment in bytes.

   .. method:: attributes()

      Returns tree node of attributes.

   .. method:: const()

      Returns ``true`` if type has ``const`` qualifier, or ``false`` otherwise.

   .. method:: main_variant()

      Returns tree node of main variant of type.

   .. method:: name()

      Returns tree node of type name.

   .. method:: packed()

      Returns ``true`` if type has attribute ``packed``.

   .. method:: restrict()

      Returns ``true`` if type has ``restrict`` qualifier, or ``false`` otherwise.

   .. method:: size()

      Returns size of type in bits.

   .. method:: size_unit()

      Returns size of type in bytes.

   .. method:: user_align()

      Returns ``true`` if type has user-defined alignment, or ``false`` otherwise.

   .. method:: volatile()

      Returns ``true`` if type has ``volatile`` qualifier, or ``false`` otherwise.

.. class:: Unary

   Unary-operand expression.

   .. method:: operand([op])

      Returns tree node of given operand ``op`` in integer range [0, 1).

      If ``op`` is absent, returns number of operands.

   .. method:: type()

      Returns tree node of type of expression.

Tree Codes
----------

All tree nodes have an associated :meth:`code <gcc.Tree.code>`, which inherits
from the code :meth:`class <gcc.Tree.class>` of the node.

.. note::

   A tree node of a code not listed below has the same attributes as its code
   :meth:`class <gcc.Tree.class>`.

.. class:: AddrExpr

   Address of an object.

   .. method:: operand([op])

      Returns tree node of given operand ``op`` in integer range [0, 1).

      If ``op`` is absent, returns number of operands.

.. class:: ArrayType

   Array type.

   .. method:: domain()

      Returns tree node of :meth:`lower <gcc.IntegerType.min>` and :meth:`upper
      <gcc.IntegerType.max>` array bounds.

   .. method:: type()

      Returns type of array elements.

.. class:: BindExpr

   Bind expression of local variables.

   .. method:: block()

      Returns tree node of block corresponding to bind expression.

   .. method:: body()

      Returns tree node of expression computed using the variables.

   .. method:: vars()

      Returns tree node of chain of declarations.

.. class:: Block

   Symbol-binding block.

   .. method:: vars()

      Returns tree node of chain of declarations.

.. class:: ConstDecl

   C enum value.

   .. method:: initial()

      Returns tree node of constant value.

.. class:: EnumeralType

   C enum.

   .. method:: values()

      Returns tree node of enum values.

.. class:: FieldDecl

   C struct or union field.

   .. method:: align()

      Returns alignment in bits.

   .. method:: align_unit()

      Returns alignment in bytes.

   .. method:: attributes()

      Returns tree node of attributes.

   .. method:: initial()

      Returns initial value.

   .. method:: size()

      Returns size in bits.

   .. method:: size_unit()

      Returns size in bytes.

   .. method:: user_align()

      Returns ``true`` if type has user-defined alignment, or ``false`` otherwise.

.. class:: FunctionDecl

   Function declaration.

   .. method:: args()

      Returns tree node of argument types.

   .. method:: body()

      Returns tree node of body expression.

   .. method:: extern()

      Returns ``true`` if function has external visibility, or ``false`` otherwise.

.. class:: FunctionType

   Function type.

   .. method:: args()

      Returns tree node of argument types.

   .. method:: retn()

      Returns tree node of return type.

.. class:: IdentifierNode

   Identifier.

   .. method:: value()

      Returns identifier string.

.. class:: IntegerCst

   Integer constant.

   .. method:: value()

      Returns value.

.. class:: IntegerType

   Integer type.

   .. method:: max()

      Returns tree node of maximum value.

   .. method:: min()

      Returns tree node of minimum value.

.. class:: PointerType

   Pointer type.

   .. method:: type()

      Returns tree node of pointed type.

.. class:: RealCst

   Floating-point constant.

   .. method:: value()

      Returns value.

.. class:: RecordType

   C struct.

   .. method:: fields()

      Returns tree node of struct fields.

.. class:: StatementList

   Statement list.

   .. method:: statements()

      Returns sequence of tree nodes of statements.

.. class:: StringCst

   String constant.

   .. method:: value()

      Returns value.

.. class:: TranslationUnitDecl

   Translation unit.

   .. method:: block()

      Returns tree node of block.

   .. method:: language()

      Returns language of translation unit.

.. class:: TreeList

   Singly-linked list of key-value pairs.

   .. method:: chain()

      Returns tree node of next element in list.

   .. method:: purpose()

      Returns tree node of key.

   .. method:: value()

      Returns tree node of value.

.. class:: TypeDecl

   Type declaration.

   .. method:: extern()

      Returns ``true`` if type has external visibility, or ``false`` otherwise.

.. class:: UnionType

   C union.

   .. method:: fields()

      Returns tree node of union fields.

.. class:: VarDecl

   Variable declaration.

   .. method:: align()

      Returns alignment in bits.

   .. method:: align_unit()

      Returns alignment in bytes.

   .. method:: attributes()

      Returns tree node of attributes.

   .. method:: extern()

      Returns ``true`` if variable has external visibility, or ``false`` otherwise.

   .. method:: initial()

      Returns tree node of initial value.

   .. method:: size()

      Returns size in bits.

   .. method:: size_unit()

      Returns size in bytes.

   .. method:: user_align()

      Returns ``true`` if variable has user-defined alignment, or ``false`` otherwise.

.. class:: VectorType

   GCC vector type.

   .. method:: type()

      Returns tree node of vector element type.

   .. method:: units()

      Returns number of vector elements.
