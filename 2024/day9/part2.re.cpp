/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"
#include "geometry_tools.hpp"
#include "graph_tools.hpp"
#include "string_tools.hpp"

#include <algorithm>
#include <charconv>
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

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string data(std::istreambuf_iterator<char>{in},
                   std::istreambuf_iterator<char>{});
  int id = 0;
  std::vector<int> disk;
  for (int i = 0; i < data.size(); ++i) {
    int val = data[i] - '0';
    if (i % 2 == 0) {
      // file
      for (int j = 0; j < val; ++j) {
        disk.emplace_back(id);
      }
      ++id;
    } else {
      // freespace
      for (int j = 0; j < val; ++j) {
        disk.emplace_back(-1);
      }
    }
  }
  // now compact
  int to_move = id - 1;
  for (int to_move = id - 1; to_move >= 0; --to_move) {
    // first find where to_move starts and ends
    int fstart = 0;
    for (; fstart < disk.size(); ++fstart) {
      if (disk[fstart] == to_move) {
        break;
      }
    }
    int fstop = fstart;
    for (; fstop < disk.size(); ++fstop) {
      if (disk[fstop] != to_move) {
        break;
      }
    }
    int free_start = 0;
    while (true) {
      if (free_start >= fstart) {
        break;
      }
      if (disk[free_start] < 0) {
        int free_stop = free_start;
        for (; free_stop < disk.size(); ++free_stop) {
          if (disk[free_stop] >= 0) {
            break;
          }
        }
        if (free_stop - free_start >= fstop - fstart) {
          // can move
          for (int i = 0; i < fstop - fstart; ++i) {
            disk[free_start + i] = to_move;
            disk[fstart + i] = -1;
          }
          break;
        }
        else
        {
          // can't move
          free_start = free_stop;
        }
      } else {
        ++free_start;
      }
    }
  }

  long long checksum = 0;
  for (int i = 0; i < disk.size(); ++i) {
    if (disk[i] >= 0) {
      checksum += disk[i] * i;
    }
  }
  std::cout << checksum << std::endl;
}
