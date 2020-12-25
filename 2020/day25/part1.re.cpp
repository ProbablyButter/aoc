/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

long transform(long subject_num, long loop_size)
{
  long val = 1;
  for (size_t i = 0; i < loop_size; ++i)
  {
    val = (val * subject_num) % 20201227;
  }
  return val;
}

long find_loop_val(long pub_val)
{
  long res = 1;
  long val = 1;
  while(true)
  {
    val = (val * 7) % 20201227;
    if(val == pub_val)
    {
      break;
    }
    ++res;
  }
  return res;
}

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");

  std::ifstream in(in_path);
  std::string line;

  // parse input
  long pub_val1, pub_val2;
  in >> pub_val1;
  in >> pub_val2;
  // test values
  //pub_val1 = 5764801;
  //pub_val2 = 17807724;
  constexpr long rem = 20201227;
  long loop_size_1 = find_loop_val(pub_val1);
  long loop_size_2 = find_loop_val(pub_val2);

  long encrypt_1 = transform(pub_val1, loop_size_2);
  long encrypt_2 = transform(pub_val2, loop_size_1);
  std::cout << loop_size_1 << ", " << loop_size_2 << std::endl;
  std::cout << encrypt_1 << ", " << encrypt_2 << std::endl;
}
