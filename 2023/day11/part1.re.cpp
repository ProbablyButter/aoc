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
  std::filesystem::path in_path = get_resource_path("input_small.txt");
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
  // expand the universe
  for (size_t row = 0; row < board.size(); ++row) {
    bool expand = true;
    for (size_t col = 0; col < board[row].size(); ++col) {
      if (board[row][col] == '#') {
        expand = false;
        break;
      }
    }
    if (expand) {
      board.insert(board.begin() + row, board[row]);
      ++row;
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
      for (size_t row = 0; row < board.size(); ++row) {
        board[row].insert(board[row].begin() + col, '.');
      }
      ++col;
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
  // build a graph
  int64_t dist_sum = 0;
  for (auto &v1 : galaxies) {
    int64_t row1 = v1 / board[0].size();
    int64_t col1 = v1 % board[0].size();
    for (auto &v2 : galaxies) {
      int64_t row2 = v2 / board[0].size();
      int64_t col2 = v2 % board[0].size();
      if (v1 < v2) {
        auto tmp =std::abs(row2 - row1) + std::abs(col2 - col1);
        std::cout << "(" << row1 << ", " << col1 << "), (" << row2 << ", "
                  << col2 << "): " << tmp << std::endl;
        dist_sum += tmp;
      }
    }
  }
  std::cout << dist_sum << std::endl;
}
