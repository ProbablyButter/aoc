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

void neighbor_molecules(
    const std::string &base,
    const std::unordered_map<std::string, std::vector<std::string>> &transforms,
    std::unordered_set<std::string> &res) {
  res.clear();
  for (auto &key : transforms) {
    // find all places in base containing key
    for (size_t i = 0; i < base.size(); ++i) {
      std::string_view sub(base.data() + i,
                           std::min<size_t>(base.size() - i, key.first.size()));
      if (sub == key.first) {
        for (auto &val : key.second) {
          std::stringstream out;
          out << std::string_view(base.data(), i) << val
              << std::string_view(base.data() + i + key.first.size(),
                                  base.size() - i - key.first.size());
          res.emplace(out.str());
        }
      }
    }
  }
}

void rneighbor_molecules(
    const std::string &base,
    const std::unordered_map<std::string, std::string> &rtransforms,
    std::vector<std::string> &res) {
  res.clear();
  for (auto &key : rtransforms) {
    // find all places in base containing key
    for (size_t i = 0; i < base.size(); ++i) {
      std::string_view sub(base.data() + i,
                           std::min<size_t>(base.size() - i, key.first.size()));
      if (sub == key.first) {
        std::stringstream out;
        out << std::string_view(base.data(), i) << key.second
            << std::string_view(base.data() + i + key.first.size(),
                                base.size() - i - key.first.size());
        res.emplace_back(out.str());
      }
    }
  }
}

std::pair<int64_t, bool>
visit(const std::string &target, const std::string &base,
      const std::unordered_map<std::string, std::string> &rtransforms,
      std::unordered_set<std::string> &visited) {
  std::pair<int64_t, bool> res;
  if (base == target) {
    res.first = 0;
    res.second = true;
    return res;
  }
  if (visited.emplace(base).second) {
    // haven't visited yet
    if (base.size() == 1) {
      std::cout << base << std::endl;
    }
    std::vector<std::string> neighbors;
    {
      rneighbor_molecules(base, rtransforms, neighbors);
    }
#if 0
    std::sort(neighbors.begin(), neighbors.end(),
              [](auto &a, auto &b) { return a.size() < b.size(); });
#endif
    for (auto &neighbor : neighbors) {
      auto tmp = visit(target, neighbor, rtransforms, visited);
      if (tmp.second) {
        res = tmp;
        ++res.first;
        return res;
      }
    }
    res.first = 0;
    res.second = false;
    //std::cout << "term " << base << std::endl;
    return res;
  }
  std::cout << "visited " << base << std::endl;
  res.first = 0;
  res.second = false;
  return res;
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;

  std::unordered_map<std::string, std::vector<std::string>> transforms;
  std::unordered_map<std::string, std::string> rtransforms;

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
    rtransforms.emplace(value, key);
  }
  std::string start;
  std::getline(in, start);

  std::string target = "e";
  std::unordered_set<std::string> visited;
  auto res = visit(target, start, rtransforms, visited);
  std::cout << res.first << ", " << res.second << std::endl;
#if 0
  using node_type = std::string;
  std::unordered_map<node_type, int64_t> dists;
  std::unordered_map<node_type, node_type> prev;
  auto terminate_functor = [&](const node_type &node) {
    return node == target;
  };
  std::unordered_set<std::string> cache;
  auto gen_neighbors = [&](const node_type &node) {
    rneighbor_molecules(node, rtransforms, cache);
    std::vector<std::pair<node_type, int64_t>> neighbors;
    neighbors.reserve(cache.size());
    for (auto &v : cache) {
      neighbors.emplace_back(v, 1);
    }
    return neighbors;
  };
  aoc::gen_dijkstra(start, terminate_functor, gen_neighbors, dists, prev);
  std::cout << dists.at(target) << std::endl;
#endif
}
