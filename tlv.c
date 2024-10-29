#include "tlv.h"

#include <stdio.h>

static const uint8_t source[] = {
  2, 0, 0, 0, /* Count */
    2, 0, 0, 0, /* Type  */
      0x12, 0x34, 0, 0,
      0x55, 0x66, 0, 0,
      1, 2, 3, 4, 5, 6, 7, 8,
    1, 0, 0, 0, /* Type  */
      0x44, 0x33, 0x22, 0x11,
      0x88, 0x77, 0x66, 0x55,
      10, 11, 12, 13, 14, 15, 16, 17,
      17, 16, 15, 14, 13, 12, 11, 10,
};



int main(void) {
  struct Payload_bytes bytes = {source};
  size_t count = Payload_get_count(bytes);
  printf("Count: %d\n", (int)count);
  for (int i = 0; i < count; i++) {
    struct TLV_bytes tlv = Payload_get_values(bytes, i);
    printf("%d\n", TLV_size(tlv));
    uint8_t type = TLV_get_type(tlv);
    printf("  TLV: type: %d", type);
    printf("\n");
  }
  return 0;
}

