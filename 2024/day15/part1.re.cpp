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
  std::string line;
  std::vector<std::string> grid;
  std::string instrs;
  bool read_instrs = false;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    if (read_instrs) {
      instrs += line;
    } else {
      if (line.empty()) {
        read_instrs = true;
      } else {
        grid.emplace_back(line);
      }
    }
  }
  int pos_x = 0, pos_y = 0;
  int height = grid.size();
  int width = grid[0].size();
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      if (grid[row][col] == '@') {
        pos_x = col;
        pos_y = row;
        grid[row][col] = '.';
        goto found;
      }
    }
  }
found:
  for (auto i : instrs) {
    int vx, vy;
    switch (i) {
    case '>':
      vx = 1;
      vy = 0;
      break;
    case '<':
      vx = -1;
      vy = 0;
      break;
    case '^':
      vx = 0;
      vy = -1;
      break;
    case 'v':
      vx = 0;
      vy = 1;
      break;
    }
    // try to move
    int n = 1;
    int test_x = pos_x + vx;
    int test_y = pos_y + vy;
    while (true) {

      switch (grid[test_y][test_x]) {
      case '#':
        // can't move
        goto next_instr;
      case '.':
        // can move
        {
          for (int i = 1; i < n; ++i) {
            grid[pos_y + vy * (i + 1)][pos_x + vx * (i + 1)] = 'O';
          }
          pos_x += vx;
          pos_y += vy;
          grid[pos_y][pos_x] = '.';
        }
        goto next_instr;
      case 'O':
        // try to move, need to check next spot
        test_x += vx;
        test_y += vy;
        ++n;
        break;
      default:
        throw std::runtime_error("unknown character");
        break;
      }
    }
  next_instr:;
  }
  int gps = 0;
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      if (grid[row][col] == 'O') {
        gps += row * 100 + col;
      }
    }
  }
  std::cout << gps << std::endl;
}
