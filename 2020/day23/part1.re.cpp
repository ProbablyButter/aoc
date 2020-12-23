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

void advance(std::vector<int> &cups, int min, int max)
{
  // assume curr_idx is always 0, at end rotate cups to make sure this is so
  int dest_val = cups.front() - 1;
  if (dest_val < min)
  {
    dest_val = max;
  }
  while (dest_val == cups[1] || dest_val == cups[2] || dest_val == cups[3])
  {
    --dest_val;
    if (dest_val < min)
    {
      dest_val = max;
    }
  }
  // find dest_val in cups
  auto dest_pos = std::find(cups.begin(), cups.end(), dest_val);
  // extract 3 cups then move the rest of the range into position
  std::rotate(cups.begin() + 1, cups.begin() + 4, dest_pos + 1);
  // select next current cup
  // TODO: anyway to simplify this?
  std::rotate(cups.begin(), cups.begin() + 1, cups.end());
}

int main(int argc, char **argv)
{
  std::vector<int> cups = {2, 5, 3, 1, 4, 9, 8, 6, 7};
  int min_val, max_val;
  {
    auto minmax = std::minmax_element(cups.begin(), cups.end());
    min_val = *minmax.first;
    max_val = *minmax.second;
  }

  for (size_t i = 0; i < 100; ++i)
  {
    advance(cups, min_val, max_val);
  }

  // move cup 1 to the front
  {
    auto iter = std::find(cups.begin(), cups.end(), 1);
    std::rotate(cups.begin(), iter, cups.end());
  }

  for (auto v : cups)
  {
    std::cout << v;
  }
  std::cout << std::endl;
}
