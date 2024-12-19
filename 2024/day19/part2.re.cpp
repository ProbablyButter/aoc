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
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

long long count_ways(const std::string &pattern, long long pos,
               std::unordered_map<long long, long long> &cache,
               const std::vector<std::string> &towels) {
  auto iter = cache.find(pos);
  if (iter != cache.end()) {
    return iter->second;
  }
  std::string_view rem(pattern.data() + pos);
  long long sum = 0;
  for (auto &t : towels) {
    // does rem start with t?
    if (rem.size() >= t.size()) {
      bool check = true;
      for (long long i = 0; i < t.size(); ++i) {
        if (rem[i] != t[i]) {
          check = false;
          break;
        }
      }
      if (check) {
        if (t.size() == rem.size()) {
          ++sum;
        } else {
          // recurse
          sum += count_ways(pattern, pos + t.size(), cache, towels);
        }
      }
    }
  }
  cache.emplace(pos, sum);
  return sum;
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);

  using node_type = std::array<long long, 2>;
  std::string line;
  // available towels
  std::getline(in, line);
  std::vector<std::string> towels;
  {
    auto tmp = aoc::split(line, ", ");
    for (auto &v : tmp) {
      towels.emplace_back(v);
    }
  }

  std::vector<std::string> patterns;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    if (line.size()) {
      patterns.emplace_back(line);
    }
  }

  long long count = 0;
  std::unordered_map<long long, long long> cache;
  for (auto &v : patterns) {
    cache.clear();
    count += count_ways(v, 0, cache, towels);
  }
  std::cout << count << std::endl;
}
