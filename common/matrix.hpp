#ifndef AOC_MATRIX_HPP
#define AOC_MATRIX_HPP

#include <vector>

namespace aoc {

/// finds x, y, gcd(a, b) such that
/// a x + b y = gcd(a, b)
void extended_euclid(long long a, long long b, long long &x, long long &y,
                    long long &gcd);

/// dense integer matrix
struct imatrix {
  std::vector<long long> data;
  long long width;
  long long height;

  imatrix() : width(0), height(0) {}
  imatrix(long long h_, long long w_) : data(w_ * h_), width(w_), height(h_) {}

  imatrix(const imatrix &) = default;

  imatrix(imatrix &&o) noexcept
      : data(std::move(o.data)), width(o.width), height(o.height) {}

  imatrix &operator=(const imatrix &) = default;

  imatrix &operator=(imatrix &&o) noexcept {
    if (&o != this) {
      data = std::move(o.data);
      width = o.width;
      height = o.height;
    }
    return *this;
  }

  long long &operator()(long long row, long long col) {
    return data[row * width + col];
  }

  const long long &operator()(long long row, long long col) const {
    return data[row * width + col];
  }

  imatrix &operator+=(const imatrix &o);
  imatrix &operator-=(const imatrix &o);
  imatrix operator*(const imatrix &o) const;

  void set_eye(long long d);
};
} // namespace aoc

#endif
