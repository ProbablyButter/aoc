#ifndef AOC_GRAPH_TOOLS_TCC
#define AOC_GRAPH_TOOLS_TCC

#include <algorithm>
#include <stdexcept>
#include <unordered_set>

namespace aoc {
template <class V>
std::unordered_map<std::pair<size_t, size_t>, V, pair_hasher>
directed_graph<V>::floyd_warshall() const {
  using pair_type = std::pair<size_t, size_t>;
  std::unordered_map<std::pair<size_t, size_t>, V, pair_hasher> res;
  // add initial edges
  for (size_t i = 0; i < connectivity.size(); ++i) {
    for (auto &edge : connectivity[i]) {
      res[pair_type(i, edge.first)] = edge.second;
    }
  }
  // dist from every node to itself is 0
  for (size_t i = 0; i < connectivity.size(); ++i) {
    res[pair_type(i, i)] = 0;
  }
  for (size_t k = 0; k < connectivity.size(); ++k) {
    for (size_t i = 0; i < connectivity.size(); ++i) {
      for (size_t j = 0; j < connectivity.size(); ++j) {
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

template <class V>
std::vector<size_t>
directed_graph<V>::strongly_connected_components(size_t &count) const {
  count = 0;
  // use Tarjan's algorithm
  std::vector<size_t> stack;
  std::vector<size_t> res;
  size_t idx = 0;
  res.resize(connectivity.size(), 0xffffffffu);
  std::vector<int64_t> idcs(connectivity.size(), -1);
  std::vector<int64_t> low_links(connectivity.size(), -1);
  std::unordered_set<size_t> on_stack;
  for (size_t i = 0; i < connectivity.size(); ++i) {
    if (idcs[i] == -1) {
      strong_connect(i, idx, count, stack, idcs, low_links, on_stack, res);
    }
  }

  return res;
}

template <class V>
void directed_graph<V>::strong_connect(size_t node, size_t &idx, size_t &count,
                                       std::vector<size_t> &stack,
                                       std::vector<int64_t> &idcs,
                                       std::vector<int64_t> &low_links,
                                       std::unordered_set<size_t> &on_stack,
                                       std::vector<size_t> &res) const {
  idcs[node] = idx;
  low_links[node] = idx;
  ++idx;
  stack.push_back(node);
  on_stack.emplace(node);
  for (auto &w : connectivity.at(node)) {
    if (idcs[w.first] == -1) {
      // w not visited yet
      strong_connect(w.first, idx, count, stack, idcs, low_links, on_stack,
                     res);
      low_links[node] = std::min(low_links[node], low_links[w.first]);
    } else if (on_stack.find(w.first) != on_stack.end()) {
      low_links[node] = std::min(low_links[node], idcs[w.first]);
    }
  }
  if (low_links[node] == idcs[node]) {
    // start a new strongly component
    while (true) {
      auto w = stack.back();
      stack.pop_back();
      on_stack.erase(w);
      res[w] = count;
      if (w == node) {
        break;
      }
    }
    ++count;
  }
}

template <class V>
template <class F>
void directed_graph<V>::dijkstra(size_t src,
                                 std::unordered_map<size_t, V> &dists,
                                 std::unordered_map<size_t, size_t> &prev,
                                 F &&terminate_functor) const {
  dists.clear();
  prev.clear();
  dists.emplace(src, 0);

  // heap functions are a max heap, reverse comparator
  auto comparator = [&](size_t a, size_t b) {
    auto ia = dists.find(a);
    auto ib = dists.find(b);
    if (ia == dists.end()) {
      if (ib == dists.end()) {
        // both inf dist
        return a < b;
      }
      return true;
    } else {
      if (ib == dists.end()) {
        return false;
      }
      return ia->second > ib->second;
    }
  };
  std::vector<size_t> queue;
  std::unordered_set<size_t> visited;
  // assume at least one node
  queue.push_back(src);
  visited.emplace(src);
  while (!queue.empty()) {
    size_t u = queue[0];
    if (terminate_functor(u)) {
      return;
    }
    std::pop_heap(queue.begin(), queue.end(), comparator);
    queue.pop_back();
    for (auto &neighbor : connectivity[u]) {
      V alt = dists.at(u) + neighbor.second;
      auto iter = dists.find(neighbor.first);
      if (iter == dists.end()) {
        dists.emplace(neighbor.first, alt);
        prev[neighbor.first] = u;
        if (visited.find(neighbor.first) == visited.end()) {
          visited.emplace(neighbor.first);
          queue.push_back(neighbor.first);
          std::push_heap(queue.begin(), queue.end(), comparator);
        }
      } else if (alt < iter->second) {
        iter->second = alt;
        prev[neighbor.first] = u;
        if (visited.find(neighbor.first) == visited.end()) {
          visited.emplace(neighbor.first);
          queue.push_back(neighbor.first);
          std::push_heap(queue.begin(), queue.end(), comparator);
        }
      }
    }
  }
}

template <class V>
template <class F>
void directed_graph<V>::depth_first_search(size_t src, F &&node_callback) {
  if (!node_callback(src)) {
    auto &children = connectivity[src];
    for (auto &c : children) {
      depth_first_search(c.first, node_callback);
    }
  }
}

template <class NodeType, class DistMetric, class NodeHash, class Term,
          class Gen>
void gen_dijkstra(const NodeType &src, Term &&terminate_functor,
                  Gen &&gen_neighbors,
                  std::unordered_map<NodeType, DistMetric, NodeHash> &dists,
                  std::unordered_map<NodeType, NodeType, NodeHash> &prev) {
  dists.clear();
  prev.clear();
  dists.emplace(src, 0);
  // heap functions are a max heap, reverse comparator
  auto comparator = [&](const NodeType &a, const NodeType &b) {
    auto ia = dists.find(a);
    auto ib = dists.find(b);
    if (ia == dists.end()) {
      if (ib == dists.end()) {
        // both inf dist
        // I think it should be impossible to get here
        throw std::runtime_error("a and b both inf dist");
      }
      return true;
    } else {
      if (ib == dists.end()) {
        return false;
      }
      return ia->second > ib->second;
    }
  };
  std::vector<NodeType> queue;
  std::unordered_set<NodeType, NodeHash> visited;
  // assume at least one node
  queue.push_back(src);
  visited.emplace(src);
  while (!queue.empty()) {
    auto u = queue[0];
    if (terminate_functor(u)) {
      return;
    }
    std::pop_heap(queue.begin(), queue.end(), comparator);
    queue.pop_back();
    // pair of neighbor and weight
    auto neighbors = gen_neighbors(u);
    for (auto &neighbor : neighbors) {
      DistMetric alt = dists.at(u) + neighbor.second;
      auto iter = dists.find(neighbor.first);
      if (iter == dists.end()) {
        dists.emplace(neighbor.first, alt);
        prev[neighbor.first] = u;
        if (visited.find(neighbor.first) == visited.end()) {
          visited.emplace(neighbor.first);
          queue.push_back(neighbor.first);
          std::push_heap(queue.begin(), queue.end(), comparator);
        }
      } else if (alt < iter->second) {
        iter->second = alt;
        prev[neighbor.first] = u;
        if (visited.find(neighbor.first) == visited.end()) {
          visited.emplace(neighbor.first);
          queue.push_back(neighbor.first);
          std::push_heap(queue.begin(), queue.end(), comparator);
        }
      }
    }
  }
}
} // namespace aoc

#endif
