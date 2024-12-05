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
  std::string match = "XMAS";
  int count = 0;
  for (int row = 0; row < grid.size(); ++row) {
    for (int col = 0; col < grid[row].size(); ++col) {
      // look right
      int add = 1;
      for (int i = 0; i < match.size(); ++i) {
        if (col + i < grid[row].size()) {
          if (grid[row][col + i] != match[i]) {
            add = 0;
            break;
          }
        } else {
          add = 0;
          break;
        }
      }
      count += add;
      // look left
      add = 1;
      for (int i = 0; i < match.size(); ++i) {
        if (col - i >= 0) {
          if (grid[row][col - i] != match[i]) {
            add = 0;
            break;
          }
        } else {
          add = 0;
          break;
        }
      }
      count += add;
      // look up
      add = 1;
      for (int i = 0; i < match.size(); ++i) {
        if (row - i >= 0) {
          if (grid[row - i][col] != match[i]) {
            add = 0;
            break;
          }
        } else {
          add = 0;
          break;
        }
      }
      count += add;
      // look down
      add = 1;
      for (int i = 0; i < match.size(); ++i) {
        if (row + i < grid.size()) {
          if (grid[row + i][col] != match[i]) {
            add = 0;
            break;
          }
        } else {
          add = 0;
          break;
        }
      }
      count += add;
      // look up-left
      add = 1;
      for (int i = 0; i < match.size(); ++i) {
        if (row - i >= 0 && col - i >= 0) {
          if (grid[row - i][col - i] != match[i]) {
            add = 0;
            break;
          }
        } else {
          add = 0;
          break;
        }
      }
      count += add;
      // look up-right
      add = 1;
      for (int i = 0; i < match.size(); ++i) {
        if (row - i >= 0 && col + i < grid[row - i].size()) {
          if (grid[row - i][col + i] != match[i]) {
            add = 0;
            break;
          }
        } else {
          add = 0;
          break;
        }
      }
      count += add;
      // look down-right
      add = 1;
      for (int i = 0; i < match.size(); ++i) {
        if (row + i < grid.size() && col + i < grid[row + i].size()) {
          if (grid[row + i][col + i] != match[i]) {
            add = 0;
            break;
          }
        } else {
          add = 0;
          break;
        }
      }
      count += add;
      // look down-left
      add = 1;
      for (int i = 0; i < match.size(); ++i) {
        if (row + i < grid.size() && col - i >= 0) {
          if (grid[row + i][col - i] != match[i]) {
            add = 0;
            break;
          }
        } else {
          add = 0;
          break;
        }
      }
      count += add;
    }
  }
  std::cout << count << std::endl;
}
