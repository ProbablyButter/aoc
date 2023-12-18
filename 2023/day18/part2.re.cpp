/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"
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
  using point_type = std::array<int64_t, 2>;
  std::vector<point_type> poly;
  std::vector<char> dirs;
  std::array<int64_t, 4> bounds = {0, 0, 1, 1};
  int64_t row = 0;
  int64_t col = 0;
  poly.push_back(point_type({row, col}));
  char last_instr = '.';
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line);
    auto dist = aoc::to_int<int64_t>(tmp[2].substr(2, 5), 16);
    switch (tmp[2][2 + 5]) {
    case '0':
      // right
      col += dist;
      break;
    case '1':
      // down
      row += dist;
      break;
    case '2':
      // left
      col -= dist;
      break;
    case '3':
      // up
      row -= dist;
      break;
    }
    bounds[0] = std::min<int64_t>(bounds[0], row);
    bounds[1] = std::min<int64_t>(bounds[1], col);
    bounds[2] = std::max<int64_t>(bounds[2], row + 1);
    bounds[3] = std::max<int64_t>(bounds[3], col + 1);
    if (last_instr == tmp[2][2 + 5]) {
      poly.back()[0] = row;
      poly.back()[1] = col;
    } else {
      poly.push_back(point_type({row, col}));
      dirs.emplace_back(tmp[2][2 + 5]);
    }
    last_instr = tmp[2][2 + 5];
  }
  // is our path clockwise or counterclockwise?
  double area = 0;
  for (size_t i = 0; i + 1 < poly.size(); ++i) {
    auto j = i + 1;
    double px0 = poly[i][1];
    double py0 = poly[i][0];
    double px1 = poly[j][1];
    double py1 = poly[j][0];
    area = area + px0 * py1;
    area = area - py0 * px1;
  }
  bool cw = (area < 0);
  // now re-compute with an outset
  using dpoint_type = std::array<double, 2>;
  std::vector<dpoint_type> dupe;
  for (size_t i = 0; i + 1 < poly.size(); ++i) {
    auto j = i + 1;
    double px0 = poly[i][1];
    double py0 = poly[i][0];
    double px1 = poly[j][1];
    double py1 = poly[j][0];
    switch (dirs[i]) {
    case '0':
      // right
      if (cw) {
        py0 += 0.5;
        py1 += 0.5;
      } else {
        py0 -= 0.5;
        py1 -= 0.5;
      }
      break;
    case '1':
      // down
      if (cw) {
        px0 -= 0.5;
        px1 -= 0.5;
      } else {
        px0 += 0.5;
        px1 += 0.5;
      }
      break;
    case '2':
      // left
      if (cw) {
        py0 -= 0.5;
        py1 -= 0.5;
      } else {
        py0 += 0.5;
        py1 += 0.5;
      }
      break;
    case '3':
      // up
      if (cw) {
        px0 += 0.5;
        px1 += 0.5;
      } else {
        px0 -= 0.5;
        px1 -= 0.5;
      }
      break;
    }
    dupe.push_back(dpoint_type({px0, py0}));
    dupe.push_back(dpoint_type({px1, py1}));
  }
  double area2 = 0;
  for (size_t i = 0; i < dupe.size(); i += 2) {
    size_t prev = i - 2;
    if (i == 0) {
      prev = dupe.size() - 2;
    }
    size_t next = i + 2;
    if (next >= dupe.size()) {
      next = 0;
    }

    // p0 is prev intersect curr
    // p1 is curr intersect next
    double a1 = dupe[i + 1][0] - dupe[i][0];
    double b1 = -(dupe[prev + 1][0] - dupe[prev][0]);
    double a2 = dupe[i + 1][1] - dupe[i][1];
    double b2 = -(dupe[prev + 1][1] - dupe[prev][1]);
    double c1 = dupe[prev][0] - dupe[i][0];
    double c2 = dupe[prev][1] - dupe[i][1];
    double t0 = (c1 * b2 - b1 * c2) / (a1 * b2 - b1 * a2);

    a1 = dupe[i + 1][0] - dupe[i][0];
    b1 = -(dupe[next + 1][0] - dupe[next][0]);
    a2 = dupe[i + 1][1] - dupe[i][1];
    b2 = -(dupe[next + 1][1] - dupe[next][1]);
    c1 = dupe[next][0] - dupe[i][0];
    c2 = dupe[next][1] - dupe[i][1];
    double t1 = (c1 * b2 - b1 * c2) / (a1 * b2 - b1 * a2);

    double px0 = t0 * (dupe[i + 1][0] - dupe[i][0]) + dupe[i][0];
    double py0 = t0 * (dupe[i + 1][1] - dupe[i][1]) + dupe[i][1];
    double px1 = t1 * (dupe[i + 1][0] - dupe[i][0]) + dupe[i][0];
    double py1 = t1 * (dupe[i + 1][1] - dupe[i][1]) + dupe[i][1];

    area2 = area2 + px0 * py1;
    area2 = area2 - py0 * px1;
  }
  std::cout << llround(area2) / 2 << std::endl;
}
