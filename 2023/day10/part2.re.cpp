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
  int64_t furthest_node = -1;
  for (auto &v : dists) {
    if (v.second > max_len) {
      max_len = v.second;
      furthest_node = v.first;
    }
  }
  // follow the furthest node back to start
  std::vector<size_t> path;
  std::unordered_set<size_t> path_nodes;
  path.emplace_back(furthest_node);
  while (path.back() != start_idx) {
    // backtrack
    path.emplace_back(prev.at(path.back()));
    path_nodes.emplace(path.back());
  }
  // now continue until we get back to furthest node
  {
    size_t curr_idx = start_idx;
    while (path.back() != furthest_node) {
      // what is connected to start_idx?
      for (auto &v : graph.connectivity.at(path.back())) {
        auto res = path_nodes.emplace(v.first);
        if (res.second) {
          // this is the right direction to follow
          path.emplace_back(v.first);
          break;
        }
        if (v.first == furthest_node) {
          // done
          path.emplace_back(v.first);
          goto done;
        }
      }
    }
  done:;
  }
  // replace S with an appropriate character
  {
    std::vector<size_t> tmp;
    for (auto &v : graph.connectivity.at(start_idx)) {
      if (path_nodes.find(v.first) != path_nodes.end()) {
        tmp.push_back(v.first);
      }
    }
    std::sort(tmp.begin(), tmp.end());
    int64_t start_row = start_idx / board[0].size();
    int64_t start_col = start_idx % board[0].size();

    if (tmp[0] == start_idx + 1) {
      // tmp[0] to the right
      if (tmp[1] == start_idx + 1) {
        // tmp[1] to the right
      } else if (tmp[1] == start_idx - 1) {
        // tmp[1] to the left
        board[start_row][start_col] = '-';
      } else if (tmp[1] == start_idx - board[0].size()) {
        // tmp[1] above
        board[start_row][start_col] = 'J';
      } else if (tmp[1] == start_idx + board[0].size()) {
        // tmp[1] below
        board[start_row][start_col] = 'F';
      }
    } else if (tmp[0] == start_idx - 1) {
      // tmp[0] to the left
      if (tmp[1] == start_idx + 1) {
        // tmp[1] to the right
        board[start_row][start_col] = '-';
      } else if (tmp[1] == start_idx - 1) {
        // tmp[1] to the left
      } else if (tmp[1] == start_idx - board[0].size()) {
        // tmp[1] above
        board[start_row][start_col] = 'J';
      } else if (tmp[1] == start_idx + board[0].size()) {
        // tmp[1] below
        board[start_row][start_col] = '7';
      }
    } else if (tmp[0] == start_idx - board[0].size()) {
      // tmp[0] above
      if (tmp[1] == start_idx + 1) {
        // tmp[1] to the right
        board[start_row][start_col] = 'L';
      } else if (tmp[1] == start_idx - 1) {
        // tmp[1] to the left
        board[start_row][start_col] = 'J';
      } else if (tmp[1] == start_idx - board[0].size()) {
        // tmp[1] above
      } else if (tmp[1] == start_idx + board[0].size()) {
        // tmp[1] below
        board[start_row][start_col] = '|';
      }
    } else if (tmp[0] == start_idx + board[0].size()) {
      // tmp[0] below
      if (tmp[1] == start_idx + 1) {
        // tmp[1] to the right
        board[start_row][start_col] = 'F';
      } else if (tmp[1] == start_idx - 1) {
        // tmp[1] to the left
        board[start_row][start_col] = '7';
      } else if (tmp[1] == start_idx - board[0].size()) {
        // tmp[1] above
        board[start_row][start_col] = '|';
      } else if (tmp[1] == start_idx + board[0].size()) {
        // tmp[1] below
      }
    }
  }

  int64_t inside_count = 0;
  std::vector<std::pair<int64_t, int64_t>> insides;
  std::vector<std::pair<int64_t, int64_t>> outsides;
  for (int64_t row = 0; row < board.size(); ++row) {
    for (int64_t col = 0; col < board[0].size(); ++col) {
      if (path_nodes.find(row * board[0].size() + col) == path_nodes.end()) {
        // determine if this is inside or outside
        int winding = 0;
        int64_t i = col + 1;
        while (i < board[0].size()) {
          int64_t idx = row * board[0].size() + i;
          if (path_nodes.find(idx) != path_nodes.end()) {
            switch (board[row][i]) {
            case 'F': {
              while (true) {
                ++i;
                // assume we will find a matching end
                int64_t idx2 = row * board[0].size() + i;
                if (path_nodes.find(idx2) != path_nodes.end()) {
                  switch (board[row][i]) {
                  case 'J':
                    ++winding;
                    goto termF;
                  case '7':
                    goto termF;
                  case 'F':
                  case 'L':
                  case '|':
                    throw std::runtime_error("impossible");
                  default:
                    break;
                  }
                }
              }
            termF:
              ++i;
            } break;
            case 'L': {
              while (true) {
                ++i;
                // assume we will find a matching end
                int64_t idx2 = row * board[0].size() + i;
                if (path_nodes.find(idx2) != path_nodes.end()) {
                  switch (board[row][i]) {
                  case '7':
                    ++winding;
                    goto termL;
                  case 'J':
                    goto termL;
                  case 'F':
                  case 'L':
                  case '|':
                    throw std::runtime_error("impossible");
                  default:
                    break;
                  }
                }
              }
            termL:

              ++i;
            } break;
            case 'J':
            case '7':
              std::cout << row << ", " << col << ": " << i << std::endl;
              throw std::runtime_error("impossible first");
            case '|':
              ++winding;
            default:
              ++i;
              break;
            }
          } else {
            ++i;
          }
        }
        if (winding % 2 == 1) {
          // inside
          board[row][col] = 'I';
          insides.emplace_back(row, col);
          ++inside_count;
        } else {
          outsides.emplace_back(row, col);
          board[row][col] = 'O';
        }
      } else {
        int64_t idx = row * board[0].size() + col;
        if (path_nodes.find(idx) == path_nodes.end()) {
          board[row][col] = 'x';
        }
      }
    }
  }
#if 0
  std::ofstream out("path.svg");
  out << "<?xml version=\"1.0\" standalone=\"no\"?>" << std::endl;
  out << "<svg>" << std::endl;
  out << "<path d=\""
      << "M " << 10 * (path[0] % board[0].size()) << " "
      << 10 * (path[0] / board[0].size());
  for (size_t i = 1; i < path.size(); ++i) {
    int64_t row1 = path[i] / board[0].size();
    int64_t col1 = path[i] % board[0].size();
    out << " L " << 10 * col1 << " " << 10 * row1;
  }
  out << " z\" fill=\"red\" stroke=\"blue\" stroke-width=\"1\"/>" << std::endl;
  for (auto &v : insides) {
    out << "<circle r=\"1\" cx=\"" << 10 * v.second << "\" cy=\""
        << 10 * v.first << "\""
        << " stroke=\"black\"/>" << std::endl;
  }
  for (auto &v : outsides) {
    out << "<circle r=\"1\" cx=\"" << 10 * v.second << "\" cy=\""
        << 10 * v.first << "\""
        << " stroke=\"red\"/>" << std::endl;
  }

  for (int64_t row = 0; row < board.size(); ++row) {
    for (int64_t col = 0; col < board[0].size(); ++col) {
      if (board[row][col] == 'x') {
        out << "<circle r=\"1\" cx=\"" << 10 * col << "\" cy=\""
            << 10 * row << "\""
            << " stroke=\"orange\"/>" << std::endl;
      }
    }
  }
  out << "</svg>" << std::endl;
#endif

  std::cout << inside_count << std::endl;
}
