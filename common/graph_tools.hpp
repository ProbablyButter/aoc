#ifndef AOC_GRAPH_TOOLS_HPP
#define AOC_GRAPH_TOOLS_HPP

#include "hash.hpp"

#include <cstddef>
#include <map>
#include <unordered_map>
#include <vector>

namespace aoc {
/// V: edge weight type
template <class V> struct directed_graph {
  // connectivity from each node
  // connectivity[i][j].first is dst node from node i
  // connectivity[i][j].first is edge weight from node i
  std::vector<std::unordered_map<size_t, V>> connectivity;

  /// computes the shortest path (if any) from all nodes to all other nodes
  std::unordered_map<std::pair<size_t, size_t>, V, pair_hasher>
  floyd_warshall() const;

  template <class F>
  void dijkstra(size_t src, std::unordered_map<size_t, V> &dists,
                std::unordered_map<size_t, size_t> &prev,
                F &&terminate_functor) const;

  /// shortest path from src node to all other reachable nodes
  void dijkstra(size_t src, std::unordered_map<size_t, V> &dists,
                std::unordered_map<size_t, size_t> &prev) const {
    dijkstra(src, dists, prev, [](size_t n) { return false; });
  }

  /// shortest path from src node to dst node
  void dijkstra(size_t src, size_t dst, std::unordered_map<size_t, V> &dists,
                std::unordered_map<size_t, size_t> &prev) const {
    dijkstra(src, dists, prev, [=](size_t n) { return n == dst; });
  }

  bool add_edge(size_t src, size_t dst, V weight) {
    while (connectivity.size() <= src) {
      connectivity.emplace_back();
    }
    while (connectivity.size() <= dst) {
      connectivity.emplace_back();
    }
    return connectivity[src].emplace(dst, weight).second;
  }
};

template <class NodeType, class DistMetric, class NodeHash, class Term,
          class Gen, class NodeCmp = std::less<NodeType>>
void gen_dijkstra(const NodeType &src, Term &&terminate_functor,
                  Gen &&gen_neighbors,
                  std::unordered_map<NodeType, DistMetric, NodeHash> &dists,
                  std::unordered_map<NodeType, NodeType, NodeHash> &prev,
                  NodeCmp &&node_cmp);

} // namespace aoc

#include "detail/graph_tools.tcc"

#endif
