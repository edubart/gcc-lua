/*
 * Lua plugin for the GNU Compiler Collection.
 * Copyright © 2012 Peter Colberg.
 * For conditions of distribution and use, see copyright notice in LICENSE.
 */

/* anonymous struct and union */
static union {
  struct {
    int i;
    double d;
  } unnamed;
  struct named_struct_t {
    int i;
    double d;
  } named;
} unnamed_union __attribute__((unused));
typedef union named_union_t {
  struct {
    int i;
    double d;
  } unnamed;
  struct named_struct_t {
    int i;
    double d;
  } named;
} named_union_t;
named_union_t named_union;

/* anonymous enum */
static enum {
  MONOMER  = 1,
  DIMER    = 2
} unnamed_oligomer __attribute__((unused));
static enum named_enum_t {
  TRIMER   = 3,
  TETRAMER = 4
} named_oligomer __attribute__((unused));