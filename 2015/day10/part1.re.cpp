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
#include <memory>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

int main(int argc, char **argv) {
  std::string seq = "1113222113";
  for (size_t iter = 0; iter < 40; ++iter) {
    std::stringstream out;
    size_t i = 0;
    size_t count = 1;
    char curr = seq[0];
    while (true) {
      if (i >= seq.size()) {
        out << count << curr;
        break;
      }
      if (i + 1 < seq.size()) {
        if (curr == seq[i + 1]) {
          ++count;
        } else {
          out << count << curr;
          count = 1;
          curr = seq[i + 1];
        }
      }
      ++i;
    }
    seq = out.str();
  }
  std::cout << seq.size() << std::endl;
}
