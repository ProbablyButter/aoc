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

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string data(std::istreambuf_iterator<char>{in},
                   std::istreambuf_iterator<char>{});

  auto grid = aoc::split(data);
  int height = grid.size();
  int width = grid[0].size();

  aoc::directed_graph<int> graph;
  std::vector<std::pair<int, int>> starts;
  std::vector<std::pair<int, int>> stops;
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      if (grid[row][col] == '0') {
        starts.emplace_back(row, col);
      } else if (grid[row][col] == '9') {
        stops.emplace_back(row, col);
      }
      // our node can go to any other node that is exactly 1 higher than our
      // current node
      // left
      if (col - 1 >= 0) {
        if (grid[row][col - 1] - grid[row][col] == 1) {
          graph.add_edge(row * width + col, row * width + (col - 1), 1);
        }
      }
      // right
      if (col + 1 < width) {
        if (grid[row][col + 1] - grid[row][col] == 1) {
          graph.add_edge(row * width + col, row * width + (col + 1), 1);
        }
      }
      // up
      if (row - 1 >= 0) {
        if (grid[row - 1][col] - grid[row][col] == 1) {
          graph.add_edge(row * width + col, (row - 1) * width + col, 1);
        }
      }
      // down
      if (row + 1 < height) {
        if (grid[row + 1][col] - grid[row][col] == 1) {
          graph.add_edge(row * width + col, (row + 1) * width + col, 1);
        }
      }
    }
  }
  std::cout << "finding paths" << std::endl;
  std::unordered_map<size_t, int> dists;
  std::unordered_map<size_t, size_t> prev;
  int count = 0;
  for (auto &start : starts) {
    // every node reachable from start
    graph.dijkstra(start.first * width + start.second, dists, prev);
    for (auto &stop : stops) {
      size_t stop_idx = stop.first * width + stop.second;
      if (prev.find(stop_idx) != prev.end()) {
        ++count;
      }
    }
  }
  std::cout << count << std::endl;
}
