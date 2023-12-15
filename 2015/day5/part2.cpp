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
#include "hash.hpp"
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

  size_t nice_count = 0;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }

    bool aba = false;
    std::unordered_multimap<std::string, int64_t> char_pairs;
    for (size_t i = 0; i < line.size(); ++i) {
      if (i + 1 < line.size()) {
        char_pairs.emplace(line.substr(i, 2), i);
      }
      if (i + 2 < line.size()) {
        if (line[i] == line[i + 2]) {
          aba = true;
        }
      }
    }
    bool dups = false;
    for (auto &v : char_pairs) {
      auto range = char_pairs.equal_range(v.first);
      while (range.first != range.second) {
        int64_t diff = std::abs(v.second - range.first->second);
        if (diff > 1) {
          dups = true;
          goto done;
        }
        ++range.first;
      }
    }
  done:
    if (aba && dups) {
      ++nice_count;
    }
  }
  std::cout << nice_count << std::endl;
}
