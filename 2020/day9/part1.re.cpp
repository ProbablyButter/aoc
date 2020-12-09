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

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");

  std::ifstream in(in_path);
  std::vector<int> code;
  std::string line;
  while (true)
  {
    std::getline(in, line);
    if (in.eof())
    {
      break;
    }
    code.push_back(atoi(line.data()));
  }

  std::unordered_map<long long, int> sums;
  for (size_t i = 0; i < 25; ++i)
  {
    for (size_t j = i + 1; j < 25; ++j)
    {
      auto v = sums.emplace(code[i] + code[j], 1);
      if (!v.second)
      {
        ++(*v.first).second;
      }
    }
  }

  for (size_t i = 25; i < code.size(); ++i)
  {
    if (sums.find(code[i]) == sums.end())
    {
      std::cout << i << ": " << code[i] << std::endl;
      break;
    }
    for (size_t j = 1; j < 25; ++j)
    {
      // add new code
      {
        auto v = sums.emplace(code[i] + code[i - j], 1);
        if (!v.second)
        {
          ++(*v.first).second;
        }
      }
      // remove old code
      auto iter = sums.find(code[i - 25] + code[i - 25 + j]);
      --(iter->second);
      if (iter->second == 0)
      {
        sums.erase(iter);
      }
    }
  }
}
