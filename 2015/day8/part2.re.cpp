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

std::string encode_string(const std::string &line) {
  std::stringstream out;
  out << "\"";
  const char *YYCURSOR = line.data();
  const char *YYMARKER;
  /*!stags:re2c format = 'const char *@@;'; */
parse_start:
  /*!re2c
"\\\\" {
  out << "\\\\\\\\";
  goto parse_start;
}
"\\\"" {
  out << "\\\\\\\"";
  goto parse_start;
}
"\\x" [0-9a-fA-F]{2} {
  out << "\\\\x" << YYMARKER[1] << YYMARKER[2];
  goto parse_start;
}
"\"" {
  out << "\\\"";
  goto parse_start;
}
[\x00] {
  out << "\"";
  return out.str();
}
[0-9a-zA-Z] {
  out << *(YYCURSOR-1);
  goto parse_start;
}
* {
  out << *(YYCURSOR-1);
  goto parse_start;
}
   */
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  int64_t code_len = 0;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto encoded = encode_string(line);
    code_len += encoded.size() - line.size();
  }
  std::cout << code_len << std::endl;
}
