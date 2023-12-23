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

std::pair<node_type, int64_t>
trace_path(node_type prev, node_type curr,
           const std::vector<std::string> &board) {
  std::pair<node_type, int64_t> res;
  res.first = curr;
  res.second = 1;
  while (true) {
    int64_t count = 0;
    if (curr[0] > 0) {
      node_type next = {curr[0] - 1, curr[1]};
      if (board[next[0]][next[1]] != '#') {
        ++count;
      }
    }
    if (curr[0] + 1 < board.size()) {
      node_type next = {curr[0] + 1, curr[1]};
      if (board[next[0]][next[1]] != '#') {
        ++count;
      }
    }
    if (curr[1] > 0) {
      node_type next = {curr[0], curr[1] - 1};
      if (board[next[0]][next[1]] != '#') {
        ++count;
      }
    }
    if (curr[1] + 1 < board[0].size()) {
      node_type next = {curr[0], curr[1] + 1};
      if (board[next[0]][next[1]] != '#') {
        ++count;
      }
    }
    if (count == 2) {
      if (curr[0] > 0) {
        node_type next = {curr[0] - 1, curr[1]};
        if (board[next[0]][next[1]] != '#') {
          if (next != prev) {
            prev = curr;
            res.first = next;
            curr = next;
            ++res.second;
            continue;
          }
        }
      }
      if (curr[0] + 1 < board.size()) {
        node_type next = {curr[0] + 1, curr[1]};
        if (board[next[0]][next[1]] != '#') {
          if (next != prev) {
            prev = curr;
            res.first = next;
            curr = next;
            ++res.second;
            continue;
          }
        }
      }
      if (curr[1] > 0) {
        node_type next = {curr[0], curr[1] - 1};
        if (board[next[0]][next[1]] != '#') {
          if (next != prev) {
            prev = curr;
            res.first = next;
            curr = next;
            ++res.second;
            continue;
          }
        }
      }
      if (curr[1] + 1 < board[0].size()) {
        node_type next = {curr[0], curr[1] + 1};
        if (board[next[0]][next[1]] != '#') {
          if (next != prev) {
            prev = curr;
            res.first = next;
            curr = next;
            ++res.second;
            continue;
          }
        }
      }
    } else {
      break;
    }
  }
  return res;
}

std::vector<std::pair<node_type, int64_t>>
gen_neighbors(const node_type &curr, const std::vector<std::string> &board) {
  std::vector<std::pair<node_type, int64_t>> res;
  if (curr[0] > 0) {
    node_type next = {curr[0] - 1, curr[1]};
    if (board[next[0]][next[1]] != '#') {
      res.push_back(trace_path(curr, next, board));
    }
  }
  if (curr[0] + 1 < board.size()) {
    node_type next = {curr[0] + 1, curr[1]};
    if (board[next[0]][next[1]] != '#') {
      res.push_back(trace_path(curr, next, board));
    }
  }
  if (curr[1] > 0) {
    node_type next = {curr[0], curr[1] - 1};
    if (board[next[0]][next[1]] != '#') {
      res.push_back(trace_path(curr, next, board));
    }
  }
  if (curr[1] + 1 < board[0].size()) {
    node_type next = {curr[0], curr[1] + 1};
    if (board[next[0]][next[1]] != '#') {
      res.push_back(trace_path(curr, next, board));
    }
  }

  return res;
}

void build_graph(
    const node_type &curr,
    std::unordered_set<node_type, aoc::array_hasher> &visited,
    const std::vector<std::string> &board,
    std::unordered_map<node_type, size_t, aoc::array_hasher> &node_to_idx,
    aoc::directed_graph<int64_t> &graph) {
  visited.emplace(curr);

  auto neighbors = gen_neighbors(curr, board);
  auto idx = node_to_idx.emplace(curr, node_to_idx.size()).first->second;

  for (auto &n : neighbors) {
    auto nidx = node_to_idx.emplace(n.first, node_to_idx.size()).first->second;
    graph.add_edge(idx, nidx, n.second);
    // graph.add_edge(nidx, idx, n.second);
    if (visited.find(n.first) == visited.end()) {
      // recurse
      build_graph(n.first, visited, board, node_to_idx, graph);
    }
  }
}

void search(std::vector<size_t> &path, int64_t curr_dist,
            const aoc::directed_graph<int64_t> &graph,
            std::unordered_set<size_t> &visited,
            std::unordered_map<size_t, int64_t> &max_dists) {
  auto &neighbors = graph.connectivity[path.back()];

  max_dists[path.back()] = std::max<int64_t>(max_dists[path.back()], curr_dist);
  for (auto &n : neighbors) {
    if (visited.find(n.first) == visited.end()) {
      // recurse
      path.emplace_back(n.first);
      visited.emplace(n.first);
      search(path, curr_dist + n.second, graph, visited, max_dists);
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
  std::unordered_map<node_type, size_t, aoc::array_hasher> node_to_idx;
  aoc::directed_graph<int64_t> graph;
  {
    std::unordered_set<node_type, aoc::array_hasher> visited;
    build_graph(start, visited, board, node_to_idx, graph);
  }
  std::vector<node_type> idx_to_node;
  idx_to_node.resize(node_to_idx.size());
  for (auto &v : node_to_idx) {
    idx_to_node[v.second] = v.first;
  }
  {
    std::vector<size_t> path;
    std::unordered_set<size_t> visited;
    std::unordered_map<size_t, int64_t> max_dists;
    path.push_back(node_to_idx.at(start));
    search(path, 0, graph, visited, max_dists);
    auto end_idx = node_to_idx.at(stop);
    std::cout << max_dists.at(end_idx) << std::endl;
  }
}
