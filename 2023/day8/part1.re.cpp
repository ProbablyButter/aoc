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
  std::string directions;
  std::getline(in, directions);
  std::string line;
  std::getline(in, line);
  std::map<std::string, std::string> left_moves;
  std::map<std::string, std::string> right_moves;
  std::string curr = "AAA";
  std::string dst = "ZZZ";
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line, " = ");
    auto l = tmp[1].substr(1, 3);
    auto r = tmp[1].substr(1 + 3 + 2, 3);
    left_moves.emplace(tmp[0], l);
    right_moves.emplace(tmp[0], r);
  }

  size_t pos = 0;

  size_t steps = 0;

  while (curr != dst) {
    ++steps;
    switch(directions[pos])
    {
    case 'L':
      curr = left_moves.at(curr);
      break;
    case 'R':
      curr = right_moves.at(curr);
      break;
    }
    pos = (pos + 1) % directions.size();
  }
  std::cout << steps << std::endl;
}
