#pragma once
#include <cstdint>

// Minimal Philox-4x32-10 (counter-based). Returns 4x32-bit per invocation.
struct Philox4x32 {
  static inline void single_round(uint32_t& x0, uint32_t& x1, uint32_t& x2, uint32_t& x3,
                                  uint32_t k0, uint32_t k1) {
    const uint64_t M0 = 0xD2511F53ull;
    const uint64_t M1 = 0xCD9E8D57ull;
    uint64_t p0 = M0 * (uint64_t)x0;
    uint64_t p1 = M1 * (uint64_t)x2;
    uint32_t hi0 = (uint32_t)(p0 >> 32), lo0 = (uint32_t)p0;
    uint32_t hi1 = (uint32_t)(p1 >> 32), lo1 = (uint32_t)p1;
    uint32_t y0 = hi1 ^ x1 ^ k0;
    uint32_t y1 = lo1;
    uint32_t y2 = hi0 ^ x3 ^ k1;
    uint32_t y3 = lo0;
    x0=y0; x1=y1; x2=y2; x3=y3;
  }

  static inline void bumpkey(uint32_t& k0, uint32_t& k1) {
    k0 += 0x9E3779B9u; // W0
    k1 += 0xBB67AE85u; // W1
  }

  static inline void generate(uint64_t ctr_hi, uint64_t ctr_lo, uint64_t key0, uint64_t key1,
                              uint32_t out[4]) {
    uint32_t x0 = (uint32_t)ctr_lo;
    uint32_t x1 = (uint32_t)(ctr_lo >> 32);
    uint32_t x2 = (uint32_t)ctr_hi;
    uint32_t x3 = (uint32_t)(ctr_hi >> 32);
    uint32_t k0 = (uint32_t)key0;
    uint32_t k1 = (uint32_t)key1;
    // 10 rounds
    for (int i=0;i<10;i++) {
      single_round(x0,x1,x2,x3,k0,k1);
      bumpkey(k0,k1);
    }
    out[0]=x0; out[1]=x1; out[2]=x2; out[3]=x3;
  }
};
