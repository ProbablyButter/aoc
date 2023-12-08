#ifndef AOC_GRAPH_TOOLS_TCC
#define AOC_GRAPH_TOOLS_TCC

#include <algorithm>

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
    for (size_t i = 0; i < nodes.size(); ++i) {
      for (size_t j = 0; j < nodes.size(); ++j) {
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

template <class T, class V>
void directed_graph<T, V>::dijkstra(
    size_t src, std::unordered_map<size_t, V> &dists,
    std::unordered_map<size_t, size_t> &prev) const {
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
  // assume at least one node
  queue.reserve(nodes.size());
  queue.push_back(src);
  for (size_t i = 0; i < nodes.size(); ++i) {
    // add all nodes which are not the src
    if (i != src) {
      queue.push_back(i);
    }
  }
  std::make_heap(queue.begin(), queue.end(), comparator);
  std::unordered_map<size_t, size_t> heap_map;
  heap_map.reserve(queue.size());
  for (size_t i = 0; i < queue.size(); ++i) {
    heap_map[queue[i]] = i;
  }
  // this is implemented for a max heap because that's what our comparator
  // expects
  auto pop_heap = [&]() {
    // move first entry to last
    {
      heap_map.at(queue.back()) = 0;
      heap_map.erase(queue[0]);
    }
    std::swap(queue[0], queue.back());
    // fix up our heap if necessary
    auto idx = 0;
    while (true) {
      auto left = idx * 2;
      auto right = left + 1;
      auto largest = idx;
      if (left + 1 < queue.size() && comparator(left, largest)) {
        largest = left;
      }
      if (right + 1 < queue.size() && comparator(right, largest)) {
        largest = right;
      }
      if (largest != idx) {
        heap_map.at(queue[idx]) = largest;
        heap_map.at(queue[largest]) = idx;
        std::swap(queue[idx], queue[largest]);
        idx = largest;
      } else {
        break;
      }
    }
  };
  auto decrease_priority = [&](size_t node) {
    // find where node is located
    auto idx = heap_map.at(node);
    // now bubble up as needed
    while (true) {
      if (idx > 0) {
        auto parent = idx / 2;
        if (comparator(queue[parent], queue[idx])) {
          // need to swap
          heap_map.at(queue[idx]) = parent;
          heap_map.at(queue[parent]) = idx;
          std::swap(queue[idx], queue[parent]);
          idx = parent;
        }
      } else {
        break;
      }
    }
  };
  while (!queue.empty()) {
    pop_heap();
    // std::pop_heap(queue.begin(), queue.end(), comparator);
    size_t u = queue.back();
    queue.pop_back();
    for (auto &neighbor : connectivity[u]) {
      V alt = dists.at(u) + neighbor.second;
      auto iter = dists.find(neighbor.first);
      if (iter == dists.end()) {
        dists.emplace(neighbor.first, alt);
        prev[neighbor.first] = u;
        decrease_priority(neighbor.first);
      } else if (alt < iter->second) {
        iter->second = alt;
        prev[neighbor.first] = u;
        decrease_priority(neighbor.first);
      }
    }
  }
}
} // namespace aoc

#endif
