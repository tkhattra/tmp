// sequence number arithmetic
// https://en.wikipedia.org/wiki/Serial_number_arithmetic

#include <stdint.h>

#define SEQMAX ((uint64_t)~0)

static inline int64_t   seq_diff(uint64_t x, uint64_t y, uint8_t seqbits)   { uint8_t seqshift = 64-seqbits; return (int64_t)((x<<seqshift) - (y<<seqshift)) >> seqshift; }
static inline uint64_t  seq_plus(uint64_t x, uint64_t y, uint8_t seqbits)   { return (x+y) & (SEQMAX >> (64-seqbits)); }
static inline int       seq_gt(uint64_t x, uint64_t y, uint8_t seqbits)     { return seq_diff(x, y, seqbits) > 0; }
static inline int       seq_ge(uint64_t x, uint64_t y, uint8_t seqbits)     { return seq_diff(x, y, seqbits) >= 0; }
static inline int       seq_lt(uint64_t x, uint64_t y, uint8_t seqbits)     { return seq_diff(x, y, seqbits) < 0; }
static inline int       seq_le(uint64_t x, uint64_t y, uint8_t seqbits)     { return seq_diff(x, y, seqbits) <= 0; }
