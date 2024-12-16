#ifndef AOC_MATRIX_HPP
#define AOC_MATRIX_HPP

#include <cmath>
#include <vector>

namespace aoc {
double abs(double v) { return fabs(v); }

// doesn't have any concept of data ownership, but implements basic matrix
// operations predecated on some virtual function calls
template <class T> struct matrix_base {
  long long width;
  long long height;

  matrix_base() : width(0), height(0) {}
  matrix_base(long long h_, long long w_) : width(w_), height(h_) {}

  matrix_base(const matrix_base &) = default;

  virtual ~matrix_base() = default;

  virtual T &operator()(long long row, long long col) = 0;
  virtual const T &operator()(long long row, long long col) const {
    return const_cast<matrix_base<T> &>(*this).operator()(row, col);
  }

  template <class U> matrix_base &operator+=(const matrix_base<U> &o) {
    for (long long row = 0; row < height; ++row) {
      for (long long col = 0; col < width; ++col) {
        this->operator()(row, col) += o(row, col);
      }
    }
    return *this;
  }

  template <class U> matrix_base &operator-=(const matrix_base<U> &o) {
    for (long long row = 0; row < height; ++row) {
      for (long long col = 0; col < width; ++col) {
        this->operator()(row, col) -= o(row, col);
      }
    }
    return *this;
  }
};

// parity of the permutation, +/- 1
long long permute_parity(const std::vector<long long> &P);

struct permute_matrix : public matrix_base<const long long> {
  using matrix_base<const long long>::width;
  using matrix_base<const long long>::height;
  std::vector<long long> data;
  long long internal[2] = {0, 1};

  permute_matrix() : matrix_base<const long long>(0, 0) {}

  permute_matrix(const permute_matrix &) = default;

  permute_matrix(permute_matrix &&o) noexcept
      : matrix_base<const long long>(o), data(std::move(o.data)) {}

  // identity permutation
  permute_matrix(long long w) : matrix_base<const long long>(w, w), data(w) {
    for (long long i = 0; i < w; ++i) {
      data[i] = i;
    }
  }

  permute_matrix &operator=(const permute_matrix &) = default;

  permute_matrix &operator=(permute_matrix &&o) {
    if (&o != this) {
      width = o.width;
      height = o.height;
      data = std::move(o.data);
    }
    return *this;
  }

  const long long &operator()(long long row, long long col) override {
    return internal[data[row] == col];
  }

  long long parity() const { return permute_parity(data); }

  void invert();

  // A = P A
  // swaps rows
  template <class U> void apply_P_left(U &&A) {
    std::vector<bool> done(height);
    for (long long i = 0; i < data.size(); ++i) {
      if (done[i]) {
        continue;
      }
      done[i] = true;
      long long prev_j = i;
      long long j = data[i];
      while (i != j) {
        // swap prev_j and j
        for (size_t k = 0; k < width; ++k) {
          std::swap(A(prev_j, k), A(j, k));
        }
        done[j] = true;
        prev_j = j;
        j = data[j];
      }
    }
  }

  // A = A P
  // swaps columns
  template <class U> void apply_P_right(U &&A) {
    std::vector<bool> done(width);
    for (long long i = 0; i < data.size(); ++i) {
      if (done[i]) {
        continue;
      }
      done[i] = true;
      long long prev_j = i;
      long long j = data[i];
      while (i != j) {
        // swap prev_j and j
        for (size_t k = 0; k < height; ++k) {
          std::swap(A(k, prev_j), A(k, j));
        }
        done[j] = true;
        prev_j = j;
        j = A[j];
      }
    }
  }
};

template <class T> struct minor_matrix : public matrix_base<T> {
  using matrix_base<const long long>::width;
  using matrix_base<const long long>::height;

  std::vector<long long> rows;
  std::vector<long long> cols;

  matrix_base<T> *base = nullptr;

  minor_matrix() : matrix_base<T>() {}
  minor_matrix(matrix_base<T> &b_)
      : matrix_base<T>(b_.height, b_.width), base(&b_) {
    rows.resize(height);
    cols.resize(width);
    for (long long i = 0; i < height; ++i) {
      rows[i] = i;
    }

    for (long long i = 0; i < width; ++i) {
      cols[i] = i;
    }
  }
  minor_matrix &operator=(const minor_matrix &o) = default;

  minor_matrix &operator=(minor_matrix &&o) {
    if (&o != this) {
      width = o.width;
      height = o.height;
      rows = std::move(o.rows);
      cols = std::move(o.cols);
      base = o.base;
    }
    return *this;
  }

  T &operator()(long long row, long long col) override {
    return (*base)(rows[row], cols[col]);
  }

  template <class U> minor_matrix &operator=(const matrix_base<U> &o) {
    for (long long row = 0; row < height; ++row) {
      for (long long col = 0; col < width; ++col) {
        this->operator()(row, col) = o(row, col);
      }
    }
    return *this;
  }

  void remove_row(long long r) {
    rows.erase(rows.begin() + r);
    --height;
  }

  void remove_col(long long r) {
    cols.erase(cols.begin() + r);
    --width;
  }
};

// dense matrix
template <class T> struct matrix : public matrix_base<T> {
  using matrix_base<T>::width;
  using matrix_base<T>::height;
  std::vector<T> data;

  matrix() : matrix_base<T>() {}
  matrix(long long h_, long long w_) : matrix_base<T>(h_, w_), data(w_ * h_) {}

  matrix(const matrix &) = default;

  matrix(matrix &&o) noexcept : matrix_base<T>(o), data(std::move(o.data)) {}

  matrix &operator=(const matrix &) = default;

  template <class U> matrix &operator=(const matrix_base<U> &o) {
    if (&o != this) {
      width = o.width;
      height = o.height;
      data.resize(width * height);
      for (long long row = 0; row < height; ++row) {
        for (long long col = 0; col < width; ++col) {
          this->operator()(row, col) = o(row, col);
        }
      }
    }
    return *this;
  }

  matrix &operator=(matrix &&o) noexcept {
    if (&o != this) {
      data = std::move(o.data);
      width = o.width;
      height = o.height;
    }
    return *this;
  }

  using matrix_base<T>::operator();

  T &operator()(long long row, long long col) override {
    return data[row * width + col];
  }

  // P A = L U
  // stores L and U into *this, diagonals belong to U (diagonals of L are
  // implicitly unity)
  // must be square
  // uses partial pivoting
  void LUP_decomposition(permute_matrix &P) {
    P = permute_matrix(width);
    for (long long row = 0; row < height; ++row) {
      // find pivot
      {
        long long prow = row;
        for (long long tmp = row; tmp < height; ++tmp) {
          using namespace std;
          if (abs((*this)(prow, row)) < abs((*this)(tmp, row))) {
            prow = tmp;
          }
        }
        if (prow != row) {
          // swap rows prow and row
          std::swap(P.data[row], P.data[prow]);
          for (long long col = 0; col < width; ++col) {
            std::swap((*this)(prow, col), (*this)(row, col));
          }
        }
      }
      // perform gaussian elimination
      for (long long prow = row + 1; prow < height; ++prow) {
        // assume LU(row,row) != 0, otherwise singular
        // TODO: should this be negative?
        (*this)(prow, row) /= (*this)(row, row);
        for (long long col = row + 1; col < width; ++col) {
          (*this)(prow, col) -= (*this)(prow, row) * (*this)(row, col);
        }
      }
    }
  }

  // split this combined LU matrix into two separate L and U matrices
  void split_LU(matrix &L, matrix &U) const {
    L = *this;
    U = *this;
    for (long long row = 0; row < height; ++row) {
      for (long long col = row + 1; col < width; ++col) {
        L(row, col) = 0;
      }
      L(row, row) = 1;
    }

    for (long long row = 0; row < height; ++row) {
      for (long long col = 0; col < row; ++col) {
        U(row, col) = 0;
      }
    }
  }
};

template <class T, class U>
matrix<T> operator*(const matrix_base<T> &a, const matrix_base<U> &o) {
  matrix<T> res(a.height, o.width);
  for (long long i = 0; i < a.height; ++i) {
    for (long long j = 0; j < o.width; ++j) {
      res(i, j) = 0;
      for (long long k = 0; k < a.width; ++k) {
        res(i, j) += a(i, k) * o(k, j);
      }
    }
  }
  return res;
}

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
