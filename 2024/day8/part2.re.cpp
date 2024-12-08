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
  auto height = grid.size();
  auto width = grid[0].size();
  // identify all antenna frequencies and locations
  std::unordered_multimap<char, std::pair<int, int>> antenna;
  std::unordered_set<char> freqs;
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      if (grid[row][col] != '.') {
        antenna.emplace(grid[row][col], std::make_pair(row, col));
        freqs.emplace(grid[row][col]);
      }
    }
  }
  // identify antinodes
  std::unordered_set<std::tuple<int, int>, aoc::tuple_hasher> antinodes;
  for (auto freq : freqs) {
    auto a = antenna.equal_range(freq);
    for (auto iter = a.first; iter != a.second; ++iter) {
      auto other = iter;
      ++other;
      while (other != a.second) {
        auto row0 = iter->second.first;
        auto row1 = other->second.first;
        auto col0 = iter->second.second;
        auto col1 = other->second.second;
        auto drow = row0 - row1;
        auto dcol = col0 - col1;
        {
          auto trow = row0;
          auto tcol = col0;
          while (true) {
            antinodes.emplace(trow, tcol);
            trow += drow;
            tcol += dcol;
            if (!(trow >= 0 && trow < height && tcol >= 0 && tcol < width)) {
              break;
            }
          }
        }
        {
          auto trow = row1;
          auto tcol = col1;
          while (true) {
            antinodes.emplace(trow, tcol);
            trow -= drow;
            tcol -= dcol;
            if (!(trow >= 0 && trow < height && tcol >= 0 && tcol < width)) {
              break;
            }
          }
        }
        ++other;
      }
    }
  }
  std::cout << antinodes.size() << std::endl;
}
