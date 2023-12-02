/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"

#include <algorithm>
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
  std::vector<int> splits;
  std::vector<std::pair<std::string, int>> record;
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
"Game " @digit_start [0-9]+ ": " {
  games.emplace_back();
  games.back().id = atoi(digit_start);
  games.back().splits.push_back(0);
  goto parse_reveals;
}
[\x00] {
  return;
}
* {
  goto parse_start;
}
*/

parse_reveals:
  /*!re2c
@digit_start [0-9]+ " " @color_start [a-z]+ @color_stop {
  games.back().record.emplace_back(std::string(color_start, color_stop),
atoi(digit_start)); goto parse_reveals;
}
", " {
  goto parse_reveals;
}
"; " {
  games.back().splits.push_back(games.back().record.size());
  goto parse_reveals;
}
[\x00] {
  games.back().splits.push_back(games.back().record.size());
  return;
}
* {
  goto parse_reveals;
}
   */
  return;
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  std::vector<game> games;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    parse_game(line, games);
  }
  int target_red = 12;
  int target_green = 13;
  int target_blue = 14;

  int res = 0;
  for (auto &g : games) {
    int max_red = 0;
    int max_green = 0;
    int max_blue = 0;
    for (int seg = 0; seg < g.splits.size() - 1; ++seg) {
      for (int i = g.splits[seg]; i < g.splits[seg + 1]; ++i) {
        if (g.record[i].first == "red") {
          max_red = std::max<int>(g.record[i].second, max_red);
        } else if (g.record[i].first == "green") {
          max_green = std::max<int>(g.record[i].second, max_green);
        } else if (g.record[i].first == "blue") {
          max_blue = std::max<int>(g.record[i].second, max_blue);
        }
      }
    }
    if (max_red <= target_red && max_blue <= target_blue &&
        max_green <= target_green) {
      res += g.id;
    }
  }
  std::cout << res << std::endl;
}
