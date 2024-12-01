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
#include "hash.hpp"
#include "string_tools.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <cstring>
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

  std::unordered_map<std::string, std::vector<std::string>> transforms;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    if (line.empty()) {
      break;
    }
    auto tmp = aoc::split(line, " => ");
    std::string key(tmp[0]);
    std::string value(tmp[1]);
    auto iter = transforms.find(key);
    if (iter == transforms.end()) {
      transforms.emplace(key, std::vector<std::string>({value}));
    } else {
      iter->second.emplace_back(value);
    }
  }
  std::string base;
  std::getline(in, base);

  std::unordered_set<std::string> molecules;
  for (auto &key : transforms) {
    // find all places in base containing key
    for (size_t i = 0; i < base.size(); ++i) {
      std::string_view sub(base.data() + i,
                           std::min<size_t>(base.size() - i, key.first.size()));
      if (sub == key.first) {
        for(auto& val : key.second)
        {
          std::stringstream out;
          out << std::string_view(base.data(), i) << val
              << std::string_view(base.data() + i + key.first.size(),
                                  base.size() - i - key.first.size());
          molecules.emplace(out.str());
        }
      }
    }
  }
  std::cout << molecules.size() << std::endl;
}
