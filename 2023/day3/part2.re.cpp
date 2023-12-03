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
#include <cctype>
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
  int sum = 0;

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
      case '*': {
        // only a gear if we have exactly 2 surrounding part numbers
        bool has_digit[9] = {false};
        if (row > 0) {
          if (col > 0) {
            has_digit[0] = std::isdigit(lines[row - 1][col - 1]);
          }
          has_digit[1] = std::isdigit(lines[row - 1][col]);
          if (col + 1 < lines[row - 1].size()) {
            has_digit[2] = std::isdigit(lines[row - 1][col + 1]);
          }
        }
        if (col > 0) {
          has_digit[3] = std::isdigit(lines[row][col - 1]);
        }
        if (col + 1 < lines[row].size()) {
          has_digit[5] = std::isdigit(lines[row][col + 1]);
        }
        if (row + 1 < lines.size()) {
          if (col > 0) {
            has_digit[6] = std::isdigit(lines[row + 1][col - 1]);
          }
          has_digit[7] = std::isdigit(lines[row + 1][col]);
          if (col + 1 < lines[row + 1].size()) {
            has_digit[8] = std::isdigit(lines[row + 1][col + 1]);
          }
        }
        // count number of neighbor parts
        int neighbors = 0;
        if (has_digit[0]) {
          ++neighbors;
        }
        if (has_digit[1] && !has_digit[0]) {
          ++neighbors;
        }
        if (has_digit[2] && !has_digit[1]) {
          ++neighbors;
        }
        if (has_digit[3]) {
          ++neighbors;
        }
        if (has_digit[5]) {
          ++neighbors;
        }

        if (has_digit[6]) {
          ++neighbors;
        }
        if (has_digit[7] && !has_digit[6]) {
          ++neighbors;
        }
        if (has_digit[8] && !has_digit[7]) {
          ++neighbors;
        }
        if (neighbors == 2) {
          // identify the two gears
          int tmp = 1;
          if (has_digit[0]) {
            // gear top left
            int ctmp = col - 1;
            while (ctmp >= 1 && std::isdigit(lines[row - 1][ctmp])) {
              --ctmp;
            }
            if (!std::isdigit(lines[row - 1][ctmp])) {
              ++ctmp;
            }
            tmp *= atoi(&lines[row - 1][ctmp]);
          }
          if (has_digit[1] && !has_digit[0]) {
            // gear top
            tmp *= atoi(&lines[row - 1][col]);
          }
          if (has_digit[2] && !has_digit[1]) {
            // gear top right
            tmp *= atoi(&lines[row - 1][col + 1]);
          }
          if (has_digit[3]) {
            // gear left
            int ctmp = col - 1;
            while (ctmp >= 1 && std::isdigit(lines[row][ctmp])) {
              --ctmp;
            }
            if (!std::isdigit(lines[row][ctmp])) {
              ++ctmp;
            }
            tmp *= atoi(&lines[row][ctmp]);
          }
          if (has_digit[5]) {
            // gear right
            tmp *= atoi(&lines[row][col + 1]);
          }

          if (has_digit[6]) {
            // gear bottom left
            int ctmp = col - 1;
            while (ctmp >= 1 && std::isdigit(lines[row + 1][ctmp])) {
              --ctmp;
            }
            if (!std::isdigit(lines[row + 1][ctmp])) {
              ++ctmp;
            }
            tmp *= atoi(&lines[row + 1][ctmp]);
          }
          if (has_digit[7] && !has_digit[6]) {
            // gear bottom
            tmp *= atoi(&lines[row + 1][col]);
          }
          if (has_digit[8] && !has_digit[7]) {
            // gear bottom right
            tmp *= atoi(&lines[row + 1][col + 1]);
          }
          sum += tmp;
        }
      }

      default:
        // everything else is a symbol
        break;
      }
    }
  }

  std::cout << sum << std::endl;
}
