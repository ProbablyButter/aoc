#include "aoc.hpp"

#include <algorithm>
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
  for (int &v1 : vals)
  {
    for (int &v2 : vals)
    {
      int desired = 2020 - v1 - v2;
      // assume ordering: v2 <= v1 <= desired
      auto pos = std::lower_bound(std::max(&v1, &v2), stop, desired);
      if (pos != stop && *pos == desired)
      {
        std::cout << desired * v1 * v2 << std::endl;
        return 0;
      }
    }
  }
}
