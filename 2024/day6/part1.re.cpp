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

template <class T, class U>
int simulate(T &grid, U &visit_map, int gx, int gy, int dir, int height,
             int width) {
  constexpr static int dxs[] = {0, 1, 0, -1};
  constexpr static int dys[] = {-1, 0, 1, 0};
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      visit_map[row][col] = 0;
    }
  }

  while (true) {
    if (visit_map[gy][gx] & (1 << dir)) {
      // loop
      return -1;
    }
    visit_map[gy][gx] |= (1 << dir);
    // try to move
    int tx, ty;
    while (true) {
      tx = gx + dxs[dir];
      ty = gy + dys[dir];
      if (tx < 0 || tx >= width || ty < 0 || ty >= height) {
        // left map
        goto done;
      }
      if (grid[ty][tx] == '#') {
        // obstacle, try to turn 90 degrees
        dir = (dir + 1) % 4;
      } else {
        break;
      }
    }
    gx = tx;
    gy = ty;
  }
done:;
  int count = 0;
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      count += (visit_map[row][col] != 0);
    }
  }
  return count;
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string data(std::istreambuf_iterator<char>{in},
                   std::istreambuf_iterator<char>{});
  auto grid = aoc::split(data);
  auto height = grid.size();
  auto width = grid[0].size();
  std::vector<std::vector<int>> visit_map;
  visit_map.resize(height, std::vector<int>(width));
  // find the initial guard position
  int gx = -1, gy = -1;
  int dir = 0;
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      switch (grid[row][col]) {
      case '^':
        gx = col;
        gy = row;
        dir = 0;

        goto found_guard;
      case '>':
        gx = col;
        gy = row;
        dir = 1;
        goto found_guard;
      case 'v':
        gx = col;
        gy = row;
        dir = 2;
        goto found_guard;
      case '<':
        gx = col;
        gy = row;
        dir = 3;
        goto found_guard;
      default:
        break;
      }
    }
  }
found_guard:

  int count = simulate(grid, visit_map, gx, gy, dir, height, width);
  std::cout << count << std::endl;
}
