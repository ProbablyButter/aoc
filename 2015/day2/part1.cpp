/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"
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
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  int64_t total = 0;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto dims = aoc::split(line, "x");
    std::vector<int64_t> idims;
    for (auto &v : dims) {
      idims.emplace_back(aoc::to_int<int64_t>(v));
    }
    auto f0 = idims[0] * idims[1];
    auto f1 = idims[1] * idims[2];
    auto f2 = idims[0] * idims[2];
    total += 2 * f0 + 2 * f1 + 2 * f2;
    total += std::min(f0, std::min(f1, f2));
  }
  std::cout << total << std::endl;
}
