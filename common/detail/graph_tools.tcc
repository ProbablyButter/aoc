#ifndef AOC_GRAPH_TOOLS_TCC
#define AOC_GRAPH_TOOLS_TCC

#include <algorithm>
#include <unordered_set>

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
  std::unordered_set<size_t> in_queue;
  // assume at least one node
  queue.push_back(src);
  in_queue.emplace(src);
  while (!queue.empty()) {
    std::pop_heap(queue.begin(), queue.end(), comparator);
    size_t u = queue.back();
    queue.pop_back();
    in_queue.erase(u);
    for (auto &neighbor : connectivity[u]) {
      V alt = dists.at(u) + neighbor.second;
      auto iter = dists.find(neighbor.first);
      if (iter == dists.end()) {
        dists.emplace(neighbor.first, alt);
        prev[neighbor.first] = u;
        if (in_queue.find(neighbor.first) != in_queue.end()) {
          in_queue.emplace(neighbor.first);
          queue.push_back(neighbor.first);
          std::push_heap(queue.begin(), queue.end(), comparator);
        }
      } else if (alt < iter->second) {
        iter->second = alt;
        prev[neighbor.first] = u;
        if (in_queue.find(neighbor.first) != in_queue.end()) {
          in_queue.emplace(neighbor.first);
          queue.push_back(neighbor.first);
          std::push_heap(queue.begin(), queue.end(), comparator);
        }
      }
    }
  }
}

template <class T, class V>
void directed_graph<T, V>::dijkstra(
    size_t src, size_t dst, std::unordered_map<size_t, V> &dists,
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
  std::unordered_set<size_t> in_queue;
  // assume at least one node
  queue.push_back(src);
  in_queue.emplace(src);
  while (!queue.empty()) {
    size_t u = queue[0];
    if (u == dst) {
      return;
    }
    std::pop_heap(queue.begin(), queue.end(), comparator);
    queue.pop_back();
    in_queue.erase(u);
    for (auto &neighbor : connectivity[u]) {
      V alt = dists.at(u) + neighbor.second;
      auto iter = dists.find(neighbor.first);
      if (iter == dists.end()) {
        dists.emplace(neighbor.first, alt);
        prev[neighbor.first] = u;
        if (in_queue.find(neighbor.first) != in_queue.end()) {
          in_queue.emplace(neighbor.first);
          queue.push_back(neighbor.first);
          std::push_heap(queue.begin(), queue.end(), comparator);
        }
      } else if (alt < iter->second) {
        iter->second = alt;
        prev[neighbor.first] = u;
        if (in_queue.find(neighbor.first) != in_queue.end()) {
          in_queue.emplace(neighbor.first);
          queue.push_back(neighbor.first);
          std::push_heap(queue.begin(), queue.end(), comparator);
        }
      }
    }
  }
}
} // namespace aoc

#endif
