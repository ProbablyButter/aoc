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

  std::vector<std::vector<int>> neighbors;
  std::vector<std::vector<std::pair<int, int>>> match_order;
  std::vector<int> corners;
  std::vector<int> edges;
  neighbors.resize(tiles.size());
  match_order.resize(tiles.size());

  int width = tiles[0][0].size();
  std::array<int, 4> lface = {0, 0, 0, 1};
  std::array<int, 4> rface = {width - 1, 0, 0, 1};
  std::array<int, 4> tface = {0, 0, 1, 0};
  std::array<int, 4> bface = {0, width - 1, 1, 0};
  std::array<int, 4> rev_lface = {0, width - 1, 0, -1};
  std::array<int, 4> rev_rface = {width - 1, width - 1, 0, -1};
  std::array<int, 4> rev_tface = {width - 1, 0, -1, 0};
  std::array<int, 4> rev_bface = {width - 1, width - 1, -1, 0};
  std::vector<std::array<int, 4>> all_combos = {
    lface, rface, tface, bface, rev_lface, rev_rface, rev_tface, rev_bface};
  std::vector<std::array<int, 4>> all_faces = {lface, rface, tface, bface};
  // map all neighbors
  for (size_t i = 0; i < tiles.size(); ++i)
  {
    for (size_t j = 0; j < tiles.size(); ++j)
    {
      if (i == j)
      {
        continue;
      }
      for(size_t k = 0; k < all_combos.size(); ++k)
      {
        auto& fi = all_combos[k];
        for (size_t l = 0; l < all_faces.size(); ++l)
        {
          auto& fj = all_faces[l];
          if (edge_matches(tiles[i], tiles[j], fi, fj, width))
          {
            neighbors[i].push_back(j);
            match_order[i].emplace_back(k, l);
          }
        }
      }
    }
    if (match_order[i].size() == 2)
    {
      corners.push_back(i);
    }
    else if (match_order[i].size() == 3)
    {
      edges.push_back(i);
    }
  }
  // now problem is reduced, start from one corner piece and expand outwards
  // assume first corner is the top-left
  std::vector<int> order(neighbors.size(), -1);
  order[0] = corners[0];
  // orientate top-left so it has the two edges appropriately matched
  
}
