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

void check_ray(std::vector<std::string> &seating,
               long row,
               long col,
               long row_inc,
               long col_inc,
               long &occ_count,
               long &free_count)
{
  long i = row + row_inc;
  long j = col + col_inc;
  while (i >= 0 && i < seating.size() && j >= 0 && j < seating[i].size())
  {
    if (seating[i][j] == '#')
    {
      ++occ_count;
      return;
    }
    if (seating[i][j] == 'L')
    {
      ++free_count;
      return;
    }
    i += row_inc;
    j += col_inc;
  }
}

bool step(std::vector<std::string> &seating, std::vector<std::string> &out)
{
  bool changed = false;
  for (long i = 0; i < seating.size(); ++i)
  {
    for (long j = 0; j < seating[i].size(); ++j)
    {
      // cout how many empty next to seat
      long occ_count = 0;
      long free_count = 0;
      check_ray(seating, i, j, -1, 0, occ_count, free_count);
      check_ray(seating, i, j, 1, 0, occ_count, free_count);
      check_ray(seating, i, j, 0, -1, occ_count, free_count);
      check_ray(seating, i, j, 0, 1, occ_count, free_count);
      
      check_ray(seating, i, j, -1, -1, occ_count, free_count);
      check_ray(seating, i, j, 1, 1, occ_count, free_count);
      check_ray(seating, i, j, -1, 1, occ_count, free_count);
      check_ray(seating, i, j, 1, -1, occ_count, free_count);

      if (seating[i][j] == 'L' && occ_count == 0)
      {
        out[i][j] = '#';
        changed = true;
      }
      else if (seating[i][j] == '#' && occ_count >= 5)
      {
        out[i][j] = 'L';
        changed = true;
      }
      else
      {
        out[i][j] = seating[i][j];
      }
    }
  }
  return changed;
}

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");

  std::ifstream in(in_path);
  std::string line;
  std::vector<std::string> seating;
  while (true)
  {
    std::getline(in, line);
    if (in.eof())
    {
      break;
    }
    seating.push_back(line);
  }
  std::vector<std::string> seating2 = seating;
  while (step(seating, seating2))
  {
    seating = seating2;
  }
  seating = seating2;
  long occ_count = 0;
  for (size_t i = 0; i < seating.size(); ++i)
  {
    for (size_t j = 0; j < seating[i].size(); ++j)
    {
      occ_count += (seating[i][j] == '#');
    }
  }
  std::cout << occ_count << std::endl;
}
