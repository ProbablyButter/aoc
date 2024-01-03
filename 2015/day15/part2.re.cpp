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
  std::vector<std::array<int64_t, 3>> params;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line, "");
    params.emplace_back();
    params.back()[0] = aoc::to_int<int64_t>(tmp[3]);
    params.back()[1] = aoc::to_int<int64_t>(tmp[6]);
    params.back()[2] = aoc::to_int<int64_t>(tmp[13]);
  }
  std::vector<int64_t> points(params.size());
  std::vector<int64_t> curr_dists(params.size());
  int64_t end_time = 2503;
  for (int64_t t = 1; t <= end_time; ++t) {
    int64_t max_dist = 0;
    for (size_t i = 0; i < params.size(); ++i) {
      auto &v = params[i];
      auto full = t / (v[1] + v[2]);
      auto rem = t % (v[1] + v[2]);
      auto dist = full * v[0] * v[1];
      if (rem >= v[1]) {
        dist += v[0] * v[1];
      } else {
        dist += v[0] * rem;
      }
      curr_dists[i] = dist;
      if (dist >= max_dist) {
        max_dist = dist;
      }
    }
    for (size_t i = 0; i < params.size(); ++i) {
      if (curr_dists[i] >= max_dist) {
        ++points[i];
      }
    }
  }
  std::cout << *std::max_element(points.begin(), points.end()) << std::endl;
}
