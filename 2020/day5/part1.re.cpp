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
  int max_id = 0;
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
    max_id = std::max(id, max_id);
  }
  std::cout << max_id << std::endl;
}
