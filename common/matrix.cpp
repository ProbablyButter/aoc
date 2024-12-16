#include "matrix.hpp"
#include "fraction.hpp"

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

void permute_matrix::invert() {
  std::vector<long long> tmp(data.size());
  for (long long i = 0; i < data.size(); ++i) {
    tmp[data[i]] = i;
  }
  data = std::move(tmp);
}

void hermite_normal_form(imatrix &H, imatrix &U) {
  U.resize(H.height, H.height);
  // set U to identity
  for (long long row = 0; row < U.height; ++row) {
    for (long long col = 0; col < U.height; ++col) {
      U(row, col) = 0;
    }
    U(row, row) = 1;
  }
  for (long long row = 0; row < U.height; ++row) {
    // do we need to pivot?
    if (!H(row, row)) {
      // TODO: find pivot
      long long prow;
      for (prow = row + 1; prow < U.height; ++prow) {
        if (H(prow, row)) {
          break;
        }
      }
      if (prow == U.height) {
        // entire column of 0, I guess we can skip it?
        continue;
      }
      // swap rows in H and U, optionally negate to get H(row,row) >= 0
      bool negate = (H(prow, row) < 0);
      negate = false;
      for (long long col = row; col < H.width; ++col) {
        std::swap(H(row, col), H(prow, col));
        if (negate) {
          H(row, col) = -H(row, col);
        }
      }
      for (long long col = 0; col < U.width; ++col) {
        std::swap(U(row, col), U(prow, col));
        if (negate) {
          U(row, col) = -U(row, col);
        }
      }
    } else {
      // ensure diagonal is non-negative
      if (H(row, row) < 0) {
        for (long long col = 0; col < U.width; ++col) {
          U(row, col) *= -1;
        }
        for (long long col = row; col < H.width; ++col) {
          H(row, col) = -H(row, col);
        }
      }
    }
    for (long long prow = row + 1; prow < H.height; ++prow) {
      if (H(prow, row)) {
        // gaussian elimination so everything below pivot is zero
        long long x, y, gcd;
        extended_euclid(H(row, row), H(prow, row), x, y, gcd);
        // L = [[x, y], [-H(prow,row), H(row,row)]], multiply rows/cols
        // [row,prow] by L
        // do U first
        auto L10 = -H(prow, row) / gcd;
        auto L11 = H(row, row) / gcd;
        for (long long j = 0; j < U.width; ++j) {
          long long tmp0 = x * U(row, j) + y * U(prow, j);
          long long tmp1 = L10 * U(row, j) + L11 * U(prow, j);
          U(row, j) = tmp0;
          U(prow, j) = tmp1;
        }
        for (long long j = row; j < H.width; ++j) {
          long long tmp0 = x * H(row, j) + y * H(prow, j);
          long long tmp1 = L10 * H(row, j) + L11 * H(prow, j);
          H(row, j) = tmp0;
          H(prow, j) = tmp1;
        }
      }
    }
    // TODO: now ensure entries above pivot are non-negative, and smaller than
    // pivot
  }
}
} // namespace aoc
