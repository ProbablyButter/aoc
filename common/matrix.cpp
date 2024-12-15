#include "matrix.hpp"

#include <iostream>

namespace aoc {

long long permute_parity(const std::vector<long long> &P) {
  // https://codeforces.com/blog/entry/97849
  switch (P.size()) {
  case 0:
    return 0;
  case 1:
    return 1;
  default: {
    std::vector<long long> cur(P.size());
    std::vector<long long> inv_cur(P.size());
    for (size_t i = 0; i < cur.size(); ++i) {
      cur[i] = i;
      inv_cur[i] = i;
    }
    auto do_cycle = [&](long long i, long long j, long long k) {
      long long tmp = cur[k];
      cur[k] = cur[j];
      cur[j] = cur[i];
      cur[i] = tmp;

      inv_cur[cur[i]] = i;
      inv_cur[cur[j]] = j;
      inv_cur[cur[k]] = k;
    };
    for (long long i = 0; i < P.size() - 2; ++i) {
      if (cur[i] != P[i]) {
        long long j = inv_cur[P[i]];
        if (j != P.size() - 1) {
          do_cycle(P.size() - 1, j, i);
        } else {
          do_cycle(P.size() - 2, j, i);
        }
      }
    }
    if (cur.back() != P.back()) {
      return -1;
    }
    return 1;
  }
  }
}

#if 0
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

long long imatrix::det() const {
  
}

void imatrix::hermite_normal_form(imatrix &H, imatrix &K) const {
  K.set_eye(height);
  // construct permutation matrix K so every principal minor of A is nonsingular
}
#endif
} // namespace aoc
