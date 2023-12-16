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

int64_t recurse_count(const std::string_view &line,
                      const std::vector<int64_t> &damage, size_t didx,
                      size_t pos,
                      std::unordered_map<std::pair<size_t, size_t>, int64_t,
                                         aoc::pair_hasher> &cache) {
  std::pair<size_t, size_t> key(didx, pos);
  {
    auto iter = cache.find(key);
    if (iter != cache.end()) {
      return iter->second;
    }
  }
  std::vector<int64_t> valid_starts;
  int64_t first_broken =
      std::distance(line.begin(), std::find(line.begin(), line.end(), '#'));
  for (size_t i = 0; i < line.size(); ++i) {
    bool valid = true;
    for (size_t j = 0; j < damage[didx]; ++j) {
      if (i + j >= line.size() || line[i + j] == '.') {
        valid = false;
        break;
      }
    }
    if (valid) {
      if (damage[didx] + i != line.size()) {
        // this start position needs at least one . after
        if (line[damage[didx] + i] == '#') {
          valid = false;
        }
      }
      if (valid) {
        valid_starts.emplace_back(i);
      }
    }
    if (i >= first_broken) {
      break;
    }
  }
  if (valid_starts.empty()) {
    return 0;
  }

  int64_t total = 0;
  if (didx + 1 == damage.size()) {
    // make sure for each start pos we can reach end
    for (auto i : valid_starts) {
      bool valid = true;
      for (size_t j = i + damage[didx]; j < line.size() && valid; ++j) {
        if (line[j] == '#') {
          valid = false;
        }
      }
      total += valid;
    }
  } else {
    for (auto i : valid_starts) {
      auto start = i + damage[didx] + 1;
      if (start >= line.size()) {
        break;
      }
      total += recurse_count(line.substr(start), damage, didx + 1, pos + start,
                             cache);
    }
  }
  cache.emplace(key, total);
  return total;
}

void compute_expand(std::vector<std::string> maps,
                    std::vector<std::vector<int64_t>> damage,
                    size_t unfold_count, std::vector<size_t> &counts) {
  // unfold
  for (auto &v : maps) {
    std::stringstream repl;
    repl << v;
    for (size_t i = 1; i < unfold_count; ++i) {
      repl << "?" << v;
    }
    v = repl.str();
  }
  for (auto &v : damage) {
    auto init_size = v.size();
    for (size_t i = 1; i < unfold_count; ++i) {
      for (size_t j = 0; j < init_size; ++j) {
        v.emplace_back(v[j]);
      }
    }
  }

  for (size_t i = 0; i < maps.size(); ++i) {
    std::unordered_map<std::pair<size_t, size_t>, int64_t, aoc::pair_hasher>
        cache;
    auto count = recurse_count(maps[i], damage[i], 0, 0, cache);
    std::cout << "count: " << count << std::endl;

    counts.push_back(count);
  }
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  std::vector<std::string> maps;
  std::vector<std::vector<int64_t>> damage;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line, "");
    maps.emplace_back(tmp[0]);
    damage.emplace_back();
    auto tmp2 = aoc::split(tmp[1], ",");
    damage.back().reserve(tmp2.size());
    for (auto &v : tmp2) {
      damage.back().emplace_back(aoc::to_int<int64_t>(v));
    }
  }

  std::vector<size_t> orig;
  compute_expand(maps, damage, 5, orig);
  size_t total = 0;
  for (auto &v : orig) {
    total += v;
  }
  std::cout << total << std::endl;
}
