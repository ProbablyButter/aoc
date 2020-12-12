/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <cmath>

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");

  std::ifstream in(in_path);
  std::string line;
  // east is 0
  int facing = 0;
  double pos_x = 0;
  double pos_y = 0;
  while (true)
  {
    std::getline(in, line);
    if (in.eof())
    {
      break;
    }
    int arg = atoi(&line[1]);
    switch (line[0])
    {
    case 'N':
      pos_y += arg;
      break;
    case 'S':
      pos_y -= arg;
      break;
    case 'E':
      pos_x += arg;
      break;
    case 'W':
      pos_x -= arg;
      break;
    case 'L':
      facing += arg;
      break;
    case 'R':
      facing -= arg;
      break;
    case 'F':
      pos_x += arg * cos(facing * 0.017453292519943295);
      pos_y += arg * sin(facing * 0.017453292519943295);
      break;
    }
  }
  std::cout << fabs(pos_x) + fabs(pos_y) << std::endl;
}
