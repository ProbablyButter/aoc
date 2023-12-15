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

long long count_ways(const std::vector<int> &adapters,
                     std::vector<long long> &lookup,
                     size_t start,
                     size_t stop)
{
  // traverse every path, but use caching
  if (lookup[start] == 0)
  {
    long long count = 0;
    for (size_t i = start + 1; i < stop; ++i)
    {
      if (adapters[start] + 3 >= adapters[i])
      {
        // valid path for traversal
        count += count_ways(adapters, lookup, i, stop);
      }
      else
      {
        break;
      }
    }
    lookup[start] = count;
    return count;
  }
  else
  {
    return lookup[start];
  }
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
  std::sort(adapters.begin(), adapters.end());
  adapters.emplace_back(adapters.back() + 3);

  std::vector<long long> lookup;
  lookup.resize(adapters.size(), 0);
  lookup.back() = 1;
  long long num_ways = count_ways(adapters, lookup, 0, adapters.size());
  std::cout << num_ways << std::endl;
}
