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
#include <unordered_map>
#include <unordered_set>
#include <vector>

int main(int argc, char **argv)
{
  std::vector<int> sequence = {0, 20, 7, 16, 1, 18, 15};
  // keep track of the turn a number was previously spoken before last
  std::unordered_map<int, int> last_seen;
  for (int i = 0; i < 2020; ++i)
  {
    if (i < sequence.size())
    {
      last_seen[sequence[i]] = i;
    }
    else
    {
      auto iter = last_seen.find(sequence.back());
      if (iter == last_seen.end())
      {
        // not found
        last_seen[sequence.back()] = i - 1;
        sequence.emplace_back(0);
      }
      else
      {
        auto tmp = iter->second;
        iter->second = i - 1;
        sequence.emplace_back(i - 1 - tmp);
      }
    }
  }
  std::cout << sequence.back() << std::endl;
}
