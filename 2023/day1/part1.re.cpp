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

int parse_digits(const std::string &line) {
  const char *YYCURSOR = line.data();
  const char *YYMARKER;
  const char *digit_start;
  int first_val = -1;
  int curr_val = 0;
  /*!stags:re2c format = 'const char *@@;'; */
parse_start:
  /*!re2c
@digit_start [0-9] {
  if (first_val == -1)
  {
    first_val = digit_start[0] - '0';
    curr_val = first_val;
  }
  else {
    curr_val = digit_start[0] - '0';
  }
  goto parse_start;
}
[\x00] {
  return first_val * 10 + curr_val;
}
* {
  goto parse_start;
}
*/
  return first_val * 10 + curr_val;
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  int sum = 0;
  while (true)
  {
    std::getline(in, line);
    if (in.eof())
    {
      break;
    }
    int tmp = parse_digits(line);
    sum += tmp;
  }
  std::cout << sum << std::endl;
}
