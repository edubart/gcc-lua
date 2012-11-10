--
-- Lua plugin for the GNU Compiler Collection.
-- Copyright © 2012 Peter Colberg.
-- For conditions of distribution and use, see copyright notice in LICENSE.
--

local gcc = require("gcc")

gcc.set_asm_file_name(gcc.HOST_BIT_BUCKET)

local result

gcc.register_callback(gcc.PLUGIN_START_UNIT, function()
  result = {}
end)

gcc.register_callback(gcc.PLUGIN_FINISH_TYPE, function(decl)
  assert(decl:class_name() == "type")
  assert(decl:code_name() == "union_type")
  assert(decl:fields():class_name() == "declaration")
  assert(decl:fields():code_name() == "field_decl")
  assert(decl:fields():name():value() == "up")
  assert(decl:fields():chain():name():value() == "charm")
  assert(decl:fields():chain():chain():name():value() == "top")
  table.insert(result, {"PLUGIN_FINISH_TYPE", decl})
end)

gcc.register_callback(gcc.PLUGIN_PRE_GENERICIZE, function(decl)
  assert(decl:class_name() == "declaration")
  assert(decl:code_name() == "function_decl")
  assert(decl:body():class_name() == "expression")
  assert(decl:body():code_name() == "bind_expr")
  assert(decl:body():vars():class_name() == "declaration")
  assert(decl:body():vars():code_name() == "var_decl")
  assert(decl:body():vars():name():value() == "down")
  assert(decl:body():vars():chain():name():value() == "strange")
  assert(decl:body():vars():chain():chain():name():value() == "bottom")
  table.insert(result, {"PLUGIN_PRE_GENERICIZE", decl})
end)

if gcc.PLUGIN_FINISH_DECL then -- GCC ≥ 4.7
  gcc.register_callback(gcc.PLUGIN_FINISH_DECL, function(decl)
    assert(decl:class_name() == "declaration")
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
    assert(decl:class_name() == "declaration")
    assert(decl:code_name() == "function_decl")
    assert(decl:name():value() == "down_strange_bottom_quarks")
  end
  if gcc.PLUGIN_FINISH_DECL then -- GCC ≥ 4.7
    do
      local event, decl = unpack(table.remove(result))
      assert(event == "PLUGIN_FINISH_DECL")
      assert(decl:class_name() == "declaration")
      assert(decl:code_name() == "var_decl")
      assert(decl:name():value() == "bottom")
    end
    do
      local event, decl = unpack(table.remove(result))
      assert(event == "PLUGIN_FINISH_DECL")
      assert(decl:class_name() == "declaration")
      assert(decl:code_name() == "var_decl")
      assert(decl:name():value() == "strange")
    end
    do
      local event, decl = unpack(table.remove(result))
      assert(event == "PLUGIN_FINISH_DECL")
      assert(decl:class_name() == "declaration")
      assert(decl:code_name() == "var_decl")
      assert(decl:name():value() == "down")
    end
    do
      local event, decl = unpack(table.remove(result))
      assert(event == "PLUGIN_FINISH_DECL")
      assert(decl:class_name() == "declaration")
      assert(decl:code_name() == "parm_decl")
      assert(decl:type():class_name() == "type")
      assert(decl:type():code_name() == "void_type")
    end
    do
      local event, decl = unpack(table.remove(result))
      assert(event == "PLUGIN_FINISH_DECL")
      assert(decl:class_name() == "declaration")
      assert(decl:code_name() == "type_decl")
      assert(decl:type():class_name() == "type")
      assert(decl:type():code_name() == "union_type")
      assert(decl:name():value() == "up_charm_top_quarks")
    end
  end
  do
    local event, decl = unpack(table.remove(result))
    assert(event == "PLUGIN_FINISH_TYPE")
    assert(decl:class_name() == "type")
    assert(decl:code_name() == "union_type")
    assert(decl:name() == nil)
  end
  if gcc.PLUGIN_FINISH_DECL then -- GCC ≥ 4.7
    do
      local event, decl = unpack(table.remove(result))
      assert(event == "PLUGIN_FINISH_DECL")
      assert(decl:class_name() == "declaration")
      assert(decl:code_name() == "field_decl")
      assert(decl:name():value() == "top")
    end
    do
      local event, decl = unpack(table.remove(result))
      assert(event == "PLUGIN_FINISH_DECL")
      assert(decl:class_name() == "declaration")
      assert(decl:code_name() == "field_decl")
      assert(decl:name():value() == "charm")
    end
    do
      local event, decl = unpack(table.remove(result))
      assert(event == "PLUGIN_FINISH_DECL")
      assert(decl:class_name() == "declaration")
      assert(decl:code_name() == "field_decl")
      assert(decl:name():value() == "up")
    end
  end
  assert(#result == 0)
end)
