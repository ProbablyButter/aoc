#ifndef AOC_STRING_TOOL_HPP
#define AOC_STRING_TOOL_HPP

#include <charconv>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <string_view>
#include <vector>

namespace aoc {
///
/// @param txt
/// @param split_str empty string to split on all whitespace (ignoring all empty
/// string)
/// @param maxsplit max number of splits of result, or -1 for no limit.
/// maxsplit=0 means result is full txt.
///
std::vector<std::string_view> split(const std::string &txt,
                                    const std::string &split_str,
                                    int64_t maxsplit = -1);

std::string_view trim(const std::string &txt);

std::string_view trim(const std::string_view &txt);

std::string join(const std::vector<std::string_view> &strs,
                 const std::string_view &delim);

std::string join(const std::vector<std::string> &strs,
                 const std::string_view &delim);

template <class T> T to_int(const char *start, int base = 10) {
  char *tmp;
  return std::strtoll(start, &tmp, base);
}

template <class T>
T to_int(const char *start, const char *stop, int base = 10) {
  T res;
  std::from_chars(start, stop, res, base);
  return res;
}

template <class T> T to_int(const std::string_view &str, int base = 10) {
  T res;
  std::from_chars(str.data(), str.data() + str.size(), res, base);
  return res;
}

template <class T> T to_int(const std::string &str, int base = 10) {
  T res;
  std::from_chars(str.data(), str.data() + str.size(), res, base);
  return res;
}

double to_double(const char *str);
double to_double(const char *start, const char *stop);

double to_double(const std::string_view &str);
double to_double(const std::string &str);
} // namespace aoc

#endif
