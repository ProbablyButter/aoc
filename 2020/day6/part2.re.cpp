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
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string str;
  int sum = 0;
  int people = 0;
  std::unordered_map<char, int> questions;
  while (!in.eof())
  {
    std::getline(in, str);
    if (str.size())
    {
      ++people;
      for (auto c : str)
      {
        questions[c];
        ++questions[c];
#if 0
        if (questions.insert(std::make_pair(c, 1)).second)
        {
          ++questions[c];
        }
#endif
      }
    }
    else
    {
      for (auto& k : questions)
      {
        sum += (k.second == people);
      }
      questions.clear();
          people = 0;
    }
  }
  std::cout << sum << std::endl;
}
