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
#include <memory>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

void parse_string(const std::string &line, int64_t &sum) {
  const char *YYCURSOR = line.data();
  const char *YYMARKER;
  const char *d0, *d1;
  /*!stags:re2c format = 'const char *@@;'; */
parse_start:
  /*!re2c
@d0 [-]? " "* [0-9]+ @d1 {
  sum += aoc::to_int<int64_t>(d0, d1);
  goto parse_start;
}
[\x00] {
  return;
}
* {
  goto parse_start;
}
   */
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  int64_t sum = 0;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    parse_string(line, sum);
  }
  std::cout << sum << std::endl;
}
