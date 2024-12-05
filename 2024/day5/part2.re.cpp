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
  // read in entire file
  // key[page] which must come before value[page]
  std::unordered_multimap<int, int> forward_rules;
  std::vector<std::vector<int>> pages;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    {
      // parse rule
      auto tmp = aoc::split(line, "|");
      if (tmp.size() == 2) {
        forward_rules.emplace(aoc::to_int<int>(tmp[0]),
                              aoc::to_int<int>(tmp[1]));
        continue;
      }
    }
    {
      auto tmp = aoc::split(line, ",");
      if (tmp.size()) {
        pages.emplace_back();
        for (auto &v : tmp) {
          pages.back().emplace_back(aoc::to_int<int>(v));
        }
      }
    }
  }
  // for each update, figure out if everything's in the right order
  int sum = 0;
  std::vector<int> incorrect;
  for (auto &update : pages) {
    // identify all pages which are in the incorrect order
    incorrect.clear();
    for (int i = 0; i < update.size(); ++i) {
      auto p = forward_rules.equal_range(update[i]);
      for (int j = 0; j < i; ++j) {
        for (auto iter = p.first; iter != p.second; ++iter) {
          if (iter->second == update[j]) {
            // update[i] must come before update[j]
            incorrect.push_back(update[i]);
            // remove the incorrect page
            update.erase(update.begin() + i);
            --i;
            goto next_page;
          }
        }
      }
    next_page:;
    }
    // figure out where to put all the incorrect pages
    for (auto page : incorrect) {
      auto p = forward_rules.equal_range(page);
      // place page as late as possible
      for (int i = 0; i < update.size(); ++i) {
        for (auto iter = p.first; iter != p.second; ++iter) {
          if (iter->second == update[i]) {
            // found the latest valid spot for page
            update.insert(update.begin() + i, page);
            goto done;
          }
        }
      }
    done:;
    }
    if (incorrect.size()) {
      sum += update[update.size() / 2];
    }
  }
  std::cout << sum << std::endl;
}
