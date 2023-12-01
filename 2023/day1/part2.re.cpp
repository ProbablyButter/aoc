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
  int tmp_val = 0;
  /*!stags:re2c format = 'const char *@@;'; */
parse_start:
  /*!re2c
@digit_start [1-9] {
tmp_val = digit_start[0] - '0';
goto process_val;
}
"oneight" {
  if (first_val == -1)
  {
    first_val = 1;
  }
  curr_val = 8;
goto parse_start;
}
"threeight" {
  if (first_val == -1)
  {
    first_val = 3;
  }
  curr_val = 8;
goto parse_start;
}
"fiveight" {
  if (first_val == -1)
  {
    first_val = 5;
  }
  curr_val = 8;
goto parse_start;
}
"nineight" {
  if (first_val == -1)
  {
    first_val = 9;
  }
  curr_val = 8;
goto parse_start;
}
"twone" {
  if (first_val == -1)
  {
    first_val = 2;
  }
  curr_val = 1;
goto parse_start;
}
"sevenine" {
  if (first_val == -1)
  {
    first_val = 7;
  }
  curr_val = 9;
goto parse_start;
}
"eightwo" {
  if (first_val == -1)
  {
    first_val = 8;
  }
  curr_val = 2;
goto parse_start;
}
"eighthree" {
  if (first_val == -1)
  {
    first_val = 8;
  }
  curr_val = 3;
goto parse_start;
}
"one" {
  tmp_val = 1;
  goto process_val;
}
"two" {
  tmp_val = 2;
  goto process_val;
}
"three" {
  tmp_val = 3;
  goto process_val;
}
"four" {
  tmp_val = 4;
  goto process_val;
}
"five" {
  tmp_val = 5;
  goto process_val;
}
"six" {
  tmp_val = 6;
  goto process_val;
}
"seven" {
  tmp_val = 7;
  goto process_val;
}
"eight" {
  tmp_val = 8;
  goto process_val;
}
"nine" {
  tmp_val = 9;
  goto process_val;
}
[\x00] {
  if (first_val <= 0 || first_val > 9)
  {
    throw std::runtime_error("invalid first val");
  }
  if (curr_val <= 0 || curr_val > 9)
  {
    throw std::runtime_error("invalid last val");
  }
  return first_val * 10 + curr_val;
}
* {
  goto parse_start;
}
*/
  goto done;

process_val:
  if (first_val <= 0) {
    first_val = tmp_val;
    curr_val = tmp_val;
  } else {
    curr_val = tmp_val;
  }
  goto parse_start;
  
done:
  // shouldn't get here
  throw std::runtime_error("done");
  return 0;
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  long long sum = 0;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    int tmp = parse_digits(line);
    sum += tmp;
  }
  std::cout << sum << std::endl;
}
