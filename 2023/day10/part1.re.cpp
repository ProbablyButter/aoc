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
  int64_t start_idx;
  int64_t idx = 0;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    board.emplace_back(line);
  }
  aoc::directed_graph<int64_t> graph;

  for (int64_t row = 0; row < board.size(); ++row) {
    for (int64_t col = 0; col < board[row].size(); ++col) {
      auto check_up = [&]() {
        if (row > 0) {
          char other_char = board[row - 1][col];
          bool allowed = false;
          switch (other_char) {
          case '|':
          case 'F':
          case '7':
          case 'S':
            allowed = true;
            break;
          default:
            break;
          }
          if (allowed) {
            int64_t other = (row - 1) * board[row].size() + col;
            graph.add_edge(idx, other, 1);
            graph.add_edge(other, idx, 1);
          }
        }
      };
      auto check_down = [&]() {
        if (row + 1 < board.size()) {
          char other_char = board[row + 1][col];
          bool allowed = false;
          switch (other_char) {
          case '|':
          case 'L':
          case 'J':
          case 'S':
            allowed = true;
            break;
          default:
            break;
          }
          if (allowed) {
            int64_t other = (row + 1) * board[row].size() + col;
            graph.add_edge(idx, other, 1);
            graph.add_edge(other, idx, 1);
          }
        }
      };

      auto check_left = [&]() {
        if (col > 0) {
          char other_char = board[row][col - 1];
          bool allowed = false;
          switch (other_char) {
          case '-':
          case 'L':
          case 'F':
          case 'S':
            allowed = true;
            break;
          default:
            break;
          }
          if (allowed) {
            int64_t other = row * board[row].size() + col - 1;
            graph.add_edge(idx, other, 1);
            graph.add_edge(other, idx, 1);
          }
        }
      };

      auto check_right = [&]() {
        if (col + 1 < board[row].size()) {
          char other_char = board[row][col + 1];
          bool allowed = false;
          switch (other_char) {
          case '-':
          case 'J':
          case '7':
          case 'S':
            allowed = true;
            break;
          default:
            break;
          }
          if (allowed) {
            int64_t other = row * board[row].size() + col + 1;
            graph.add_edge(idx, other, 1);
            graph.add_edge(other, idx, 1);
          }
        }
      };

      switch (board[row][col]) {
      case 'S':
        start_idx = idx;
        break;
      case '|': {
        check_up();
        check_down();
      } break;
      case '-': {
        check_left();
        check_right();
      } break;
      case 'L': {
        check_up();
        check_right();
      } break;
      case 'J': {
        check_up();
        check_left();
      } break;
      case '7': {
        check_down();
        check_left();
      } break;
      case 'F': {
        check_down();
        check_right();
      } break;
      default:
        break;
      }
      ++idx;
    }
  }
  std::unordered_map<size_t, int64_t> dists;
  std::unordered_map<size_t, size_t> prev;
  graph.dijkstra(start_idx, dists, prev);
  int64_t max_len = 0;
  std::vector<std::vector<int64_t>> all_dists;
  all_dists.resize(board.size(), std::vector<int64_t>(board[0].size(), -1));
  for (auto &v : dists) {
    max_len = std::max(v.second, max_len);
    int64_t row = v.first / board[0].size();
    int64_t col = v.first % board[0].size();
    all_dists[row][col] = v.second;
  }
  std::cout << max_len << std::endl;
}
