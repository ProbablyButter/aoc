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
  std::vector<size_t> region_nodes;
  // maps node idx -> side idx
  std::unordered_map<size_t, size_t> top_sides;
  std::unordered_map<size_t, size_t> left_sides;
  std::unordered_map<size_t, size_t> right_sides;
  std::unordered_map<size_t, size_t> bottom_sides;
  std::vector<int> side_lengths;
  for (int region = 0; region < num_regions; ++region) {
    region_nodes.clear();
    side_lengths.clear();
    top_sides.clear();
    left_sides.clear();
    right_sides.clear();
    bottom_sides.clear();
    // find all cell in the region
    for (size_t i = 0; i < comps.size(); ++i) {
      auto c = comps[i];
      if (c == region) {
        region_nodes.emplace_back(i);
      }
    }
    long long area = region_nodes.size();
    std::sort(region_nodes.begin(), region_nodes.end());
    for (auto curr : region_nodes) {
      int row = curr / width;
      int col = curr % width;
      // top
      if (top_sides.find(curr) == top_sides.end()) {
        if (row == 0 || grid[row - 1][col] != grid[row][col]) {
          top_sides.emplace(curr, side_lengths.size());
          side_lengths.emplace_back(1);
          for (int c = col + 1; c < width; ++c) {
            if (grid[row][c] == grid[row][col] &&
                (row == 0 || grid[row - 1][c] != grid[row][col])) {
              // extend side
              top_sides.emplace(row * width + c, side_lengths.size() - 1);
              ++side_lengths.back();
            } else {
              break;
            }
          }
        }
      }
      // bottom
      if (bottom_sides.find(curr) == bottom_sides.end()) {
        if (row + 1 == height || grid[row + 1][col] != grid[row][col]) {
          bottom_sides.emplace(curr, side_lengths.size());
          side_lengths.emplace_back(1);
          for (int c = col + 1; c < width; ++c) {
            if (grid[row][c] == grid[row][col] &&
                (row + 1 == height || grid[row + 1][c] != grid[row][col])) {
              // extend side
              bottom_sides.emplace(row * width + c, side_lengths.size() - 1);
              ++side_lengths.back();
            } else {
              break;
            }
          }
        }
      }
      // left
      if (left_sides.find(curr) == left_sides.end()) {
        if (col == 0 || grid[row][col - 1] != grid[row][col]) {
          left_sides.emplace(curr, side_lengths.size());
          side_lengths.emplace_back(1);
          for (int r = row + 1; r < height; ++r) {
            if (grid[r][col] == grid[row][col] &&
                (col == 0 || grid[r][col - 1] != grid[row][col])) {
              // extend side
              left_sides.emplace(r * width + col, side_lengths.size() - 1);
              ++side_lengths.back();
            } else {
              break;
            }
          }
        }
      }
      // right
      if (right_sides.find(curr) == right_sides.end()) {
        if (col + 1 == width || grid[row][col + 1] != grid[row][col]) {
          right_sides.emplace(curr, side_lengths.size());
          side_lengths.emplace_back(1);
          for (int r = row + 1; r < height; ++r) {
            if (grid[r][col] == grid[row][col] &&
                (col + 1 == width || grid[r][col + 1] != grid[row][col])) {
              // extend side
              right_sides.emplace(r * width + col, side_lengths.size() - 1);
              ++side_lengths.back();
            } else {
              break;
            }
          }
        }
      }
    }
    cost += area * side_lengths.size();
  }
  std::cout << cost << std::endl;
}
