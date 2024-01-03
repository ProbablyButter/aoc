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

int64_t recurse(std::vector<std::array<int64_t, 5>> &params, int64_t idx,
                int64_t rem_vals, std::array<int64_t, 5> vals) {
  if (idx + 1 >= params.size()) {
    // everything in rem_vals is this ingredient
    int64_t score = 1;
    int64_t calories = params[idx][4] * rem_vals + vals[4];
    if (calories != 500) {
      return 0;
    }
    for (int64_t j = 0; j < 4; ++j) {
      int64_t tmp = params[idx][j] * rem_vals + vals[j];
      if (tmp < 0) {
        score = 0;
        break;
      } else {
        score *= tmp;
      }
    }
    return score;
  } else {
    int64_t max_score = 0;
    std::array<int64_t, 5> sum;
    for (int64_t i = 0; i <= rem_vals; ++i) {
      for (int64_t j = 0; j < 5; ++j) {
        sum[j] = params[idx][j] * i + vals[j];
      }
      auto tmp = recurse(params, idx + 1, rem_vals - i, sum);
      max_score = std::max(max_score, tmp);
    }
    return max_score;
  }
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  std::vector<std::array<int64_t, 5>> params;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line, "");
    params.emplace_back();
    params.back()[0] =
        aoc::to_int<int64_t>(tmp[2].substr(0, tmp[2].size() - 1));
    params.back()[1] =
        aoc::to_int<int64_t>(tmp[4].substr(0, tmp[4].size() - 1));
    params.back()[2] =
        aoc::to_int<int64_t>(tmp[6].substr(0, tmp[6].size() - 1));
    params.back()[3] =
        aoc::to_int<int64_t>(tmp[8].substr(0, tmp[8].size() - 1));
    params.back()[4] = aoc::to_int<int64_t>(tmp[10]);
  }
  int64_t total_vals = 100;
  std::array<int64_t, 5> vals = {0, 0, 0, 0, 0};
  auto score = recurse(params, 0, total_vals, vals);
  std::cout << score << std::endl;
}
