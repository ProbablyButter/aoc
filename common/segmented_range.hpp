#ifndef AOC_SEGMENTED_RANGE_HPP
#define AOC_SEGMENTED_RANGE_HPP

#include <cstdint>
#include <cstdlib>
#include <utility>
#include <vector>

namespace aoc {
///
/// @brief Represents a 1D interval of segments
///
struct segmented_range {
  /// when normalized, guarantees:
  /// no overlapping segments
  /// segments sorted by start, then stop
  /// segments are [inclusive, exclusive)
  std::vector<std::pair<int64_t, int64_t>> segments;

  /// note: output is not normalized
  void add(int64_t lower, int64_t upper);
  void add(const std::pair<int64_t, int64_t> &segment);

  /// must normalize *this before calling this function
  segmented_range subtract(const std::pair<int64_t, int64_t> &segment) const;

  /// note: output is only normalized if *this was initially normalized
  segmented_range intersect(const std::pair<int64_t, int64_t> &segment) const;

  /// must normalize *this and other before calling this function
  /// output is normalized
  segmented_range add(const segmented_range &other) const;

  /// must normalize *this and other before calling this function
  /// output is normalized
  segmented_range subtract(const segmented_range &other) const;

  /// must normalize *this and other before calling this function
  /// output is normalized
  segmented_range intersect(const segmented_range &other) const;

  std::vector<size_t> get_sort_permute() const;

  void normalize();

  /// must normalie *this before calling this function
  void merge_adjacent();
};
} // namespace aoc

#endif
