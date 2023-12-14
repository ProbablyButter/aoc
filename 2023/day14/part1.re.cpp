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
  for (int64_t row = 1; row < board.size(); ++row) {
    for (int64_t col = 0; col < board[0].size(); ++col) {
      if (board[row][col] == 'O') {
        // see if we can move this rock north
        bool move = false;
        int64_t i = row - 1;
        for (; i >= 0; --i) {
          if (board[i][col] != '.') {
            // can't move any more north
            break;
          }
          move = true;
        }
        if (move) {
          ++i;
          board[i][col] = 'O';
          board[row][col] = '.';
        }
      }
    }
  }
#if 0
  for (auto &v : board) {
    std::cout << v << std::endl;
  }
#endif
  // compute load
  int64_t load = 0;
  for (int64_t row = 0; row < board.size(); ++row) {
    for (int64_t col = 0; col < board[0].size(); ++col) {
      if (board[row][col] == 'O') {
        load += board.size() - row;
      }
    }
  }
  std::cout << load << std::endl;
}
