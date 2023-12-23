/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"
#include "geometry_tools.hpp"
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

using node_type = std::array<int64_t, 2>;

void search(
    std::vector<node_type> &path,
    std::unordered_set<node_type, aoc::array_hasher> &visited,
    const std::vector<std::string> &board,
    std::unordered_map<node_type, int64_t, aoc::array_hasher> &max_dists) {
  auto gen_neighbors = [&](const node_type &curr) {
    std::vector<std::pair<node_type, int64_t>> res;
    if (curr[0] > 0) {
      node_type next = {curr[0] - 1, curr[1]};
      if (board[next[0]][next[1]] != '#') {
        res.emplace_back(next, 1);
      }
    }
    if (curr[0] + 1 < board.size()) {
      node_type next = {curr[0] + 1, curr[1]};
      if (board[next[0]][next[1]] != '#') {
        res.emplace_back(next, 1);
      }
    }
    if (curr[1] > 0) {
      node_type next = {curr[0], curr[1] - 1};
      if (board[next[0]][next[1]] != '#') {
        res.emplace_back(next, 1);
      }
    }
    if (curr[1] + 1 < board[0].size()) {
      node_type next = {curr[0], curr[1] + 1};
      if (board[next[0]][next[1]] != '#') {
        res.emplace_back(next, 1);
      }
    }

    return res;
  };
  auto neighbors = gen_neighbors(path.back());

  size_t visit_count = 0;
  max_dists[path.back()] =
      std::max<int64_t>(max_dists[path.back()], path.size() - 1);
  for (auto &n : neighbors) {
    if (visited.find(n.first) == visited.end()) {
      // recurse
      ++visit_count;
      path.emplace_back(n.first);
      visited.emplace(n.first);
      search(path, visited, board, max_dists);
      path.pop_back();
      visited.erase(n.first);
    }
  }
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;

  std::vector<std::string> board;
  node_type start;
  node_type stop;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    board.emplace_back(line);
  }
  for (size_t col = 0; col < board[0].size(); ++col) {
    if (board[0][col] == '.') {
      start[0] = 0;
      start[1] = col;
      break;
    }
  }
  for (size_t col = 0; col < board[0].size(); ++col) {
    if (board.back()[col] == '.') {
      stop[0] = board.size() - 1;
      stop[1] = col;
      break;
    }
  }
  std::vector<node_type> path;
  path.push_back(start);
  std::unordered_set<node_type, aoc::array_hasher> visited;
  std::unordered_map<node_type, int64_t, aoc::array_hasher> max_dists;
  search(path, visited, board, max_dists);
  for (auto &v : max_dists) {
    if (v.first[0] + 1 == board.size()) {
      std::cout << v.second << std::endl;
    }
  }
#if 0
  // first find the topology ordering of our DAG
  auto term_func = [](const auto &v) { return false; };
  std::unordered_map<node_type, int64_t, aoc::array_hasher> dists;
  std::unordered_map<node_type, node_type, aoc::array_hasher> prev;
  auto node_cmp = [](const auto &a, const auto &b) {
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
  };
  auto gen_neighbors = [&](const node_type &curr) {
    std::vector<std::pair<node_type, int64_t>> res;
    switch (board[curr[0]][curr[1]]) {
    case '^': {
      if (curr[0] > 0) {
        node_type next = {curr[0] - 1, curr[1]};
        if (board[next[0]][next[1]] != '#') {
          res.emplace_back(next, 1);
        }
      }
    } break;
    case '<': {
      if (curr[1] > 0) {
        node_type next = {curr[0], curr[1] - 1};
        if (board[next[0]][next[1]] != '#') {
          res.emplace_back(next, 1);
        }
      }
    } break;
    case '>': {
      if (curr[1] + 1 < board[0].size()) {
        node_type next = {curr[0], curr[1] + 1};
        if (board[next[0]][next[1]] != '#') {
          res.emplace_back(next, 1);
        }
      }
    } break;
    case 'v': {
      if (curr[0] + 1 < board.size()) {
        node_type next = {curr[0] + 1, curr[1]};
        if (board[next[0]][next[1]] != '#') {
          res.emplace_back(next, 1);
        }
      }
    } break;
    default: {
      if (curr[0] > 0) {
        node_type next = {curr[0] - 1, curr[1]};
        if (board[next[0]][next[1]] != '#' && board[next[0]][next[1]] != 'v') {
          res.emplace_back(next, 1);
        }
      }
      if (curr[0] + 1 < board.size()) {
        node_type next = {curr[0] + 1, curr[1]};
        if (board[next[0]][next[1]] != '#' && board[next[0]][next[1]] != '^') {
          res.emplace_back(next, 1);
        }
      }
      if (curr[1] > 0) {
        node_type next = {curr[0], curr[1] - 1};
        if (board[next[0]][next[1]] != '#' && board[next[0]][next[1]] != '>') {
          res.emplace_back(next, 1);
        }
      }
      if (curr[1] + 1 < board[0].size()) {
        node_type next = {curr[0], curr[1] + 1};
        if (board[next[0]][next[1]] != '#' && board[next[0]][next[1]] != '<') {
          res.emplace_back(next, 1);
        }
      }
    } break;
    }

    return res;
  };
  std::vector<node_type> idx_to_node;
  idx_to_node.reserve(dists.size());
  for (auto &v : dists) {
    idx_to_node.emplace_back(v.first);
  }
  std::sort(idx_to_node.begin(), idx_to_node.end(),
            [&](const node_type &a, const node_type &b) {
              return dists.at(a) < dists.at(b);
            });
  std::unordered_map<node_type, size_t, aoc::array_hasher> node_to_idx;
  node_to_idx.reserve(idx_to_node.size());
  for (size_t i = 0; i < idx_to_node.size(); ++i) {
    node_to_idx.emplace(idx_to_node[i], i);
  }
  // build our graph
  aoc::directed_graph<int64_t> graph;
  for (size_t idx = 0; idx < idx_to_node.size(); ++idx) {
    const auto &curr = idx_to_node[idx];
    auto neighbors = gen_neighbors(curr);
    for (auto &n : neighbors) {
      // TODO: do something to remove cycles
      auto n_idx = node_to_idx.at(n.first);
      graph.add_edge(n_idx, idx, n.second);
    }
  }
  std::vector<int64_t> max_dists(idx_to_node.size());
  for (size_t i = 0; i < idx_to_node.size(); ++i) {
    for (auto &n : graph.connectivity[i]) {
      max_dists[i] = std::max<int64_t>(max_dists[i], max_dists[n.first] + 1);
    }
  }
  for (size_t i = 0; i < max_dists.size(); ++i) {
    auto node = idx_to_node[i];
    if (node[0] + 1 == board.size()) {
      std::cout << max_dists[i] << std::endl;
      break;
    }
  }
#endif
}
