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
  std::vector<std::string> maps;
  std::vector<std::vector<int64_t>> damage;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line, "");
    maps.emplace_back(tmp[0]);
    damage.emplace_back();
    auto tmp2 = aoc::split(tmp[1], ",");
    damage.back().reserve(tmp2.size());
    for (auto &v : tmp2) {
      damage.back().emplace_back(aoc::to_int<int64_t>(v));
    }
  }

  int64_t total_count = 0;

  for (size_t i = 0; i < maps.size(); ++i) {
    // build up regex matching string
    std::stringstream match_pattern;
    // any number of working springs at the beginning
    match_pattern << "\\.*";
    size_t expected_count = 0;
    for (size_t j = 0; j < damage[i].size(); ++j) {
      match_pattern << "#{" << damage[i][j] << "}";
      if (j + 1 < damage[i].size()) {
        match_pattern << "\\.+";
      } else {
        match_pattern << "\\.*";
      }
      expected_count += damage[i][j];
    }
    std::regex matcher(match_pattern.str(), std::regex_constants::ECMAScript);
    std::string working = maps[i];
    std::vector<size_t> variable_pos;
    // known broken count
    size_t broken_count = 0;
    for (size_t j = 0; j < working.size(); ++j) {
      switch (working[j]) {
      case '?':
        variable_pos.push_back(j);
        break;
      case '#':
        ++broken_count;
        break;
      default:
        break;
      }
    }
    std::stringstream init_pattern;
    for (size_t j = broken_count; j < expected_count; ++j) {
      init_pattern << '#';
    }
    for (size_t j = expected_count - broken_count; j < variable_pos.size();
         ++j) {
      init_pattern << '.';
    }
    std::string pattern = init_pattern.str();
    do {
      for (size_t j = 0; j < pattern.size(); ++j) {
        working[variable_pos[j]] = pattern[j];
      }
      if (std::regex_match(working, matcher)) {
        ++total_count;
      }
    } while (std::next_permutation(pattern.begin(), pattern.end()));
  }
  std::cout << total_count << std::endl;
}
