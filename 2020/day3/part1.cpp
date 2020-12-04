#include "aoc.hpp"

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

  int64_t tree_count = 0;
  for (size_t i = 0; i < map.size(); ++i)
  {
    if (map[i][3 * i % map[i].size()] == '#')
    {
      ++tree_count;
    }
  }
  std::cout << tree_count << std::endl;
}
