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

int64_t hash(const std::string_view &str, int64_t curr = 0) {
  for (size_t i = 0; i < str.size(); ++i) {
    curr += str[i];
    curr *= 17;
    curr %= 256;
  }
  return curr;
}

void parse(const std::string &instr,
           std::vector<std::vector<std::pair<std::string, int64_t>>> &boxes) {
  const char *YYCURSOR = instr.data();
  const char *YYMARKER;
  const char *start, *stop;
  /*!stags:re2c format = 'const char *@@;'; */
  std::string curr_label;
  int64_t box_idx = -1;
parse_start:
  /*!re2c
@start [a-z]+ @stop {
  curr_label = std::string(start, stop-start);
  box_idx = hash(curr_label);
  goto parse_start;
}
"-" {
  goto remove_lens;
}
"=" @start [0-9]+ @stop {
  goto insert_lens;
  return;
}
[\x00] {
return;
}
* {
goto parse_start;
}
  */
remove_lens:
  for (size_t i = 0; i < boxes[box_idx].size(); ++i) {
    if (boxes[box_idx][i].first == curr_label) {
      boxes[box_idx].erase(boxes[box_idx].begin() + i);
      break;
    }
  }
  return;
insert_lens:
  for(size_t i = 0; i < boxes[box_idx].size(); ++i)
  {
    if(boxes[box_idx][i].first == curr_label) {
      boxes[box_idx][i].second = aoc::to_int<int64_t>(start, stop);
      return;
    }
  }
  boxes[box_idx].emplace_back(curr_label, aoc::to_int<int64_t>(start, stop));
  return;
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  std::vector<std::vector<std::pair<std::string, int64_t>>> boxes;
  boxes.resize(256);
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line, ",");
    for (auto &v : tmp) {
      parse(std::string(v), boxes);
    }
  }
  // compute the focusing power
  int64_t sum = 0;
  for(size_t i = 0; i < boxes.size(); ++i)
  {
    for(size_t j = 0; j < boxes[i].size(); ++j)
    {
      auto power = (j+1) * boxes[i][j].second * (i+1);
      sum += power;
    }
  }
  std::cout << sum << std::endl;
}
