#ifndef AOC_ENDIAN_TCC
#define AOC_ENDIAN_TCC

int8_t endian_reverse(int8_t x) noexcept { return x; }

int16_t endian_reverse(int16_t x) noexcept {
  return endian_reverse(static_cast<uint16_t>(x));
}

int32_t endian_reverse(int32_t x) noexcept {
  return endian_reverse(static_cast<uint32_t>(x));
}

int64_t endian_reverse(int64_t x) noexcept {
  return endian_reverse(static_cast<uint64_t>(x));
}

uint8_t endian_reverse(uint8_t x) noexcept { return x; }

uint16_t endian_reverse(uint16_t x) noexcept {
  return ((x & 0xffu) << 8) | (x >> 8);
}

uint32_t endian_reverse(uint32_t x) noexcept {
  return ((x & 0xffu) << 24) | ((x & 0xff00u) << 8) | ((x & 0xff0000u) >> 8) |
         (x >> 24);
}

uint64_t endian_reverse(uint64_t x) noexcept {
  return ((x & 0xffull) << 56) | ((x & 0xff00ull) << 40) |
         ((x & 0xff0000ull) << 24) | ((x & 0xff000000ull) << 8) |
         ((x & 0xff00000000ull) >> 8) | ((x & 0x0000ff0000000000ull) >> 24) |
         ((x & 0x00ff000000000000ull) >> 40) | (x >> 56);
}

template <class T> T big_to_native(T x) noexcept {
#if AOC_BIG_ENDIAN
  return x;
#else
  return endian_reverse(x);
#endif
}

template <class T> T little_to_native(T x) noexcept {
#if AOC_BIG_ENDIAN
  return endian_reverse(x);
#else
  return x;
#endif
}

template <class T> T native_to_big(T x) noexcept {
#if AOC_BIG_ENDIAN
  return x;
#else
  return endian_reverse(x);
#endif
}

template <class T> T native_to_little(T x) noexcept {
#if AOC_BIG_ENDIAN
  return endian_reverse(x);
#else
  return x;
#endif
}

float endian_reverse(float x) noexcept {
  uint32_t v = endian_reverse(*reinterpret_cast<uint32_t *>(&x));
  return *reinterpret_cast<float *>(&v);
}

double endian_reverse(double x) noexcept {
  uint64_t v = endian_reverse(*reinterpret_cast<uint64_t *>(&x));
  return *reinterpret_cast<double *>(&v);
}

#endif
