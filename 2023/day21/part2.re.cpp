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
  using node_type = std::array<int64_t, 2>;
  // maps a gate to its outputs
  std::vector<std::string> board;
  node_type start;

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
        start[0] = row;
        start[1] = col;
      }
    }
  }
  size_t nsteps = 26501365;
  // 3x3 around start
  std::vector<std::unordered_map<node_type, int64_t, aoc::array_hasher>> dists;
  dists.resize(9);
  std::unordered_map<node_type, node_type, aoc::array_hasher> prev;
  auto term_func = [&](const node_type &node) { return false; };
  auto gen_neighbors = [&](const node_type &node) {
    std::vector<std::pair<node_type, int64_t>> res;
    // left
    if (node[1] > 0) {
      node_type next = {node[0], node[1] - 1};
      int64_t row = next[0], col = next[1];
      if (board[row][col] != '#') {
        res.emplace_back(next, 1);
      }
    }
    // right
    if (node[1] + 1 < board[0].size()) {
      node_type next = {node[0], node[1] + 1};
      int64_t row = next[0], col = next[1];
      if (board[row][col] != '#') {
        res.emplace_back(next, 1);
      }
    }
    // up
    if (node[0] > 0) {
      node_type next = {node[0] - 1, node[1]};
      int64_t row = next[0], col = next[1];
      if (board[row][col] != '#') {
        res.emplace_back(next, 1);
      }
    }
    // down
    if (node[0] + 1 < board.size()) {
      node_type next = {node[0] + 1, node[1]};
      int64_t row = next[0], col = next[1];
      if (board[row][col] != '#') {
        res.emplace_back(next, 1);
      }
    }
    return res;
  };
  auto node_cmp = [](const node_type &a, const node_type &b) {
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
  };

  // top left
  aoc::gen_dijkstra(node_type{static_cast<int64_t>(board.size()) - 1,
                              static_cast<int64_t>(board[0].size()) - 1},
                    term_func, gen_neighbors, dists[0], prev, node_cmp);
  // top
  aoc::gen_dijkstra(node_type{static_cast<int64_t>(board.size()) - 1, start[1]},
                    term_func, gen_neighbors, dists[1], prev, node_cmp);
  // top right
  aoc::gen_dijkstra(node_type{static_cast<int64_t>(board.size()) - 1, 0},
                    term_func, gen_neighbors, dists[2], prev, node_cmp);
  // left
  aoc::gen_dijkstra(
      node_type{start[0], static_cast<int64_t>(board[0].size()) - 1}, term_func,
      gen_neighbors, dists[3], prev, node_cmp);
  // mid
  aoc::gen_dijkstra(start, term_func, gen_neighbors, dists[3 + 1], prev,
                    node_cmp);
  // right
  aoc::gen_dijkstra(node_type{start[0], 0}, term_func, gen_neighbors,
                    dists[3 + 2], prev, node_cmp);
  // bottom left
  aoc::gen_dijkstra(node_type{0, static_cast<int64_t>(board[0].size()) - 1},
                    term_func, gen_neighbors, dists[2 * 3], prev, node_cmp);
  // bottom
  aoc::gen_dijkstra(node_type{0, start[1]}, term_func, gen_neighbors,
                    dists[2 * 3 + 1], prev, node_cmp);
  // bottom right
  aoc::gen_dijkstra(node_type{0, 0}, term_func, gen_neighbors, dists[2 * 3 + 2],
                    prev, node_cmp);

  std::vector<int64_t> max_dists(9, 0);

  // first split dists into odd/even
  std::vector<std::unordered_map<node_type, int64_t, aoc::array_hasher>>
      odd_dists;
  odd_dists.resize(dists.size());
  std::vector<std::unordered_map<node_type, int64_t, aoc::array_hasher>>
      even_dists;
  even_dists.resize(dists.size());
  for (size_t i = 0; i < dists.size(); ++i) {
    for (auto &v : dists[i]) {
      if (v.second % 2) {
        odd_dists[i].insert(v);
      } else {
        even_dists[i].insert(v);
      }
      max_dists[i] = std::max<int64_t>(max_dists[i], v.second);
    }
  }

  auto odd = nsteps % 2;
  int64_t sum = 0;
  auto count_board =
      [&](std::unordered_map<node_type, int64_t, aoc::array_hasher> &dist,
          int64_t dist_offset, int64_t odd) {
        for (auto &v : dist) {
          if (v.second + dist_offset <= nsteps) {
            if (((v.second + dist_offset) % 2) == odd) {
              ++sum;
            }
          }
        }
      };
  count_board(dists[3 + 1], 0, odd);
  // left
  {
    int64_t offset = start[1] + 1;
    while (true) {
      if (offset > nsteps) {
        // zero tiles in this square
        break;
      }
      if (offset + max_dists[3] <= nsteps) {
        if (offset % 2) {
          if (odd) {
            // keep even
            sum += even_dists[3].size();
          } else {
            // keep odd
            sum += odd_dists[3].size();
          }
        } else {
          if (odd) {
            // keep odd
            sum += odd_dists[3].size();
          } else {
            // keep even
            sum += even_dists[3].size();
          }
        }
      } else {
        // need to do a partial check
        count_board(dists[3], offset, odd);
      }
      offset += board[0].size();
    }
  }
  // top left
  {
    int64_t offset_base = start[1] + 2 + start[0];

    while (true) {
      if (offset_base > nsteps) {
        break;
      }
      int64_t offset = offset_base;
      while (true) {
        if (offset > nsteps) {
          break;
        }
        if (offset + max_dists[0] <= nsteps) {
          if (offset % 2) {
            if (odd) {
              // keep even
              sum += even_dists[0].size();
            } else {
              // keep odd
              sum += odd_dists[0].size();
            }
          } else {
            if (odd) {
              // keep odd
              sum += odd_dists[0].size();
            } else {
              // keep even
              sum += even_dists[0].size();
            }
          }
        } else {
          // need to do a partial check
          count_board(dists[0], offset, odd);
        }
        offset += board[0].size();
      }
      offset_base += board.size();
    }
  }
  // top
  {
    int64_t offset = start[0] + 1;
    while (true) {
      if (offset > nsteps) {
        // zero tiles in this square
        break;
      }
      if (offset + max_dists[1] <= nsteps) {
        if (offset % 2) {
          if (odd) {
            // keep even
            sum += even_dists[1].size();
          } else {
            // keep odd
            sum += odd_dists[1].size();
          }
        } else {
          if (odd) {
            // keep odd
            sum += odd_dists[1].size();
          } else {
            // keep even
            sum += even_dists[1].size();
          }
        }
      } else {
        // need to do a partial check
        count_board(dists[1], offset, odd);
      }
      offset += board.size();
    }
  }
  // top right
  {
    int64_t offset_base = start[0] + 2 + (board[0].size() - 1 - start[1]);

    while (true) {
      if (offset_base > nsteps) {
        break;
      }
      int64_t offset = offset_base;
      while (true) {
        if (offset > nsteps) {
          break;
        }
        if (offset + max_dists[2] <= nsteps) {
          if (offset % 2) {
            if (odd) {
              // keep even
              sum += even_dists[2].size();
            } else {
              // keep odd
              sum += odd_dists[2].size();
            }
          } else {
            if (odd) {
              // keep odd
              sum += odd_dists[2].size();
            } else {
              // keep even
              sum += even_dists[2].size();
            }
          }
        } else {
          // need to do a partial check
          count_board(dists[2], offset, odd);
        }
        offset += board[0].size();
      }
      offset_base += board.size();
    }
  }
  // right
  {
    int64_t offset = (board[0].size() - 1 - start[1]) + 1;
    while (true) {
      if (offset > nsteps) {
        // zero tiles in this square
        break;
      }
      if (offset + max_dists[3 + 2] <= nsteps) {
        if (offset % 2) {
          if (odd) {
            // keep even
            sum += even_dists[3 + 2].size();
          } else {
            // keep odd
            sum += odd_dists[3 + 2].size();
          }
        } else {
          if (odd) {
            // keep odd
            sum += odd_dists[3 + 2].size();
          } else {
            // keep even
            sum += even_dists[3 + 2].size();
          }
        }
      } else {
        // need to do a partial check
        count_board(dists[3 + 2], offset, odd);
      }
      offset += board[0].size();
    }
  }
  // bottom right
  {
    int64_t offset_base =
        (board.size() - 1 - start[0]) + 2 + (board[0].size() - 1 - start[1]);

    while (true) {
      if (offset_base > nsteps) {
        break;
      }
      int64_t offset = offset_base;
      while (true) {
        if (offset > nsteps) {
          break;
        }
        if (offset + max_dists[2 * 3 + 2] <= nsteps) {
          if (offset % 2) {
            if (odd) {
              // keep even
              sum += even_dists[2 * 3 + 2].size();
            } else {
              // keep odd
              sum += odd_dists[2 * 3 + 2].size();
            }
          } else {
            if (odd) {
              // keep odd
              sum += odd_dists[2 * 3 + 2].size();
            } else {
              // keep even
              sum += even_dists[2 * 3 + 2].size();
            }
          }
        } else {
          // need to do a partial check
          count_board(dists[2 * 3 + 2], offset, odd);
        }
        offset += board[0].size();
      }
      offset_base += board.size();
    }
  }
  // bottom
  {
    int64_t offset = (board.size() - 1 - start[0]) + 1;
    while (true) {
      if (offset > nsteps) {
        // zero tiles in this square
        break;
      }
      if (offset + max_dists[2 * 3 + 1] <= nsteps) {
        if (offset % 2) {
          if (odd) {
            // keep even
            sum += even_dists[2 * 3 + 1].size();
          } else {
            // keep odd
            sum += odd_dists[2 * 3 + 1].size();
          }
        } else {
          if (odd) {
            // keep odd
            sum += odd_dists[2 * 3 + 1].size();
          } else {
            // keep even
            sum += even_dists[2 * 3 + 1].size();
          }
        }
      } else {
        // need to do a partial check
        count_board(dists[2 * 3 + 1], offset, odd);
      }
      offset += board.size();
    }
  }
  // bottom left
  {
    int64_t offset_base = (board.size() - 1 - start[0]) + 2 + start[1];
    while (true) {
      if (offset_base > nsteps) {
        break;
      }
      int64_t offset = offset_base;
      while (true) {
        if (offset > nsteps) {
          break;
        }
        if (offset + max_dists[2 * 3] <= nsteps) {
          if (offset % 2) {
            if (odd) {
              // keep even
              sum += even_dists[2 * 3].size();
            } else {
              // keep odd
              sum += odd_dists[2 * 3].size();
            }
          } else {
            if (odd) {
              // keep odd
              sum += odd_dists[2 * 3].size();
            } else {
              // keep even
              sum += even_dists[2 * 3].size();
            }
          }
        } else {
          // need to do a partial check
          count_board(dists[2 * 3], offset, odd);
        }
        offset += board[0].size();
      }
      offset_base += board.size();
    }
  }
  std::cout << sum << std::endl;
}
