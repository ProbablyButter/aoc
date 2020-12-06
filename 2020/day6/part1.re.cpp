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
#include <unordered_set>

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string str;
  int sum = 0;
  std::unordered_set<char> questions;
  while (!in.eof())
  {
    std::getline(in, str);
    if (str.size())
    {
      for(auto c : str)
      {
        questions.insert(c);
      }
    }
    else
    {
      sum += questions.size();
      questions.clear();
    }
  }
  std::cout << sum << std::endl;
}
