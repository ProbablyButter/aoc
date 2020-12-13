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
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");

  std::ifstream in(in_path);
  std::string line;
  long long earliest_time;
  std::getline(in, line);
  earliest_time = atoll(line.data());

  std::vector<long long> times;
  std::getline(in, line);
  {
    size_t i = 0;
    size_t j = 0;
    for (; j < line.size(); ++j)
    {
      if (line[j] == ',')
      {
        if (line[i] == 'x')
        {
          // unavailable
        }
        else
        {
          times.emplace_back(atoll(line.data() + i));
        }
        i = j + 1;
      }
    }
    // last bus
    if (line[i] == 'x')
    {
      // unavailable
    }
    else
    {
      times.emplace_back(atoll(line.data() + i));
    }
  }
  int closest = 0;
  auto next_closest =
    (1 + earliest_time / times[closest]) * times[closest] - earliest_time;
  for (size_t i = 1; i < times.size(); ++i)
  {
    auto next = (1 + earliest_time / times[i]) * times[i] - earliest_time;
    if (next < next_closest)
    {
      closest = i;
      next_closest = next;
    }
  }
  std::cout << times[closest] << ", " << next_closest << ", "
            << times[closest] * next_closest << std::endl;
}
