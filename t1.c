#include <stdint.h>
#include <stdlib.h>

uint8_t decode_u8(const uint8_t u[]) {
  return u[0];
}

uint32_t decode_u16(const uint8_t u[]) {
  return ((uint32_t)u[0])       |
         ((uint32_t)u[1] << 8);
}

uint32_t decode_u32(const uint8_t u[]) {
  return ((uint32_t)u[0])       |
         ((uint32_t)u[1] << 8)  |
         ((uint32_t)u[2] << 16) |
         ((uint32_t)u[3] << 24);
}

struct VarSizedMessage_bytes {
  uint8_t *bytes;
};

uint8_t VarSizedMessage_get_count(const struct VarSizedMessage_bytes b) {
  const size_t offset = 0;
  return decode_u8(&b.bytes[offset]);
}

uint32_t VarSizedMessage_get_another(const struct VarSizedMessage_bytes b) {
  const size_t offset = 4;
  return decode_u32(&b.bytes[offset]);
}

uint16_t VarSizedMessage_get_numbers(const struct VarSizedMessage_bytes b, size_t idx) {
  const size_t offset = 4 
                      + 4
                      + (idx * 2);
  return decode_u16(&b.bytes[offset]);
}


uint8_t VarSizedMessage_get_final(const struct VarSizedMessage_bytes b) {
  const size_t offset = 4 /* count */
                      + 4 /* another */
                      + (VarSizedMessage_get_count(b) * 2);
  return decode_u8(&b.bytes[offset]);
}

size_t VarSizedMessage_size(const struct VarSizedMessage_bytes b) {
  const size_t size = 4 /* count */
                    + 4 /* another */
                    + (VarSizedMessage_get_count(b) * 2) /* numbers */
                    + 4 /* final */;
  return size;
}

int do_thing(const struct VarSizedMessage_bytes _b) {
  const struct VarSizedMessage_bytes b = {
      .bytes = (uint8_t*)__builtin_assume_aligned(_b.bytes, 4)
    };
  for (unsigned int i = 0; i < VarSizedMessage_get_count(b); i++) {
    if (VarSizedMessage_get_numbers(b, i) == 0) {
      return 1;
    }
  }
  return 0;
}

struct alt {
  uint32_t count;
  uint32_t another;
  int16_t numbers[16];
};

int do_comparison(struct alt *alt) {
  for (unsigned int i = 0; i < alt->count; i++) {
    if (alt->numbers[i] == 0) {
      return 1;
    }
  }
  return 0;

}

struct AdcUpdate_bytes {
  uint8_t *bytes;
};

enum ConsoleMessageType {
  AdcUpdate = 1,
  TriggerUpdate = 2,
  CalculationsUpdate = 3,
};

struct ConsoleMessage_bytes {
  uint8_t *bytes;
};

enum ConsoleMessageType ConsoleMessage_get_type(struct ConsoleMessage_bytes m) {
  uint8_t _type = decode_u8(&m.bytes[0]);
  return (enum ConsoleMessageType)_type;
}

struct AdcUpdate_bytes ConsoleMessage_get_adc(struct ConsoleMessage_bytes m) {
  return (struct AdcUpdate_bytes){.bytes = &m.bytes[4]};
}

