/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"
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
  long long offset = 10000000000000ll;
  // offset = 0;
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
  for (size_t i = 0; i < prize.size(); ++i) {
    // Diophantine equation system:
    // A Ax + B Bx = Px
    // A Ay + B By = Py
    // [Ax Bx] [A] = [Px]
    // [Ay By] [B] = [Py]
    long long x0, y0, gcd0;
    aoc::extended_euclid(A[i].first, B[i].first, x0, y0, gcd0);
    if (prize[i].first % gcd0 == 0) {
      // solution might exist
      long long s0 = x0 * prize[i].first / gcd0;
      long long s1 = y0 * prize[i].first / gcd0;
      long long u0 = A[i].first / gcd0;
      long long v0 = B[i].first / gcd0;

      // any s0 + k0 * v0 >= 0 could be a solution
      //
      long long min_k0 = -s0 / v0;
      long long max_k0 = s1 / u0;
      if (min_k0 > max_k0) {
        std::swap(min_k0, max_k0);
      }
      // sometimes off by 1, idk
      --min_k0;
      ++max_k0;
      long long min_A = -1;
      long long min_B = -1;
      std::cout << min_k0 << ", " << max_k0 << std::endl;
      for (long long k = min_k0; k <= max_k0; ++k) {
        long long A_count = s0 + k * v0;
        long long B_count = s1 - k * u0;
        if (A_count * A[i].second + B_count * B[i].second == prize[i].second) {
          // valid solution
          if (min_A >= 0) {
            auto cost = A_count * A_cost + B_count * B_cost;
            auto min_cost = min_A * A_cost + min_B * B_cost;
            if (cost < min_cost) {
              min_A = A_count;
              min_B = B_count;
            }
          } else {
            min_A = A_count;
            min_B = B_count;
          }
        }
      }
      if (min_A >= 0) {
        std::cout << "min sol: " << min_A << ", " << min_B << std::endl;
        auto min_cost = min_A * A_cost + min_B * B_cost;
        total += min_cost;
      }
      else
      {
        std::cout << "no sol inner" << std::endl;
      }
      // std::cout << "max " << max_k0 << ", " << s0 + max_k0 * v0 << std::endl;
      // std::cout << "min " << min_k0 << ", " << s0 + min_k0 * v0 << ", "
      //           << s1 - min_k0 * u0 << std::endl;
      // std::cout << "max " << max_k0 << ", " << s0 + max_k0 * v0 << ", "
      //           << s1 - max_k0 * u0 << std::endl;
      // total += std::abs(min_k0 - max_k0);
      // std::cout << std::endl;
    }
    else
    {
      std::cout << "no sol outer" << std::endl;
    }
  }
  std::cout << total << std::endl;
}
