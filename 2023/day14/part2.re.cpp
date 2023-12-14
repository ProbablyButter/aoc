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
  int64_t num_cycles = 1000000000;
  // num_cycles = 100000;
  int64_t cycle = 0;
  std::vector<std::vector<std::string>> prior_cycles;
  int64_t match_cycle = 0;
  for (; cycle < num_cycles; ++cycle) {
    // tilt north
    prior_cycles.emplace_back(board);
    for (int64_t row = 1; row < board.size(); ++row) {
      for (int64_t col = 0; col < board[0].size(); ++col) {
        if (board[row][col] == 'O') {
          // see if we can move this rock
          bool move = false;
          int64_t i = row - 1;
          for (; i >= 0; --i) {
            if (board[i][col] != '.') {
              // can't move any more
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
    // tilt west
    for (int64_t col = 1; col < board[0].size(); ++col) {
      for (int64_t row = 0; row < board.size(); ++row) {
        if (board[row][col] == 'O') {
          // see if we can move this rock
          bool move = false;
          int64_t i = col - 1;
          for (; i >= 0; --i) {
            if (board[row][i] != '.') {
              // can't move any more
              break;
            }
            move = true;
          }
          if (move) {
            ++i;
            board[row][i] = 'O';
            board[row][col] = '.';
          }
        }
      }
    }
    // tilt south
    for (int64_t row = board.size() - 2; row >= 0; --row) {
      for (int64_t col = 0; col < board[0].size(); ++col) {
        if (board[row][col] == 'O') {
          // see if we can move this rock
          bool move = false;
          int64_t i = row + 1;
          for (; i < board.size(); ++i) {
            if (board[i][col] != '.') {
              // can't move any more
              break;
            }
            move = true;
          }
          if (move) {
            --i;
            board[i][col] = 'O';
            board[row][col] = '.';
          }
        }
      }
    }
    // tilt east
    for (int64_t col = board[0].size() - 2; col >= 0; --col) {
      for (int64_t row = 0; row < board.size(); ++row) {
        if (board[row][col] == 'O') {
          // see if we can move this rock
          bool move = false;
          int64_t i = col + 1;
          for (; i < board[0].size(); ++i) {
            if (board[row][i] != '.') {
              // can't move any more
              break;
            }
            move = true;
          }
          if (move) {
            --i;
            board[row][i] = 'O';
            board[row][col] = '.';
          }
        }
      }
    }
    bool done = false;
    for (size_t j = 0; j < prior_cycles.size(); ++j) {
      auto &pre_cycle = prior_cycles[j];
      bool matches = true;
      for (size_t i = 0; i < board.size(); ++i) {
        if (board[i] != pre_cycle[i]) {
          matches = false;
          break;
        }
      }
      if (matches) {
        match_cycle = j;
        done = true;
        break;
      }
    }
    if (done) {
      break;
    }
  }
  int64_t period = cycle - match_cycle + 1;
  // num_cycles = period * N + match_cycle + offset
  int64_t offset = 0;
  for (; offset < period; ++offset) {
    int64_t i = num_cycles - match_cycle - offset;
    int64_t rem = i % period;
    // don't actually need to compute N, just need offset

    if (rem == 0) {
      break;
    }
  }
  // compute load
  int64_t load = 0;
  board = prior_cycles[match_cycle + offset];
  for (int64_t row = 0; row < board.size(); ++row) {
    for (int64_t col = 0; col < board[0].size(); ++col) {
      if (board[row][col] == 'O') {
        load += board.size() - row;
      }
    }
  }
  std::cout << load << std::endl;
}
