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

void find_tile_loc(const char *YYCURSOR, int &x, int &y)
{
  const char *YYMARKER;
  /*!stags:re2c format = 'const char *@@;'; */
parse_start:
  /*!re2c
  "se" {
--y;
++x;
goto parse_start;
  }
"sw" {
--x;
--y;
goto parse_start;
}
"nw" {
--x;
++y;
goto parse_start;
}
"ne" {
++x;
++y;
goto parse_start;
}
"e" {
x += 2;
goto parse_start;
}
"w" {
x -= 2;
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

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");

  std::ifstream in(in_path);
  std::string line;
  // coordinates are slighty rotated
  std::set<std::pair<int, int>> flipped;

  // parse input
  {
    while (true)
    {
      std::getline(in, line);
      if (in.eof())
      {
        break;
      }
      if (line.empty())
      {
        break;
      }
      std::pair<int, int> loc = {0, 0};
      find_tile_loc(line.data(), loc.first, loc.second);
      auto iter = flipped.find(loc);
      if (iter == flipped.end())
      {
        flipped.emplace(loc);
      }
      else
      {
        flipped.erase(iter);
      }
    }
  }
  std::cout << flipped.size() << std::endl;
}
