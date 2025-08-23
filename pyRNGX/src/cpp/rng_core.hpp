#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <cmath>

struct StreamState {
  uint64_t key0;        // derived from seed + stream_id
  uint64_t key1;
  uint64_t counter_hi;  // substream id (unused in MVP)
  uint64_t counter_lo;  // offset
};

inline uint64_t splitmix64(uint64_t x) {
  x += 0x9e3779b97f4a7c15ull;
  x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ull;
  x = (x ^ (x >> 27)) * 0x94d049bb133111ebull;
  return x ^ (x >> 31);
}

// Convert 53 random bits to [0,1) double deterministically
inline double u53_to_unit(uint64_t r53) {
  // take top 53 bits and scale
  const double inv = 1.0 / static_cast<double>(1ull << 53);
  return static_cast<double>(r53) * inv;
}
