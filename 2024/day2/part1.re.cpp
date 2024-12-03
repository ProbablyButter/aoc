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

  std::vector<std::vector<int>> reports;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto vals = aoc::split(line);
    reports.emplace_back();
    reports.back().reserve(vals.size());
    for (auto &v : vals) {
      reports.back().emplace_back(aoc::to_int<int>(v));
    }
  }
  int num_safe = 0;
  for (auto &r : reports) {
    bool all_dec = true;
    bool all_inc = true;
    bool valid = true;
    for (size_t i = 0; i < r.size() - 1; ++i) {
      if (r[i + 1] >= r[i]) {
        all_dec &= false;
      }
      if (r[i + 1] <= r[i]) {
        all_inc &= false;
      }
      int dist = std::abs(r[i] - r[i + 1]);
      if (dist >= 1 && dist <= 3) {

      } else {
        valid = false;
        break;
      }
    }
    if (valid) {
      if (all_dec || all_inc) {
        ++num_safe;
      }
    }
  }
  std::cout << num_safe << std::endl;
}
