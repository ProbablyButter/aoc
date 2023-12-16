#include "hash.hpp"

#include "endian.hpp"

#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <sstream>

namespace aoc {
constexpr static uint64_t rotl64(uint64_t x, int r) {
  return (x << r) | (x >> (64 - r));
}

constexpr static uint64_t rotl32(uint32_t x, int r) {
  return (x << r) | (x >> (32 - r));
}

void hasher::init(uint64_t seed) {
  data[0] = seed;
  data[1] = seed;
  nbytes = 0;
}

void hasher::add_block(uint64_t k1, uint64_t k2) {
  constexpr uint64_t c1 = 0x87c37b91114253d5ull;
  constexpr uint64_t c2 = 0x4cf5ad432745937full;

  k1 *= c1;
  k1 = rotl64(k1, 31);
  k1 *= c2;
  data[0] ^= k1;

  data[0] = rotl64(data[0], 27);
  data[0] += data[1];
  data[0] = data[0] * 5 + 0x52dce729ull;

  // TODO: how to reduce this to 64-bit?
  k2 *= c2;
  k2 = rotl64(k2, 33);
  k2 *= c1;
  data[1] ^= k2;

  data[1] = rotl64(data[1], 31);
  data[1] += data[0];
  data[1] = data[1] * 5 + 0x38495ab5ull;
}

static uint64_t fmix64(uint64_t k) {
  // http://zimbry.blogspot.com/2011/09/better-bit-mixing-improving-on.html
  // mix13
  k ^= k >> 30;
  k *= 0xbf58476d1ce4e5b9ull;
  k ^= k >> 27;
  k *= 0x94d049bb133111ebull;
  k ^= k >> 31;
  return k;
}

void hasher::append(const uint8_t *vs, uint64_t bytes) {
  if (bytes == 0) {
    return;
  }
  auto rem = nbytes % 16;
  nbytes += bytes;
  uint8_t *tmp = reinterpret_cast<uint8_t *>(buf_);
  while (true) {
    if (bytes + rem >= 16) {
      std::copy(vs, vs + 16 - rem, tmp + rem);
      add_block(buf_[0], buf_[1]);
      vs += (16 - rem);
      bytes -= (16 - rem);
      rem = 0;
    } else {
      std::copy(vs, vs + bytes, tmp + rem);
      return;
    }
  }
}

void hasher::finalize() {
  auto rem = nbytes % 16;
  if (rem > 0) {
    nbytes -= rem;
    if (rem <= 8) {
      finalize(buf_[0], rem);
    } else {
      finalize(buf_[0], buf_[1], rem);
    }
    return;
  }
  data[0] ^= nbytes;
  data[1] ^= nbytes;

  data[0] += data[1];
  data[1] += data[0];

  data[0] = fmix64(data[0]);
  data[1] = fmix64(data[1]);

  data[0] += data[1];
  data[1] += data[0];
}

void hasher::finalize(uint64_t k1, int num) {
  constexpr uint64_t c1 = 0x87c37b91114253d5ull;
  constexpr uint64_t c2 = 0x4cf5ad432745937full;
  nbytes += num;
  k1 *= c1;
  k1 = rotl64(k1, 31);
  k1 *= c2;
  data[0] ^= k1;
  finalize();
}

void hasher::finalize(uint64_t k1, uint64_t k2, int num) {
  constexpr uint64_t c1 = 0x87c37b91114253d5ull;
  constexpr uint64_t c2 = 0x4cf5ad432745937full;
  nbytes += num;
  k2 *= c2;
  k2 = rotl64(k2, 33);
  k2 *= c1;
  data[1] ^= k2;

  k1 *= c1;
  k1 = rotl64(k1, 31);
  k1 *= c2;
  data[0] ^= k1;
  finalize();
}

const uint32_t *md5::s() {
  static uint32_t vals_[] = {
      7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,

      5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20,

      4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,

      6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};
  return vals_;
}
const uint32_t *md5::k() {
  static uint32_t vals_[] = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
                             0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,

                             0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
                             0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,

                             0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
                             0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,

                             0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
                             0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,

                             0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
                             0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,

                             0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
                             0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,

                             0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
                             0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,

                             0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
                             0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};
  return vals_;
}

void md5::init() {
  vals[0] = 0x67452301;
  vals[1] = 0xefcdab89;
  vals[2] = 0x98badcfe;
  vals[3] = 0x10325476;
  nbytes = 0;
}

void md5::append(const uint8_t *vs, uint64_t bytes) {
  if (bytes == 0) {
    return;
  }
  auto rem = nbytes % 64;
  nbytes += bytes;
  uint8_t *tmp = reinterpret_cast<uint8_t *>(buf_);
  while (true) {
    if (bytes + rem >= 64) {
      std::copy(vs, vs + 64 - rem, tmp + rem);
      transform();
      vs += (64 - rem);
      bytes -= (64 - rem);
      rem = 0;
    } else {
      std::copy(vs, vs + bytes, tmp + rem);
      return;
    }
  }
}

void md5::transform() {
  uint32_t pvals[4] = {vals[0], vals[1], vals[2], vals[3]};
  for (int64_t i = 0; i < 16; ++i) {
    uint32_t F = pvals[1] & pvals[2] | ((~pvals[1]) & pvals[3]);
    auto g = i;

    F = F + pvals[0] + k()[i] + aoc::native_to_little<uint32_t>(buf_[g]);
    pvals[0] = pvals[3];
    pvals[3] = pvals[2];
    pvals[2] = pvals[1];
    pvals[1] = pvals[1] + rotl32(F, s()[i]);
  }

  for (int64_t i = 16; i < 32; ++i) {
    uint32_t F = pvals[3] & pvals[1] | ((~pvals[3]) & pvals[2]);
    auto g = (5 * i + 1) % 16;

    F = F + pvals[0] + k()[i] + aoc::native_to_little<uint32_t>(buf_[g]);
    pvals[0] = pvals[3];
    pvals[3] = pvals[2];
    pvals[2] = pvals[1];
    pvals[1] = pvals[1] + rotl32(F, s()[i]);
  }

  for (int64_t i = 32; i < 48; ++i) {
    uint32_t F = pvals[1] ^ pvals[2] ^ pvals[3];
    auto g = (3 * i + 5) % 16;

    F = F + pvals[0] + k()[i] + aoc::native_to_little<uint32_t>(buf_[g]);
    pvals[0] = pvals[3];
    pvals[3] = pvals[2];
    pvals[2] = pvals[1];
    pvals[1] = pvals[1] + rotl32(F, s()[i]);
  }

  for (int64_t i = 48; i < 64; ++i) {
    uint32_t F = pvals[2] ^ (pvals[1] | (~pvals[3]));
    auto g = (7 * i) % 16;

    F = F + pvals[0] + k()[i] + aoc::native_to_little<uint32_t>(buf_[g]);
    pvals[0] = pvals[3];
    pvals[3] = pvals[2];
    pvals[2] = pvals[1];
    pvals[1] = pvals[1] + rotl32(F, s()[i]);
  }

  for (size_t i = 0; i < 4; ++i) {
    vals[i] += pvals[i];
  }
}

void md5::finalize() {
  constexpr static uint8_t padding[64] = {
      0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  auto nbits = aoc::native_to_little<uint64_t>(nbytes * 8);
  uint64_t rem = nbytes % 64;
  if (rem + 8 > 64) {
    // finish this block
    append(padding, 64 - rem);
    append(padding + 1, 64 - 8);
  } else {
    append(padding, 64 - 8 - rem);
  }
  append(reinterpret_cast<uint8_t *>(&nbits), 8);
}

std::string md5::str() const {
  uint32_t pvals[] = {aoc::native_to_little<uint32_t>(vals[0]),
                      aoc::native_to_little<uint32_t>(vals[1]),
                      aoc::native_to_little<uint32_t>(vals[2]),
                      aoc::native_to_little<uint32_t>(vals[3])};
  uint8_t *tmp = reinterpret_cast<uint8_t *>(pvals);
  std::stringstream out;
  
  for (size_t i = 0; i < 16; ++i) {
    out << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)tmp[i];
  }
  return out.str();
}
} // namespace aoc
