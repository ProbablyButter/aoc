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

struct beam_hasher {
  size_t operator()(const std::array<int64_t, 4> &v) const noexcept {
    aoc::hasher hash;
    hash.init(123456789ull);
    for (auto &val : v) {
      hash.append(reinterpret_cast<const uint8_t *>(&val), sizeof(int64_t));
    }
    hash.finalize();
    // only return the low bits?
    return hash.data[1];
  }
};

int64_t count_energized(const std::vector<std::string> &board, int64_t erow,
                        int64_t ecol, int64_t ex, int64_t ey) {
  std::vector<std::array<int64_t, 4>> beams;
  std::unordered_set<std::pair<size_t, size_t>, aoc::ss_pair_hasher> energized;
  std::unordered_set<std::array<int64_t, 4>, beam_hasher> history;
  energized.emplace(erow, ecol);
  // row, col, vel_x, vel_y
  beams.emplace_back(std::array<int64_t, 4>{erow, ecol, ex, ey});
  history.emplace(beams.back());
  while (true) {
    auto orig_size = beams.size();
    for (size_t i = 0; i < orig_size; ++i) {
      switch (board[beams[i][0]][beams[i][1]]) {
      case '.':
        break;
      case '/':
        std::swap(beams[i][2], beams[i][3]);
        beams[i][2] = -beams[i][2];
        beams[i][3] = -beams[i][3];
        break;
      case '\\':
        std::swap(beams[i][2], beams[i][3]);
        break;
      case '|':
        if (beams[i][3]) {
          break;
        }
        // split beam
        beams[i][2] = 0;
        beams[i][3] = 1;
        beams.emplace_back(
            std::array<int64_t, 4>{beams[i][0] - 1, beams[i][1], 0, -1});
        break;
      case '-':
        if (beams[i][2]) {
          break;
        }
        // split beam
        beams[i][2] = 1;
        beams[i][3] = 0;
        beams.emplace_back(
            std::array<int64_t, 4>{beams[i][0], beams[i][1] - 1, -1, 0});
        break;
      }
      beams[i][0] += beams[i][3];
      beams[i][1] += beams[i][2];
    }
    // remove beams which leave board or repeat history
    beams.erase(std::remove_if(beams.begin(), beams.end(),
                               [&](auto &beam) {
                                 bool oob =
                                     beam[0] < 0 || beam[0] >= board.size() ||
                                     beam[1] < 0 || beam[1] >= board[0].size();
                                 if (oob) {
                                   return true;
                                 }
                                 energized.emplace(beam[0], beam[1]);
                                 return !history.emplace(beam).second;
                               }),
                beams.end());

    if (beams.empty()) {
      break;
    }
  }
  return energized.size();
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
  int64_t max_energized = 0;
  for (size_t row = 0; row < board.size(); ++row) {
    int64_t tmp = count_energized(board, row, 0, 1, 0);
    max_energized = std::max(tmp, max_energized);
    tmp = count_energized(board, row, board[0].size() - 1, -1, 0);
    max_energized = std::max(tmp, max_energized);
  }

  for (size_t col = 0; col < board[0].size(); ++col) {
    int64_t tmp = count_energized(board, 0, col, 0, 1);
    max_energized = std::max(tmp, max_energized);
    tmp = count_energized(board, board.size() - 1, col, 0, -1);
    max_energized = std::max(tmp, max_energized);
  }

  std::cout << max_energized << std::endl;
}
