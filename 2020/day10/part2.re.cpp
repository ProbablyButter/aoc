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
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

long long count_ways(const std::vector<int> &adapters, size_t start, size_t stop)
{
  // algorithm:
  // build an implicit dag from start to stop
  // count how many ways to get from start to stop-1
  if (start + 1 >= stop)
  {
    return 1;
  }
  long long count = 0;
  for (size_t i = start + 1; i < stop; ++i)
  {
    if (adapters[start] + 3 >= adapters[i])
    {
      // valid path for traversal
      count += count_ways(adapters, i, stop);
    }
    else
    {
      break;
    }
  }

  return count;
}

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");

  std::ifstream in(in_path);
  std::vector<int> adapters = {0};
  std::string line;
  while (true)
  {
    std::getline(in, line);
    if (in.eof())
    {
      break;
    }
    int v = atoi(line.data());
    adapters.emplace_back(v);
  }
  // chain is just the sorted list of adapters
  std::sort(adapters.begin(), adapters.end());
  adapters.emplace_back(adapters.back() + 3);

  // break up adapters at points there is a 3 jolt gap between adapters i and i+1
  // count the number of ways to get from the start of each section to the end of each
  // section multiply all these values together
  long long num_ways = 1;
  size_t pos = 0;
  while (true)
  {
    size_t curr_end = pos + 1;
    while (curr_end < adapters.size() && adapters[curr_end] != adapters[curr_end - 1] + 3)
    {
      ++curr_end;
    }
    long long res = count_ways(adapters, pos, curr_end);
    num_ways *= res;
    pos = curr_end;
    if (pos >= adapters.size())
    {
      break;
    }
  }
  std::cout << num_ways << std::endl;
}
