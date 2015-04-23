--
-- Lua plugin for the GNU Compiler Collection.
-- Copyright © 2012–2015 Peter Colberg.
-- Distributed under the MIT license. (See accompanying file LICENSE.)
--

local gcc = require("gcc")

local test = {}

gcc.set_asm_file_name(gcc.HOST_BIT_BUCKET)

gcc.register_callback(gcc.PLUGIN_FINISH_UNIT, function()
  local vars = gcc.get_variables()
  for i = #vars, 1, -1 do
    local decl = assert(vars[i])
    local name = assert(decl:name():value())
    test[name](decl)
  end
end)

-- anonymous struct and union
function test.unnamed_union(decl)
  assert(decl:code() == "var_decl")
  assert(decl:type():code() == "union_type")
  assert(decl:type():anonymous() == true)
  assert(decl:type():name():code() == "type_decl")
  assert(decl:type():name():artificial() == true)
  assert(decl:type():fields():code() == "field_decl")
  assert(decl:type():fields():name():value() == "unnamed")
  assert(decl:type():fields():type():code() == "record_type")
  assert(decl:type():fields():type():anonymous() == true)
  assert(decl:type():fields():chain():code() == "field_decl")
  assert(decl:type():fields():chain():name():value() == "named")
  assert(decl:type():fields():chain():type():code() == "record_type")
  assert(decl:type():fields():chain():type():anonymous() == false)
  assert(decl:type():fields():chain():chain():code() == "type_decl")
  assert(decl:type():fields():chain():chain():artificial() == true)
  assert(decl:type():fields():chain():chain():type():canonical() == decl:type():canonical())
  assert(decl:type():fields():chain():chain():chain():code() == "type_decl")
  assert(decl:type():fields():chain():chain():chain():artificial() == true)
  assert(decl:type():fields():chain():chain():chain():type():canonical() == decl:type():fields():type():canonical())
  assert(decl:type():fields():chain():chain():chain():chain():code() == "type_decl")
  assert(decl:type():fields():chain():chain():chain():chain():artificial() == true)
  assert(decl:type():fields():chain():chain():chain():chain():type():canonical() == decl:type():fields():chain():type():canonical())
  assert(decl:type():fields():chain():chain():chain():chain():chain() == nil)
end

function test.named_union(decl)
  assert(decl:code() == "var_decl")
  assert(decl:type():code() == "union_type")
  assert(decl:type():anonymous() == false)
  assert(decl:type():name():code() == "type_decl")
  assert(decl:type():name():artificial() == false)
  assert(decl:type():fields():code() == "field_decl")
  assert(decl:type():fields():name():value() == "unnamed")
  assert(decl:type():fields():type():code() == "record_type")
  assert(decl:type():fields():type():anonymous() == true)
  assert(decl:type():fields():chain():code() == "field_decl")
  assert(decl:type():fields():chain():name():value() == "named")
  assert(decl:type():fields():chain():type():code() == "record_type")
  assert(decl:type():fields():chain():type():anonymous() == false)
  assert(decl:type():fields():chain():chain():code() == "type_decl")
  assert(decl:type():fields():chain():chain():artificial() == true)
  assert(decl:type():fields():chain():chain():type():canonical() == decl:type():canonical())
  assert(decl:type():fields():chain():chain():chain():code() == "type_decl")
  assert(decl:type():fields():chain():chain():chain():artificial() == true)
  assert(decl:type():fields():chain():chain():chain():type():canonical() == decl:type():fields():type():canonical())
  assert(decl:type():fields():chain():chain():chain():chain():code() == "type_decl")
  assert(decl:type():fields():chain():chain():chain():chain():artificial() == true)
  assert(decl:type():fields():chain():chain():chain():chain():type():canonical() == decl:type():fields():chain():type():canonical())
  assert(decl:type():fields():chain():chain():chain():chain():chain() == nil)
end

-- anonymous enum
function test.unnamed_oligomer(decl)
  assert(decl:code() == "var_decl")
  assert(decl:type():code() == "enumeral_type")
  assert(decl:type():anonymous() == true)
  assert(decl:type():name():artificial() == true)
end

function test.named_oligomer(decl)
  assert(decl:code() == "var_decl")
  assert(decl:type():code() == "enumeral_type")
  assert(decl:type():anonymous() == false)
  assert(decl:type():name():artificial() == true)
end
