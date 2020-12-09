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

void part1(const std::vector<int> &code, size_t &pos, long long &val)
{
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
      pos = i;
      val = code[i];
      return;
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

int main(int argc, const char **argv)
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

  size_t pos = 0;
  long long target = 0;
  part1(code, pos, target);

  // part 2 algorithm:
  // start with a guess that the sequence starts at the beginning
  // find the longest sequence until it sums to > target (if equal, lucky and we're done)
  // if greater than, try removing the start
  // keep inching forward until the sequence end reaches pos, then re-start after pos

  size_t seq_start = 0;
  size_t seq_stop = 1;
  size_t curr_stop = pos;
  long long sum = code[0];
  do
  {
    if (sum > target)
    {
      sum -= code[seq_start];
      ++seq_start;
    }
    if (sum < target)
    {
      if (seq_stop == curr_stop)
      {
        // can't increase the sequence anymore
        if (curr_stop == code.size())
        {
          break;
        }
        else
        {
          seq_start = pos + 1;
          seq_stop = seq_start + 1;
          sum = code[seq_start];
        }
      }
      else
      {
        sum += code[seq_stop];
        ++seq_stop;
      }
    }
  } while (sum != target);

  if (sum == target)
  {
    auto extrema = std::minmax_element(code.begin() + seq_start, code.begin() + seq_stop);
    std::cout << *extrema.first + *extrema.second << std::endl;
  }
}
