/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = "char";
re2c:yyfill:enable = 0;
re2c:flags:tags = 0;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string str;
  std::vector<bool> taken;
  while (!in.eof())
  {
    int row = 0;
    int col = 0;
    std::getline(in, str);
    for (int i = 0; i < 7; ++i)
    {
      row *= 2;
      row += (str[i] == 'B');
    }

    for (int i = 0; i < 3; ++i)
    {
      col *= 2;
      col += (str[i + 7] == 'R');
    }
    int id = row * 8 + col;
    taken.resize(std::max<size_t>(taken.size(), id + 1), false);
    taken[id] = true;
  }
  for (int i = 1; i < taken.size() - 1; ++i)
  {
    if (taken[i - 1] && !taken[i] && taken[i + 1])
    {
      std::cout << i << std::endl;
      break;
    }
  }
}
