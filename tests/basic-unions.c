#include <stdio.h>
#include <assert.h>

#include "basic-unions.h"

static void check_fixedunion_eight(void) {
  uint8_t fixedunion_bytes[] = {
    0x1, 0x0, 0x0, 0x0, /* "eight" */
    0x5, 0x0, 0x0, 0x0, /* Fixed8::one */
    0x6, 0x7, 0x8, 0x9, /* Fixed8::two */
  };

  const struct FixedUnion_bytes fixedunion = {fixedunion_bytes};
  assert(FixedUnion_size(fixedunion) == sizeof(fixedunion_bytes));
  assert(FixedUnion_has_eight(fixedunion) == true);
  assert(FixedUnion_has_sixteen(fixedunion) == false);

  const struct Fixed8_bytes eight = FixedUnion_get_eight(fixedunion);
  assert(Fixed8_get_one(eight) == 0x5);
  assert(Fixed8_get_two(eight, 0) == 0x6);
  assert(Fixed8_get_two(eight, 1) == 0x7);
  assert(Fixed8_get_two(eight, 2) == 0x8);
  assert(Fixed8_get_two(eight, 3) == 0x9);
}

static void check_fixedunion_sixteen(void) {
  uint8_t fixedunion_bytes[] = {
    0x2, 0x0, 0x0, 0x0, /* "sixteen" */
    0x5, 0x4, 0x0, 0x0, /* Fixed8::one */
    0x6, 0x7, 0x8, 0x9, /* Fixed8::two */
    0xa, 0xb, 0xc, 0xd  /* ^^ */
  };

  const struct FixedUnion_bytes fixedunion = {fixedunion_bytes};
  assert(FixedUnion_size(fixedunion) == sizeof(fixedunion_bytes));
  assert(FixedUnion_has_eight(fixedunion) == false);
  assert(FixedUnion_has_sixteen(fixedunion) == true);

  const struct Fixed16_bytes sixteen = FixedUnion_get_sixteen(fixedunion);
  assert(Fixed16_get_one(sixteen) == 0x0405);
  assert(Fixed16_get_two(sixteen, 0) == 0x0706);
  assert(Fixed16_get_two(sixteen, 1) == 0x0908);
  assert(Fixed16_get_two(sixteen, 2) == 0x0b0a);
  assert(Fixed16_get_two(sixteen, 3) == 0x0d0c);
}

static void check_varunion_eight(void) {
  uint8_t varunion_bytes[] = {
    0x3, 0x0, 0x0, 0x0, /* "eight" */
    0x3, 0x0, 0x0, 0x0, /* Var8::count */
    0x3, 0x4, 0x5,      /* Var8::one */
  };

  const struct VarUnion_bytes varunion = {varunion_bytes};
  assert(VarUnion_size(varunion) == sizeof(varunion_bytes));
  assert(VarUnion_has_eight(varunion) == true);
  assert(VarUnion_has_sixteen(varunion) == false);

  const struct Var8_bytes eight = VarUnion_get_eight(varunion);
  assert(Var8_get_count(eight) == 0x3);
  assert(Var8_get_one(eight, 0) == 0x3);
  assert(Var8_get_one(eight, 1) == 0x4);
  assert(Var8_get_one(eight, 2) == 0x5);
}

static void check_varunion_sixteen(void) {
  uint8_t varunion_bytes[] = {
    0x4, 0x0, 0x0, 0x0, /* "sixteen" */
    0x3, 0x0, 0x0, 0x0, /* Var16::count */
    0x3, 0x4,
    0x5, 0x6,      /* Var16::one */
    0x7, 0x8,
  };

  const struct VarUnion_bytes varunion = {varunion_bytes};
  assert(VarUnion_size(varunion) == sizeof(varunion_bytes));
  assert(VarUnion_has_eight(varunion) == false);
  assert(VarUnion_has_sixteen(varunion) == true);

  const struct Var16_bytes sixteen = VarUnion_get_sixteen(varunion);
  assert(Var16_get_count(sixteen) == 0x3);
  assert(Var16_get_one(sixteen, 0) == 0x0403);
  assert(Var16_get_one(sixteen, 1) == 0x0605);
  assert(Var16_get_one(sixteen, 2) == 0x0807);
}

int main(void) {
 check_fixedunion_eight();
 check_fixedunion_sixteen();

 check_varunion_eight();
 check_varunion_sixteen();
 return 0;
}
