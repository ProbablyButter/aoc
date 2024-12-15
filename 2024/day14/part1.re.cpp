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
#include "matrix.hpp"
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
  std::string line;
  std::vector<std::pair<int, int>> pos;
  std::vector<std::pair<int, int>> vel;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line);
    {
      auto ptmp = aoc::split(tmp[0], ",");
      pos.emplace_back(aoc::to_int<int>(ptmp[0].data() + 2),
                       aoc::to_int<int>(ptmp[1].data()));
    }
    {
      auto vtmp = aoc::split(tmp[1], ",");
      vel.emplace_back(aoc::to_int<int>(vtmp[0].data() + 2),
                       aoc::to_int<int>(vtmp[1].data()));
    }
  }
  int height = 103;
  int width = 101;
  // height = 7;
  // width = 11;
  int sim_time = 100;
  for (int step = 0; step < sim_time; ++step) {
    for (int i = 0; i < pos.size(); ++i) {
      pos[i].first += vel[i].first;
      pos[i].second += vel[i].second;
      while (pos[i].first < 0) {
        pos[i].first += width;
      }
      while (pos[i].second < 0) {
        pos[i].second += height;
      }
      pos[i].first = pos[i].first % width;
      pos[i].second = pos[i].second % height;
    }
  }
  // aoc::matrix<int> mat(height, width);
  // for (auto &p : pos) {
  //   ++mat(p.second, p.first);
  // }
  // for(int row = 0; row < height; ++row){
  //   for(int col = 0; col < width; ++col){
  //     std::cout << mat(row,col);
  //   }
  //   std::cout << std::endl;
  // }
  // count robots in each quadrant
  int quad_count[4] = {0, 0, 0, 0};
  for (auto &p : pos) {
    if (p.first < width / 2) {
      if (p.second < height / 2) {
        ++quad_count[0];
      } else if (p.second > height / 2) {
        ++quad_count[1];
      }
    } else if (p.first > width / 2) {
      if (p.second < height / 2) {
        ++quad_count[2];
      } else if (p.second > height / 2) {
        ++quad_count[3];
      }
    }
  }
  std::cout << quad_count[0] * quad_count[1] * quad_count[2] * quad_count[3]
            << std::endl;
}
