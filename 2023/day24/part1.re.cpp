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
  std::string line;

  std::vector<std::array<int64_t, 6>> hail;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    hail.emplace_back();
    auto tmp = aoc::split(line, "@");
    {
      auto tmp2 = aoc::split(tmp[0], ",");
      hail.back()[0] = aoc::to_int<int64_t>(aoc::trim(tmp2[0]));
      hail.back()[1] = aoc::to_int<int64_t>(aoc::trim(tmp2[1]));
      hail.back()[2] = aoc::to_int<int64_t>(aoc::trim(tmp2[2]));
    }
    {
      auto tmp2 = aoc::split(tmp[1], ",");
      hail.back()[3] = aoc::to_int<int64_t>(aoc::trim(tmp2[0]));
      hail.back()[4] = aoc::to_int<int64_t>(aoc::trim(tmp2[1]));
      hail.back()[5] = aoc::to_int<int64_t>(aoc::trim(tmp2[2]));
    }
  }
  int64_t x_min = 200000000000000;
  int64_t y_min = 200000000000000;
  int64_t x_max = 400000000000000;
  int64_t y_max = 400000000000000;
  int64_t count = 0;
  for (size_t i = 0; i < hail.size(); ++i) {
    for (size_t j = i + 1; j < hail.size(); ++j) {
      double s, t;
      aoc::line_segment_intersection(
          hail[i][0], hail[i][1], hail[i][0] + hail[i][3],
          hail[i][1] + hail[i][4], hail[j][0], hail[j][1],
          hail[j][0] + hail[j][3], hail[j][1] + hail[j][4], s, t);
      if (s >= 0 && t >= 0) {
        // forwards in time
        double x = hail[i][0] + s * hail[i][3];
        double y = hail[i][1] + s * hail[i][4];
        if (x >= x_min && x <= x_max && y >= y_min && y <= y_max) {
          ++count;
        }
      }
    }
  }
  std::cout << count << std::endl;
}
