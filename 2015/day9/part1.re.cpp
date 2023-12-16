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
  aoc::directed_graph<int64_t> graph;

  std::unordered_map<std::string, size_t> cities_map;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line);
    auto c0 = cities_map.emplace(std::string(tmp[0]), cities_map.size());
    auto c1 = cities_map.emplace(std::string(tmp[2]), cities_map.size());
    auto dist = aoc::to_int<int64_t>(tmp.back());
    graph.add_edge(c0.first->second, c1.first->second, dist);
    graph.add_edge(c1.first->second, c0.first->second, dist);
  }
  std::vector<size_t> order;
  order.reserve(cities_map.size());
  for (size_t i = 0; i < cities_map.size(); ++i) {
    order.emplace_back(i);
  }
  int64_t min_dist = 0x7FFFFFFFFFFFFFFFull;
  while (true) {
    bool valid = true;
    int64_t dist = 0;
    for (size_t i = 0; i + 1 < order.size(); ++i) {
      auto iter = graph.connectivity[order[i]].find(order[i + 1]);
      if (iter == graph.connectivity[order[i]].end()) {
        valid = false;
        break;
      }
      dist += iter->second;
    }
    if (valid) {
      min_dist = std::min(dist, min_dist);
    }
    if (!std::next_permutation(order.begin(), order.end())) {
      break;
    }
  }
  std::cout << min_dist << std::endl;
}
