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
  std::string data(std::istreambuf_iterator<char>{in},
                   std::istreambuf_iterator<char>{});
  auto grid = aoc::split(data);
  // find the start
  int srow, scol;
  for (srow = 0; srow < grid.size(); ++srow) {
    for (scol = 0; scol < grid[srow].size(); ++scol) {
      if (grid[srow][scol] == 'S') {
        goto found_start;
      }
    }
  }
found_start:
  // gen djikstra, but node ID is position and facing direction
  // 0: north
  // 1: east
  // 2: south
  // 3: west
  using node_type = std::array<int64_t, 3>;
  node_type start = {srow, scol, 1};
  std::unordered_map<node_type, int64_t, aoc::array_hasher> dist;
  std::unordered_map<node_type, node_type, aoc::array_hasher> prev;
  node_type end_node;
  auto term_func = [&](const node_type &node) {
    if (grid[node[0]][node[1]] == 'E') {
      end_node = node;
      return true;
    }
    return false;
  };
  auto gen_neighbors = [&](const node_type &node) {
    std::vector<std::pair<node_type, int64_t>> res;
    // left turn
    {
      node_type next = {node[0], node[1], node[2] - 1};
      while (next[2] < 0) {
        next[2] += 4;
      }
      res.emplace_back(next, 1000);
    }
    // right turn
    {
      node_type next = {node[0], node[1], (node[2] + 1) % 4};
      res.emplace_back(next, 1000);
    }
    int vx, vy;
    switch (node[2]) {
    case 0:
      vx = 0;
      vy = -1;
      break;
    case 1:
      vx = 1;
      vy = 0;
      break;
    case 2:
      vx = 0;
      vy = 1;
      break;
    case 3:
      vx = -1;
      vy = 0;
      break;
    }
    if (grid[node[0] + vy][node[1] + vx] != '#') {
      // go straight
      node_type next = {node[0] + vy, node[1] + vx, node[2]};
      res.emplace_back(next, 1);
    }
    return res;
  };
  aoc::gen_dijkstra(start, term_func, gen_neighbors, dist, prev);
  std::cout << dist[end_node] << std::endl;
}
