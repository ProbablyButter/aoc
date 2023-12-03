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

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::vector<std::string> lines;
  std::string line;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    lines.emplace_back(line);
  }
  // find all symbols
  std::set<std::pair<long long, long long>> symbols;
  for (long long row = 0; row < lines.size(); ++row) {
    for (long long col = 0; col < lines[row].size(); ++col) {
      switch (lines[row][col]) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '.':
        // all of these are not symbols
        break;
      default:
        // everything else is a symbol
        symbols.emplace(row, col);
        break;
      }
    }
  }
  // now sum up parts
  int sum = 0;
  for (long long row = 0; row < lines.size(); ++row) {
    long long col = 0;
    while (true) {
      if (col >= lines[row].size()) {
        break;
      }
      if (lines[row][col] >= '0' && lines[row][col] <= '9') {
        // start
        int v = atoi(&lines[row][col]);
        long long cstart = col;
        while (col < lines[row].size() &&
               (lines[row][col] >= '0' && lines[row][col] <= '9')) {
          ++col;
        }
        // check all neighbors for a symbol, including diagonals
        // top
        bool in_symbol = false;
        for (long long i = cstart - 1; i <= col; ++i) {
          if (symbols.find(std::pair<long long, long long>(row - 1, i)) !=
              symbols.end()) {
            in_symbol = true;
            break;
          }
        }
        // bottom
        for (long long i = cstart - 1; i <= col; ++i) {
          if (symbols.find(std::pair<long long, long long>(row + 1, i)) !=
              symbols.end()) {
            in_symbol = true;
            break;
          }
        }
        // left
        if (symbols.find(std::pair<long long, long long>(row, cstart - 1)) !=
            symbols.end()) {
          in_symbol = true;
        }
        // right
        if (symbols.find(std::pair<long long, long long>(row, col)) !=
            symbols.end()) {
          in_symbol = true;
        }
        if (in_symbol) {
          sum += v;
        }
      } else {
        ++col;
      }
    }
  }

  std::cout << sum << std::endl;
}
