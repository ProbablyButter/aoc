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
  std::string line;
  std::vector<long long> results;
  std::vector<std::vector<long long>> args;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line);
    results.emplace_back(aoc::to_int<long long>(tmp[0].data()));
    args.emplace_back();
    for (int i = 1; i < tmp.size(); ++i) {
      args.back().emplace_back(aoc::to_int<long long>(tmp[i]));
    }
  }
  long long sum = 0;
  for (int idx = 0; idx < results.size(); ++idx) {

    uint64_t limit = 1;
    for (int i = 1; i < args[idx].size(); ++i) {
      limit *= 3;
    }
    for (uint64_t bitmap = 0; bitmap < limit; ++bitmap) {
      long long res = args[idx][0];
      uint64_t tmp = bitmap;
      for (int i = 1; i < args[idx].size(); ++i) {
        auto op = tmp % 3;
        tmp /= 3;
        switch (op) {
        case 0:
          res += args[idx][i];
          break;
        case 1:
          res *= args[idx][i];
          break;
        case 2:
          // concatenate
          res = aoc::to_int<long long>(std::to_string(res) +
                                       std::to_string(args[idx][i]));
          break;
        }
      }
      if (res == results[idx]) {
        sum += res;
        break;
      }
    }
  }
  std::cout << sum << std::endl;
}
