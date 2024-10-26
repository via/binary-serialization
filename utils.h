#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdlib.h>

static inline void encode_u32le(uint8_t *bytes, uint32_t value) {
  bytes[0] = value & 0xFF;
  bytes[1] = (value >> 8) & 0xFF;
  bytes[2] = (value >> 16) & 0xFF;
  bytes[3] = (value >> 24) & 0xFF;
}

static inline uint32_t decode_u32le(const uint8_t *bytes) {
  return ((uint32_t)bytes[3] << 24) |
         ((uint32_t)bytes[2] << 16) |
         ((uint32_t)bytes[1] << 8) |
         ((uint32_t)bytes[0]);
}

static inline void encode_u16le(uint8_t *bytes, uint16_t value) {
  bytes[0] = value & 0xFF;
  bytes[1] = (value >> 8) & 0xFF;
}

static inline uint16_t decode_u16le(const uint8_t *bytes) {
  return ((uint32_t)bytes[3] << 24) |
         ((uint32_t)bytes[2] << 16) |
         ((uint32_t)bytes[1] << 8) |
         ((uint32_t)bytes[0]);
}

static inline float decode_f32le(const uint8_t *bytes) {
  uint32_t u32bytes = decode_u32le(bytes);
  float f32bytes;
  memcpy(&f32bytes, &u32bytes, sizeof(f32bytes));
  return f32bytes;
}

/* Returns size rounded up to 32-bit word */
static inline size_t pad(const size_t size) {
  size_t overflow = size & 0x3; /* Number of bytes past 4 byte alignment */
  return (size & ~0x3UL) + (overflow ? 4 : 0); 
}

#endif
