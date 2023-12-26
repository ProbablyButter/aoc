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

  std::unordered_map<std::string, size_t> node_to_idx;
  aoc::directed_graph<int64_t> graph;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line, ":");
    auto node_idx = node_to_idx.emplace(std::string(tmp[0]), node_to_idx.size())
                        .first->second;
    {
      auto tmp2 = aoc::split(tmp[1], "");
      for (auto &v : tmp2) {
        auto oidx = node_to_idx.emplace(std::string(v), node_to_idx.size())
                        .first->second;
        graph.add_edge(node_idx, oidx, 1);
        graph.add_edge(oidx, node_idx, 1);
      }
    }
  }
  std::vector<std::string> idx_to_node;
  idx_to_node.resize(node_to_idx.size());
  for (auto &v : node_to_idx) {
    idx_to_node[v.second] = v.first;
  }

  // jlt - sjr
  // mhb -- zqg
  // mzb -- fjn
  graph.connectivity[node_to_idx.at("jlt")].erase(node_to_idx.at("sjr"));
  graph.connectivity[node_to_idx.at("sjr")].erase(node_to_idx.at("jlt"));
  graph.connectivity[node_to_idx.at("mhb")].erase(node_to_idx.at("zqg"));
  graph.connectivity[node_to_idx.at("zqg")].erase(node_to_idx.at("mhb"));
  graph.connectivity[node_to_idx.at("mzb")].erase(node_to_idx.at("fjn"));
  graph.connectivity[node_to_idx.at("fjn")].erase(node_to_idx.at("mzb"));

  size_t count = 0;
  auto ssc = graph.strongly_connected_components(count);

  std::cout << count << std::endl;
  std::vector<size_t> nnodes(count);
  for (size_t i = 0; i < ssc.size(); ++i) {
    ++nnodes[ssc[i]];
  }
  for (auto &v : nnodes) {
    std::cout << v << std::endl;
  }

#if 0
  std::cout << "graph {" << std::endl;
  for (size_t i = 0; i < idx_to_node.size(); ++i) {
    auto curr = idx_to_node[i];
    for (auto &v : graph.connectivity[i]) {
      auto neighbor = idx_to_node[v.first];
      if (i <= v.first) {
        std::cout << "\"" << curr << "\""
                  << " -- "
                  << "\"" << neighbor << "\"" << std::endl;
      }
    }
  }
  std::cout << "}" << std::endl;
#endif
}
