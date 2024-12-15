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
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      int src = row * width + col;
      graph.ensure_node(src);
      // left
      if (col - 1 >= 0) {
        if (grid[row][col] == grid[row][col - 1]) {
          int dst = row * width + (col - 1);
          graph.add_edge(src, dst, 1);
          graph.add_edge(dst, src, 1);
        }
      }
      // right
      if (col + 1 < width) {
        if (grid[row][col] == grid[row][col + 1]) {
          int dst = row * width + (col + 1);
          graph.add_edge(src, dst, 1);
          graph.add_edge(dst, src, 1);
        }
      }
      // up
      if (row - 1 >= 0) {
        if (grid[row][col] == grid[row - 1][col]) {
          int dst = (row - 1) * width + col;
          graph.add_edge(src, dst, 1);
          graph.add_edge(dst, src, 1);
        }
      }
      // down
      if (row + 1 < height) {
        if (grid[row][col] == grid[row + 1][col]) {
          int dst = (row + 1) * width + col;
          graph.add_edge(src, dst, 1);
          graph.add_edge(dst, src, 1);
        }
      }
    }
  }
  size_t num_regions;
  auto comps = graph.strongly_connected_components(num_regions);
  long long cost = 0;
  std::unordered_map<size_t, int> dists;
  std::unordered_map<size_t, size_t> prev;
  std::unordered_set<size_t> visited;
  for (int region = 0; region < num_regions; ++region) {
    visited.clear();
    long long area = 0;
    long long perimeter = 0;
    // find any cell in the region
    for (size_t i = 0; i < comps.size(); ++i) {
      auto c = comps[i];
      if (c == region) {
        graph.dijkstra(i, dists, prev, [&](size_t n) {
          if (visited.emplace(n).second) {
            // only update if this is our first visit
            ++area;
            perimeter += 4 - graph.connectivity[n].size();
          }
          return false;
        });
        break;
      }
    }
    cost += area * perimeter;
  }
  std::cout << cost << std::endl;
}
