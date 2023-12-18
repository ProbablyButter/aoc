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
  using node_type = std::array<int64_t, 2 + 10>;
  node_type term_node = {0};
  auto term_func = [&](const node_type &node) {
    if (node[0] + 1 == board.size() && node[1] + 1 == board[0].size()) {
      for (size_t i = node.size() - 4; i < node.size(); ++i) {
        if (node[i] != node[node.size() - 4]) {
          // need minimum of 4 straight movements to end
          return false;
        }
      }
      term_node = node;
      return true;
    }
    return false;
  };
  auto gen_neighbors = [&](const node_type &node) {
    std::vector<std::pair<node_type, int64_t>> neighbors;

    auto avail_test = [&](int64_t dir, int64_t rdir) {
      if (node.back() == rdir) {
        return false;
      }
      bool can = false;
      // straight line test
      for (size_t i = 2; i < node.size(); ++i) {
        if (node[i] != dir) {
          can = true;
          break;
        }
      }
      if (!can) {
        return false;
      } else {
        if (dir != node.back()) {
          bool can2 = true;
          for (size_t i = node.size() - 4; i < node.size(); ++i) {
            // has the node gone 4 tiles in the same direction?
            if (node[i] != node[node.size() - 4]) {
              return false;
            }
          }
        }
      }
      return true;
    };
    // up
    if (avail_test(1, 2)) {
      node_type neighbor = node;
      --neighbor[0];
      if (neighbor[0] >= 0) {
        for (size_t i = 2 + 1; i < neighbor.size(); ++i) {
          neighbor[i - 1] = neighbor[i];
        }
        neighbor.back() = 1;
        neighbors.emplace_back(neighbor, board[neighbor[0]][neighbor[1]] - '0');
      }
    }
    // down
    if (avail_test(2, 1)) {
      node_type neighbor = node;
      ++neighbor[0];
      if (neighbor[0] < board.size()) {
        for (size_t i = 2 + 1; i < neighbor.size(); ++i) {
          neighbor[i - 1] = neighbor[i];
        }
        neighbor.back() = 2;
        neighbors.emplace_back(neighbor, board[neighbor[0]][neighbor[1]] - '0');
      }
    }
    // left
    if (avail_test(3, 4)) {
      node_type neighbor = node;
      --neighbor[1];
      if (neighbor[1] >= 0) {
        for (size_t i = 2 + 1; i < neighbor.size(); ++i) {
          neighbor[i - 1] = neighbor[i];
        }
        neighbor.back() = 3;
        neighbors.emplace_back(neighbor, board[neighbor[0]][neighbor[1]] - '0');
      }
    }
    // right
    if (avail_test(4, 3)) {
      node_type neighbor = node;
      ++neighbor[1];
      if (neighbor[1] < board[0].size()) {
        for (size_t i = 2 + 1; i < neighbor.size(); ++i) {
          neighbor[i - 1] = neighbor[i];
        }
        neighbor.back() = 4;
        neighbors.emplace_back(neighbor, board[neighbor[0]][neighbor[1]] - '0');
      }
    }
    return neighbors;
  };
  auto node_cmp = [&](const node_type &a, const node_type &b) {
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
  };
  node_type start = {0};
  for (auto &v : start) {
    v = 0;
  }
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
    for (auto &c : v) {
      if (c >= '0' && c <= '9') {
        c = '.';
      }
    }
    std::cout << v << std::endl;
  }
#endif
}
