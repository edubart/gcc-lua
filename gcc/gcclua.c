/*
 * Lua plugin for the GNU Compiler Collection.
 * Copyright © 2012 Peter Colberg.
 * Distributed under the MIT license. (See accompanying file LICENSE.)
 */

#include "gcc-plugin.h"
#include "plugin-version.h"
#ifndef GCCPLUGIN_VERSION
#define GCCPLUGIN_VERSION BUILDING_GCC_VERSION
#include "bversion.h"
#endif
#include "coretypes.h"
#include "tm.h"
#include "real.h"
#include "tree.h"
#include "cgraph.h"
#include "cp/cp-tree.h"
#include "diagnostic.h"
#include "tree-iterator.h"
#if GCCPLUGIN_VERSION >= 4006
#include "c-family/c-pragma.h"
#else
#include "c-pragma.h"
#endif
#if GCCPLUGIN_VERSION >= 4009
#include "stringpool.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#ifdef __cplusplus
}
#endif
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if LUA_VERSION_NUM < 502
#define luaL_setfuncs(L, l, nup) luaL_register(L, NULL, l)
#endif

#if GCCPLUGIN_VERSION <= 4007
#undef FOR_EACH_VEC_ELT
#define FOR_EACH_VEC_ELT(v, i, t) for (i = 0; VEC_iterate(tree, &(v), (i), (t)); ++(i))
#define FOR_EACH_FUNCTION(node) for ((node) = cgraph_nodes; (node); (node) = (node)->next)
#define FOR_EACH_VARIABLE(node) for ((node) = varpool_nodes; (node); (node) = (node)->next)
#endif

#if GCCPLUGIN_VERSION <= 4008
#define get_tree_code_name(code) tree_code_name[(int)(code)]
#endif

/* http://www.gnu.org/licenses/license-list.html#GPLCompatibleLicenses */
int plugin_is_GPL_compatible;

/* weak symbol is non-NULL for C++ frontend */
__typeof__(namespace_binding) namespace_binding __attribute__((weak));

/* registry key for tree userdata cache */
static char gcclua_tree_cache;
/* registry keys for tree metatable per tree code */
static char gcclua_tree_meta[MAX_TREE_CODES];

static void gcclua_tree_new(lua_State *L, const tree node)
{
  enum tree_code code;
  tree *t;
  lua_pushlightuserdata(L, &gcclua_tree_cache);
  lua_rawget(L, LUA_REGISTRYINDEX);
  if (lua_isnil(L, -1)) {
    lua_pushliteral(L, "cannot get tree cache table");
    lua_error(L);
  }
  lua_pushlightuserdata(L, node);
  lua_rawget(L, -2);
  if (!lua_isnil(L, -1)) {
    lua_replace(L, -2);
    return;
  }
  lua_pop(L, 1);
  t = (tree *)lua_newuserdata(L, sizeof(*t));
  *t = node;
  code = TREE_CODE(node);
  lua_pushlightuserdata(L, &gcclua_tree_meta[(int)code]);
  lua_rawget(L, LUA_REGISTRYINDEX);
  if (lua_isnil(L, -1)) {
    lua_pushliteral(L, "cannot get tree code metatable");
    lua_error(L);
  }
  lua_setmetatable(L, -2);
  lua_pushlightuserdata(L, node);
  lua_pushvalue(L, -2);
  lua_rawset(L, -4);
  lua_replace(L, -2);
}

static int gcclua_tree_get_bind_expr_block(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = BIND_EXPR_BLOCK(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_bind_expr_body(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = BIND_EXPR_BODY(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_bind_expr_vars(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = BIND_EXPR_VARS(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_block_vars(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = BLOCK_VARS(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_chain(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = TREE_CHAIN(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_code_name(lua_State *L)
{
  const tree *t;
  enum tree_code code;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  code = TREE_CODE(*t);
  lua_pushstring(L, get_tree_code_name(code));
  return 1;
}

static int gcclua_tree_get_code_class_name(lua_State *L)
{
  const tree *t;
  enum tree_code code;
  enum tree_code_class code_class;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  code = TREE_CODE(*t);
  code_class = TREE_CODE_CLASS(code);
  lua_pushstring(L, TREE_CODE_CLASS_STRING(code_class));
  return 1;
}

static int gcclua_tree_get_constructor_elts(lua_State *L)
{
  const tree *t;
  int i, n;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  n = CONSTRUCTOR_NELTS(*t);
  lua_createtable(L, n, 0);
  for (i = 0; i < n; i++) {
    gcclua_tree_new(L, CONSTRUCTOR_ELT(*t, i)->value);
    lua_rawseti(L, -2, i + 1);
  }
  return 1;
}

static int gcclua_tree_get_decl_align(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  lua_pushnumber(L, DECL_ALIGN(*t));
  return 1;
}

static int gcclua_tree_get_decl_align_unit(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  lua_pushnumber(L, DECL_ALIGN_UNIT(*t));
  return 1;
}

static int gcclua_tree_get_decl_assembler_name(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  if (!DECL_ASSEMBLER_NAME_SET_P(*t)) {
    return 0;
  }
  child = DECL_ASSEMBLER_NAME(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_decl_artificial(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  lua_pushboolean(L, DECL_ARTIFICIAL(*t));
  return 1;
}

static int gcclua_tree_get_decl_attributes(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = DECL_ATTRIBUTES(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_decl_arguments(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = DECL_ARGUMENTS(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_decl_bit_field(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  lua_pushboolean(L, DECL_BIT_FIELD(*t));
  return 1;
}

static int gcclua_tree_get_decl_bit_field_type(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = DECL_BIT_FIELD_TYPE(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_decl_chain(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
#if GCCPLUGIN_VERSION >= 4006
  child = DECL_CHAIN(*t);
#else
  child = TREE_CHAIN(DECL_MINIMAL_CHECK(*t));
#endif
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_decl_external(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  lua_pushboolean(L, DECL_EXTERNAL(*t));
  return 1;
}

static int gcclua_tree_get_decl_initial(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = DECL_INITIAL(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_decl_name(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = DECL_NAME(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_decl_uid(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  lua_pushnumber(L, DECL_UID(*t));
  return 1;
}

static int gcclua_tree_get_decl_saved_tree(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = DECL_SAVED_TREE(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_decl_size(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = DECL_SIZE(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_decl_size_unit(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = DECL_SIZE_UNIT(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_decl_source_location(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  if (DECL_IS_BUILTIN(*t)) {
    return 0;
  }
  lua_pushstring(L, DECL_SOURCE_FILE(*t));
  lua_pushnumber(L, DECL_SOURCE_LINE(*t));
#if GCCPLUGIN_VERSION <= 4007
  return 2;
#else
  lua_pushnumber(L, DECL_SOURCE_COLUMN(*t));
  return 3;
#endif
}

static int gcclua_tree_get_decl_user_align(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  lua_pushboolean(L, DECL_USER_ALIGN(*t));
  return 1;
}

static int gcclua_tree_get_identifier(lua_State *L)
{
  const tree *t;
  const char *s;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  s = IDENTIFIER_POINTER(*t);
  if (!s) {
    return 0;
  }
  lua_pushstring(L, s);
  return 1;
}

static int gcclua_tree_get_int_cst(lua_State *L)
{
  const tree *t;
  uint64_t u;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
#if HOST_BITS_PER_WIDE_INT == 64
  u = TREE_INT_CST_LOW(*t);
#elif HOST_BITS_PER_WIDE_INT == 32
  u = ((uint64_t)TREE_INT_CST_HIGH(*t)) << 32 | TREE_INT_CST_LOW(*t);
#else
#error unsupported HOST_BITS_PER_WIDE_INT
#endif
  lua_pushnumber(L, (int64_t)u);
  return 1;
}

static int gcclua_tree_get_call_expr_fn(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  gcclua_tree_new(L, CALL_EXPR_FN(*t));
  return 1;
}

static int gcclua_tree_get_call_expr_arg(lua_State *L)
{
  const tree *t;
  int i, n;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  n = call_expr_nargs(*t);
  for (i = 0; i < n; i++) {
    gcclua_tree_new(L, CALL_EXPR_ARG(*t, i));
  }
  return n;
}

static int gcclua_tree_get_operand(lua_State *L)
{
  const tree *t;
  int i, n;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  n = TREE_OPERAND_LENGTH(*t);
  for (i = 0; i < n; i++) {
    gcclua_tree_new(L, TREE_OPERAND(*t, i));
  }
  return n;
}

static int gcclua_tree_get_public(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  lua_pushboolean(L, TREE_PUBLIC(*t));
  return 1;
}

static int gcclua_tree_get_purpose(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = TREE_PURPOSE(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_real_cst(lua_State *L)
{
  const tree *t;
#if DOUBLE_TYPE_SIZE == 64
  long buf[2];
  union {
    uint32_t i[2];
    double d;
  } u;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  REAL_VALUE_TO_TARGET_DOUBLE(TREE_REAL_CST(*t), buf);
  u.i[0] = (buf[0] & 0xffffffff);
  u.i[1] = (buf[1] & 0xffffffff);
  lua_pushnumber(L, u.d);
#else
#error unsupported DOUBLE_TYPE_SIZE
#endif
  return 1;
}

static int gcclua_tree_get_statement_list(lua_State *L)
{
  const tree *t;
  tree_stmt_iterator it;
  int i;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  lua_newtable(L);
  for (it = tsi_start(*t), i = 1; !tsi_end_p(it); tsi_next(&it), i++) {
    gcclua_tree_new(L, tsi_stmt(it));
    lua_rawseti(L, -2, i);
  }
  return 1;
}

static int gcclua_tree_get_string_cst(lua_State *L)
{
  const tree *t;
  const char *s;
  size_t size;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  s = TREE_STRING_POINTER(*t);
  if (!s) {
    return 0;
  }
  size = TREE_STRING_LENGTH(*t);
  lua_pushlstring(L, s, size - 1);
  return 1;
}

#if GCCPLUGIN_VERSION >= 4006
static int gcclua_tree_get_translation_unit_language(lua_State *L)
{
  const tree *t;
  const char *s;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  s = TRANSLATION_UNIT_LANGUAGE(*t);
  if (!s) {
    return 0;
  }
  lua_pushstring(L, s);
  return 1;
}
#endif

static int gcclua_tree_get_type(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = TREE_TYPE(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_type_align(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  lua_pushnumber(L, TYPE_ALIGN(*t));
  return 1;
}

static int gcclua_tree_get_type_align_unit(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  lua_pushnumber(L, TYPE_ALIGN_UNIT(*t));
  return 1;
}

static int gcclua_tree_get_type_anonymous(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  if (!namespace_binding) {
    return 0;
  }
  lua_pushboolean(L, TYPE_ANONYMOUS_P(*t));
  return 1;
}

static int gcclua_tree_get_type_arg_types(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = TYPE_ARG_TYPES(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_type_attributes(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = TYPE_ATTRIBUTES(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_type_canonical(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = TYPE_CANONICAL(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_type_const(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  lua_pushboolean(L, TYPE_READONLY(*t));
  return 1;
}

static int gcclua_tree_get_type_domain(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = TYPE_DOMAIN(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_type_fields(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = TYPE_FIELDS(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_type_main_variant(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = TYPE_MAIN_VARIANT(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_type_max_value(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = TYPE_MAX_VALUE(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_type_min_value(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = TYPE_MIN_VALUE(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_type_name(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = TYPE_NAME(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_type_packed(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  lua_pushboolean(L, TYPE_PACKED(*t));
  return 1;
}

static int gcclua_tree_get_type_restrict(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  lua_pushboolean(L, TYPE_RESTRICT(*t));
  return 1;
}

static int gcclua_tree_get_type_size(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = TYPE_SIZE(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_type_size_unit(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = TYPE_SIZE_UNIT(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_type_stub_decl(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = TYPE_STUB_DECL(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_type_unsigned(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  lua_pushboolean(L, TYPE_UNSIGNED(*t));
  return 1;
}

static int gcclua_tree_get_type_user_align(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  lua_pushboolean(L, TYPE_USER_ALIGN(*t));
  return 1;
}

static int gcclua_tree_get_type_values(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = TYPE_VALUES(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_tree_get_type_vector_subparts(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  lua_pushnumber(L, TYPE_VECTOR_SUBPARTS(*t));
  return 1;
}

static int gcclua_tree_get_type_volatile(lua_State *L)
{
  const tree *t;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  lua_pushboolean(L, TYPE_VOLATILE(*t));
  return 1;
}

static int gcclua_tree_get_value(lua_State *L)
{
  const tree *t;
  tree child;
  luaL_checktype(L, 1, LUA_TUSERDATA);
  t = (const tree *)lua_touserdata(L, 1);
  child = TREE_VALUE(*t);
  if (!child) {
    return 0;
  }
  gcclua_tree_new(L, child);
  return 1;
}

static int gcclua_traceback(lua_State *L)
{
  lua_getglobal(L, "debug");
  lua_getfield(L, -1, "traceback");
  lua_pushvalue(L, 1);
  lua_pushnumber(L, 2);
  lua_call(L, 2, 1);
  return 1;
}

struct gcclua_cbdata {
  int ref_cb;
  int ref_fn;
  lua_State *L;
};

static void gcclua_callback(void *event_data, void *data)
{
  const struct gcclua_cbdata *cb;
  lua_State *L;
  cb = (const struct gcclua_cbdata *)data;
  L = (lua_State *)cb->L;
  lua_pushcfunction(L, gcclua_traceback);
  lua_rawgeti(L, LUA_REGISTRYINDEX, cb->ref_fn);
  if (lua_pcall(L, 0, 0, -2)) {
    error("%s", lua_tostring(L, -1));
    return;
  }
}

static void gcclua_callback_decl(void *event_data, void *data)
{
  const struct gcclua_cbdata *cb;
  lua_State *L;
  cb = (const struct gcclua_cbdata *)data;
  L = (lua_State *)cb->L;
  lua_pushcfunction(L, gcclua_traceback);
  lua_rawgeti(L, LUA_REGISTRYINDEX, cb->ref_fn);
  gcclua_tree_new(L, (tree)event_data);
  if (lua_pcall(L, 1, 0, -3)) {
    error("%s", lua_tostring(L, -1));
    return;
  }
}

struct gcclua_plugin_event_reg {
  const char *name;
  enum plugin_event ev;
  plugin_callback_func fn;
};

static const struct gcclua_plugin_event_reg gcclua_plugin_event[] = {
  {"PLUGIN_START_UNIT",     PLUGIN_START_UNIT,     gcclua_callback},
  {"PLUGIN_PRE_GENERICIZE", PLUGIN_PRE_GENERICIZE, gcclua_callback_decl},
#if GCCPLUGIN_VERSION >= 4007
  {"PLUGIN_FINISH_DECL",    PLUGIN_FINISH_DECL,    gcclua_callback_decl},
#endif
  {"PLUGIN_FINISH_TYPE",    PLUGIN_FINISH_TYPE,    gcclua_callback_decl},
  {"PLUGIN_FINISH_UNIT",    PLUGIN_FINISH_UNIT,    gcclua_callback},
  {NULL},
};

static int gcclua_register_callback(lua_State *L)
{
  const struct plugin_name_args *info;
  struct gcclua_cbdata *cb;
  enum plugin_event ev;
  const struct gcclua_plugin_event_reg *ev_reg;
  ev = (enum plugin_event)luaL_checknumber(L, 1);
  luaL_checkany(L, 2);
  lua_pushlightuserdata(L, &plugin_is_GPL_compatible);
  lua_rawget(L, LUA_REGISTRYINDEX);
  info = (struct plugin_name_args *)lua_touserdata(L, -1);
  if (!info) {
    lua_pushliteral(L, "cannot get plugin info");
    lua_error(L);
  }
  lua_pop(L, 1);
  cb = (struct gcclua_cbdata *)lua_newuserdata(L, sizeof(*cb));
  cb->ref_cb = luaL_ref(L, LUA_REGISTRYINDEX);
  cb->ref_fn = luaL_ref(L, LUA_REGISTRYINDEX);
  cb->L = L;
  for (ev_reg = gcclua_plugin_event; ev_reg->name; ev_reg++) {
    if (ev_reg->ev == ev) {
      break;
    }
  }
  if (!ev_reg->name) {
    lua_pushliteral(L, "unsupported plugin event");
    lua_error(L);
  }
  register_callback(info->base_name, ev, ev_reg->fn, cb);
  return 0;
}

static int gcclua_get_asm_file_name(lua_State *L)
{
  lua_pushstring(L, asm_file_name);
  return 1;
}

static int gcclua_set_asm_file_name(lua_State *L)
{
  asm_file_name = luaL_checkstring(L, 1);
  lua_pushlightuserdata(L, &asm_file_name);
  lua_pushvalue(L, -2);
  lua_rawset(L, LUA_REGISTRYINDEX);
  return 0;
}

#if GCCPLUGIN_VERSION >= 4006
static int gcclua_get_main_input_basename(lua_State *L)
{
  lua_pushstring(L, main_input_basename);
  return 1;
}
#endif

static int gcclua_get_main_input_filename(lua_State *L)
{
  lua_pushstring(L, main_input_filename);
  return 1;
}

#if GCCPLUGIN_VERSION >= 4006
static int gcclua_get_translation_units(lua_State *L)
{
  tree t;
  int i;
  lua_newtable(L);
  FOR_EACH_VEC_ELT(*all_translation_units, i, t)
  {
    gcclua_tree_new(L, t);
    lua_rawseti(L, -2, i + 1);
  }
  return 1;
}
#endif

static int gcclua_get_functions(lua_State *L)
{
  struct cgraph_node *node;
  int i;
  lua_createtable(L, cgraph_n_nodes, 0);
  i = 1;
  FOR_EACH_FUNCTION(node) {
#if GCCPLUGIN_VERSION == 4008
    gcclua_tree_new(L, node->symbol.decl);
#else
    gcclua_tree_new(L, node->decl);
#endif
    lua_rawseti(L, -2, i);
    i++;
  }
  return 1;
}

static int gcclua_get_variables(lua_State *L)
{
  struct varpool_node *node;
  int i;
  lua_newtable(L);
  i = 1;
  FOR_EACH_VARIABLE(node) {
#if GCCPLUGIN_VERSION == 4008
    gcclua_tree_new(L, node->symbol.decl);
#else
    gcclua_tree_new(L, node->decl);
#endif
    lua_rawseti(L, -2, i);
    i++;
  }
  return 1;
}

static int gcclua_get_identifier(lua_State *L)
{
  const char *s;
  s = luaL_checkstring(L, 1);
  gcclua_tree_new(L, get_identifier(s));
  return 1;
}

static int gcclua_define_macro(lua_State *L)
{
  const char *s;
  s = luaL_checkstring(L, 1);
  cpp_define(parse_in, s);
  return 0;
}

static const luaL_Reg gcclua_tree[] = {
  {"class",      gcclua_tree_get_code_class_name},
  {"code",       gcclua_tree_get_code_name},
  {NULL, NULL},
};

static const luaL_Reg gcclua_binary[] = {
  {"operand", gcclua_tree_get_operand},
  {"type",    gcclua_tree_get_type},
  {NULL, NULL},
};

static const luaL_Reg gcclua_constant[] = {
  {"type", gcclua_tree_get_type},
  {NULL, NULL},
};

static const luaL_Reg gcclua_declaration[] = {
  {"align",          gcclua_tree_get_decl_align},
  {"align_unit",     gcclua_tree_get_decl_align_unit},
  {"artificial",     gcclua_tree_get_decl_artificial},
  {"assembler_name", gcclua_tree_get_decl_assembler_name},
  {"attributes",     gcclua_tree_get_decl_attributes},
  {"chain",          gcclua_tree_get_decl_chain},
  {"location",       gcclua_tree_get_decl_source_location},
  {"name",           gcclua_tree_get_decl_name},
  {"size",           gcclua_tree_get_decl_size},
  {"size_unit",      gcclua_tree_get_decl_size_unit},
  {"type",           gcclua_tree_get_type},
  {"uid",            gcclua_tree_get_decl_uid},
  {"user_align",     gcclua_tree_get_decl_user_align},
  {NULL, NULL},
};

static const luaL_Reg gcclua_expression[] = {
  {"operand", gcclua_tree_get_operand},
  {"type",    gcclua_tree_get_type},
  {NULL, NULL},
};

static const luaL_Reg gcclua_type[] = {
  {"align",        gcclua_tree_get_type_align},
  {"align_unit",   gcclua_tree_get_type_align_unit},
  {"attributes",   gcclua_tree_get_type_attributes},
  {"canonical",    gcclua_tree_get_type_canonical},
  {"const",        gcclua_tree_get_type_const},
  {"main_variant", gcclua_tree_get_type_main_variant},
  {"name",         gcclua_tree_get_type_name},
  {"packed",       gcclua_tree_get_type_packed},
  {"restrict",     gcclua_tree_get_type_restrict},
  {"size",         gcclua_tree_get_type_size},
  {"size_unit",    gcclua_tree_get_type_size_unit},
  {"stub_decl",    gcclua_tree_get_type_stub_decl},
  {"user_align",   gcclua_tree_get_type_user_align},
  {"volatile",     gcclua_tree_get_type_volatile},
  {NULL, NULL},
};

static const luaL_Reg gcclua_unary[] = {
  {"operand", gcclua_tree_get_operand},
  {"type",    gcclua_tree_get_type},
  {NULL, NULL},
};

static const luaL_Reg gcclua_vl_exp[] = {
  {"type",    gcclua_tree_get_type},
  {NULL, NULL},
};

struct gcclua_tree_code_class_reg {
  const luaL_Reg *reg;
  enum tree_code_class cls;
};

static const struct gcclua_tree_code_class_reg gcclua_tree_code_class[] = {
  {gcclua_binary,      tcc_binary},
  {gcclua_constant,    tcc_constant},
  {gcclua_declaration, tcc_declaration},
  {gcclua_expression,  tcc_expression},
  {gcclua_type,        tcc_type},
  {gcclua_unary,       tcc_unary},
  {gcclua_vl_exp,      tcc_vl_exp},
  {NULL},
};

static const luaL_Reg gcclua_array_type[] = {
  {"domain", gcclua_tree_get_type_domain},
  {"type",   gcclua_tree_get_type},
  {NULL, NULL},
};

static const luaL_Reg gcclua_bind_expr[] = {
  {"block", gcclua_tree_get_bind_expr_block},
  {"body",  gcclua_tree_get_bind_expr_body},
  {"vars",  gcclua_tree_get_bind_expr_vars},
  {NULL, NULL},
};

static const luaL_Reg gcclua_block[] = {
  {"vars", gcclua_tree_get_block_vars},
  {NULL, NULL},
};

static const luaL_Reg gcclua_call_expr[] = {
  {"function", gcclua_tree_get_call_expr_fn},
  {"args", gcclua_tree_get_call_expr_arg},
  {NULL, NULL},
};

static const luaL_Reg gcclua_constructor[] = {
  {"elements", gcclua_tree_get_constructor_elts},
  {NULL, NULL},
};

static const luaL_Reg gcclua_const_decl[] = {
  {"initial",    gcclua_tree_get_decl_initial},
  {NULL, NULL},
};

static const luaL_Reg gcclua_enumeral_type[] = {
  {"values",    gcclua_tree_get_type_values},
  /* C++ frontend */
  {"anonymous", gcclua_tree_get_type_anonymous},
  {NULL, NULL},
};

static const luaL_Reg gcclua_field_decl[] = {
  {"bit_field",      gcclua_tree_get_decl_bit_field},
  {"bit_field_type", gcclua_tree_get_decl_bit_field_type},
  {"initial",        gcclua_tree_get_decl_initial},
  {NULL, NULL},
};

static const luaL_Reg gcclua_function_decl[] = {
  {"args",     gcclua_tree_get_decl_arguments},
  {"body",     gcclua_tree_get_decl_saved_tree},
  {"external", gcclua_tree_get_decl_external},
  {"public",   gcclua_tree_get_public},
  {NULL, NULL},
};

static const luaL_Reg gcclua_function_type[] = {
  {"args", gcclua_tree_get_type_arg_types},
  {"type", gcclua_tree_get_type},
  {NULL, NULL},
};

static const luaL_Reg gcclua_identifier_node[] = {
  {"value", gcclua_tree_get_identifier},
  {NULL, NULL},
};

static const luaL_Reg gcclua_integer_cst[] = {
  {"value", gcclua_tree_get_int_cst},
  {NULL, NULL},
};

static const luaL_Reg gcclua_integer_type[] = {
  {"max",      gcclua_tree_get_type_max_value},
  {"min",      gcclua_tree_get_type_min_value},
  {"unsigned", gcclua_tree_get_type_unsigned},
  {NULL, NULL},
};

static const luaL_Reg gcclua_pointer_type[] = {
  {"type",     gcclua_tree_get_type},
  {"unsigned", gcclua_tree_get_type_unsigned},
  {NULL, NULL},
};

static const luaL_Reg gcclua_real_cst[] = {
  {"value", gcclua_tree_get_real_cst},
  {NULL, NULL},
};

static const luaL_Reg gcclua_record_type[] = {
  {"fields",    gcclua_tree_get_type_fields},
  /* C++ frontend */
  {"anonymous", gcclua_tree_get_type_anonymous},
  {NULL, NULL},
};

static const luaL_Reg gcclua_reference_type[] = {
  {"type", gcclua_tree_get_type},
  {NULL, NULL},
};

static const luaL_Reg gcclua_return_expr[] = {
  {"operand", gcclua_tree_get_operand},
  {NULL, NULL},
};

static const luaL_Reg gcclua_statement_list[] = {
  {"statements", gcclua_tree_get_statement_list},
  {NULL, NULL},
};

static const luaL_Reg gcclua_string_cst[] = {
  {"value", gcclua_tree_get_string_cst},
  {NULL, NULL},
};

static const luaL_Reg gcclua_translation_unit_decl[] = {
  {"block",    gcclua_tree_get_decl_initial},
#if GCCPLUGIN_VERSION >= 4006
  {"language", gcclua_tree_get_translation_unit_language},
#endif
  {NULL, NULL},
};

static const luaL_Reg gcclua_tree_list[] = {
  {"chain",   gcclua_tree_get_chain},
  {"purpose", gcclua_tree_get_purpose},
  {"value",   gcclua_tree_get_value},
  {NULL, NULL},
};

static const luaL_Reg gcclua_type_decl[] = {
  {"public", gcclua_tree_get_public},
  {NULL, NULL},
};

static const luaL_Reg gcclua_union_type[] = {
  {"fields",    gcclua_tree_get_type_fields},
  /* C++ frontend */
  {"anonymous", gcclua_tree_get_type_anonymous},
  {NULL, NULL},
};

static const luaL_Reg gcclua_var_decl[] = {
  {"external", gcclua_tree_get_decl_external},
  {"initial",  gcclua_tree_get_decl_initial},
  {"public",   gcclua_tree_get_public},
  {NULL, NULL},
};

static const luaL_Reg gcclua_vector_type[] = {
  {"type",  gcclua_tree_get_type},
  {"units", gcclua_tree_get_type_vector_subparts},
  {NULL, NULL},
};

static const luaL_Reg gcclua[] = {
  {"register_callback",       gcclua_register_callback},
  {"get_asm_file_name",       gcclua_get_asm_file_name},
  {"set_asm_file_name",       gcclua_set_asm_file_name},
#if GCCPLUGIN_VERSION >= 4006
  {"get_main_input_basename", gcclua_get_main_input_basename},
#endif
  {"get_main_input_filename", gcclua_get_main_input_filename},
#if GCCPLUGIN_VERSION >= 4006
  {"get_translation_units",   gcclua_get_translation_units},
#endif
  {"get_functions",           gcclua_get_functions},
  {"get_variables",           gcclua_get_variables},
  {"get_identifier",          gcclua_get_identifier},
  {"define_macro",            gcclua_define_macro},
  {NULL, NULL},
};

struct gcclua_tree_code_reg {
  const luaL_Reg *reg;
  enum tree_code code;
};

static const struct gcclua_tree_code_reg gcclua_tree_code[] = {
  {gcclua_array_type,            ARRAY_TYPE},
  {gcclua_bind_expr,             BIND_EXPR},
  {gcclua_block,                 BLOCK},
  {gcclua_call_expr,             CALL_EXPR},
  {gcclua_constructor,           CONSTRUCTOR},
  {gcclua_const_decl,            CONST_DECL},
  {gcclua_enumeral_type,         ENUMERAL_TYPE},
  {gcclua_field_decl,            FIELD_DECL},
  {gcclua_function_decl,         FUNCTION_DECL},
  {gcclua_function_type,         FUNCTION_TYPE},
  {gcclua_identifier_node,       IDENTIFIER_NODE},
  {gcclua_integer_cst,           INTEGER_CST},
  {gcclua_integer_type,          INTEGER_TYPE},
  {gcclua_pointer_type,          POINTER_TYPE},
  {gcclua_real_cst,              REAL_CST},
  {gcclua_record_type,           RECORD_TYPE},
  {gcclua_reference_type,        REFERENCE_TYPE},
  {gcclua_return_expr,           RETURN_EXPR},
  {gcclua_statement_list,        STATEMENT_LIST},
  {gcclua_string_cst,            STRING_CST},
  {gcclua_translation_unit_decl, TRANSLATION_UNIT_DECL},
  {gcclua_tree_list,             TREE_LIST},
  {gcclua_type_decl,             TYPE_DECL},
  {gcclua_union_type,            UNION_TYPE},
  {gcclua_var_decl,              VAR_DECL},
  {gcclua_vector_type,           VECTOR_TYPE},
  {NULL},
};

static int gcclua_loadlib(lua_State *L)
{
  const struct gcclua_tree_code_class_reg *cls_reg;
  const struct gcclua_tree_code_reg *code_reg;
  const struct gcclua_plugin_event_reg *event;
  enum tree_code_class cls;
  int i;
  lua_pushlightuserdata(L, &gcclua_tree_cache);
  lua_newtable(L);
  lua_pushvalue(L, -1);
  lua_pushliteral(L, "v");
  lua_setfield(L, -2, "__mode");
  lua_setmetatable(L, -2);
  lua_rawset(L, LUA_REGISTRYINDEX);
  lua_newtable(L);
  for (i = 0; i < MAX_TREE_CODES; ++i) {
    lua_pushlightuserdata(L, &gcclua_tree_meta[i]);
    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_setfield(L, -1, "__index");
    luaL_setfuncs(L, gcclua_tree, 0);
    cls = TREE_CODE_CLASS(i);
    for (cls_reg = gcclua_tree_code_class; cls_reg->reg; cls_reg++) {
      if (cls == cls_reg->cls) {
        luaL_setfuncs(L, cls_reg->reg, 0);
        break;
      }
    }
    for (code_reg = gcclua_tree_code; code_reg->reg; code_reg++) {
      if (i == code_reg->code) {
        luaL_setfuncs(L, code_reg->reg, 0);
        break;
      }
    }
    lua_rawset(L, LUA_REGISTRYINDEX);
  }
  luaL_setfuncs(L, gcclua, 0);
  for (event = gcclua_plugin_event; event->name; event++) {
    lua_pushnumber(L, event->ev);
    lua_setfield(L, -2, event->name);
  }
  lua_pushnumber(L, GCCPLUGIN_VERSION);
  lua_setfield(L, -2, "GCC_VERSION");
  lua_pushliteral(L, HOST_BIT_BUCKET);
  lua_setfield(L, -2, "HOST_BIT_BUCKET");
  return 1;
}

static void gcclua_ggc_start(void *event_data, void *data)
{
  ggc_protect_identifiers = true;
}

static void gcclua_ggc_marking(void *event_data, void *data)
{
  const tree *t;
  lua_State *L = (lua_State *)data;
  lua_pushlightuserdata(L, &gcclua_tree_cache);
  lua_rawget(L, LUA_REGISTRYINDEX);
  if (lua_isnil(L, -1)) {
    error("cannot get tree cache table");
    return;
  }
  lua_pushnil(L);
  while (lua_next(L, -2)) {
    t = (const tree *)lua_touserdata(L, -1);
    if (t) ggc_mark(*t);
    lua_pop(L, 1);
  }
  lua_pop(L, 1);
}

static void gcclua_finish(void *event_data, void *data)
{
  lua_State *L = (lua_State *)data;
  lua_close(L);
}

int plugin_init(struct plugin_name_args *info,
                struct plugin_gcc_version *version)
{
  const char *filename = NULL;
  int i;
  if (!plugin_default_version_check(version, &gcc_version)) {
    return 1;
  }
  lua_State *L = luaL_newstate();
  if (!L) {
    error("%s: cannot create state: out of memory", info->base_name);
    return 1;
  }
  register_callback(info->base_name, PLUGIN_GGC_START, gcclua_ggc_start, L);
  register_callback(info->base_name, PLUGIN_GGC_MARKING, gcclua_ggc_marking, L);
  register_callback(info->base_name, PLUGIN_FINISH, gcclua_finish, L);
  lua_gc(L, LUA_GCSTOP, 0);
  luaL_openlibs(L);
  lua_gc(L, LUA_GCRESTART, 0);
  lua_pushlightuserdata(L, &plugin_is_GPL_compatible);
  lua_pushlightuserdata(L, info);
  lua_rawset(L, LUA_REGISTRYINDEX);
  lua_getglobal(L, "package");
  lua_getfield(L, -1, "preload");
  lua_pushcfunction(L, gcclua_loadlib);
  lua_setfield(L, -2, "gcc");
  lua_pop(L, 2);
  lua_createtable(L, 0, info->argc);
  for (i = 0; i < info->argc; i++) {
    if (!strcmp(info->argv[i].key, "script")) {
      filename = info->argv[i].value;
    }
    lua_pushstring(L, info->argv[i].value);
    lua_setfield(L, -2, info->argv[i].key);
  }
  lua_setglobal(L, "arg");
  if (!filename) {
    error("%s: missing script filename", info->base_name);
    return 1;
  }
  lua_pushcfunction(L, gcclua_traceback);
  if ((luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, -2))) {
    error("%s", lua_tostring(L, -1));
    return 1;
  }
  return 0;
}
