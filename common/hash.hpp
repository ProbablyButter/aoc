#ifndef AOC_HASH_HPP
#define AOC_HASH_HPP

#include <array>
#include <cstdint>
#include <string>
#include <utility>

namespace aoc {
///
/// @brief streaming implementation for murmurhash3 128 (x64).
/// Constructs the hash in 3 stages: init, append, finalize
///
struct hasher {
  uint64_t data[2] = {0, 0};
  uint64_t nbytes = 0;

  uint64_t buf_[2] = {0, 0};

  void init(uint64_t seed = 0);
  void append(const uint8_t *vs, uint64_t bytes);

  void finalize();

private:
  // add 16 bytes
  void add_block(uint64_t k1, uint64_t k2);

  // add [1-8] more bytes, then finalize
  void finalize(uint64_t k1, int num);

  // add [1-15] more bytes, then finalize
  // 0 < num < 16
  void finalize(uint64_t k1, uint64_t k2, int num);
};

struct md5 {
  static const uint32_t *s();
  static const uint32_t *k();
  uint32_t vals[4];
  uint64_t nbytes = 0;

  uint32_t buf_[16];

  void init();

  // append a given number of bytes
  void append(const uint8_t *vs, uint64_t bytes);
  void finalize();

  std::string str() const;

private:
  void transform();
};

struct pair_hasher {
  template <class T, class V>
  size_t operator()(const std::pair<T, V> &v) const noexcept {
    hasher hash;
    hash.init(123456789ull);
    hash.append(reinterpret_cast<const uint8_t *>(&v.first), sizeof(T));
    hash.append(reinterpret_cast<const uint8_t *>(&v.second), sizeof(V));
    hash.finalize();
    return hash.data[1];
  };
};

struct array_hasher {
  template <class T, size_t N>
  size_t operator()(const std::array<T, N> &v) const noexcept {
    hasher hash;
    hash.init(123456789ull);
    for (size_t i = 0; i < N; ++i) {
      hash.append(reinterpret_cast<const uint8_t *>(&v[i]), sizeof(T));
    }
    hash.finalize();
    return hash.data[1];
  };
};
} // namespace aoc

#endif
