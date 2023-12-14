/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"
#include "hash.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace std {
template <> struct hash<std::pair<int, int>> {
  uint64_t operator()(const std::pair<int, int> &v) const {
    aoc::hasher hash_;
    hash_.append(reinterpret_cast<const uint8_t *>(&v.first), sizeof(int));
    hash_.append(reinterpret_cast<const uint8_t *>(&v.second), sizeof(int));
    hash_.finalize();
    return hash_.data[0];
  }
};
} // namespace std

void find_tile_loc(const char *YYCURSOR, int &x, int &y) {
  const char *YYMARKER;
  /*!stags:re2c format = 'const char *@@;'; */
parse_start:
  /*!re2c
  "se" {
--y;
++x;
goto parse_start;
  }
"sw" {
--x;
--y;
goto parse_start;
}
"nw" {
--x;
++y;
goto parse_start;
}
"ne" {
++x;
++y;
goto parse_start;
}
"e" {
x += 2;
goto parse_start;
}
"w" {
x -= 2;
goto parse_start;
}
[\x00] {
return;
}
* {
goto parse_start;
}
  */
}

void update(const std::unordered_set<std::pair<int, int>> &orig,
            std::unordered_set<std::pair<int, int>> &res,
            std::unordered_set<std::pair<int, int>> &work_buf) {
  res.clear();
  work_buf.clear();
  for (auto &v : orig) {
    // add v and all its neighbors
    work_buf.insert(v);
    std::pair<int, int> loc;
    // w
    loc.first = v.first - 2;
    loc.second = v.second;
    work_buf.insert(loc);
    // e
    loc.first = v.first + 2;
    loc.second = v.second;
    work_buf.insert(loc);
    // nw
    loc.first = v.first - 1;
    loc.second = v.second + 1;
    work_buf.insert(loc);
    // ne
    loc.first = v.first + 1;
    loc.second = v.second + 1;
    work_buf.insert(loc);
    // sw
    loc.first = v.first - 1;
    loc.second = v.second - 1;
    work_buf.insert(loc);
    // se
    loc.first = v.first + 1;
    loc.second = v.second - 1;
    work_buf.insert(loc);
  }
  // now perform update
  for (auto &v : work_buf) {
    int flipped_neighbors = 0;
    std::pair<int, int> loc;
    // w
    loc.first = v.first - 2;
    loc.second = v.second;
    flipped_neighbors += orig.find(loc) != orig.end();
    // e
    loc.first = v.first + 2;
    loc.second = v.second;
    flipped_neighbors += orig.find(loc) != orig.end();
    // nw
    loc.first = v.first - 1;
    loc.second = v.second + 1;
    flipped_neighbors += orig.find(loc) != orig.end();
    // ne
    loc.first = v.first + 1;
    loc.second = v.second + 1;
    flipped_neighbors += orig.find(loc) != orig.end();
    // sw
    loc.first = v.first - 1;
    loc.second = v.second - 1;
    flipped_neighbors += orig.find(loc) != orig.end();
    // se
    loc.first = v.first + 1;
    loc.second = v.second - 1;
    flipped_neighbors += orig.find(loc) != orig.end();
    if (orig.find(v) == orig.end()) {
      // originally white
      if (flipped_neighbors == 2) {
        res.insert(v);
      }
    } else {
      // originally black
      if (flipped_neighbors == 0 || flipped_neighbors > 2) {
        // becomes white
      } else {
        res.insert(v);
      }
    }
  }
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");

  std::ifstream in(in_path);
  std::string line;
  // coordinates are slighty rotated
  std::unordered_set<std::pair<int, int>> flipped;

  // parse input
  {
    while (true) {
      std::getline(in, line);
      if (in.eof()) {
        break;
      }
      if (line.empty()) {
        break;
      }
      std::pair<int, int> loc = {0, 0};
      find_tile_loc(line.data(), loc.first, loc.second);
      auto iter = flipped.find(loc);
      if (iter == flipped.end()) {
        flipped.emplace(loc);
      } else {
        flipped.erase(iter);
      }
    }
  }
  // run updater
  std::unordered_set<std::pair<int, int>> work_buf;
  for (size_t i = 0; i < 100; ++i) {
    std::unordered_set<std::pair<int, int>> res;
    update(flipped, res, work_buf);
    flipped = std::move(res);
  }
  std::cout << flipped.size() << std::endl;
}
