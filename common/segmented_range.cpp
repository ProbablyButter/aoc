#include "segmented_range.hpp"

#include <algorithm>

#include <iostream>

namespace aoc {
void segmented_range::add(int64_t lower, int64_t upper) {
  segments.emplace_back(lower, upper);
}
void segmented_range::add(const std::pair<int64_t, int64_t> &segment) {
  segments.push_back(segment);
}

segmented_range
segmented_range::subtract(const std::pair<int64_t, int64_t> &segment) const {
  segmented_range res;
  if (segments.empty()) {
    return res;
  }
  size_t i = 0;
  int64_t curr = segments[0].first;
  bool done = false;
  while (true) {
    if (i >= segments.size()) {
      // nothing left to compare
      break;
    }
    if (done) {
      // nothing left to compare, add everything else
      if (curr < segments[i].second) {
        res.segments.emplace_back(curr, segments[i].second);
        ++i;
      }
      for (; i < segments.size(); ++i) {
        res.segments.push_back(segments[i]);
      }
      break;
    }
    if (curr >= segments[i].second) {
      ++i;
      continue;
    }
    if (curr >= segment.second) {
      done = true;
      continue;
    }
    if (curr < segments[i].first) {
      curr = segments[i].first;
      continue;
    }
    if (curr < segment.first) {
      // add segment
      if (segments[i].second < segment.first) {
        res.segments.emplace_back(curr, segments[i].second);
        ++i;
        if (i < segments.size()) {
          curr = segments[i].first;
        } else {
          break;
        }
      } else {
        res.segments.emplace_back(curr, segment.first);
        curr = segment.second;
      }
    } else {
      curr = segment.second;
    }
  }

  return res;
}

segmented_range
segmented_range::intersect(const std::pair<int64_t, int64_t> &segment) const {
  segmented_range res;
  if (segments.empty()) {
    return res;
  }

  size_t i = 0;

  int64_t curr = segments[0].first;
  while (true) {
    if (i >= segments.size()) {
      // nothing left to compare
      break;
    }
    if (curr >= segment.second) {
      // nothing left to compare
      break;
    }
    if (curr >= segments[i].second) {
      ++i;
      continue;
    }
    long long a = std::max(segments[i].first, segment.first);
    long long b = std::min(segments[i].second, segment.second);
    if (a < b) {
      // some intersection
      res.segments.emplace_back(a, b);
      curr = b;
    } else {
      curr = a;
    }
  }

  return res;
}

segmented_range segmented_range::add(const segmented_range &other) const {
  segmented_range res;
  size_t i = 0;
  size_t j = 0;
  int64_t curr;
  if (!segments.empty()) {
    curr = segments[0].first;
  }
  if (!other.segments.empty()) {
    curr = std::min(curr, other.segments[0].first);
  }
  while (true) {
    if (i >= segments.size()) {
      // nothing left to compare, add everything else from other
      if (curr < other.segments[j].second) {
        res.segments.emplace_back(curr, other.segments[j].second);
        ++j;
      }
      for (; j < other.segments.size(); ++j) {
        res.segments.push_back(other.segments[j]);
      }
      break;
    }
    if (j >= other.segments.size()) {
      // nothing left to compare, add everything else from *this
      if (curr < segments[i].second) {
        res.segments.emplace_back(curr, segments[i].second);
        ++i;
      }
      for (; i < segments.size(); ++i) {
        res.segments.push_back(segments[i]);
      }
      break;
    }
    if (curr >= segments[i].second) {
      ++i;
      continue;
    }
    if (curr >= other.segments[j].second) {
      ++j;
      continue;
    }
    if (curr == segments[i].first) {
      int64_t next_stop = std::min(segments[i].second, other.segments[j].first);
      if (curr < next_stop) {
        res.segments.emplace_back(curr, next_stop);
      }
    } else {
      int64_t next_stop = std::min(other.segments[j].second, segments[i].first);
      if (curr < next_stop) {
        res.segments.emplace_back(curr, next_stop);
      }
    }
  }
  return res;
}

segmented_range segmented_range::subtract(const segmented_range &other) const {
  segmented_range res;
  if (segments.empty()) {
    return res;
  }
  if (other.segments.empty()) {
    res = *this;
    return res;
  }

  size_t i = 0;
  size_t j = 0;
  int64_t curr = segments[0].first;

  while (true) {
    if (i >= segments.size()) {
      // nothing left to compare
      break;
    }
    if (j >= other.segments.size()) {
      // nothing left to compare, add everything else
      if (curr < segments[i].second) {
        res.segments.emplace_back(curr, segments[i].second);
        ++i;
      }
      for (; i < segments.size(); ++i) {
        res.segments.push_back(segments[i]);
      }
      break;
    }
    if (curr >= segments[i].second) {
      ++i;
      continue;
    }
    if (curr >= other.segments[j].second) {
      ++j;
      continue;
    }
    if (curr < segments[i].first) {
      curr = segments[i].first;
    }
    if (curr < other.segments[j].first) {
      // add segment
      if (segments[i].second < other.segments[j].first) {
        res.segments.emplace_back(curr, segments[i].second);
        ++i;
        if (i < segments.size()) {
          curr = segments[i].first;
        } else {
          break;
        }
      } else {
        res.segments.emplace_back(curr, other.segments[j].first);
        curr = other.segments[j].second;
      }
    } else {
      curr = other.segments[j].second;
    }
  }

  return res;
}

segmented_range segmented_range::intersect(const segmented_range &other) const {
  segmented_range res;
  if (segments.empty() || other.segments.empty()) {
    // no intersection
    return res;
  }

  size_t i = 0;
  size_t j = 0;
  long long curr = segments[0].first;

  while (true) {
    if (i >= segments.size()) {
      // nothing left to compare
      break;
    }
    if (j >= other.segments.size()) {
      // nothing left to compare
      break;
    }
    if (curr >= segments[i].second) {
      ++i;
      continue;
    }
    if (curr >= other.segments[j].second) {
      ++j;
      continue;
    }
    // some chance for an intersection
    long long a = std::max(segments[i].first, other.segments[j].first);
    long long b = std::min(segments[i].second, other.segments[j].second);
    if (a < b) {
      // some intersection
      res.segments.emplace_back(a, b);
      curr = b;

    } else {
      curr = a;
    }
  }

  return res;
}

std::vector<size_t> segmented_range::get_sort_permute() const {
  std::vector<size_t> idcs;
  idcs.reserve(segments.size());
  for (size_t i = 0; i < segments.size(); ++i) {
    idcs.push_back(i);
  }
  std::sort(idcs.begin(), idcs.end(), [&](auto &a, auto &b) {
    if (segments[a].first != segments[b].first) {
      return segments[a].first < segments[b].first;
    }
    return segments[a].second < segments[b].second;
  });
  return idcs;
}

void segmented_range::normalize() {
  // sort first
  std::sort(segments.begin(), segments.end(), [](auto &a, auto &b) {
    if (a.first != b.first) {
      return a.first < b.first;
    }
    return a.second < b.second;
  });
  // resolve overlaps by creating new segments
  if (segments.size()) {
    size_t i = 0;
    bool any_overlaps = false;
    while (true) {
      if (i + 1 >= segments.size()) {
        break;
      }
      if (segments[i].second > segments[i + 1].first) {
        any_overlaps = true;
        break;
      }
      ++i;
    }
    if (any_overlaps) {
      segmented_range res;
      size_t i = 0;
      size_t j = 1;
      std::vector<int64_t> breaks;
      while (true) {
        // identify all break points
        breaks.clear();
        breaks.push_back(segments[i].first);
        breaks.push_back(segments[i].second);
        while (true) {
          if (j >= segments.size()) {
            break;
          }
          if (segments[j].first >= segments[i].second) {
            break;
          }
          breaks.push_back(segments[j].first);
          breaks.push_back(segments[j].second);
          ++j;
        }
        // get unique breaks
        std::sort(breaks.begin(), breaks.end());
        breaks.erase(std::unique(breaks.begin(), breaks.end()), breaks.end());
        // add all segments
        for (size_t idx = 0; idx + 1 < breaks.size(); ++idx) {
          res.segments.emplace_back(breaks[idx], breaks[idx + 1]);
        }
        i = j;
        if (i >= segments.size()) {
          break;
        }
      }

      segments = std::move(res.segments);
    }
  }
}

void segmented_range::merge_adjacent() {
  size_t i = 0;
  // merge candidate
  size_t j = 1;
  size_t len = segments.size();
  while (true) {
    if (i >= len) {
      break;
    }
    if (j >= segments.size()) {
      break;
    }
    // can we merge i and j?
    if (segments[i].second == segments[j].first) {
      // merge
      segments[i].second = segments[j].second;
      ++j;
      --len;
    } else {
      ++i;
      if (i != j) {
        // shift up
        segments[i] = segments[j];
      }
      ++j;
    }
  }
  segments.erase(segments.begin() + len, segments.end());
}

int64_t segmented_range::size() const {
  int64_t res = 0;
  for (auto &v : segments) {
    res += v.second - v.first;
  }
  return res;
}
} // namespace aoc
