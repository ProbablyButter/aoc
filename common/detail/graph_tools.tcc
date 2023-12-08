#ifndef AOC_GRAPH_TOOLS_TCC
#define AOC_GRAPH_TOOLS_TCC

namespace aoc {
template <class T, class V>
std::unordered_map<std::pair<size_t, size_t>, V, ss_pair_hasher>
directed_graph<T, V>::floyd_warshall() const {
  using pair_type = std::pair<size_t, size_t>;
  std::unordered_map<std::pair<size_t, size_t>, V, ss_pair_hasher> res;
  // add initial edges
  for (size_t i = 0; i < connectivity.size(); ++i) {
    for (auto &edge : connectivity[i]) {
      res[pair_type(i, edge.first)] = edge.second;
    }
  }
  // dist from every node to itself is 0
  for (size_t i = 0; i < nodes.size(); ++i) {
    res[pair_type(i, i)] = 0;
  }
  for (size_t k = 0; k < nodes.size(); ++k) {
    for (size_t i = 0; k < nodes.size(); ++k) {
      for (size_t j = 0; k < nodes.size(); ++k) {
        auto ij_dist = res.find(pair_type(i, j));
        auto ik_dist = res.find(pair_type(i, k));
        auto kj_dist = res.find(pair_type(k, j));
        if (ik_dist != res.end() && kj_dist != res.end()) {
          V tmp = ik_dist->second + kj_dist->second;
          if (ij_dist == res.end()) {
            res[pair_type(i, j)] = tmp;
          } else if (ij_dist->second > tmp) {
            ij_dist->second = tmp;
          }
        }
      }
    }
  }
  return res;
}
} // namespace aoc

#endif
