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
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main(int argc, char **argv)
{
  std::vector<int> sequence = {0, 20, 7, 16, 1, 18, 15};
  // keep track of the turn a number was previously spoken before last
  std::unordered_map<int, int> last_seen;
  // just assume every entry in sequence will be unique
  last_seen.reserve(30000000);
  int next_num = sequence.back();
  for (int i = 0; i < 30000000; ++i)
  {
    if (i < sequence.size())
    {
      last_seen[sequence[i]] = i;
    }
    else
    {
      auto iter = last_seen.find(next_num);
      if (iter == last_seen.end())
      {
        // not found
        last_seen.emplace(next_num, i - 1);
        next_num = 0;
      }
      else
      {
        auto tmp = iter->second;
        iter->second = i - 1;
        next_num = i-1-tmp;
        //sequence.emplace_back(i - 1 - tmp);
      }
    }
  }
  //std::cout << last_seen.size() << std::endl;
  std::cout << next_num << std::endl;
}
