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

using node_type = std::array<int64_t, 3>;

void get_vx_vy(int dir, int &vx, int &vy) {
  switch (dir) {
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
}

void reverse_search(
    node_type &curr, std::vector<std::string_view> &grid,
    std::unordered_set<node_type, aoc::array_hasher> &searched,
    const std::unordered_map<node_type, int64_t, aoc::array_hasher> &dist,
    const std::unordered_map<node_type, node_type, aoc::array_hasher> &prev) {
  if (!searched.insert(curr).second) {
    return;
  }
  const_cast<char &>(grid[curr[0]][curr[1]]) = 'O';
  int64_t dist_rem = dist.at(curr);
  node_type pnode = curr;
  // do any neighbor nodes have a chance of matching dist_rem?
  // left turn
  pnode[2] = (curr[2] + 1) % 4;
  if (dist.at(pnode) + 1000 == dist_rem) {
    reverse_search(pnode, grid, searched, dist, prev);
  }
  // right turn
  pnode[2] = (curr[2] - 1);
  while (pnode[2] < 0) {
    pnode[2] += 4;
  }
  if (dist.at(pnode) + 1000 == dist_rem) {
    reverse_search(pnode, grid, searched, dist, prev);
  }
  // go forward
  int vx, vy;
  get_vx_vy(curr[2], vx, vy);
  pnode[0] = curr[0] - vy;
  pnode[1] = curr[1] - vx;
  pnode[2] = curr[2];
  if (grid[pnode[0]][pnode[1]] != '#' && dist.at(pnode) + 1 == dist_rem) {
    reverse_search(pnode, grid, searched, dist, prev);
  }
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string data(std::istreambuf_iterator<char>{in},
                   std::istreambuf_iterator<char>{});
  auto grid = aoc::split(data);
  // find the start
  node_type start, end_node;
  for (start[0] = 0; start[0] < grid.size(); ++start[0]) {
    for (start[1] = 0; start[1] < grid[start[0]].size(); ++start[1]) {
      if (grid[start[0]][start[1]] == 'S') {
        goto found_start;
      }
    }
  }
found_start:
  for (end_node[0] = 0; end_node[0] < grid.size(); ++end_node[0]) {
    for (end_node[1] = 0; end_node[1] < grid[end_node[0]].size();
         ++end_node[1]) {
      if (grid[end_node[0]][end_node[1]] == 'E') {
        goto found_end_node;
      }
    }
  }
found_end_node:
  // gen djikstra, but node ID is position and facing direction
  // 0: north
  // 1: east
  // 2: south
  // 3: west
  start[2] = 1;
  std::unordered_map<node_type, int64_t, aoc::array_hasher> dist;
  std::unordered_map<node_type, node_type, aoc::array_hasher> prev;
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
    get_vx_vy(node[2], vx, vy);
    if (grid[node[0] + vy][node[1] + vx] != '#') {
      // go straight
      node_type next = {node[0] + vy, node[1] + vx, node[2]};
      res.emplace_back(next, 1);
    }
    return res;
  };

  // first find the lowest cost to get to any position/direction
  {
    auto term_func = [&](const node_type &node) { return false; };
    aoc::gen_dijkstra(start, term_func, gen_neighbors, dist, prev);
  }
  int64_t min_cost = -1;
  for (int dir = 0; dir < 4; ++dir) {
    end_node[2] = dir;
    if (min_cost < 0 || dist[end_node] < min_cost) {
      min_cost = dist[end_node];
    }
  }
  // now do a reverse DFS of all paths which could lead to an end node and has
  // the same cost
  std::unordered_set<node_type, aoc::array_hasher> searched;
  for (int dir = 0; dir < 4; ++dir) {
    end_node[2] = dir;
    if (dist[end_node] == min_cost) {
      reverse_search(end_node, grid, searched, dist, prev);
    }
  }
  int count = 0;
  for (auto &row : grid) {
    for (auto c : row) {
      if (c == 'O') {
        ++count;
      }
    }
  }
  std::cout << count << std::endl;
}
