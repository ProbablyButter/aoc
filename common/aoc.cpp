#include "aoc.hpp"

#include "whereami.h"

#include <cctype>

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

namespace aoc
{
  long long svtoll(std::string_view str)
  {
    long long res = 0;
    bool neg = false;
    auto pos = str.begin();
    while (pos != str.end())
    {
      if (!std::isspace(*pos))
      {
        break;
      }
      ++pos;
    }
    if (pos != str.end())
    {
      if (*pos == '-')
      {
        neg = true;
        ++pos;
      }
      else if (*pos == '+')
      {
        neg = false;
        ++pos;
      }
      while (pos != str.end())
      {
        if (std::isdigit(*pos))
        {
          res *= 10;
          res += (*pos) - '0';
        }
        ++pos;
      }
    }
    return res;
  }
} // namespace aoc
