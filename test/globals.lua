--
-- Lua plugin for the GNU Compiler Collection.
-- Copyright © 2012 Peter Colberg.
-- For conditions of distribution and use, see copyright notice in LICENSE.
--

local gcc = require("gcc")

assert(arg.script == "globals.lua")

assert(gcc.get_asm_file_name() == "globals.s")
gcc.set_asm_file_name(gcc.HOST_BIT_BUCKET)
assert(gcc.get_asm_file_name() == gcc.HOST_BIT_BUCKET)

gcc.register_callback(gcc.PLUGIN_FINISH_UNIT, function()
  do
    local units = gcc.get_translation_units()
    assert(#units == 1)
    local unit = units[1]
    assert(unit:code_name() == "translation_unit_decl")
    assert(unit:block():code_name() == "block")
    local vars = assert(unit:block():vars())
    assert(vars:code_name() == "type_decl")
    assert(vars:name():value() == "electron")
    assert(vars:name() == gcc.get_identifier("electron"))
    assert(vars:chain():code_name() == "type_decl")
    assert(vars:chain():name():value() == "muon")
    assert(vars:chain():name() == gcc.get_identifier("muon"))
    assert(vars:chain():chain():code_name() == "type_decl")
    assert(vars:chain():chain():type():code_name() == "enumeral_type")
    assert(vars:chain():chain():type():name():value() == "tau")
    assert(vars:chain():chain():type():name() == gcc.get_identifier("tau"))
    assert(vars:chain():chain():chain():code_name() == "const_decl")
    assert(vars:chain():chain():chain():name():value() == "TAU")
    assert(vars:chain():chain():chain():chain() == nil)
  end
  do
    local functions = gcc.get_functions()
    assert(#functions == 2)
    local decl = functions[1]
    assert(decl:code_name() == "function_decl")
    assert(decl:name():value() == "tau_neutrino")
    assert(decl:name() == gcc.get_identifier("tau_neutrino"))
    assert(decl:name() ~= gcc.get_identifier("electron_neutrino"))
    local decl = functions[2]
    assert(decl:code_name() == "function_decl")
    assert(decl:name():value() == "muon_neutrino")
    assert(decl:name() == gcc.get_identifier("muon_neutrino"))
  end
  do
    local vars = gcc.get_variables()
    assert(#vars == 2)
    local decl = vars[1]
    assert(decl:code_name() == "var_decl")
    assert(decl:name():value() == "anti_tau")
    assert(decl:name() == gcc.get_identifier("anti_tau"))
    assert(decl:name() ~= gcc.get_identifier("anti_muon"))
    local decl = vars[2]
    assert(decl:code_name() == "var_decl")
    assert(decl:name():value() == "positron")
    assert(decl:name() == gcc.get_identifier("positron"))
  end
end)
