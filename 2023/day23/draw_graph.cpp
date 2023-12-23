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

std::pair<node_type, int64_t> trace_path(node_type prev, node_type curr,
                                         const std::vector<std::string> &board,
                                         bool ignore_slopes) {
  std::pair<node_type, int64_t> res;
  res.first = curr;
  res.second = 1;
  if (ignore_slopes) {
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
        // couldn't continue any further
        throw std::runtime_error("shouldn't be here");
      } else {
        break;
      }
    }
  } else {
    // prev is a '.'
    // if curr is a slope, return either curr or the tile after curr
    switch (board[curr[0]][curr[1]]) {
    case '^': {
      if (curr[0] > 0) {
        node_type next = {curr[0] - 1, curr[1]};
        if (board[next[0]][next[1]] != '#' && board[next[0]][next[1]] != 'v') {
          res.first = next;
          res.second = 2;
        }
      }
    } break;
    case '<': {
      if (curr[1] > 0) {
        node_type next = {curr[0], curr[1] - 1};
        if (board[next[0]][next[1]] != '#' && board[next[0]][next[1]] != '>') {
          res.first = next;
          res.second = 2;
        }
      }
    } break;
    case '>': {
      if (curr[1] + 1 < board[0].size()) {
        node_type next = {curr[0], curr[1] + 1};
        if (board[next[0]][next[1]] != '#' && board[next[0]][next[1]] != '<') {
          res.first = next;
          res.second = 2;
        }
      }
    } break;
    case 'v': {
      if (curr[0] + 1 < board.size()) {
        node_type next = {curr[0] + 1, curr[1]};
        if (board[next[0]][next[1]] != '#' && board[next[0]][next[1]] != '^') {
          res.first = next;
          res.second = 2;
        }
      }
    } break;
    case '.': {
      // follow .'s until we reach a junction
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
            if (board[next[0]][next[1]] == '.') {
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
            if (board[next[0]][next[1]] == '.') {
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
            if (board[next[0]][next[1]] == '.') {
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
            if (board[next[0]][next[1]] == '.') {
              if (next != prev) {
                prev = curr;
                res.first = next;
                curr = next;
                ++res.second;
                continue;
              }
            }
          }
          // couldn't continue any further
          break;
        } else {
          break;
        }
      }
    } break;
    default:
      break;
    }
  }
  return res;
}

std::vector<std::pair<node_type, int64_t>>
gen_neighbors(const node_type &curr, const std::vector<std::string> &board,
              bool ignore_slopes) {
  // if ignore_slopes = false,
  // - searches up to a slope, and doesn't cross the slope unless the slope is
  // adjacent to curr
  // - if adjacent or on a slope, only goes to the next free tile after the
  // slope (or stops on the slope if no tile after slope)
  std::vector<std::pair<node_type, int64_t>> res;
  if (!ignore_slopes) {
    switch (board[curr[0]][curr[1]]) {
    case '^': {
      if (curr[0] > 0) {
        node_type next = {curr[0] - 1, curr[1]};
        if (board[next[0]][next[1]] != '#' && board[next[0]][next[1]] != 'v') {
          res.emplace_back(next, 1);
        }
      }
    } break;
    case '<': {
      if (curr[1] > 0) {
        node_type next = {curr[0], curr[1] - 1};
        if (board[next[0]][next[1]] != '#' && board[next[0]][next[1]] != '>') {
          res.emplace_back(next, 1);
        }
      }
    } break;
    case '>': {
      if (curr[1] + 1 < board[0].size()) {
        node_type next = {curr[0], curr[1] + 1};
        if (board[next[0]][next[1]] != '#' && board[next[0]][next[1]] != '<') {
          res.emplace_back(next, 1);
        }
      }
    } break;
    case 'v': {
      if (curr[0] + 1 < board.size()) {
        node_type next = {curr[0] + 1, curr[1]};
        if (board[next[0]][next[1]] != '#' && board[next[0]][next[1]] != '^') {
          res.emplace_back(next, 1);
        }
      }
    } break;
    case '.': {
      if (curr[0] > 0) {
        node_type next = {curr[0] - 1, curr[1]};
        if (board[next[0]][next[1]] != '#' && board[next[0]][next[1]] != 'v') {
          res.push_back(trace_path(curr, next, board, ignore_slopes));
        }
      }
      if (curr[0] + 1 < board.size()) {
        node_type next = {curr[0] + 1, curr[1]};
        if (board[next[0]][next[1]] != '#' && board[next[0]][next[1]] != '^') {
          res.push_back(trace_path(curr, next, board, ignore_slopes));
        }
      }
      if (curr[1] > 0) {
        node_type next = {curr[0], curr[1] - 1};
        if (board[next[0]][next[1]] != '#' && board[next[0]][next[1]] != '>') {
          res.push_back(trace_path(curr, next, board, ignore_slopes));
        }
      }
      if (curr[1] + 1 < board[0].size()) {
        node_type next = {curr[0], curr[1] + 1};
        if (board[next[0]][next[1]] != '#' && board[next[0]][next[1]] != '<') {
          res.push_back(trace_path(curr, next, board, ignore_slopes));
        }
      }
    } break;
    default:
      break;
    }
  } else {
    if (curr[0] > 0) {
      node_type next = {curr[0] - 1, curr[1]};
      if (board[next[0]][next[1]] != '#') {
        res.push_back(trace_path(curr, next, board, ignore_slopes));
      }
    }
    if (curr[0] + 1 < board.size()) {
      node_type next = {curr[0] + 1, curr[1]};
      if (board[next[0]][next[1]] != '#') {
        res.push_back(trace_path(curr, next, board, ignore_slopes));
      }
    }
    if (curr[1] > 0) {
      node_type next = {curr[0], curr[1] - 1};
      if (board[next[0]][next[1]] != '#') {
        res.push_back(trace_path(curr, next, board, ignore_slopes));
      }
    }
    if (curr[1] + 1 < board[0].size()) {
      node_type next = {curr[0], curr[1] + 1};
      if (board[next[0]][next[1]] != '#') {
        res.push_back(trace_path(curr, next, board, ignore_slopes));
      }
    }
  }

  return res;
}

void build_graph(
    const node_type &curr,
    std::unordered_set<node_type, aoc::array_hasher> &visited,
    const std::vector<std::string> &board,
    std::unordered_map<node_type, size_t, aoc::array_hasher> &node_to_idx,
    aoc::directed_graph<int64_t> &graph, bool ignore_slopes) {
  visited.emplace(curr);

  auto neighbors = gen_neighbors(curr, board, ignore_slopes);
  auto idx = node_to_idx.emplace(curr, node_to_idx.size()).first->second;

  for (auto &n : neighbors) {
    auto nidx = node_to_idx.emplace(n.first, node_to_idx.size()).first->second;
    graph.add_edge(idx, nidx, n.second);
    // graph.add_edge(nidx, idx, n.second);
    if (visited.find(n.first) == visited.end()) {
      // recurse
      build_graph(n.first, visited, board, node_to_idx, graph, ignore_slopes);
    }
  }
}

int main(int argc, char **argv) {

  bool ignore_slopes = true;
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
    build_graph(start, visited, board, node_to_idx, graph, ignore_slopes);
  }
  std::vector<node_type> idx_to_node;
  idx_to_node.resize(node_to_idx.size());
  for (auto &v : node_to_idx) {
    idx_to_node[v.second] = v.first;
  }

  std::cout << "digraph {" << std::endl;
  for (size_t i = 0; i < idx_to_node.size(); ++i) {
    auto curr = idx_to_node[i];
    for (auto &v : graph.connectivity[i]) {
      auto neighbor = idx_to_node[v.first];
      std::cout << "\"(" << curr[0] << ", " << curr[1] << ")\""
                << " -> "
                << "\"(" << neighbor[0] << ", " << neighbor[1] << ")\""
                << " [ label=\"" << v.second << "\"];" << std::endl;
    }
  }
  std::cout << "}" << std::endl;
}
