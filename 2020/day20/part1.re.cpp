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
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <class T>
bool edge_matches(const T &tile1,
                  const T &tile2,
                  std::array<int, 4> &face1,
                  std::array<int, 4> &face2,
                  int len)
{
  for (size_t i = 0; i < len; ++i)
  {
    if (tile1[face1[1] + i * face1[3]][face1[0] + i * face1[2]] !=
        tile2[face2[1] + i * face2[3]][face2[0] + i * face2[2]])
    {
      return false;
    }
  }
  return true;
}

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");

  using Tile = std::vector<std::string>;

  std::vector<int> ids;
  std::vector<Tile> tiles;

  std::ifstream in(in_path);
  std::string line;

  // parse tiles
  {
    while (true)
    {
      std::getline(in, line);
      if (in.eof())
      {
        break;
      }
      if (line.empty())
      {
        continue;
      }
      if (line[0] == 'T')
      {
        ids.push_back(atoi(line.data() + 5));
        tiles.emplace_back();
      }
      else
      {
        tiles.back().emplace_back(line);
      }
    }
  }

  std::vector<int> corners;

  int width = tiles[0][0].size();
  std::array<int, 4> lface = {0, 0, 0, 1};
  std::array<int, 4> rev_lface = {0, width - 1, 0, -1};
  std::array<int, 4> rface = {width - 1, 0, 0, 1};
  std::array<int, 4> rev_rface = {width - 1, width - 1, 0, -1};
  std::array<int, 4> tface = {0, 0, 1, 0};
  std::array<int, 4> rev_tface = {width - 1, 0, -1, 0};
  std::array<int, 4> bface = {0, width - 1, 1, 0};
  std::array<int, 4> rev_bface = {width - 1, width - 1, -1, 0};
  std::vector<std::array<int, 4>> all_combos = {
    lface, rev_lface, rface, rev_rface, tface, rev_tface, bface, rev_bface};
  std::vector<std::array<int, 4>> all_faces = {lface, rface, tface, bface};
  // corner pieces will have 2 edges
  // figure out the corner pieces
  for (size_t i = 0; i < tiles.size(); ++i)
  {
    int num_matches = 0;
    for (size_t j = 0; j < tiles.size(); ++j)
    {
      if (i == j)
      {
        continue;
      }
      for (auto &fi : all_combos)
      {
        for (auto &fj : all_faces)
        {
          if (edge_matches(tiles[i], tiles[j], fi, fj, width))
          {
            ++num_matches;
          }
        }
      }
    }
    if (num_matches == 2)
    {
      corners.push_back(i);
    }
  }
  long long res = 1;
  for (auto &v : corners)
  {
    res *= ids[v];
  }
  std::cout << res << std::endl;
}
