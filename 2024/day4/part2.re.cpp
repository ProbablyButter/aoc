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
  // read in entire file
  std::string data(std::istreambuf_iterator<char>{in},
                   std::istreambuf_iterator<char>{});
  auto grid = aoc::split(data);
  std::string match = "MAS";
  int count = 0;
  for (int row = 1; row < grid.size() - 1; ++row) {
    for (int col = 1; col < grid[row].size() - 1; ++col) {
      // look up-left
      int add = 0;
      add += 1;
      for (int i = -1; i <= 1; ++i) {
        if (grid[row - i][col - i] != match[i + 1]) {
          add -= 1;
          break;
        }
      }
      // look up-right
      add += 1;
      for (int i = -1; i <= 1; ++i) {
        if (grid[row - i][col + i] != match[i + 1]) {
          add -= 1;
          break;
        }
      }
      // look down-right
      add += 1;
      for (int i = -1; i <= 1; ++i) {
        if (grid[row + i][col + i] != match[i + 1]) {
          add -= 1;
          break;
        }
      }
      // look down-left
      add += 1;
      for (int i = -1; i <= 1; ++i) {
        if (grid[row + i][col - i] != match[i + 1]) {
          add -= 1;
          break;
        }
      }
      if (add == 2) {
        ++count;
      }
    }
  }
  std::cout << count << std::endl;
}
