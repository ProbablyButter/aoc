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
  std::getline(in, line);
  int64_t row[2] = {0, 0};
  int64_t col[2] = {0, 0};
  std::set<std::pair<int64_t, int64_t>> visited;
  visited.emplace(row[0], col[0]);
  for (size_t i = 0; i < line.size(); ++i) {
    auto c = line[i];
    auto pos = i % 2;
    switch (c) {
    case '>':
      ++col[pos];
      break;
    case '^':
      --row[pos];
      break;
    case '<':
      --col[pos];
      break;
    case 'v':
      ++row[pos];
      break;
    }
    visited.emplace(row[pos], col[pos]);
  }
  std::cout << visited.size() << std::endl;
}
