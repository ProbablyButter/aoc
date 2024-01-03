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
#include "hash.hpp"
#include "string_tools.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;

  std::vector<int64_t> capacity;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    capacity.emplace_back(aoc::to_int<int64_t>(line));
  }
  std::sort(capacity.begin(), capacity.end());
  aoc::hslab_iterator iter;
  {
    iter.idx.push_back(0);
    iter.lower.push_back(0);
    iter.upper.push_back(2);
    size_t idx = 0;
    for (size_t i = 1; i < capacity.size(); ++i) {
      iter.idx.push_back(0);
      iter.lower.push_back(0);
      iter.upper.push_back(2);
      idx = i;
    }
  }
  // capacity.erase(std::unique(capacity.begin(), capacity.end()),
  // capacity.end());

  int target_cap = 150;
  int count = 0;
  while (true) {
    int cap = 0;
    for (size_t i = 0; i < capacity.size(); ++i) {
      cap += iter.idx[i] * capacity[i];
    }
    if (cap == target_cap) {
      ++count;
    }
    if (!iter.advance()) {
      break;
    }
  }
  std::cout << count << std::endl;
}
