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

void rot_ccw(std::vector<std::string> &tile)
{
  // diagonal going in
  for (size_t i = 0; i < tile.size() / 2; ++i)
  {
    for (size_t j = i; j < tile.size() - i - 1; ++j)
    {
      auto tmp0 = tile[i][j];
      tile[i][j] = tile[j][tile.size() - 1 - i];
      tile[j][tile.size() - 1 - i] = tile[tile.size() - 1 - i][tile.size() - 1 - j];
      tile[tile.size() - 1 - i][tile.size() - 1 - j] = tile[tile.size() - 1 - j][i];
      tile[tile.size() - 1 - j][i] = tmp0;
    }
  }
}

void flip_x(std::vector<std::string> &tile)
{
  for (auto &v : tile)
  {
    std::reverse(v.begin(), v.end());
  }
}

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

int mark_seamonsters(std::vector<std::string> &image,
                     const std::vector<std::string> &monster,
                     const std::vector<std::pair<int, int>> &positions)
{
  int num_found = 0;
  for (size_t i = 0; i < image.size() - monster.size(); ++i)
  {
    for (size_t j = 0; j < image[0].size() - monster[0].size(); ++j)
    {
      bool matches = true;
      for (auto &p : positions)
      {
        if (image[i + p.first][j + p.second] != '#')
        {
          matches = false;
          break;
        }
      }
      if (matches)
      {
        ++num_found;
        for (auto &p : positions)
        {
          image[i + p.first][j + p.second] = 'o';
        }
      }
    }
  }
  return num_found;
}

int count_roughness(const std::vector<std::string> &image)
{
  int count = 0;
  for (auto &l : image)
  {
    for (auto c : l)
    {
      count += (c == '#');
    }
  }
  return count;
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
  std::vector<int> corners;
  std::vector<int> edges;
  std::vector<Tile> tiles_copy = tiles;
  neighbors.resize(tiles.size());

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
      for (auto &fi : all_combos)
      {
        for (auto &fj : all_faces)
        {
          if (edge_matches(tiles[i], tiles[j], fi, fj, width))
          {
            neighbors[i].push_back(j);
          }
        }
      }
    }
    if (neighbors[i].size() == 2)
    {
      corners.push_back(i);
    }
    else if (neighbors[i].size() == 3)
    {
      edges.push_back(i);
    }
  }
  // now problem is reduced, start from one corner piece and expand outwards
  // assume first corner is the top-left
  // 8 possible orientations: original, rot90, rot180, rot270, flip_x, flip_x+rot90,
  // flip_x+rot180, flip_x+rot270
  std::vector<int> order;
  order.resize(tiles.size());
  int ntile_width = sqrt(tiles.size());
  order[0] = corners[0];

  // first tile needs to be orientated
  {
    // 3 rotations
    for (size_t i = 0; i < 4; ++i)
    {
      for (auto &fi : all_combos)
      {
        for (auto &fj : all_combos)
        {
          // is first neighbor is to the right?
          if (edge_matches(
                tiles[order[0]], tiles[neighbors[order[0]][0]], rface, fi, width) &&
              edge_matches(
                tiles[order[0]], tiles[neighbors[order[0]][1]], bface, fj, width))
          {
            order[1] = neighbors[order[0]][0];
            order[ntile_width] = neighbors[order[0]][1];
            goto oriented;
          }
          // is first neighbor is to the bottom?
          if (edge_matches(
                tiles[order[0]], tiles[neighbors[order[0]][0]], bface, fi, width) &&
              edge_matches(
                tiles[order[0]], tiles[neighbors[order[0]][1]], rface, fj, width))
          {
            order[1] = neighbors[order[0]][1];
            order[ntile_width] = neighbors[order[0]][0];
            goto oriented;
          }
        }
      }
      rot_ccw(tiles[order[0]]);
    }
    // flip x
    flip_x(tiles[order[0]]);
    // 4 rotations
    for (size_t i = 0; i < 4; ++i)
    {
      for (auto &fi : all_combos)
      {
        for (auto &fj : all_combos)
        {
          // is first neighbor is to the right?
          if (edge_matches(
                tiles[order[0]], tiles[neighbors[order[0]][0]], rface, fi, width) &&
              edge_matches(
                tiles[order[0]], tiles[neighbors[order[0]][1]], bface, fj, width))
          {
            order[1] = neighbors[order[0]][0];
            order[ntile_width] = neighbors[order[0]][1];
            goto oriented;
          }
          // is first neighbor is to the bottom?
          if (edge_matches(
                tiles[order[0]], tiles[neighbors[order[0]][0]], bface, fi, width) &&
              edge_matches(
                tiles[order[0]], tiles[neighbors[order[0]][1]], rface, fj, width))
          {
            order[1] = neighbors[order[0]][1];
            order[ntile_width] = neighbors[order[0]][0];
            goto oriented;
          }
        }
      }
      rot_ccw(tiles[order[0]]);
    }
  }
  throw std::runtime_error("couldn't orient first tile");
oriented:
  for (size_t i = 1; i < order.size(); ++i)
  {
    // orient tile[order[i]]
    int orient_idx = (i % ntile_width == 0) ? (order[i - ntile_width]) : (order[i - 1]);
    auto &orient_to = tiles[orient_idx];
    auto &orig_face = (i % ntile_width == 0) ? bface : rface;
    auto &new_face = (i % ntile_width == 0) ? tface : lface;

    for (size_t j = 0; j < 4; ++j)
    {
      if (edge_matches(orient_to, tiles[order[i]], orig_face, new_face, width))
      {
        goto next;
      }
      rot_ccw(tiles[order[i]]);
    }
    flip_x(tiles[order[i]]);
    for (size_t j = 0; j < 4; ++j)
    {
      if (edge_matches(orient_to, tiles[order[i]], orig_face, new_face, width))
      {
        goto next;
      }
      rot_ccw(tiles[order[i]]);
    }
    if (edge_matches(orient_to, tiles[order[i]], orig_face, new_face, width))
    {
      goto next;
    }
    throw std::runtime_error("failed to align neighbor tile");
  next:
    // figure out which tile belongs to the right/below
    for (auto &n : neighbors[order[i]])
    {
      for (auto &f : all_combos)
      {
        if (edge_matches(tiles[order[i]], tiles[n], rface, f, width))
        {
          order[i + 1] = n;
        }
        if (edge_matches(tiles[order[i]], tiles[n], bface, f, width))
        {
          order[i + ntile_width] = n;
        }
      }
    }
  }

  // combine everything into a single image
  // trim off edges
  for (auto &tile : tiles)
  {
    tile.erase(tile.begin());
    tile.erase(tile.end() - 1);
    for (auto &l : tile)
    {
      l = l.substr(1, l.size() - 2);
    }
  }
  width -= 2;

  std::vector<std::string> image;
  image.resize(ntile_width * width);
  for (size_t i = 0; i < ntile_width; ++i)
  {
    for (size_t k = 0; k < tiles[0].size(); ++k)
    {
      for (size_t j = 0; j < ntile_width; ++j)
      {
        image[i * tiles[0].size() + k] += tiles[order[i * ntile_width + j]][k];
      }
    }
  }

  // now can finally start looking for seamonsters
  std::vector<std::string> monster = {
    "                  # ", "#    ##    ##    ###", " #  #  #  #  #  #   "};
  std::vector<std::pair<int, int>> positions;
  for (size_t i = 0; i < monster.size(); ++i)
  {
    for (size_t j = 0; j < monster[i].size(); ++j)
    {
      if (monster[i][j] == '#')
      {
        positions.emplace_back(i, j);
      }
    }
  }
  for (size_t i = 0; i < 4; ++i)
  {
    auto copy = image;
    if (mark_seamonsters(copy, monster, positions))
    {
      std::cout << count_roughness(copy) << std::endl;
      return 0;
    }
    rot_ccw(image);
  }
  flip_x(image);
  for (size_t i = 0; i < 4; ++i)
  {
    auto copy = image;
    if (mark_seamonsters(copy, monster, positions))
    {
      std::cout << count_roughness(copy) << std::endl;
      return 0;
    }
    rot_ccw(image);
  }
}
