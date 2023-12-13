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

void find_reflection(const std::vector<std::string> &board, int64_t &row_,
                     int64_t &col_) {
  row_ = 0;
  col_ = 0;
  // look for vertical reflections
  bool found_reflection = false;
  // is mirror between [row,row+1]?
  for (int64_t row = 0; row + 1 < board.size(); ++row) {
    int64_t i = 0;
    bool reflect = true;
    while (true) {
      if (row - i < 0) {
        break;
      }
      if (row + i + 1 >= board.size()) {
        break;
      }
      if (board[row - i] != board[row + i + 1]) {
        reflect = false;
        break;
      }
      ++i;
    }
    if (reflect) {
      found_reflection = true;
      row_ = (row + 1);
      break;
    }
  }
  // look for horizontal reflections
  if (!found_reflection) {
    for (int64_t col = 0; col + 1 < board[0].size(); ++col) {
      int64_t i = 0;
      bool reflect = true;
      while (true) {
        if (col - i < 0) {
          break;
        }
        if (col + i + 1 >= board[0].size()) {
          break;
        }
        bool equal = true;
        for (int64_t row = 0; row < board.size(); ++row) {
          if (board[row][col - i] != board[row][col + i + 1]) {
            equal = false;
            break;
          }
        }
        if (!equal) {
          reflect = false;
          break;
        }
        ++i;
      }
      if (reflect) {
        found_reflection = true;
        col_ = (col + 1);
        break;
      }
    }
  }
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  std::vector<std::vector<std::string>> boards;
  boards.emplace_back();
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    if (line.empty()) {
      boards.emplace_back();
    } else {
      boards.back().emplace_back(line);
    }
  }
  size_t sum = 0;

  for (auto &board : boards) {
    int64_t row, col;
    find_reflection(board, row, col);
    sum += 100 * row + col;
  }
  std::cout << sum << std::endl;
}
