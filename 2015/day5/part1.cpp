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
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;

  size_t nice_count = 0;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    int vowel_count = 0;
    bool repeat_letter = false;
    bool bad_str = false;
    for (size_t i = 0; i < line.size(); ++i) {
      switch (line[i]) {
      case 'a':
      case 'e':
      case 'i':
      case 'o':
      case 'u':
        ++vowel_count;
        break;
      default:
        break;
      }
      if (i + 1 < line.size()) {
        if (line[i] == line[i + 1]) {
          repeat_letter = true;
        }
        switch (line[i]) {
        case 'a':
          if (line[i + 1] == 'b') {
            bad_str = true;
          }
          break;
        case 'c':
          if (line[i + 1] == 'd') {
            bad_str = true;
          }
          break;
        case 'p':
          if (line[i + 1] == 'q') {
            bad_str = true;
          }
          break;
        case 'x':
          if (line[i + 1] == 'y') {
            bad_str = true;
          }
          break;
        default:
          break;
        }
      }
    }
    if (vowel_count >= 3 && repeat_letter && !bad_str) {
      ++nice_count;
    }
  }
  std::cout << nice_count << std::endl;
}
