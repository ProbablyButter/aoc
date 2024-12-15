#include "fraction.hpp"

#include <numeric>

namespace aoc {

void extended_euclid(long long a, long long b, long long &x, long long &y,
                     long long &qx, long long &qy, long long &gcd) {
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
      qx = tn;
      qy = sn;
      // fix signs
      if (gcd < 0) {
        gcd = -gcd;
        x = -x;
        y = -y;
        qx = -qx;
        qy = -qy;
      }
      if (a < 0 && qx > 0) {
        qx = -qx;
      } else if (a > 0 && qx < 0) {
        qx = -qx;
      }
      if (b < 0 && qy > 0) {
        qy = -qy;
      } else if (b > 0 && qy < 0) {
        qy = -qy;
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

void extended_euclid(long long a, long long b, long long &x, long long &y,
                     long long &gcd) {
  long long qx, qy;
  extended_euclid(a, b, x, y, qx, qy, gcd);
}

void fraction::reduce() {
  if (num == 0) {
    den = 1;
    return;
  }
  // assume den != 0
  long long x, y, gcd;
  extended_euclid(num, den, x, y, num, den, gcd);
  if (den < 0) {
    den = -den;
    num = -num;
  }
}

fraction &fraction::operator+=(const fraction &o) {
  long long lcm = std::lcm(den, o.den);
  num = num * (lcm / den) + o.num * (lcm / o.den);
  den = lcm;
  reduce();
  return *this;
}

fraction &fraction::operator-=(const fraction &o) {
  long long lcm = std::lcm(den, o.den);
  num = num * (lcm / den) - o.num * (lcm / o.den);
  den = lcm;
  reduce();
  return *this;
}

fraction &fraction::operator*=(const fraction &o) {
  // TODO: could pre-compute the GCD to prevent some overflow problems
  num *= o.num;
  den *= o.den;
  reduce();
  return *this;
}

fraction &fraction::operator/=(const fraction &o) {
  // TODO: could pre-compute the GCD to prevent some overflow problems
  // avoid problems with &o == this
  long long tmp = num * o.den;
  den *= o.num;
  num = tmp;
  reduce();
  return *this;
}

fraction &fraction::operator+=(long long o) {
  num = num + o * den;
  reduce();
  return *this;
}

fraction &fraction::operator-=(long long o) {
  num = num - o * den;
  reduce();
  return *this;
}

fraction &fraction::operator*=(long long o) {
  num *= o;
  reduce();
  return *this;
}

fraction &fraction::operator/=(long long o) {
  den *= o;
  reduce();
  return *this;
}
} // namespace aoc
