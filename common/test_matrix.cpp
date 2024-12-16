#include "fraction.hpp"
#include "matrix.hpp"

#include <algorithm>
#include <iostream>

int main() {
#if 0
  {
    using scalar_type = aoc::fraction;
    aoc::matrix<scalar_type> A(4, 4);
    A(0, 0) = 1;
    A(0, 1) = 4;
    A(0, 2) = 9;
    A(0, 3) = 16;

    A(1, 0) = 5;
    A(1, 1) = 3;
    A(1, 2) = 2;
    A(1, 3) = 1;

    A(2, 0) = 7;
    A(2, 1) = 8;
    A(2, 2) = 9;
    A(2, 3) = 10;

    A(3, 0) = 11;
    A(3, 1) = 9;
    A(3, 2) = 8;
    A(3, 3) = 7;
    for (long long row = 0; row < A.height; ++row) {
      for (long long col = 0; col < A.width; ++col) {
        std::cout << A(row, col) << ", ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
    aoc::permute_matrix P;
    aoc::matrix<scalar_type> LU = A;
    aoc::LUP_decomposition(LU, P);

    for (long long row = 0; row < A.height; ++row) {
      for (long long col = 0; col < A.width; ++col) {
        std::cout << LU(row, col) << ", ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    aoc::matrix<scalar_type> L, U;
    aoc::split_LU(LU, L, U);

    for (long long row = 0; row < A.height; ++row) {
      for (long long col = 0; col < A.width; ++col) {
        std::cout << L(row, col) << ", ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (long long row = 0; row < A.height; ++row) {
      for (long long col = 0; col < A.width; ++col) {
        std::cout << U(row, col) << ", ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    auto LUmult = L * U;

    for (long long row = 0; row < A.height; ++row) {
      for (long long col = 0; col < A.width; ++col) {
        std::cout << LUmult(row, col) << ", ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
#endif
#if 0
  {
    aoc::imatrix A(4, 4);
    A(0, 0) = 3;
    A(0, 1) = 3;
    A(0, 2) = 1;
    A(0, 3) = 4;

    A(1, 0) = 0;
    A(1, 1) = 1;
    A(1, 2) = 0;
    A(1, 3) = 0;

    A(2, 0) = 0;
    A(2, 1) = 0;
    A(2, 2) = 19;
    A(2, 3) = 16;

    A(3, 0) = 0;
    A(3, 1) = 0;
    A(3, 2) = 0;
    A(3, 3) = 3;
    aoc::imatrix U(4, 4);
    aoc::hermite_normal_form(A, U);
    for (int row = 0; row < A.height; ++row) {
      for (int col = 0; col < A.width; ++col) {
        std::cout << A(row, col) << ", ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (int row = 0; row < U.height; ++row) {
      for (int col = 0; col < U.width; ++col) {
        std::cout << U(row, col) << ", ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
#endif
  {
    aoc::imatrix A(3, 4);
    A(0, 0) = 2;
    A(0, 1) = 3;
    A(0, 2) = 6;
    A(0, 3) = 2;

    A(1, 0) = 5;
    A(1, 1) = 6;
    A(1, 2) = 1;
    A(1, 3) = 6;

    A(2, 0) = 8;
    A(2, 1) = 3;
    A(2, 2) = 1;
    A(2, 3) = 1;

    aoc::imatrix U;
    aoc::hermite_normal_form(A, U);
    for (int row = 0; row < A.height; ++row) {
      for (int col = 0; col < A.width; ++col) {
        std::cout << A(row, col) << ", ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;

    for (int row = 0; row < U.height; ++row) {
      for (int col = 0; col < U.width; ++col) {
        std::cout << U(row, col) << ", ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
    aoc::matrix<aoc::fraction> Utest(U.height, U.width);
    for (int row = 0; row < U.height; ++row) {
      for (int col = 0; col < U.width; ++col) {
        Utest(row, col) = U(row, col);
      }
    }
    aoc::permute_matrix P;
    LUP_decomposition(Utest, P);
    std::cout << LUP_determinant(Utest, P) << std::endl;
  }
  return 0;
}
