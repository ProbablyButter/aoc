/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"
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
  std::string line;
  using point_type = std::array<int64_t, 2>;
  std::unordered_map<std::array<int64_t, 2>, std::string, aoc::array_hasher>
      instrs;
  std::array<int64_t, 4> bounds = {0, 0, 1, 1};
  int64_t row = 0;
  int64_t col = 0;
  instrs.emplace(point_type({row, col}), "");
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line);
    auto dist = aoc::to_int<int64_t>(tmp[1]);
    for (size_t i = 0; i < dist; ++i) {
      switch (tmp[0][0]) {
      case 'R':
        ++col;
        break;
      case 'D':
        ++row;
        break;
      case 'L':
        --col;
        break;
      case 'U':
        --row;
        break;
      }
      bounds[0] = std::min<int64_t>(bounds[0], row);
      bounds[1] = std::min<int64_t>(bounds[1], col);
      bounds[2] = std::max<int64_t>(bounds[2], row + 1);
      bounds[3] = std::max<int64_t>(bounds[3], col + 1);
      instrs.emplace(point_type({row, col}),
                     std::string(tmp[2].substr(1, tmp[2].size() - 2)));
    }
  }
  std::vector<std::string> board(bounds[2] - bounds[0],
                                 std::string(bounds[3] - bounds[1], '?'));
  // dig boundaries
  for (auto &v : instrs) {
    board[v.first[0] - bounds[0]][v.first[1] - bounds[1]] = '#';
  }
  // fill in interior
  while (true) {
    bool any_changes = false;
    for (size_t row = 0; row < board.size(); ++row) {
      for (size_t col = 0; col < board[0].size(); ++col) {
        if (board[row][col] == '?') {
          if (row == 0 || col == 0 || row + 1 == board.size() ||
              col + 1 == board[0].size()) {
            board[row][col] = '.';
            any_changes = true;
            continue;
          }
          if (row > 0) {
            if (board[row - 1][col] == '.') {
              board[row][col] = '.';
              any_changes = true;
              continue;
            }
          }
          if (row + 1 < board.size()) {
            if (board[row + 1][col] == '.') {
              board[row][col] = '.';
              any_changes = true;
              continue;
            }
          }
          if (col > 0) {
            if (board[row][col - 1] == '.') {
              board[row][col] = '.';
              any_changes = true;
              continue;
            }
          }
          if (col + 1 < board[0].size()) {
            if (board[row][col + 1] == '.') {
              board[row][col] = '.';
              any_changes = true;
              continue;
            }
          }
        }
      }
    }
    if (!any_changes) {
      break;
    }
  }
  int64_t area = 0;
  for (auto &row : board) {
    for (auto &v : row) {
      if (v == '#' || v == '?') {
        ++area;
      }
    }
  }
  std::cout << area << std::endl;
}
