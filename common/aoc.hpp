#ifndef AOC_HPP
#define AOC_HPP

#include <filesystem>
#include <string>
#include <string_view>

std::filesystem::path get_resource_path(const std::string &name);

namespace aoc
{
  long long svtoll(std::string_view str);
}

#endif
