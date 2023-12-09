/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"
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

struct game {
  int id;
  std::set<int> winners;
  std::vector<int> vals;
};

void parse_game(const std::string &line, std::vector<game> &games) {
  const char *YYCURSOR = line.data();
  const char *YYMARKER;
  const char *digit_start;
  const char *color_start;
  const char *color_stop;
  int first_val = -1;
  int curr_val = 0;
  int id = 0;
  /*!stags:re2c format = 'const char *@@;'; */
parse_start:
  /*!re2c
"Card" " "* @digit_start [0-9]+ ": " {
  games.emplace_back();
  games.back().id = atoi(digit_start);
  goto parse_winners;
}
[\x00] {
  return;
}
* {
  goto parse_start;
}
*/

parse_winners:
  /*!re2c
@digit_start [0-9]+ {
  games.back().winners.insert(atoi(digit_start));
  goto parse_winners;
}
"|" {
  goto parse_card;
}
[\x00] {
  return;
}
* {
  goto parse_winners;
}
   */

parse_card:
  /*!re2c
@digit_start [0-9]+ {
  games.back().vals.push_back(atoi(digit_start)); goto parse_card;
}
[\x00] {
  return;
}
* {
  goto parse_card;
}
   */
  return;
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  std::vector<game> games;
  std::vector<std::vector<int64_t>> board;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto vals = aoc::split(line, "");
    board.emplace_back();
    for (auto &v : vals) {
      int64_t val;
      std::from_chars(v.data(), v.data() + v.size(), val);
      board.back().push_back(val);
    }
  }
  int64_t sum = 0;
  for (auto &row : board) {
    std::vector<std::vector<int64_t>> diffs;
    diffs.emplace_back(row);
    // diffs.reserve(row.size() - 1);
    while (true) {
      bool all_zero = true;
      diffs.emplace_back();
      auto &prev = diffs[diffs.size() - 2];
      auto &curr = diffs.back();
      curr.reserve(prev.size() - 1);
      for (size_t i = 0; i + 1 < prev.size(); ++i) {
        curr.emplace_back(prev[i + 1] - prev[i]);
        if (curr.back() != 0) {
          all_zero = false;
        }
      }
      if (all_zero) {
        break;
      }
    }
    diffs.back().push_back(0);
    for (auto iter = diffs.rbegin() + 1; iter != diffs.rend(); ++iter) {
      iter->emplace_back((*iter)[0] - (iter - 1)->back());
    }
    sum += diffs[0].back();
  }
  std::cout << sum << std::endl;
}
