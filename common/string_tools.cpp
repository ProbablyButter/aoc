#include "string_tools.hpp"

#include <cctype>
#include <cstring>
#include <sstream>

namespace aoc {
std::vector<std::string_view>
split(const std::string_view &txt, const std::string &split_str, int64_t maxsplit) {
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
    if (start != stop) {
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
  std::string_view res(start, txt.size() - start_rem - end_rem);
  return res;
}

std::string join(const std::vector<std::string_view> &strs,
                 const std::string_view &delim) {
  std::stringstream out;
  for (size_t i = 0; i + 1 < strs.size(); ++i) {
    out << strs[i] << delim;
  }
  if (strs.size()) {
    out << strs.back();
  }
  return out.str();
}

std::string join(const std::vector<std::string> &strs,
                 const std::string_view &delim) {
  std::stringstream out;
  for (size_t i = 0; i + 1 < strs.size(); ++i) {
    out << strs[i] << delim;
  }
  if (strs.size()) {
    out << strs.back();
  }
  return out.str();
}

double to_double(const char *str) {
  char* tmp;
  return std::strtod(str, &tmp);
}

double to_double(const char *start, const char *stop) {
  double res;
  std::from_chars(start, stop, res);
  return res;
}

double to_double(const std::string_view &str) {
  double res;
  std::from_chars(str.data(), str.data() + str.size(), res);
  return res;
}

double to_double(const std::string &str) {
  double res;
  std::from_chars(str.data(), str.data() + str.size(), res);
  return res;
}

} // namespace aoc
