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
  std::unordered_map<int, int> list2;

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
    auto val2 = aoc::to_int<int>(vals[1]);
    ++list2[val2];
  }
  int similarity = 0;
  for (auto v : list1) {
    auto iter = list2.find(v);
    if (iter != list2.end()) {
      similarity += v * iter->second;
    }
  }
  std::cout << similarity << std::endl;
}
