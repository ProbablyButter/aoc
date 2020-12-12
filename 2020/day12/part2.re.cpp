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
  double pos_x = 0;
  double pos_y = 0;
  // always relative to the ship
  double waypoint_x = 10;
  double waypoint_y = 1;
  
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
      waypoint_y += arg;
      break;
    case 'S':
      waypoint_y -= arg;
      break;
    case 'E':
      waypoint_x += arg;
      break;
    case 'W':
      waypoint_x -= arg;
      break;
    case 'L':
      // rotate waypoint about pos
      {
        double new_x = waypoint_x * cos(arg * 0.017453292519943295) - waypoint_y * sin(arg * 0.017453292519943295);
        double new_y = waypoint_x * sin(arg * 0.017453292519943295) + waypoint_y * cos(arg * 0.017453292519943295);
        waypoint_x = new_x;
        waypoint_y = new_y;
      }
      break;
    case 'R':
      {
        double new_x = waypoint_x * cos(-arg * 0.017453292519943295) - waypoint_y * sin(-arg * 0.017453292519943295);
        double new_y = waypoint_x * sin(-arg * 0.017453292519943295) + waypoint_y * cos(-arg * 0.017453292519943295);
        waypoint_x = new_x;
        waypoint_y = new_y;
      }
      break;
    case 'F':
      {
        pos_x += arg * waypoint_x;
        pos_y += arg * waypoint_y;
      }
      break;
    }
  }
  std::cout << fabs(pos_x) + fabs(pos_y) << std::endl;
}
