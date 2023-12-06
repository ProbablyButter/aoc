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
  // read times
  std::getline(in, line);
  std::vector<int64_t> times;
  std::vector<int64_t> distances;
  {
    std::string tmp;
    std::istringstream str(line);
    str >> tmp;
    int64_t val;
    while (true) {
      str >> val;
      times.push_back(val);
      if (str.eof()) {
        break;
      }
    }
  }
  // read distances
  std::getline(in, line);
  {
    std::string tmp;
    std::istringstream str(line);
    str >> tmp;
    int64_t val;
    while (true) {
      str >> val;
      distances.push_back(val);
      if (str.eof()) {
        break;
      }
    }
  }
  int res = 1;
  for (int i = 0; i < times.size(); ++i) {
    int count = 0;
    for (int64_t len = 1; len < times[i]; ++len) {
      int64_t rem_time = times[i] - len;
      int64_t dist = len * rem_time;
      if (dist > distances[i]) {
        ++count;
      }
    }
    std::cout << count << std::endl;
    if (count) {
      res *= count;
    }
  }
  std::cout << res << std::endl;
}
