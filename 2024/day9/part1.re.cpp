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
  int front = 0;
  int back = disk.size() - 1;
  while (front != back) {
    // find a free space
    if (disk[back] >= 0) {
      if (disk[front] < 0) {
        std::swap(disk[back], disk[front]);
        --back;
      }
      ++front;
    } else {
      --back;
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
