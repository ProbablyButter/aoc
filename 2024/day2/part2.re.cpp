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
    for (size_t remove_pos = 0; remove_pos < r.size(); ++remove_pos) {
      bool all_dec = true;
      bool all_inc = true;
      bool valid = true;
      size_t pos = 0;
      size_t next_pos = 1;
      while (true) {
        if (pos == remove_pos) {
          ++pos;
        }
        if (next_pos == remove_pos || next_pos == pos) {
          ++next_pos;
        }
        if (next_pos >= r.size()) {
          break;
        }
        if (r[next_pos] >= r[pos]) {
          all_dec &= false;
        }
        if (r[next_pos] <= r[pos]) {
          all_inc &= false;
        }
        int dist = std::abs(r[pos] - r[next_pos]);
        if (dist >= 1 && dist <= 3) {

        } else {
          valid = false;
          break;
        }
        ++pos;
        ++next_pos;
      }
      if (valid) {
        if (all_dec || all_inc) {
          ++num_safe;
          break;
        }
      }
    }
  }
  std::cout << num_safe << std::endl;
}
