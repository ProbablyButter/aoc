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
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct array_comparator
{
  bool operator()(const std::array<int, 4> &a, const std::array<int, 4> &b) const
  {
    for (int i = 0; i < a.size(); ++i)
    {
      if (a[i] != b[i])
      {
        return a[i] < b[i];
      }
    }
    // equal
    return false;
  }
};

// TODO: how to determine bounds?
template <class T, class V>
void sim_4d(const T &init,
            V &res,
            int &min_x,
            int &max_x,
            int &min_y,
            int &max_y,
            int &min_z,
            int &max_z,
            int &min_w,
            int &max_w)
{
  res.clear();
  // expand out
  --min_x;
  ++max_x;
  --min_y;
  ++max_y;
  --min_z;
  ++max_z;
  --min_w;
  ++max_w;
  for (int w = min_w; w < max_w; ++w)
  {
    for (int z = min_z; z < max_z; ++z)
    {
      for (int y = min_y; y < max_y; ++y)
      {
        for (int x = min_x; x < max_x; ++x)
        {
          std::array<int, 4> pos;
          int num_neighbors = 0;
          for (int i = -1; i <= 1; ++i)
          {
            for (int j = -1; j <= 1; ++j)
            {
              for (int k = -1; k <= 1; ++k)
              {
                for (int l = -1; l <= 1; ++l)
                {
                  if (!(i == 0 && j == 0 && k == 0 && l == 0))
                  {
                    pos[0] = x + i;
                    pos[1] = y + j;
                    pos[2] = z + k;
                    pos[3] = w + l;
                    if (init.find(pos) != init.end())
                    {
                      // active
                      ++num_neighbors;
                    }
                  }
                }
              }
            }
          }
          pos[0] = x;
          pos[1] = y;
          pos[2] = z;
          pos[3] = w;
          bool active = (init.find(pos) != init.end());
          if (active)
          {
            if (num_neighbors == 2 || num_neighbors == 3)
            {
              // remain active
              res.insert(pos);
            }
          }
          else
          {
            if (num_neighbors == 3)
            {
              // become active
              res.insert(pos);
            }
          }
        }
      }
    }
  }
}

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");

  std::ifstream in(in_path);
  std::string line;
  std::set<std::array<int, 4>, array_comparator> active1;
  std::set<std::array<int, 4>, array_comparator> active2;

  int min_x = 0;
  int max_x = 1;
  int min_y = 0;
  int max_y = 1;
  int min_z = 0;
  int max_z = 1;
  int min_w = 0;
  int max_w = 1;

  {
    size_t i = 0;
    while (true)
    {
      std::getline(in, line);
      if (in.eof())
      {
        break;
      }
      for (size_t j = 0; j < line.size(); ++j)
      {
        if (line[j] == '#')
        {
          std::array<int, 4> point = {(int)i, (int)j, 0, 0};
          active1.emplace(point);
        }
      }
      max_y = std::max<int>(max_y, line.size());
      ++i;
    }
    max_x = i;
  }
  for (size_t i = 0; i < 6; ++i)
  {
    sim_4d(active1, active2, min_x, max_x, min_y, max_y, min_z, max_z, min_w, max_w);
    std::swap(active1, active2);
  }

  std::cout << active1.size() << std::endl;
}
