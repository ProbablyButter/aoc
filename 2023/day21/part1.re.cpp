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
  // maps a gate to its outputs
  std::vector<std::string> board;
  std::unordered_set<std::pair<int64_t, int64_t>, aoc::pair_hasher> pos;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    board.emplace_back(line);
  }
  for (size_t row = 0; row < board.size(); ++row) {
    for (size_t col = 0; col < board[0].size(); ++col) {
      if (board[row][col] == 'S') {
        pos.emplace(row, col);
      }
    }
  }
  std::unordered_set<std::pair<int64_t, int64_t>, aoc::pair_hasher> next;
  for (size_t step = 0; step < 26501365; ++step) {
    next.clear();
    for (auto &v : pos) {
      int64_t row = v.first;
      int64_t col = v.second;
      // left
      if (col > 0) {
        if (board[row][col - 1] != '#') {
          // can move
          next.emplace(row, col - 1);
        }
      }
      // right
      if (col + 1 < board[0].size()) {
        if (board[row][col + 1] != '#') {
          // can move
          next.emplace(row, col + 1);
        }
      }
      // up
      if (row > 0) {
        if (board[row - 1][col] != '#') {
          // can move
          next.emplace(row - 1, col);
        }
      }
      // down
      if (row + 1 < board.size()) {
        if (board[row + 1][col] != '#') {
          // can move
          next.emplace(row + 1, col);
        }
      }
    }
    std::swap(pos, next);
  }
  std::cout << pos.size() << std::endl;
}
