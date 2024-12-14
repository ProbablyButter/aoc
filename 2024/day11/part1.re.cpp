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

long long recurse_count(long long stone, int depth,
                        std::unordered_map<std::pair<long long, int>, long long,
                                           aoc::pair_hasher> &cache) {
  auto iter = cache.find(std::make_pair(stone, depth));
  if (iter != cache.end()) {
    return iter->second;
  }
  if (depth == 0) {
    // done
    cache.emplace(std::make_pair(stone, depth), 1);
    return 1;
  }

  if (stone == 0) {
    auto val = recurse_count(1, depth - 1, cache);
    cache.emplace(std::make_pair(stone, depth), val);
    return val;
  } else {
    auto tmp = std::to_string(stone);
    if (tmp.size() % 2 == 0) {
      // split into two stones
      long long left =
          aoc::to_int<long long>(std::string_view(tmp.data(), tmp.size() / 2));
      long long right = aoc::to_int<long long>(
          std::string_view(tmp.data() + tmp.size() / 2, tmp.size() / 2));
      auto val = recurse_count(left, depth - 1, cache) +
                 recurse_count(right, depth - 1, cache);
      cache.emplace(std::make_pair(stone, depth), val);
      return val;
    } else {
      auto val = recurse_count(stone * 2024, depth - 1, cache);
      cache.emplace(std::make_pair(stone, depth), val);
      return val;
    }
  }
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string data(std::istreambuf_iterator<char>{in},
                   std::istreambuf_iterator<char>{});

  std::vector<long long> stones;
  {
    auto tmp = aoc::split(data);
    stones.reserve(tmp.size());
    for (auto &v : tmp) {
      stones.emplace_back(aoc::to_int<long long>(v));
    }
  }
  long long count = 0;
  // stone number, depth
  std::unordered_map<std::pair<long long, int>, long long, aoc::pair_hasher>
      cache;
  for (auto s : stones) {
    count += recurse_count(s, 25, cache);
  }
  std::cout << count << std::endl;
}
