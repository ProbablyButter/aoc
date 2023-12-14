#ifndef AOC_ENDIAN_HPP
#define AOC_ENDIAN_HPP

#include <cstdint>

// TODO: detect actual endian-ness
// for now assumes little endian

namespace aoc {
inline int8_t endian_reverse(int8_t x) noexcept;
inline int16_t endian_reverse(int16_t x) noexcept;
inline int32_t endian_reverse(int32_t x) noexcept;
inline int64_t endian_reverse(int64_t x) noexcept;
inline uint8_t endian_reverse(uint8_t x) noexcept;
inline uint16_t endian_reverse(uint16_t x) noexcept;
inline uint32_t endian_reverse(uint32_t x) noexcept;
inline uint64_t endian_reverse(uint64_t x) noexcept;

template <class T> T big_to_native(T x) noexcept;

template <class T> T little_to_native(T x) noexcept;

template <class T> T native_to_big(T x) noexcept;

template <class T> T native_to_little(T x) noexcept;

inline float endian_reverse(float x) noexcept;
inline double endian_reverse(double x) noexcept;

#include "detail/endian.tcc"
} // namespace aoc

#endif
