#include <stdio.h>
#include <assert.h>

#include "basic-packing.h"

static void check_allpack(void) {
  uint8_t allpack_bytes[] = {
    0x11, /* one */
    0x22, 0x11, /* two */
    0x66, 0x55, 0x44, 0x33, /* three */
  };
  const struct AllPack_bytes allpack = {allpack_bytes};
  assert(AllPack_size(allpack) == sizeof(allpack_bytes));
  assert(AllPack_get_one(allpack) == 0x11);
  assert(AllPack_get_two(allpack) == 0x1122);
  assert(AllPack_get_three(allpack) == 0x33445566);
};

static void check_packedinside(void) {
  uint8_t packedinside_bytes[] = {
    0x11, /* one */
    0x22, 0x11, /* two */
    0x66, 0x55, 0x44, 0x33, /* three */
    0x99, /* last */
  };

  const struct PackedInside_bytes packedinside = {packedinside_bytes};
  assert(PackedInside_size(packedinside) == sizeof(packedinside_bytes));
  const struct AllPack_bytes packedinside_pack = PackedInside_get_pack(packedinside); 
  assert(AllPack_get_one(packedinside_pack) == 0x11);
  assert(AllPack_get_two(packedinside_pack) == 0x1122);
  assert(AllPack_get_three(packedinside_pack) == 0x33445566);
  assert(PackedInside_get_last(packedinside) == 0x99);
}

static void check_packedarray(void) {
  uint8_t packedarray_bytes[] = {
    0x11, /* one */
    0x22, 0x11, /* two */
    0x66, 0x55, 0x44, 0x33, /* three */
    0x99, /* last */

    0x99, /* one */
    0x88, 0x77, /* two */
    0x55, 0x44, 0x33, 0x22, /* three */
    0x11, /* last */
  };

  const struct PackedArray_bytes packedarray = {packedarray_bytes};
  assert(PackedArray_size(packedarray) == sizeof(packedarray_bytes));

  const struct PackedInside_bytes first = PackedArray_get_packs(packedarray, 0);
  const struct AllPack_bytes first_pack = PackedInside_get_pack(first); 
  assert(AllPack_get_one(first_pack) == 0x11);
  assert(AllPack_get_two(first_pack) == 0x1122);
  assert(AllPack_get_three(first_pack) == 0x33445566);
  assert(PackedInside_get_last(first) == 0x99);

  const struct PackedInside_bytes second = PackedArray_get_packs(packedarray, 1);
  const struct AllPack_bytes second_pack = PackedInside_get_pack(second); 
  assert(AllPack_get_one(second_pack) == 0x99);
  assert(AllPack_get_two(second_pack) == 0x7788);
  assert(AllPack_get_three(second_pack) == 0x22334455);
  assert(PackedInside_get_last(second) == 0x11);
}


static void check_unpackedarray(void) {

  uint8_t unpackedarray_bytes[] = {
    0x11, /* one */
    0x22, 0x11, /* two */
    0x66, 0x55, 0x44, 0x33, /* three */

    0x99, /* one */
    0x88, 0x77, /* two */
    0x55, 0x44, 0x33, 0x22, /* three */
    0x0, 0x0, /* packs is padded */
    0x55, /* last */
  };

  const struct UnpackedArray_bytes unpackedarray = {unpackedarray_bytes};
  assert(UnpackedArray_size(unpackedarray) == sizeof(unpackedarray_bytes));

  const struct AllPack_bytes first_pack = UnpackedArray_get_packs(unpackedarray, 0); 
  assert(AllPack_get_one(first_pack) == 0x11);
  assert(AllPack_get_two(first_pack) == 0x1122);
  assert(AllPack_get_three(first_pack) == 0x33445566);

  const struct AllPack_bytes second = UnpackedArray_get_packs(unpackedarray, 1); 
  assert(AllPack_get_one(second) == 0x99);
  assert(AllPack_get_two(second) == 0x7788);
  assert(AllPack_get_three(second) == 0x22334455);
  assert(UnpackedArray_get_last(unpackedarray) == 0x55);
}


int main(void) {
  check_allpack();
  check_packedinside();
  check_packedarray();
  check_unpackedarray();

  return 0;
}
