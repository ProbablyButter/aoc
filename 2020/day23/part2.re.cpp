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
#include <chrono>
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

void advance(std::unordered_map<int, int> &cups, int &curr_sel, int min, int max)
{
  // extract 3 elements after curr_sel
  std::array<int, 3> removed;
  removed[0] = cups.at(curr_sel);
  removed[1] = cups.at(removed[0]);
  removed[2] = cups.at(removed[1]);
  // update curr_sel's next entry
  cups[curr_sel] = cups.at(removed[2]);
  // now find the destination
  int dest = curr_sel - 1;
  if (dest < min)
  {
    dest = max;
  }
  while (dest == removed[0] || dest == removed[1] || dest == removed[2])
  {
    --dest;
    if (dest < min)
    {
      dest = max;
    }
  }
  // insert removed elements into the destination
  cups[removed[2]] = cups.at(dest);
  cups[dest] = removed[0];
  cups[removed[0]] = removed[1];
  cups[removed[1]] = removed[2];
  // select cup after curr_sel
  curr_sel = cups.at(curr_sel);
}

int main(int argc, char **argv)
{
  std::unordered_map<int, int> cups = {
    {2, 5}, {5, 3}, {3, 1}, {1, 4}, {4, 9}, {9, 8}, {8, 6}, {6, 7}};
  int min_val = 1, max_val = 1000000;
  {
    int val = 7;
    for (int i = 10; i <= max_val; ++i)
    {
      cups.emplace(val, i);
      val = i;
    }
    // wrap-around point
    cups.emplace(max_val, 2);
  }

  int curr_sel = 2;

  for (size_t i = 0; i < 10000000; ++i)
  {
    advance(cups, curr_sel, min_val, max_val);
  }
  long long v1 = cups.at(1);
  long long v2 = cups.at(v1);
  std::cout << v1 << ", " << v2 << ", " << v1 * v2 << std::endl;
}
