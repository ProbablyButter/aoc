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
#include <numeric>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename T>
static void apply_permutation_in_place(std::vector<T> &vec, const std::vector<size_t> &p)
{
  std::vector<bool> done(vec.size());
  for (size_t i = 0; i < vec.size(); ++i)
  {
    if (done[i])
    {
      continue;
    }
    done[i] = true;
    size_t prev_j = i;
    size_t j = p[i];
    while (i != j)
    {
      std::swap(vec[prev_j], vec[j]);
      done[j] = true;
      prev_j = j;
      j = p[j];
    }
  }
}

long long part2_recurse(std::vector<long long> &pos,
                        std::vector<long long> &times,
                        size_t i = 0,
                        long long mult = 0,
                        long long off = 0)
{
  if (mult == 0)
  {
    mult = times[0];
  }
  for (long long n = 0; true; ++n)
  {
    long long t = mult * n - pos[0] + off;
    int num_success = i + 1;
    for (size_t j = i + 1; j < times.size(); ++j)
    {
      if ((t + pos[j]) % times[j] != 0)
      {
        break;
      }
      ++num_success;
    }
    if (num_success == times.size())
    {
      return t;
    }
    if (num_success > i + 1)
    {
      off = t + pos[0];
      mult = times[0];
      for (size_t j = 1; j < num_success; ++j)
      {
        mult = std::lcm(mult, times[j]);
      }
      std::cout << "recursing " << num_success - 1 << std::endl;
      return part2_recurse(pos, times, num_success - 1, mult, off);
    }
  }
  return 0;
}

long long part2_improved(const std::vector<long long> &pos,
                         const std::vector<long long> &times)
{
  long long t = times[0] - pos[0];
  long long mult = times[0];
  while (t < 0)
  {
    t += mult;
  }
  size_t num_success = 1;
  while (true)
  {
    size_t next_success = num_success;
    for (size_t j = num_success; j < times.size(); ++j)
    {
      if ((t + pos[j]) % times[j] != 0)
      {
        break;
      }
      ++next_success;
    }
    if (next_success == times.size())
    {
      return t;
    }

    if (next_success > num_success)
    {
      for (size_t j = num_success; j < next_success; ++j)
      {
        mult *= times[j];
      }
      num_success = next_success;
    }
    t += mult;
  }
}

long long part2_naive(const std::vector<long long> &pos,
                      const std::vector<long long> &times)
{
  long long t = times[0] - pos[0];
  while (t < 0)
  {
    t += times[0];
  }
  while (true)
  {
    t += times[0];
    bool success = true;
    for (size_t i = 1; i < times.size(); ++i)
    {
      if ((t + pos[i]) % times[i] != 0)
      {
        success = false;
        break;
      }
    }
    if (success)
    {
      return t;
    }
  }
}

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");

  std::ifstream in(in_path);
  std::string line;
  long long earliest_time;
  std::getline(in, line);
  earliest_time = atoll(line.data());

  std::vector<long long> pos;
  std::vector<long long> times;
  std::getline(in, line);
  {
    long long id = 0;
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
          pos.emplace_back(id);
        }
        i = j + 1;
        ++id;
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
      pos.emplace_back(id);
    }
  }

  // sort times by largest to smallest, only really """useful"" for naive implementation
#if 0
  {
    std::vector<size_t> idcs;
    idcs.reserve(times.size());
    for (size_t i = 0; i < times.size(); ++i)
    {
      idcs.push_back(i);
    }
    std::sort(
      idcs.begin(), idcs.end(), [&times](auto a, auto b) { return times[a] > times[b]; });
    apply_permutation_in_place(times, idcs);
    apply_permutation_in_place(pos, idcs);
    for (size_t i = 0; i < times.size(); ++i)
    {
      std::cout << times[i] << ", " << pos[i] << std::endl;
    }
  }
#endif

  // std::cout << part2_naive(pos, times) << std::endl;
  // std::cout << part2_recurse(pos, times, 0, times[0], times[0]) << std::endl;
  std::cout << part2_improved(pos, times) << std::endl;
  return 0;
}
