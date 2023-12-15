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

int64_t hash(const std::string_view &str, int64_t curr = 0) {
  for (size_t i = 0; i < str.size(); ++i) {
    curr += str[i];
    curr *= 17;
    curr %= 256;
  }
  return curr;
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  int64_t sum = 0;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line,",");
    for(auto& v : tmp)
    {
      auto val = hash(v);
      sum += val;
    }
  }
  std::cout << sum << std::endl;
}
