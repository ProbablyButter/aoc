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

bool step(std::vector<std::string>& seating, std::vector<std::string>& out)
{
  bool changed = false;
  for (long i = 0; i < seating.size(); ++i)
  {
    for (long j = 0; j < seating[i].size(); ++j)
    {
      // cout how many empty next to seat
      long occ_count = 0;
      long free_count = 0;
      for (long k = -1; k <= 1; ++k)
      {
        for(long l = -1; l <= 1; ++l)
        {
          long row = i + k;
          long col = j + l;
          if (row >= 0 && row < seating.size() && col >= 0 && col < seating[row].size() &&
              !(k == 0 && l == 0))
          {
            occ_count += (seating[row][col] == '#');
            free_count += (seating[row][col] == 'L');
          }
        }
      }
      if(seating[i][j] == 'L' && occ_count == 0)
      {
        out[i][j] = '#';
        changed = true;
      }
      else if(seating[i][j] == '#' && occ_count >= 4)
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
