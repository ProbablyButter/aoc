#include "aoc.hpp"

#include "whereami.h"

std::filesystem::path get_resource_path(const std::string &name)
{
  std::string p(wai_getExecutablePath(nullptr, 0, nullptr), '\0');
  int dir_name_length;
  wai_getExecutablePath(&p[0], p.size(), &dir_name_length);
  p.resize(dir_name_length);
  std::filesystem::path res(p);
  res /= name;
  return res;
}
