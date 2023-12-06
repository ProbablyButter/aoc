#ifndef AOC_HASH_HPP
#define AOC_HASH_HPP

#include <cstdint>

namespace aoc {
///
/// @brief streaming implementation for murmurhash3 128 (x64).
/// Constructs the hash in 3 stages: init, add_block, finalize
///
struct hasher {
  uint64_t h1 = 0, h2 = 0;
  uint64_t nbytes = 0;

  void init(uint64_t seed = 0);

  // add 16 bytes
  void add_block(uint64_t k1, uint64_t k2);

  // add no more bytes, just finalize
  void finalize();

  // add [1-8] more bytes, then finalize
  void finalize(uint64_t k1, int num);

  // add [1-15] more bytes, then finalize
  // 0 < num < 16
  void finalize(uint64_t k1, uint64_t k2, int num);
};
} // namespace aoc

#endif
