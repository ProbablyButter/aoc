#include "fraction.hpp"
#include "matrix.hpp"

#include <algorithm>
#include <iostream>

int main() {
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

  return 0;
}
