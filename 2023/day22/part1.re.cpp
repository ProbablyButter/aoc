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

int64_t freefall(std::vector<aoc::hslab<int64_t, 3>> &bricks) {
  int64_t count = 0;
  for (size_t i = 0; i < bricks.size(); ++i) {
    // move brick i as low as possible
    int64_t min_z = 1;
    auto tmp = bricks[i];
    tmp.lower[2] = 1;
    for (size_t j = 0; j < i; ++j) {
      // is brick j blocking brick i?
      auto inter = tmp.intersect(bricks[j]);
      if (inter.volume()) {
        min_z = std::max(min_z, bricks[j].upper[2]);
      }
    }

    int64_t mz = bricks[i].lower[2] - min_z;
    if (mz > 0) {
      ++count;
    }
    bricks[i].lower[2] = min_z;
    bricks[i].upper[2] -= mz;
  }
  return count;
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  // maps a gate to its outputs
  std::vector<aoc::hslab<int64_t, 3>> bricks;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line, "~");
    auto t0 = aoc::split(tmp[0], ",");
    std::array<int64_t, 3> left = {aoc::to_int<int64_t>(t0[0]),
                                   aoc::to_int<int64_t>(t0[1]),
                                   aoc::to_int<int64_t>(t0[2])};

    auto t1 = aoc::split(tmp[1], ",");
    std::array<int64_t, 3> right = {aoc::to_int<int64_t>(t1[0]),
                                    aoc::to_int<int64_t>(t1[1]),
                                    aoc::to_int<int64_t>(t1[2])};
    bricks.emplace_back();
    for (size_t i = 0; i < 3; ++i) {
      bricks.back().lower[i] = std::min(left[i], right[i]);
      bricks.back().upper[i] = std::max(left[i], right[i]) + 1;
    }
  }

  std::sort(bricks.begin(), bricks.end(), [](auto &a, auto &b) {
    return std::lexicographical_compare(a.lower.rbegin(), a.lower.rend(),
                                        b.lower.rbegin(), b.lower.rend());
  });

  freefall(bricks);

  // figure out which bricks support which other ones
  std::vector<std::vector<size_t>> supported;
  supported.resize(bricks.size());

  for (size_t i = 0; i < bricks.size(); ++i) {
    for (size_t j = 0; j < i; ++j) {
      auto tmp = bricks[j];
      ++tmp.upper[2];
      // is brick j supporting brick i?
      auto inter = tmp.intersect(bricks[i]);
      if (inter.volume()) {
        supported[i].push_back(j);
      }
    }
  }
  std::unordered_set<int64_t> can_remove;
  for (size_t i = 0; i < bricks.size(); ++i) {
    can_remove.emplace(i);
  }
  for (auto &v : supported) {
    if (v.size() == 1) {
      can_remove.erase(v[0]);
    }
  }
  std::cout << can_remove.size() << std::endl;
}
