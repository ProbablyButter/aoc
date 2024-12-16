#ifndef AOC_FRACTION_HPP
#define AOC_FRACTION_HPP

#include <iosfwd>

namespace aoc {
/// finds x, y, gcd(a, b) such that
/// a x + b y = gcd(a, b)
void extended_euclid(long long a, long long b, long long &x, long long &y,
                     long long &gcd);

void extended_euclid(long long a, long long b, long long &x, long long &y,
                     long long &qx, long long &qy, long long &gcd);

struct fraction {
  long long num = 0, den = 1;

  void reduce();
  
  fraction() noexcept = default;
  fraction(const fraction &) noexcept = default;
  fraction(long long n, long long d) : num(n), den(d) { reduce(); }

  fraction& operator=(const fraction&) noexcept = default;

  fraction& operator=(long long v) noexcept{
    num = v;
    den = 1;
    return *this;
  }

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

  int operator<=>(const fraction &o) const;

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
};

extern fraction abs(const fraction &f);

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

extern std::ostream &operator<<(std::ostream &out, const fraction &f);

} // namespace aoc
#endif
