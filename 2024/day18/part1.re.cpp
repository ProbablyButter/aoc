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

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);

  using node_type = std::array<long long, 2>;
  std::string line;
  std::vector<std::pair<long long, long long>> coords;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line, ",");
    coords.emplace_back(aoc::to_int<long long>(tmp[0]),
                        aoc::to_int<long long>(tmp[1]));
  }

  int width = 71, height = 71;
  int num_corrupted = 1024;
  // width = 7;
  // height = 7;
  // num_corrupted = 12;
  aoc::matrix<int> grid(height, width);
  for (int i = 0; i < num_corrupted; ++i) {
    grid(coords[i].second, coords[i].first) = 1;
  }
  auto term_func = [&](const node_type &node) {
    if (node[0] == height - 1 && node[1] == width - 1) {
      return true;
    }
    return false;
  };

  auto gen_neighbors = [&](const node_type &node) {
    std::vector<std::pair<node_type, int64_t>> res;
    if (node[0] > 0) {
      node_type next = {node[0] - 1, node[1]};
      if (!grid(next[0], next[1])) {
        res.emplace_back(next, 1);
      }
    }
    if (node[0] + 1 < height) {
      node_type next = {node[0] + 1, node[1]};
      if (!grid(next[0], next[1])) {
        res.emplace_back(next, 1);
      }
    }
    if (node[1] > 0) {
      node_type next = {node[0], node[1] - 1};
      if (!grid(next[0], next[1])) {
        res.emplace_back(next, 1);
      }
    }
    if (node[1] + 1 < width) {
      node_type next = {node[0], node[1] + 1};
      if (!grid(next[0], next[1])) {
        res.emplace_back(next, 1);
      }
    }
    return res;
  };

  std::unordered_map<node_type, int64_t, aoc::array_hasher> dist;
  std::unordered_map<node_type, node_type, aoc::array_hasher> prev;
  node_type start = {0, 0};
  aoc::gen_dijkstra(start, term_func, gen_neighbors, dist, prev);
  node_type stop = {height - 1, width - 1};
  std::cout << dist[stop] << std::endl;
}
