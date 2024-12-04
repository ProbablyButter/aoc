/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"
#include "geometry_tools.hpp"
#include "graph_tools.hpp"
#include "string_tools.hpp"

#include <algorithm>
#include <charconv>
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

long long parse_mul(const std::string &data)
{
  long long sum = 0;
  const char *YYCURSOR = data.data();
  const char *YYMARKER;
  const char *d0, *d1, *d2, *d3;
  /*!stags:re2c format = 'const char *@@;'; */
 parse_start:
  // look for mul(
  /*!re2c
"mul(" @d0 [0-9]+ @d1 "," @d2 [0-9]+ @d3 ")" {
 long long v0 = aoc::to_int<long long>(d0, d1);
 long long v1 = aoc::to_int<long long>(d2, d3);
 sum += v0 * v1;
goto parse_start;
}
[\x00] {
return sum;
}
* {
goto parse_start;
}
   */
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  // read in entire file
  std::string data(std::istreambuf_iterator<char>{in},
                   std::istreambuf_iterator<char>{});
  std::cout << parse_mul(data) << std::endl;
}
