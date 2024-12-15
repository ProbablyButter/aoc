#include "matrix.hpp"

#include <algorithm>
#include <iostream>

int main() {
  std::vector<long long> P(4);
  for (int i = 0; i < P.size(); ++i) {
    P[i] = i;
  }

  long long expected = 1;
  do {
    auto tmp = aoc::permute_parity(P);
    std::cout << tmp << ": ";
    for (auto v : P) {
      std::cout << v << ", ";
    }
    std::cout << std::endl;
  } while (std::next_permutation(P.begin(), P.end()));
  return 0;
}
