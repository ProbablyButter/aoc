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

void write_mem(std::unordered_map<uint64_t, uint64_t> &mem,
               uint64_t start_addr,
               uint64_t val,
               std::array<bool, 36> &float_mask,
               long num_writes,
               long num_bits)
{
  // float_mask is MSB first
  for (uint64_t i = 0; i < num_writes; ++i)
  {
    // shift bits of i to float_mask positions
    uint64_t waddr = start_addr;
    size_t bit = 0;
    for (size_t j = 0; j < float_mask.size(); ++j)
    {
      if (float_mask[j])
      {
        int shift_amount = (float_mask.size() - j - 1);
        // first unset the bit
        waddr = waddr & ~(1ull << shift_amount);
        // extract the next MSB bit from i, then shift that to the correct position
        uint64_t extracted_bit = 0;
        if (i & (1ull << (num_bits - bit - 1)))
        {
          extracted_bit = 1ull << shift_amount;
        }
        waddr |= extracted_bit;
        ++bit;
      }
    }
    mem[waddr] = val;
  }
}

void parse_instr(const std::string &line,
                 std::unordered_map<uint64_t, uint64_t> &mem,
                 uint64_t &or_mask,
                 std::array<bool, 36> &float_mask)
{
  const char *YYCURSOR = line.data();
  const char *YYMARKER;
  const char *addr_start;
  const char *addr_stop;
  const char *val_start;
  uint64_t addr = 0;
  uint64_t val = 0;
  uint64_t idx = 0;
  /*!stags:re2c format = 'const char *@@;'; */
parse_start:
  /*!re2c
"mask = " {
goto parse_mask;
}
"mem[" @addr_start [0-9]+ @addr_stop "] = " @val_start [0-9]+ {
addr = atoll(addr_start);
val = atoll(val_start);
long num_writes = 1;
long num_bits = 0;
for(auto v : float_mask)
{
if(v)
{
num_writes *= 2;
++num_bits;
}
}
write_mem(mem, addr | or_mask, val, float_mask, num_writes, num_bits);
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
  idx = 0;
  for (auto &v : float_mask)
  {
    v = false;
  }
mask_loop:
  /*!re2c
  [0] {
  ++idx;
  or_mask <<= 1;
  goto mask_loop;
}
  [1] {
  or_mask <<= 1;
  or_mask |= 1;
  ++idx;
  goto mask_loop;
}
  [xX] {
  float_mask[idx] = 1;
  ++idx;
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
  std::unordered_map<uint64_t, uint64_t> mem;
  uint64_t or_mask = 0;
  std::array<bool, 36> float_mask;
  while (true)
  {
    std::getline(in, line);
    if (in.eof())
    {
      break;
    }
    parse_instr(line, mem, or_mask, float_mask);
  }
  size_t sum = 0;
  size_t max_addr = 0;
  for (auto v : mem)
  {
    max_addr = std::max(v.first, max_addr);
    sum += v.second;
  }
  std::cout << sum << std::endl;
}
