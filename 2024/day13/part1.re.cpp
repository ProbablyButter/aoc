/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"
#include "fraction.hpp"
#include "geometry_tools.hpp"
#include "graph_tools.hpp"
#include "matrix.hpp"
#include "string_tools.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  std::vector<std::pair<long long, long long>> A;
  std::vector<std::pair<long long, long long>> B;
  std::vector<std::pair<long long, long long>> prize;
  long long offset = 0;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    if (line.size() > 7 && line[7] == 'A') {
      auto tmp = aoc::split(line, "+");
      A.emplace_back(aoc::to_int<long long>(tmp[1]),
                     aoc::to_int<long long>(tmp[2]));
    } else if (line.size() > 7 && line[7] == 'B') {
      auto tmp = aoc::split(line, "+");
      B.emplace_back(aoc::to_int<long long>(tmp[1]),
                     aoc::to_int<long long>(tmp[2]));
    } else if (line.size() > 5) {
      auto tmp = aoc::split(line, "=");
      prize.emplace_back(aoc::to_int<long long>(tmp[1]) + offset,
                         aoc::to_int<long long>(tmp[2]) + offset);
    }
  }
  int A_cost = 3;
  int B_cost = 1;
  long long total = 0;
  aoc::imatrix Amat(2, 2);
  aoc::imatrix Umat(2, 2);
  aoc::imatrix rhs(2, 1);
  for (size_t i = 0; i < prize.size(); ++i) {
    // Diophantine equation system:
    // A Ax + B Bx = Px
    // A Ay + B By = Py
    // [Ax Bx] [A] = [Px]
    // [Ay By] [B] = [Py]
    Amat(0, 0) = A[i].first;
    Amat(0, 1) = B[i].first;
    Amat(1, 0) = A[i].second;
    Amat(1, 1) = B[i].second;
    aoc::hermite_normal_form(Amat, Umat);
    rhs(0, 0) = prize[i].first;
    rhs(1, 0) = prize[i].second;

    aoc::imatrix tmp = Umat * rhs;

    // hope that Amat diagonals are non-zero

    if (tmp(1, 0) % Amat(1, 1)) {
      // std::cout << i << ": impossible B" << std::endl;
      continue;
    }
    long long solB = tmp(1, 0) / Amat(1, 1);
    if ((tmp(0, 0) - Amat(0, 1) * solB) % Amat(0, 0)) {
      // std::cout << i << ": impossible A" << std::endl;
      continue;
    }
    long long solA = (tmp(0, 0) - Amat(0, 1) * solB) / Amat(0, 0);
    if (solA < 0 || solB < 0) {
      std::cout << i << ": " << solA << ", " << solB << std::endl;
    }
    total += solA * A_cost + solB * B_cost;
  }
  std::cout << total << std::endl;
}
