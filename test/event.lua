--
-- Lua plugin for the GNU Compiler Collection.
-- Copyright © 2012 Peter Colberg.
-- Distributed under the MIT license. (See accompanying file LICENSE.)
--

local gcc = require("gcc")

gcc.set_asm_file_name(gcc.HOST_BIT_BUCKET)

local result

gcc.register_callback(gcc.PLUGIN_START_UNIT, function()
  result = {}
end)

gcc.register_callback(gcc.PLUGIN_FINISH_TYPE, function(decl)
  assert(decl:class() == "type")
  assert(decl:code() == "union_type")
  assert(decl:fields():class() == "declaration")
  assert(decl:fields():code() == "field_decl")
  assert(decl:fields():name():value() == "up")
  assert(decl:fields():chain():name():value() == "charm")
  assert(decl:fields():chain():chain():name():value() == "top")
  table.insert(result, {"PLUGIN_FINISH_TYPE", decl})
end)

gcc.register_callback(gcc.PLUGIN_PRE_GENERICIZE, function(decl)
  assert(decl:class() == "declaration")
  assert(decl:code() == "function_decl")
  assert(decl:body():class() == "expression")
  assert(decl:body():code() == "bind_expr")
  assert(decl:body():vars():class() == "declaration")
  assert(decl:body():vars():code() == "var_decl")
  assert(decl:body():vars():name():value() == "down")
  assert(decl:body():vars():chain():name():value() == "strange")
  assert(decl:body():vars():chain():chain():name():value() == "bottom")
  table.insert(result, {"PLUGIN_PRE_GENERICIZE", decl})
end)

if gcc.GCC_VERSION >= 4007 then
  gcc.register_callback(gcc.PLUGIN_FINISH_DECL, function(decl)
    assert(decl:class() == "declaration")
    table.insert(result, {"PLUGIN_FINISH_DECL", decl})
  end)
end

-- Lua 5.2 compatibility
local unpack = table.unpack or assert(unpack)

gcc.register_callback(gcc.PLUGIN_FINISH_UNIT, function()
  -- result table is traversed in reverse order
  do
    local event, decl = unpack(table.remove(result))
    assert(event == "PLUGIN_PRE_GENERICIZE")
    assert(decl:class() == "declaration")
    assert(decl:code() == "function_decl")
    assert(decl:name():value() == "down_strange_bottom_quarks")
  end
  if gcc.GCC_VERSION >= 4007 then
    do
      local event, decl = unpack(table.remove(result))
      assert(event == "PLUGIN_FINISH_DECL")
      assert(decl:class() == "declaration")
      assert(decl:code() == "var_decl")
      assert(decl:name():value() == "bottom")
    end
    do
      local event, decl = unpack(table.remove(result))
      assert(event == "PLUGIN_FINISH_DECL")
      assert(decl:class() == "declaration")
      assert(decl:code() == "var_decl")
      assert(decl:name():value() == "strange")
    end
    do
      local event, decl = unpack(table.remove(result))
      assert(event == "PLUGIN_FINISH_DECL")
      assert(decl:class() == "declaration")
      assert(decl:code() == "var_decl")
      assert(decl:name():value() == "down")
    end
    do
      local event, decl = unpack(table.remove(result))
      assert(event == "PLUGIN_FINISH_DECL")
      assert(decl:class() == "declaration")
      assert(decl:code() == "parm_decl")
      assert(decl:type():class() == "type")
      assert(decl:type():code() == "void_type")
    end
    do
      local event, decl = unpack(table.remove(result))
      assert(event == "PLUGIN_FINISH_DECL")
      assert(decl:class() == "declaration")
      assert(decl:code() == "type_decl")
      assert(decl:type():class() == "type")
      assert(decl:type():code() == "union_type")
      assert(decl:name():value() == "up_charm_top_quarks")
    end
  end
  do
    local event, decl = unpack(table.remove(result))
    assert(event == "PLUGIN_FINISH_TYPE")
    assert(decl:class() == "type")
    assert(decl:code() == "union_type")
    assert(decl:name() == nil)
  end
  if gcc.GCC_VERSION >= 4007 then
    do
      local event, decl = unpack(table.remove(result))
      assert(event == "PLUGIN_FINISH_DECL")
      assert(decl:class() == "declaration")
      assert(decl:code() == "field_decl")
      assert(decl:name():value() == "top")
    end
    do
      local event, decl = unpack(table.remove(result))
      assert(event == "PLUGIN_FINISH_DECL")
      assert(decl:class() == "declaration")
      assert(decl:code() == "field_decl")
      assert(decl:name():value() == "charm")
    end
    do
      local event, decl = unpack(table.remove(result))
      assert(event == "PLUGIN_FINISH_DECL")
      assert(decl:class() == "declaration")
      assert(decl:code() == "field_decl")
      assert(decl:name():value() == "up")
    end
  end
  assert(#result == 0)
end)
