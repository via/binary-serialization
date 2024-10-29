#include <stdio.h>
#include <assert.h>

#include "basic-builtins.h"

/* All fields are 32 bit aligned by default */
uint8_t bytes[] = {
  0x1, 0x2, 0x3, 0x4, /* one */
  0x5, 0x6, 0x7, 0x8, /* two */
  0x1, 0x2, 0x0, 0x0, /* three */
  0x3, 0x4, 0x0, 0x0, /* four */
  0xa, 0x0, 0x0, 0x0, /* five */
  0xb, 0x0, 0x0, 0x0, /* six */
  0x1, 0x0, 0x0, 0x0, /* seven */
  0xc, 0xd, 0xe, 0x8f, /* eight */
  0x2, 0x4, 0x8, 0xa, 0x1, 0x3, 0x5, 0x7, /* nine */
  0x11, /* ten */
};

int main(void) {

  const struct TestTypes_bytes testtypes = {bytes};
  assert(TestTypes_size(testtypes) == sizeof(bytes));
  assert(TestTypes_get_one(testtypes) == 0x04030201);
  assert(TestTypes_get_two(testtypes) == 0x08070605);
  assert(TestTypes_get_three(testtypes) == 0x0201);
  assert(TestTypes_get_four(testtypes) == 0x0403);
  assert(TestTypes_get_five(testtypes) == 0xa);
  assert(TestTypes_get_six(testtypes) == 0xb);
  assert(TestTypes_get_seven(testtypes) == true);
  assert(TestTypes_get_eight(testtypes) == -1894904564);
  assert(TestTypes_get_nine(testtypes) == 0x070503010a080402);
  assert(TestTypes_get_ten(testtypes) == 0x11);
  return 0;
}
