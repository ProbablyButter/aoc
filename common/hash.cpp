#include "hash.hpp"

namespace aoc
{
  constexpr static uint64_t rotl64(uint64_t x, int r)
  {
    return (x << r) | (x >> (64 - r));
  }

  void hasher::init(uint64_t seed)
  {
    h1 = seed;
    h2 = seed;
    nbytes = 0;
  }

  void hasher::add_block(uint64_t k1, uint64_t k2)
  {
    constexpr uint64_t c1 = 0x87c37b91114253d5ull;
    constexpr uint64_t c2 = 0x4cf5ad432745937full;

    k1 *= c1;
    k1 = rotl64(k1, 31);
    k1 *= c2;
    h1 ^= k1;

    h1 = rotl64(h1, 27);
    h1 += h2;
    h1 = h1 * 5 + 0x52dce729ull;

    // TODO: how to reduce this to 64-bit?
    k2 *= c2;
    k2 = rotl64(k2, 33);
    k2 *= c1;
    h2 ^= k2;

    h2 = rotl64(h2, 31);
    h2 += h1;
    h2 = h2 * 5 + 0x38495ab5ull;
    nbytes += 0x10;
  }

  static uint64_t fmix64(uint64_t k)
  {
    // http://zimbry.blogspot.com/2011/09/better-bit-mixing-improving-on.html
    // mix13
    k ^= k >> 30;
    k *= 0xbf58476d1ce4e5b9ull;
    k ^= k >> 27;
    k *= 0x94d049bb133111ebull;
    k ^= k >> 31;
    return k;
  }

  void hasher::finalize()
  {
    h1 ^= nbytes;
    h2 ^= nbytes;

    h1 += h2;
    h2 += h1;

    h1 = fmix64(h1);
    h2 = fmix64(h2);

    h1 += h2;
    h2 += h1;
  }

  void hasher::finalize(uint64_t k1, int num)
  {
    constexpr uint64_t c1 = 0x87c37b91114253d5ull;
    constexpr uint64_t c2 = 0x4cf5ad432745937full;
    nbytes += num;
    k1 *= c1;
    k1 = rotl64(k1, 31);
    k1 *= c2;
    h1 ^= k1;
    finalize();
  }

  void hasher::finalize(uint64_t k1, uint64_t k2, int num)
  {
    constexpr uint64_t c1 = 0x87c37b91114253d5ull;
    constexpr uint64_t c2 = 0x4cf5ad432745937full;
    nbytes += num;
    k2 *= c2;
    k2 = rotl64(k2, 33);
    k2 *= c1;
    h2 ^= k2;

    k1 *= c1;
    k1 = rotl64(k1, 31);
    k1 *= c2;
    h1 ^= k1;
    finalize();
  }
} // namespace aoc
