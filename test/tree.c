/*
 * Lua plugin for the GNU Compiler Collection.
 * Copyright © 2012–2015 Peter Colberg.
 * Distributed under the MIT license. (See accompanying file LICENSE.)
 */

/* constants */
unsigned long long fibonacci = 806515533049393ULL;
double pi = 3.14159265358979323846;
char const euler_mascheroni[] = "0.577215664901532860606512090082402431042159";

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
double _Complex *volatile *const restrict const_restrict_pointer_to_volatile_pointer_to_complex_double;
volatile _Bool *restrict (*const array_2_of_const_pointer_to_array_3_of_restrict_pointer_to_volatile_bool[2])[3];

/* enum and struct */
enum stratosphere { STRATOSPHERE_MIN = 10000, STRATOSPHERE_MAX = 50000 };
typedef enum stratosphere stratosphere_t;
enum stratosphere *const const_pointer_to_enum_stratosphere;
stratosphere_t *const const_pointer_to_stratosphere_t;
typedef struct mesosphere mesosphere_t;
struct mesosphere *const const_pointer_to_struct_mesosphere;
mesosphere_t *const const_pointer_to_mesosphere_t;

/* bitfield */
struct {
  unsigned int unsigned_int_bitfield : 1;
  int signed_int_bitfield : 2;
  unsigned int : 3;
  int : 4;
  unsigned short unsigned_short;
} bitfield;

/* vector */
double vector_2_of_double __attribute__((vector_size(16)));
int vector_8_of_int __attribute__((vector_size(32)));

/* packed struct */
struct __attribute__((packed)) {
  int i;
  double d;
} packed_struct;

/* non-default alignment */
struct {
  int __attribute__((aligned(16))) s[4];
} user_align;
struct {
  int s[4];
} default_align;

/* attributes */
typedef int user_aligned_int_t __attribute__((aligned(8)));
user_aligned_int_t user_aligned_int __attribute__((unused));

/* function */
void (*function_int_returning_void)(int);

/* operand */
const char *address_of_const_char = &const_char;

/* constructor */
int array_initializer[] = {0, 1, 1, 2, 3, 5, 8};
