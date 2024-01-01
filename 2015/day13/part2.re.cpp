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
#include "hash.hpp"
#include "string_tools.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  std::unordered_map<std::string, size_t> name_to_idx;
  aoc::directed_graph<int64_t> neighbors;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line, "");
    auto src = name_to_idx.emplace(std::string(tmp[0]), name_to_idx.size())
                   .first->second;
    auto dst =
        name_to_idx
            .emplace(std::string(tmp.back().substr(0, tmp.back().size() - 1)),
                     name_to_idx.size())
            .first->second;
    if (tmp[2] == "gain") {
      neighbors.add_edge(src, dst, aoc::to_int<int64_t>(tmp[3]));
    } else {
      neighbors.add_edge(src, dst, -aoc::to_int<int64_t>(tmp[3]));
    }
  }
  for (size_t i = 0; i < name_to_idx.size(); ++i) {
    neighbors.add_edge(name_to_idx.size(), i, 0);
    neighbors.add_edge(i, name_to_idx.size(), 0);
  }
  name_to_idx.emplace("me", name_to_idx.size());
  std::vector<size_t> ordering;
  ordering.reserve(name_to_idx.size());
  for (size_t i = 0; i < name_to_idx.size(); ++i) {
    ordering.push_back(i);
  }
  int64_t max_happy = -0xfffffff;
  while (true) {
    int64_t happy = 0;
    for (size_t i = 0; i < ordering.size(); ++i) {
      if (i == 0) {
        // left is wrap-around
        happy += neighbors.connectivity[ordering[i]].at(ordering.back());
      } else {
        happy += neighbors.connectivity[ordering[i]].at(ordering[i - 1]);
      }
      if (i + 1 >= ordering.size()) {
        // left is wrap-around
        happy += neighbors.connectivity[ordering[i]].at(ordering[0]);
      } else {
        happy += neighbors.connectivity[ordering[i]].at(ordering[i + 1]);
      }
    }
    max_happy = std::max(happy, max_happy);
    if (!std::next_permutation(ordering.begin(), ordering.end())) {
      break;
    }
  }
  std::cout << max_happy << std::endl;
}
