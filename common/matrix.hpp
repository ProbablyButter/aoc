#ifndef AOC_MATRIX_HPP
#define AOC_MATRIX_HPP

#include <vector>

namespace aoc {

// parity of the permutation, +/- 1
long long permute_parity(const std::vector<long long> &P);

// dense matrix
template <class T> struct matrix {
  std::vector<T> data;
  long long width;
  long long height;

  matrix() : width(0), height(0) {}
  matrix(long long h_, long long w_) : data(w_ * h_), width(w_), height(h_) {}

  matrix(const matrix &) = default;

  matrix(matrix &&o) noexcept
      : data(std::move(o.data)), width(o.width), height(o.height) {}

  matrix &operator=(const matrix &) = default;

  matrix &operator=(matrix &&o) noexcept {
    if (&o != this) {
      data = std::move(o.data);
      width = o.width;
      height = o.height;
    }
    return *this;
  }

  auto &operator()(long long row, long long col) {
    return data[row * width + col];
  }

  const auto &operator()(long long row, long long col) const {
    return data[row * width + col];
  }
  matrix &operator+=(const matrix &o) {
    for (size_t i = 0; i < data.size(); ++i) {
      data[i] += o.data[i];
    }
    return *this;
  }

  matrix &operator-=(const matrix &o) {
    for (size_t i = 0; i < data.size(); ++i) {
      data[i] -= o.data[i];
    }
    return *this;
  }

  matrix operator*(const matrix &o) const {
    matrix res(height, o.width);
    for (long long i = 0; i < height; ++i) {
      for (long long j = 0; j < o.width; ++j) {
        for (long long k = 0; k < width; ++k) {
          res(i, j) += operator()(i, k) * o(k, j);
        }
      }
    }
    return res;
  }

  // this = P A
  // swaps rows
  void apply_P_left(const std::vector<long long> &P) {
    std::vector<bool> done(height);
    for (long long i = 0; i < P.size(); ++i) {
      if (done[i]) {
        continue;
      }
      done[i] = true;
      long long prev_j = i;
      long long j = P[i];
      while (i != j) {
        // swap prev_j and j
        for (size_t k = 0; k < width; ++k) {
          std::swap(operator()(prev_j, k), operator()(j, k));
        }
        done[j] = true;
        prev_j = j;
        j = P[j];
      }
    }
  }

  // this = A P
  // swaps columns
  void apply_P_right(const std::vector<long long> &P) {
    std::vector<bool> done(width);
    for (long long i = 0; i < P.size(); ++i) {
      if (done[i]) {
        continue;
      }
      done[i] = true;
      long long prev_j = i;
      long long j = P[i];
      while (i != j) {
        // swap prev_j and j
        for (size_t k = 0; k < height; ++k) {
          std::swap(operator()(k, prev_j), operator()(k, j));
        }
        done[j] = true;
        prev_j = j;
        j = P[j];
      }
    }
  }

  void PLU_decomposition(std::vector<long long> &P, matrix &LU) const;
};

/// dense integer matrix
// struct imatrix : public matrix<long long> {
//   imatrix() : matrix(0, 0) {}
//   imatrix(long long h_, long long w_) : matrix(h_, w_) {}

//   imatrix(const imatrix &) = default;

//   imatrix(imatrix &&o) noexcept = default;

//   imatrix &operator=(const imatrix &) = default;

//   imatrix &operator=(imatrix &&o) noexcept = default;

//   // computes the PLU decomposition
//   void PLU_decompose(imatrix &res) const;

//   // computes the determinant of this matrix
//   long long det() const;

//   /// Computes the Hermite normal form:
//   /// H = this K
//   /// H is lower triangular
//   /// K is unimodular
//   /// requires: this is square and nonsingular
//   void hermite_normal_form(imatrix &H, imatrix &K) const;

//   void set_eye(long long d);
// };
} // namespace aoc

#endif
