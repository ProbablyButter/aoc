#include "aoc.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char **argv)
{
  std::vector<int> vals;
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  while (!in.eof())
  {
    int v;
    in >> v;
    vals.push_back(v);
  }
  std::sort(vals.begin(), vals.end());
  int *stop = vals.data() + vals.size();
  for (int &v : vals)
  {
    int desired = 2020 - v;
    auto pos = std::lower_bound(&v, stop, desired);
    if (pos != stop && *pos == desired)
    {
      std::cout << desired * v << std::endl;
    }
  }
}
