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
#include "hash.hpp"
#include "string_tools.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

int64_t sim(const std::vector<std::string> &orig,
            std::vector<std::string> &res) {
  int64_t count = 0;
  for (size_t row = 0; row < orig.size(); ++row) {
    for (size_t col = 0; col < orig[0].size(); ++col) {
      int neighbors = 0;
      if (row > 0) {
        if (col > 0) {
          // top left
          if (orig[row - 1][col - 1] == '#') {
            ++neighbors;
          }
        }
        if (col + 1 < orig[0].size()) {
          // top right
          if (orig[row - 1][col + 1] == '#') {
            ++neighbors;
          }
        }
        // top
        if (orig[row - 1][col] == '#') {
          ++neighbors;
        }
      }
      if (row + 1 < orig.size()) {
        if (col > 0) {
          // bottom left
          if (orig[row + 1][col - 1] == '#') {
            ++neighbors;
          }
        }
        if (col + 1 < orig[0].size()) {
          // bottom right
          if (orig[row + 1][col + 1] == '#') {
            ++neighbors;
          }
        }
        // bottom
        if (orig[row + 1][col] == '#') {
          ++neighbors;
        }
      }
      if (col > 0) {
        // left
        if (orig[row][col - 1] == '#') {
          ++neighbors;
        }
      }
      if (col + 1 < orig[0].size()) {
        // right
        if (orig[row][col + 1] == '#') {
          ++neighbors;
        }
      }
      if (orig[row][col] == '#') {
        if (neighbors == 2 || neighbors == 3) {
          ++count;
          res[row][col] = '#';
        } else {
          res[row][col] = '.';
        }
      } else {
        if (neighbors == 3) {
          ++count;
          res[row][col] = '#';
        } else {
          res[row][col] = '.';
        }
      }
    }
  }
  return count;
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;

  std::vector<std::string> board0;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    board0.emplace_back(line);
  }
  std::vector<std::string> board1 = board0;
  int64_t count = 0;
  for (size_t i = 0; i < 100; ++i) {
    count = sim(board0, board1);
    std::swap(board0, board1);
  }
  std::cout << count << std::endl;
}
