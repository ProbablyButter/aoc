#include "graph_tools.hpp"

#include <iostream>
#include <string>

int main() {
  aoc::directed_graph<int64_t> graph;
  graph.add_edge(0, 1, 1);
  graph.add_edge(0, 0, 1);
  graph.add_edge(0, 2, 10);
  graph.add_edge(1, 0, 5);
  graph.add_edge(2, 0, 5);
  graph.add_edge(2, 3, 5);
  auto res = graph.floyd_warshall();
  std::cout << std::endl;
  for (auto &v : res) {
    std::cout << v.first.first << "->" << v.first.second << ": " << v.second
              << std::endl;
  }
  std::cout << std::endl;

  std::unordered_map<size_t, int64_t> dists;
  std::unordered_map<size_t, size_t> prev;
  graph.dijkstra(0, dists, prev);
  for (auto &v : dists) {
    std::cout << "0->" << v.first << ": " << v.second << std::endl;
  }
  for (auto &v : prev) {
    std::cout << v.second << "->" << v.first << std::endl;
  }
}
