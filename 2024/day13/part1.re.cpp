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
#include "string_tools.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input_small.txt");
  std::ifstream in(in_path);
  std::string line;
  std::vector<std::pair<int, int>> A;
  std::vector<std::pair<int, int>> B;
  std::vector<std::pair<int, int>> prize;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    if (line.size() > 7 && line[7] == 'A') {
      auto tmp = aoc::split(line, "+");
      A.emplace_back(aoc::to_int<int>(tmp[1]), aoc::to_int<int>(tmp[2]));
    } else if (line.size() > 7 && line[7] == 'B') {
      auto tmp = aoc::split(line, "+");
      B.emplace_back(aoc::to_int<int>(tmp[1]), aoc::to_int<int>(tmp[2]));
    } else if (line.size() > 5) {
      auto tmp = aoc::split(line, "=");
      prize.emplace_back(aoc::to_int<int>(tmp[1]), aoc::to_int<int>(tmp[2]));
    }
  }
  int A_cost = 3;
  int B_cost = 1;
  int total = 0;
  for (size_t i = 0; i < prize.size(); ++i) {
    // X == a * A.x + b * B.x
    // Y == a * A.y + b * B.y
    int min_A = -1;
    int min_B = -1;
    int num_A = 0;
    while (true) {
      int num_B = (prize[i].first - num_A * A[i].first) / B[i].first;
      if (num_B < 0) {
        break;
      }
      if (num_B * B[i].first + num_A * A[i].first == prize[i].first &&
          num_B * B[i].second + num_A * A[i].second == prize[i].second) {
        // valid
        if (min_A < 0 ||
            A_cost * num_A + B_cost * num_B < A_cost * min_A + B_cost * min_B) {
          min_A = num_A;
          min_B = num_B;
        }
      }
      if (num_B <= 0) {
        break;
      }
      ++num_A;
    }
    if (min_A >= 0) {
      auto tmp = min_A * A_cost + min_B * B_cost;
      total += min_A * A_cost + min_B * B_cost;
    }
  }
  std::cout << total << std::endl;
}
