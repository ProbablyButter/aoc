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
  std::vector<std::string> board;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    board.emplace_back(line);
  }
  std::unordered_set<size_t> expanded_rows;
  std::unordered_set<size_t> expanded_cols;
  // expand the universe
  for (size_t row = 0; row < board.size(); ++row) {
    bool expand = true;
    for (size_t col = 0; col < board[0].size(); ++col) {
      if (board[row][col] == '#') {
        expand = false;
        break;
      }
    }
    if (expand) {
      expanded_rows.emplace(row);
    }
  }
  for (size_t col = 0; col < board[0].size(); ++col) {
    bool expand = true;
    for (size_t row = 0; row < board.size(); ++row) {
      if (board[row][col] == '#') {
        expand = false;
        break;
      }
    }
    if (expand) {
      expanded_cols.emplace(col);
    }
  }

  std::unordered_set<size_t> galaxies;
  for (size_t row = 0; row < board.size(); ++row) {
    for (size_t col = 0; col < board[0].size(); ++col) {
      if (board[row][col] == '#') {
        galaxies.emplace(row * board[0].size() + col);
      }
    }
  }

  constexpr size_t expand_factor = 1000000;
  int64_t dist_sum = 0;
  for (auto &v1 : galaxies) {
    for (auto &v2 : galaxies) {
      int64_t row1 = v1 / board[0].size();
      int64_t col1 = v1 % board[0].size();
      int64_t row2 = v2 / board[0].size();
      int64_t col2 = v2 % board[0].size();
      if (v1 < v2) {
        int64_t dist = 0;
        if (row1 > row2) {
          std::swap(row1, row2);
        }
        if (col1 > col2) {
          std::swap(col1, col2);
        }
        for (int64_t i = row1; i < row2; ++i) {
          if (expanded_rows.find(i) != expanded_rows.end()) {
            dist += expand_factor;
          } else {
            ++dist;
          }
        }

        for (int64_t i = col1; i < col2; ++i) {
          if (expanded_cols.find(i) != expanded_cols.end()) {
            dist += expand_factor;
          } else {
            ++dist;
          }
        }
        dist_sum += dist;
      }
    }
  }
  std::cout << dist_sum << std::endl;
}
