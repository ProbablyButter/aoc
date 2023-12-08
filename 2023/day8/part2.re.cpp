/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"
#include "string_tools.hpp"

#include <algorithm>
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
  std::string directions;
  std::getline(in, directions);
  std::string line;
  std::getline(in, line);
  std::unordered_map<std::string, std::string> left_moves;
  std::unordered_map<std::string, std::string> right_moves;
  std::vector<std::string> currs;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line, " = ");
    if (tmp[0].back() == 'A') {
      currs.emplace_back(tmp[0]);
    }
    auto l = tmp[1].substr(1, 3);
    auto r = tmp[1].substr(1 + 3 + 2, 3);
    left_moves.emplace(tmp[0], l);
    right_moves.emplace(tmp[0], r);
  }

  std::vector<size_t> path_lengths;
  path_lengths.reserve(currs.size());

  for (auto curr : currs) {
    size_t pos = 0;
    size_t steps = 0;
    std::unordered_set<std::string> visited_dsts;
    while (true) {
      if (curr.back() == 'Z') {
        if (pos == 0) {
          visited_dsts.emplace(curr);
          path_lengths.emplace_back(steps);
          // assume we can only visit on dst with each start
          break;
        }
      }
      ++steps;
      switch (directions[pos]) {
      case 'L':
        curr = left_moves.at(curr);
        break;
      case 'R':
        curr = right_moves.at(curr);
        break;
      default:
        throw std::runtime_error("invalid direction");
      }
      pos = (pos + 1) % directions.size();
    }
  }
  size_t res = 1;
  for(auto& v : path_lengths)
  {
    res = std::lcm(v, res);
  }
  std::cout << res << std::endl;
}
