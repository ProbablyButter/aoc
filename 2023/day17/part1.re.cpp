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

// history has coord + previous 3 directions
void depth_first_search(
    const std::vector<std::string> &board, std::pair<int64_t, int64_t> curr,
    std::array<int64_t, 4> prev_dirs, std::vector<std::vector<int64_t>> &dists,
    std::unordered_map<std::array<int64_t, 2 + 3>, int64_t, aoc::array_hasher>
        &history,
    std::unordered_set<std::pair<int64_t, int64_t>, aoc::pair_hasher> &visited,
    int64_t curr_len) {
  // returns true if last 4 entries of path is valid
  auto check_dir = [&]() {
    if (prev_dirs.back() == 0) {
      return true;
    }
    auto d0 = prev_dirs[0];
    for (size_t i = 1; i < prev_dirs.size(); ++i) {
      if (prev_dirs[i] != d0) {
        return true;
      }
    }
    return false;
  };

  if (prev_dirs.back() != 0) {
    for (size_t i = 1; i < prev_dirs.size(); ++i) {
      prev_dirs[i - 1] = prev_dirs[i];
    }
  }
  {
    if (dists[curr.first][curr.second] < 0 ||
        curr_len < dists[curr.first][curr.second]) {
      dists[curr.first][curr.second] = curr_len;
    }
    std::array<int64_t, 2 + 3> p = {curr.first, curr.second, prev_dirs[0],
                                    prev_dirs[1], prev_dirs[2]};
    auto res = history.emplace(p, curr_len);
    if (!res.second) {
      if (curr_len < res.first->second) {
        res.first->second = curr_len;
      } else {
        return;
      }
    }
  }

  if (curr.first + 1 == board.size() && curr.second + 1 == board[0].size()) {
    // reached the end
    return;
  }
  // down
  {
    auto pos = curr;
    ++pos.first;
    if (pos.first < board.size()) {
      auto iter = visited.emplace(pos);
      if (iter.second) {
        if (prev_dirs.back() == 0) {
          prev_dirs[visited.size() - 2] = 1;
        } else {
          prev_dirs.back() = 1;
        }
        if (!check_dir()) {
          visited.erase(iter.first);
        } else {
          depth_first_search(board, pos, prev_dirs, dists, history, visited,
                             curr_len + (board[pos.first][pos.second] - '0'));
          visited.erase(pos);
        }
      }
    }
  }
  // right
  {
    auto pos = curr;
    ++pos.second;
    if (pos.second < board[0].size()) {
      auto iter = visited.emplace(pos);
      if (iter.second) {
        if (prev_dirs.back() == 0) {
          prev_dirs[visited.size() - 2] = 2;
        } else {
          prev_dirs.back() = 2;
        }
        if (!check_dir()) {
          visited.erase(iter.first);
        } else {
          depth_first_search(board, pos, prev_dirs, dists, history, visited,
                             curr_len + (board[pos.first][pos.second] - '0'));
          visited.erase(pos);
        }
      }
    }
  }
  // up
  {
    auto pos = curr;
    --pos.first;
    if (pos.first >= 0) {
      auto iter = visited.emplace(pos);
      if (iter.second) {
        if (prev_dirs.back() == 0) {
          prev_dirs[visited.size() - 2] = 3;
        } else {
          prev_dirs.back() = 3;
        }
        if (!check_dir()) {
          visited.erase(iter.first);
        } else {
          depth_first_search(board, pos, prev_dirs, dists, history, visited,
                             curr_len + (board[pos.first][pos.second] - '0'));
          visited.erase(pos);
        }
      }
    }
  }
  // left
  {
    auto pos = curr;
    --pos.second;
    if (pos.second >= 0) {
      auto iter = visited.emplace(pos);
      if (iter.second) {
        if (prev_dirs.back() == 0) {
          prev_dirs[visited.size() - 2] = 4;
        } else {
          prev_dirs.back() = 4;
        }
        if (!check_dir()) {
          visited.erase(iter.first);
        } else {
          depth_first_search(board, pos, prev_dirs, dists, history, visited,
                             curr_len + (board[pos.first][pos.second] - '0'));
          visited.erase(pos);
        }
      }
    }
  }
}

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
  using node_type = std::array<int64_t, 2 + 3>;
  node_type term_node;
  auto term_func = [&](const node_type &node) {
    if (node[0] + 1 == board.size() && node[1] + 1 == board[0].size()) {
      term_node = node;
      return true;
    }
    return false;
  };
  auto gen_neighbors = [&](const node_type &node) {
    // TODO
    std::vector<std::pair<node_type, int64_t>> neighbors;
    // up
    if (node[2] == node[3] && node[3] == node[4] && node[4] == 1) {
    } else {
      if (node[4] != 2) {
        node_type neighbor = node;
        --neighbor[0];
        if (neighbor[0] >= 0) {
          for (size_t i = 2 + 1; i < neighbor.size(); ++i) {
            neighbor[i - 1] = neighbor[i];
          }
          neighbor.back() = 1;
          neighbors.emplace_back(neighbor,
                                 board[neighbor[0]][neighbor[1]] - '0');
        }
      }
    }
    // down
    if (node[2] == node[3] && node[3] == node[4] && node[4] == 2) {
    } else {
      if (node[4] != 1) {
        node_type neighbor = node;
        ++neighbor[0];
        if (neighbor[0] < board.size()) {
          for (size_t i = 2 + 1; i < neighbor.size(); ++i) {
            neighbor[i - 1] = neighbor[i];
          }
          neighbor.back() = 2;
          neighbors.emplace_back(neighbor,
                                 board[neighbor[0]][neighbor[1]] - '0');
        }
      }
    }
    // left
    if (node[2] == node[3] && node[3] == node[4] && node[4] == 3) {
    } else {
      if (node[4] != 4) {
        node_type neighbor = node;
        --neighbor[1];
        if (neighbor[1] >= 0) {
          for (size_t i = 2 + 1; i < neighbor.size(); ++i) {
            neighbor[i - 1] = neighbor[i];
          }
          neighbor.back() = 3;
          neighbors.emplace_back(neighbor,
                                 board[neighbor[0]][neighbor[1]] - '0');
        }
      }
    }
    // right
    if (node[2] == node[3] && node[3] == node[4] && node[4] == 4) {
    } else {
      if (node[4] != 3) {
        node_type neighbor = node;
        ++neighbor[1];
        if (neighbor[1] < board[0].size()) {
          for (size_t i = 2 + 1; i < neighbor.size(); ++i) {
            neighbor[i - 1] = neighbor[i];
          }
          neighbor.back() = 4;
          neighbors.emplace_back(neighbor,
                                 board[neighbor[0]][neighbor[1]] - '0');
        }
      }
    }
    return neighbors;
  };
  auto node_cmp = [&](const node_type &a, const node_type &b) {
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
  };
  node_type start = {0, 0, 0, 0, 0};
  std::unordered_map<node_type, int64_t, aoc::array_hasher> dists;
  std::unordered_map<node_type, node_type, aoc::array_hasher> prevs;
  gen_dijkstra(start, term_func, gen_neighbors, dists, prevs, node_cmp);
  std::cout << dists.at(term_node) << std::endl;
#if 0
  for (auto &v : term_node) {
    std::cout << v << ", ";
  }
  std::cout << std::endl;
  {
    auto curr = term_node;
    board[curr[0]][curr[1]] = 'x';
    while (curr != start) {
      switch (curr.back()) {
      case 1:
        board[curr[0]][curr[1]] = '^';
        break;
      case 2:
        board[curr[0]][curr[1]] = 'v';
        break;
      case 3:
        board[curr[0]][curr[1]] = '<';
        break;
      case 4:
        board[curr[0]][curr[1]] = '>';
        break;
      }
      curr = prevs.at(curr);

      for (auto &v : curr) {
        std::cout << v << ", ";
      }
      std::cout << std::endl;
    }
  }
  for (auto &v : board) {
    std::cout << v << std::endl;
  }
#endif
}
