#include <stdio.h>
#include <assert.h>

#include "basic-arrays.h"

/* All fields are 32 bit aligned by default */
uint8_t fixed_bytes[] = {
  0x1, 0x2, 0x3, 0x0, /* one */
  0x1, 0x2, 0x3, 0x4, 0x5, 0x6, /* two */
};

uint8_t variable_bytes[] = {
  0x5, 0x0, 0x0, 0x0, /* count */
  0x10, 0x11, 0x12, 0x13, 0x14, /* first 5 elements of one */
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 /* fill out one */
};

uint8_t variable_packed_bytes[] = {
  0x5, 0x0, 0x0, 0x0, /* count */
  0x10, 0x11, 0x12, 0x13, 0x14, /* first 5 elements of one */
};

int main(void) {

  const struct TestFixed_bytes testfixed = {fixed_bytes};
  assert(TestFixed_size(testfixed) == sizeof(fixed_bytes));
  assert(TestFixed_get_one(testfixed, 0) == 1);
  assert(TestFixed_get_one(testfixed, 1) == 2);
  assert(TestFixed_get_one(testfixed, 2) == 3);
  assert(TestFixed_get_two(testfixed, 0) == 0x0201);
  assert(TestFixed_get_two(testfixed, 1) == 0x0403);
  assert(TestFixed_get_two(testfixed, 2) == 0x0605);

  const struct TestVariable_bytes testvariable = {variable_bytes};
  assert(TestVariable_size(testvariable) == sizeof(variable_bytes));
  assert(TestVariable_get_one(testvariable, 0) == 0x10);
  assert(TestVariable_get_one(testvariable, 1) == 0x11);
  assert(TestVariable_get_one(testvariable, 2) == 0x12);
  assert(TestVariable_get_one(testvariable, 3) == 0x13);
  assert(TestVariable_get_one(testvariable, 4) == 0x14);

  const struct TestVariablePacked_bytes testvariablepacked = {variable_packed_bytes};
  assert(TestVariablePacked_size(testvariablepacked) == sizeof(variable_packed_bytes));
  assert(TestVariablePacked_get_one(testvariablepacked, 0) == 0x10);
  assert(TestVariablePacked_get_one(testvariablepacked, 1) == 0x11);
  assert(TestVariablePacked_get_one(testvariablepacked, 2) == 0x12);
  assert(TestVariablePacked_get_one(testvariablepacked, 3) == 0x13);
  assert(TestVariablePacked_get_one(testvariablepacked, 4) == 0x14);
  return 0;
}
