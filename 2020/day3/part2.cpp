#include "aoc.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv)
{
  std::vector<std::string> map;
  {
    std::filesystem::path in_path = get_resource_path("input.txt");
    std::ifstream in(in_path);

    std::string line;
    while (!in.eof())
    {
      std::getline(in, line);
      if (line.empty())
      {
        break;
      }
      map.emplace_back(std::move(line));
    }
  }

  std::array<int, 5> row_incs = {1, 1, 1, 1, 2};
  std::array<int, 5> col_incs = {1, 3, 5, 7, 1};
  int mult = 1;
  for (size_t j = 0; j < row_incs.size(); ++j)
  {
    int tree_count = 0;
    for (size_t i = 0; i < map.size(); i += row_incs[j])
    {
      int col = col_incs[j] * i / row_incs[j] % map[i].size();
      if (map[i][col] == '#')
      {
        ++tree_count;
      }
    }
    std::cout << tree_count << std::endl;
    mult *= tree_count;
  }
  std::cout << mult << std::endl;
}
