#include "string_tools.hpp"

#include <cctype>
#include <cstring>

#include <iostream>

namespace aoc {
std::vector<std::string_view>
split(const std::string &txt, const std::string &split_str, int64_t maxsplit) {
  std::vector<std::string_view> res;
  if (maxsplit == 0) {
    res.emplace_back(txt);
    return res;
  }
  const char *start = txt.data();
  const char *stop = start;
  if (split_str.empty()) {
    // split on all whitespace
    while (stop < txt.data() + txt.size()) {
      if (std::isspace(*stop)) {
        if (start == stop) {
          ++start;
        } else {
          // split point
          if (maxsplit > 0 && res.size() >= maxsplit) {
            break;
          }
          res.emplace_back(start, stop - start);
          start = stop + 1;
        }
        ++stop;
      } else {
        ++stop;
      }
    }
    // add remainder
    if (stop < txt.data() + txt.size()) {
      res.emplace_back(start, txt.data() + txt.size() - start);
    }
  } else {
    // split on specified delimiter
    while (stop + split_str.size() <= txt.data() + txt.size()) {

      if (std::strncmp(stop, split_str.data(), split_str.size()) == 0) {
        if (maxsplit > 0 && res.size() >= maxsplit) {
          break;
        }
        res.emplace_back(start, stop - start);
        stop += split_str.size();
        start = stop;
      } else {
        ++stop;
      }
    }
    // add remainder
    res.emplace_back(start, txt.data() + txt.size() - start);
  }

  return res;
}

std::string_view trim(const std::string &txt) {
  return trim(std::string_view(txt));
}

std::string_view trim(const std::string_view &txt) {
  auto start = txt.begin();
  while (start != txt.end()) {
    if (!std::isspace(*start)) {
      break;
    }
    ++start;
  }
  auto stop = txt.rbegin();
  while (stop != txt.rend()) {
    if (!std::isspace(*stop)) {
      break;
    }
    ++stop;
  }
  auto end_rem = std::distance(txt.rbegin(), stop);
  auto start_rem = std::distance(txt.begin(), start);
  std::cout << end_rem << ", " << start_rem << std::endl;
  std::string_view res(start, txt.size() - start_rem - end_rem);
  return res;
}
} // namespace aoc
