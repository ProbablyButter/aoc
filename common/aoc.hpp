#ifndef AOC_HPP
#define AOC_HPP

#include <filesystem>
#include <string>
#include <string_view>

std::filesystem::path get_resource_path(const std::string &name);

namespace aoc
{
  ///
  /// @param base what base to assume the input is in, or 0 to try and auto-detect
  ///
  long long svtoll(const std::string_view& str, size_t base = 0);
}

#endif
