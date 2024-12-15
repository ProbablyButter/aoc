#ifndef AOC_FRACTION_HPP
#define AOC_FRACTION_HPP

namespace aoc {
/// finds x, y, gcd(a, b) such that
/// a x + b y = gcd(a, b)
void extended_euclid(long long a, long long b, long long &x, long long &y,
                     long long &gcd);

void extended_euclid(long long a, long long b, long long &x, long long &y,
                     long long &qx, long long &qy, long long &gcd);

struct fraction {
  long long num = 0, den = 1;

  fraction &operator+=(const fraction &o);
  fraction &operator-=(const fraction &o);

  fraction &operator*=(const fraction &o);
  fraction &operator/=(const fraction &o);

  fraction &operator+=(long long o);
  fraction &operator-=(long long o);

  fraction &operator*=(long long o);
  fraction &operator/=(long long o);

  fraction operator+(const fraction &o) const {
    fraction res(*this);
    res += o;
    return res;
  }

  fraction operator-(const fraction &o) const {
    fraction res(*this);
    res -= o;
    return res;
  }

  fraction operator*(const fraction &o) const {
    fraction res(*this);
    res *= o;
    return res;
  }

  fraction operator/(const fraction &o) const {
    fraction res(*this);
    res /= o;
    return res;
  }

  fraction operator+(long long o) const {
    fraction res(*this);
    res += o;
    return res;
  }

  fraction operator-(long long o) const {
    fraction res(*this);
    res -= o;
    return res;
  }

  fraction operator*(long long o) const {
    fraction res(*this);
    res *= o;
    return res;
  }

  fraction operator/(long long o) const {
    fraction res(*this);
    res /= o;
    return res;
  }

  fraction operator-() const {
    fraction res{-num, den};
    return res;
  }

  void reduce();
};

inline fraction operator+(long long i, const fraction &o) { return o + i; }
inline fraction operator-(long long i, const fraction &o) {
  fraction res{i, 1};
  res -= o;
  return res;
}
inline fraction operator*(long long i, const fraction &o) { return o * i; }
inline fraction operator/(long long i, const fraction &o) {
  fraction res{i, 1};
  res /= o;
  return res;
}
} // namespace aoc
#endif
