#include <stdio.h>
#include <assert.h>

#include "basic-packing.h"

uint8_t allpack_bytes[] = {
  0x11, /* one */
  0x22, 0x11, /* two */
  0x66, 0x55, 0x44, 0x33, /* three */
};

uint8_t packedinside_bytes[] = {
  0x11, /* one */
  0x22, 0x11, /* two */
  0x66, 0x55, 0x44, 0x33, /* three */
  0x99, /* last */
};

int main(void) {
  const struct AllPack_bytes allpack = {allpack_bytes};
  assert(AllPack_size(allpack) == sizeof(allpack_bytes));
  assert(AllPack_get_one(allpack) == 0x11);
  assert(AllPack_get_two(allpack) == 0x1122);
  assert(AllPack_get_three(allpack) == 0x33445566);

  const struct PackedInside_bytes packedinside = {packedinside_bytes};
  assert(PackedInside_size(packedinside) == sizeof(packedinside_bytes));
  const struct AllPack_bytes packedinside_pack = PackedInside_get_pack(packedinside); 
  assert(AllPack_get_one(packedinside_pack) == 0x11);
  assert(AllPack_get_two(packedinside_pack) == 0x1122);
  assert(AllPack_get_three(packedinside_pack) == 0x33445566);
  assert(PackedInside_get_last(packedinside) == 0x99);
  return 0;
}
