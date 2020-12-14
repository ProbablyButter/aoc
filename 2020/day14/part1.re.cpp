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

void parse_instr(const std::string &line,
                 std::vector<uint64_t> &mem,
                 uint64_t &or_mask,
                 uint64_t &and_mask)
{
  const char *YYCURSOR = line.data();
  const char *YYMARKER;
  const char *addr_start;
  const char *addr_stop;
  const char *val_start;
  uint64_t addr = 0;
  uint64_t val = 0;
  /*!stags:re2c format = 'const char *@@;'; */
parse_start:
  /*!re2c
"mask = " {
goto parse_mask;
}
"mem[" @addr_start [0-9]+ @addr_stop "] = " @val_start [0-9]+ {
addr = atoll(addr_start);
val = atoll(val_start);
mem.resize(std::max<size_t>(mem.size(), addr+1));
mem[addr] = ((val | or_mask) & and_mask) & 0b111111111111111111111111111111111111ull;
return;
}
[\x00] {
return;
}
* {
goto parse_start;
}
   */

parse_mask:
  or_mask = 0;
  and_mask = 0;
mask_loop:
  /*!re2c
  [0] {
  and_mask <<= 1;
  or_mask <<= 1;
  goto mask_loop;
}
  [1] {
  or_mask <<= 1;
  or_mask |= 1;
  and_mask <<= 1;
  and_mask |= 1;
  goto mask_loop;
}
  [xX] {
  and_mask <<= 1;
  and_mask |= 1;
  or_mask <<= 1;
  goto mask_loop;
}
[\x00] {
  goto done;
}
* {
  goto mask_loop;
}
   */

done:
  return;
}

int main(int argc, char **argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");

  std::ifstream in(in_path);
  std::string line;
  std::vector<uint64_t> mem;
  uint64_t or_mask = 0;
  uint64_t and_mask = 0;
  while (true)
  {
    std::getline(in, line);
    if (in.eof())
    {
      break;
    }
    parse_instr(line, mem, or_mask, and_mask);
  }
  size_t sum = 0;
  for(auto v : mem)
  {
    sum += v;
  }
  std::cout << std::dec << sum << std::endl;
}
