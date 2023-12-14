/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"
#include "graph_tools.hpp"
#include "hash.hpp"
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

int main(int argc, char **argv) {
  std::string input = "iwrupvqb";
  int64_t num = 0;

  while (true) {
    std::stringstream str;
    str << input << num;
    aoc::md5 hash;
    hash.init();
    std::string tmp = str.str();
    hash.append(reinterpret_cast<const uint8_t *>(tmp.data()), tmp.size());
    hash.finalize();
    std::string digest = hash.str();
    bool valid = true;
    for (int i = 0; i < 6; ++i) {
      if (digest[i] != '0') {
        valid = false;
        break;
      }
    }
    if (valid) {
      break;
    }
    ++num;
  }
  std::cout << num << std::endl;
}
