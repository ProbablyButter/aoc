#ifndef AOC_GRAPH_TOOLS_HPP
#define AOC_GRAPH_TOOLS_HPP

#include "hash.hpp"

#include <cstddef>
#include <map>
#include <unordered_map>
#include <vector>

namespace aoc {

struct ss_pair_hasher {
  size_t operator()(const std::pair<size_t, size_t> &v) const noexcept {
    hasher hash;
    hash.init(123456789ull);
    hash.add_block(v.first, v.second);
    hash.finalize();
    // only return the low bits?
    return hash.h2;
  }
};

/// T: node value type
/// V: edge weight type
template <class T, class V> struct directed_graph {
  // value at nodes
  std::vector<T> nodes;
  // connectivity from each node
  // connectivity[i][j].first is dst node from node i
  // connectivity[i][j].first is edge weight from node i
  std::vector<std::unordered_map<size_t, V>> connectivity;

  /// computes the shortest path (if any) from all nodes to all other nodes
  std::unordered_map<std::pair<size_t, size_t>, V, ss_pair_hasher>
  floyd_warshall() const;

  /// shortest path from src node to all other reachable nodes
  void dijkstra(size_t src, std::unordered_map<size_t, V> &dists,
                std::unordered_map<size_t, size_t> &prev) const;

  /// shortest path from src node to dst node
  void dijkstra(size_t src, size_t dst, std::unordered_map<size_t, V> &dists,
                std::unordered_map<size_t, size_t> &prev) const;

  void add_node(const T &v) {
    nodes.push_back(v);
    connectivity.emplace_back();
  }

  bool add_edge(size_t src, size_t dst, V weight) {
    return connectivity[src].emplace(dst, weight).second;
  }
};

} // namespace aoc

#include "detail/graph_tools.tcc"

#endif
