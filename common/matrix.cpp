#include "matrix.hpp"

#include <iostream>

namespace aoc {

void extended_euclid(long long a, long long b, long long &x, long long &y,
                     long long &gcd) {
  // assume a > 0 and b > 0
  long long r0 = a;
  long long r1 = b;
  long long s0 = 1;
  long long s1 = 0;
  long long t0 = 0;
  long long t1 = 1;
  while (true) {
    long long q = r0 / r1;
    long long rn = r0 - q * r1;
    long long sn = s0 - q * s1;
    long long tn = t0 - q * t1;
    if (rn == 0) {
      gcd = r1;
      x = s1;
      y = t1;
      if (gcd < 0) {
        gcd = -gcd;
        x = -x;
        y = -y;
      }

      return;
    }
    r0 = r1;
    r1 = rn;
    s0 = s1;
    s1 = sn;
    t0 = t1;
    t1 = tn;
  }
}

imatrix &imatrix::operator+=(const imatrix &o) {
  for (size_t i = 0; i < data.size(); ++i) {
    data[i] += o.data[i];
  }
  return *this;
}

imatrix &imatrix::operator-=(const imatrix &o) {
  for (size_t i = 0; i < data.size(); ++i) {
    data[i] -= o.data[i];
  }
  return *this;
}

imatrix imatrix::operator*(const imatrix &o) const {
  imatrix res(height, o.width);
  for (long long i = 0; i < height; ++i) {
    for (long long j = 0; j < o.width; ++j) {
      for (long long k = 0; k < width; ++k) {
        res(i, j) += operator()(i, k) * o(k, j);
      }
    }
  }
  return res;
}

void imatrix::set_eye(long long d) {
  data.clear();
  data.resize(d * d);
  width = d;
  height = d;
  for (long long i = 0; i < d; ++i) {
    operator()(i, i) = 1;
  }
}
} // namespace aoc
