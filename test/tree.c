/*
 * Lua plugin for the GNU Compiler Collection.
 * Copyright © 2012 Peter Colberg.
 * For conditions of distribution and use, see copyright notice in LICENSE.
 */

#include <complex.h>
#include <stdbool.h>

/* pointer and array */
typedef long troposphere_t;
troposphere_t *pointer_to_troposphere_t;
troposphere_t **pointer_to_pointer_to_troposphere_t;
troposphere_t array_3_of_troposphere_t[3];
troposphere_t array_3_of_array_4_of_troposphere_t[3][4];
troposphere_t *array_4_of_pointer_to_troposphere_t[4];
troposphere_t (*pointer_to_array_5_of_troposphere_t)[5];
troposphere_t (*(*pointer_to_array_4_of_pointer_to_array_5_of_int)[4])[5];

/* qualifier */
const char const_char;
const volatile float const_volatile_float;
void *const restrict const_restrict_pointer_to_void;
complex double *volatile *const restrict const_restrict_pointer_to_volatile_pointer_to_complex_double;
volatile bool *restrict (*const array_2_of_const_pointer_to_array_3_of_restrict_pointer_to_volatile_bool[2])[3];

/* enum and struct */
enum stratosphere { STRATOSPHERE_MIN = 10000, STRATOSPHERE_MAX = 50000 };
typedef enum stratosphere stratosphere_t;
enum stratosphere *const const_pointer_to_enum_stratosphere;
stratosphere_t *const const_pointer_to_stratosphere_t;
typedef struct mesosphere mesosphere_t;
struct mesosphere *const const_pointer_to_struct_mesosphere;
mesosphere_t *const const_pointer_to_mesosphere_t;

/* vector */
double vector_2_of_double __attribute__((vector_size(16)));
int vector_8_of_int __attribute__((vector_size(32)));

/* packed struct */
struct __attribute__((packed)) {
  int i;
  double d;
} packed_struct;
struct {
  int i;
  double d;
} unpacked_struct;

/* non-default alignment */
struct {
  int __attribute__((aligned(16))) s[4];
} user_align;
struct {
  int s[4];
} default_align;

/* function */
void (*function_int_returning_void)(int);

/* operand */
const char *address_of_const_char = &const_char;
