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

  std::vector<int> list1;
  std::vector<int> list2;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto vals = aoc::split(line);
    if (vals.size() != 2) {
      break;
    }
    list1.emplace_back(aoc::to_int<int>(vals[0]));
    list2.emplace_back(aoc::to_int<int>(vals[1]));
  }
  std::sort(list1.begin(), list1.end());
  std::sort(list2.begin(), list2.end());
  int dist = 0;
  for (size_t i = 0; i < list1.size(); ++i) {
    dist += std::abs(list1[i] - list2[i]);
  }
  std::cout << dist << std::endl;
}
