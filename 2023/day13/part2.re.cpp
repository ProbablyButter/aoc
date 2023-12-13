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
                     int64_t &col_, int64_t ignore_row = -1,
                     int64_t ignore_col = -1) {
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
    if (reflect && row != ignore_row) {
      found_reflection = true;
      row_ = (row + 1);
      break;
    }
  }
  // look for horizontal reflections
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
    if (reflect && col != ignore_col) {
      found_reflection = true;
      col_ = (col + 1);
      break;
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
    int64_t orig_row, orig_col;
    int64_t new_row, new_col;
    find_reflection(board, orig_row, orig_col);
    for (int64_t srow = 0; srow < board.size(); ++srow) {
      for (int64_t scol = 0; scol < board[0].size(); ++scol) {
        auto tmp = board;
        // smudge the board
        if (tmp[srow][scol] == '.') {
          tmp[srow][scol] = '#';
        } else {
          tmp[srow][scol] = '.';
        }
        find_reflection(tmp, new_row, new_col, orig_row - 1, orig_col - 1);
        if (new_row == orig_row) {
          new_row = 0;
        }
        if (new_col == orig_col) {
          new_col = 0;
        }
        if (new_row + new_col != 0) {
          goto done;
        }
      }
    }
    for (auto &v : board) {
      std::cout << v << std::endl;
    }
    throw std::runtime_error("shouldn't be here");
  done:
    sum += 100 * new_row + new_col;
  }
  std::cout << sum << std::endl;
}
