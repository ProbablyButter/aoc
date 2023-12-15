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

  int one_above = 0;
  int two_above = 0;
  int three_above = 0;
  for (size_t i = 1; i < adapters.size(); ++i)
  {
    one_above += ((adapters[i] - adapters[i-1]) == 1);
    two_above += ((adapters[i] - adapters[i-1]) == 2);
    three_above += ((adapters[i] - adapters[i-1]) == 3);
  }
  std::cout << one_above << ", " << two_above << ", " << three_above << ": "
            << adapters.size() << std::endl;
  std::cout << one_above * three_above << std::endl;
}
