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
enum instr_type { NONE, TURN_OFF, TURN_ON, TOGGLE };

void parse_instr(const std::string &line,
                 std::vector<std::vector<int64_t>> &grid) {
  const char *YYCURSOR = line.data();
  const char *YYMARKER;
  const char *d0, *d1, *d2, *d3;
  instr_type type;
  /*!stags:re2c format = 'const char *@@;'; */
parse_start:
  /*!re2c
"turn off " {
  type = TURN_OFF;
  goto parse_c1;
}
"turn on " {
  type = TURN_ON;
  goto parse_c1;
}
"toggle " {
  type = TOGGLE;
  goto parse_c1;
}
[\x00] {
  return;
}
* {
  goto parse_start;
}
   */
parse_c1:
  /*!re2c
@d0 [0-9]+ "," @d1 [0-9]+ " through " @d2 [0-9]+ "," @d3 [0-9]+ {
  goto process;
}
[\x00] {
  return;
}
* {
  goto parse_c1;
}
   */
process:
  int64_t v0, v1, v2, v3;
  v0 = aoc::to_int<int64_t>(d0);
  v1 = aoc::to_int<int64_t>(d1);
  v2 = aoc::to_int<int64_t>(d2);
  v3 = aoc::to_int<int64_t>(d3);
  int64_t top = std::min(v0, v2);
  int64_t left = std::min(v1, v3);
  int64_t bottom = std::max(v0, v2);
  int64_t right = std::max(v1, v3);
  for (int64_t row = top; row <= bottom; ++row) {
    for (int64_t col = left; col <= right; ++col) {
      switch (type) {
      case TURN_OFF:
        grid[row][col] = std::max<int64_t>(0, grid[row][col] - 1);
        break;
      case TURN_ON:
        ++grid[row][col];
        break;
      case TOGGLE:
        grid[row][col] += 2;
        break;
      default:
        break;
      }
    }
  }
  goto parse_start;
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  std::vector<std::vector<int64_t>> grid;
  grid.resize(1000, std::vector<int64_t>(1000, 0));

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    parse_instr(line, grid);
  }
  int64_t on_count = 0;
  for (auto &row : grid) {
    for (auto col : row) {
      if (col) {
        on_count += col;
      }
    }
  }
  std::cout << on_count << std::endl;
}
