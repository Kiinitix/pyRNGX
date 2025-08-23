#include "rng_core.hpp"
#include "philox.hpp"
#include <stdexcept>

extern "C" {

// initialize state from seed and stream_id
StreamState* prx_create(uint64_t seed, uint64_t stream_id) {
  auto* st = new StreamState();
  uint64_t h = splitmix64(seed) ^ (stream_id * 0x9E3779B97F4A7C15ull);
  st->key0 = splitmix64(h);
  st->key1 = splitmix64(h ^ 0xD1B54A32D192ED03ull);
  st->counter_hi = 0ull;
  st->counter_lo = 0ull;
  return st;
}

void prx_free(StreamState* st) {
  delete st;
}

void prx_jump_ahead(StreamState* st, uint64_t n) {
  st->counter_lo += n;
  // (let wrap to counter_hi if needed in future)
}

void prx_state(const StreamState* st, uint64_t out_state[4]) {
  out_state[0]=st->key0; out_state[1]=st->key1;
  out_state[2]=st->counter_hi; out_state[3]=st->counter_lo;
}

StreamState* prx_from_state(uint64_t s0, uint64_t s1, uint64_t c_hi, uint64_t c_lo) {
  auto* st = new StreamState();
  st->key0=s0; st->key1=s1; st->counter_hi=c_hi; st->counter_lo=c_lo;
  return st;
}

// Fill buffer with N doubles uniform in [0,1)
void prx_uniform_double(StreamState* st, double* out, size_t n) {
  size_t i = 0;
  uint32_t buf[4];
  while (i < n) {
    Philox4x32::generate(st->counter_hi, st->counter_lo, st->key0, st->key1, buf);
    st->counter_lo++;
    // combine two 32-bit lanes -> 53-bit
    uint64_t r0 = ((uint64_t)(buf[0] >> 5) << 26) | (uint64_t)(buf[1] >> 6);
    uint64_t r1 = ((uint64_t)(buf[2] >> 5) << 26) | (uint64_t)(buf[3] >> 6);
    if (i < n) out[i++] = u53_to_unit(r0);
    if (i < n) out[i++] = u53_to_unit(r1);
  }
}

// Box–Muller transform using two uniforms (deterministic)
void prx_normal_double(StreamState* st, double* out, size_t n) {
  size_t i = 0;
  while (i < n) {
    double u1, u2;
    // use uniform generator
    prx_uniform_double(st, &u1, 1);
    prx_uniform_double(st, &u2, 1);
    if (u1 <= 0.0) u1 = std::ldexp(1.0, -53); // avoid log(0)
    double r = std::sqrt(-2.0 * std::log(u1));
    double theta = 2.0 * 3.14159265358979323846 * u2;
    double z0 = r * std::cos(theta);
    double z1 = r * std::sin(theta);
    out[i++] = z0;
    if (i < n) out[i++] = z1;
  }
}

} // extern "C"
