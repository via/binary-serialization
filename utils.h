#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
static inline uint32_t decode_u32(const uint8_t *bytes) {
  return decode_u32le(bytes);
}

static inline void encode_u16le(uint8_t *bytes, uint16_t value) {
  bytes[0] = value & 0xFF;
  bytes[1] = (value >> 8) & 0xFF;
}

static inline uint16_t decode_u16le(const uint8_t *bytes) {
  return ((uint32_t)bytes[1] << 8) |
         ((uint32_t)bytes[0]);
}
static inline uint16_t decode_u16(const uint8_t *bytes) {
  return decode_u16le(bytes);
}
static inline uint16_t decode_i16(const uint8_t *bytes) {
  return ((uint32_t)bytes[1] << 8) |
         ((uint32_t)bytes[0]);
}

static inline uint8_t decode_u8(const uint8_t *bytes) {
  return bytes[0];
}

static inline float decode_f32le(const uint8_t *bytes) {
  uint32_t u32bytes = decode_u32le(bytes);
  float f32bytes;
  memcpy(&f32bytes, &u32bytes, sizeof(f32bytes));
  return f32bytes;
}

/* Returns size rounded up to 32-bit word */
static inline size_t align(const size_t size) {
  return (size + 3) & ~((size_t)0x3);
}

#endif
