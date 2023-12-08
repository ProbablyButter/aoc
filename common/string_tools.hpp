#ifndef AOC_STRING_TOOL_HPP
#define AOC_STRING_TOOL_HPP

#include <cstdint>
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
} // namespace aoc

#endif
