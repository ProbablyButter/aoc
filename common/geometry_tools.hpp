#ifndef AOC_GEOMETRY_TOOLS_HPP
#define AOC_GEOMETRY_TOOLS_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace aoc {

template <class T, size_t Dims> struct hslab {
  std::array<T, Dims> lower = {0};
  std::array<T, Dims> upper = {0};

  hslab intersect(const hslab &other) const {
    hslab res = *this;
    for (size_t i = 0; i < Dims; ++i) {
      res.lower[i] = std::max(res.lower[i], other.lower[i]);
      res.upper[i] = std::min(res.upper[i], other.upper[i]);
      // cap
      res.upper[i] = std::max(res.lower[i], res.upper[i]);
    }
    return res;
  }
  T volume() const {
    T res = 1;
    for (size_t i = 0; i < Dims; ++i) {
      res *= upper[i] - lower[i];
    }
    return res;
  }
};

struct hslab_iterator {
  std::vector<int64_t> idx;
  std::vector<int64_t> lower;
  std::vector<int64_t> upper;

  bool advance() {
    size_t i = 0;
    while (true) {
      ++idx[i];
      if (idx[i] >= upper[i]) {
        idx[i] = lower[i];
      } else {
        return true;
      }
      ++i;
      if (i >= idx.size()) {
        return false;
      }
    }
    return true;
  }
};

void cramer2d(double a00, double a01, double a10, double a11, double b0,
              double b1, double &x0, double &x1);

bool ray_intersection(double x0, double y0, double vx0, double vy0, double x1,
                      double y1, double vx1, double vy1, double &s, double &t);

// 2D
bool line_segment_intersection(double x0, double y0, double x1, double y1,
                               double x2, double y2, double x3, double y3,
                               double &s, double &t);
} // namespace aoc

#endif
