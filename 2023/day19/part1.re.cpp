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

struct instr {
  std::string attr;
  enum instr_type { NONE, LT, GT };
  instr_type type;
  int64_t value;
  std::string next;
  int execute(const std::unordered_map<std::string, int64_t> &part,
              std::string &n) {
    n = "";
    switch (type) {
    case NONE:
      if (next == "R") {
        return -1;
      } else if (next == "A") {
        return 1;
      }
      n = next;
      break;
    case LT:
      if (part.at(attr) < value) {
        if (next == "R") {
          return -1;
        } else if (next == "A") {
          return 1;
        }
        n = next;
      }
      break;
    case GT:
      if (part.at(attr) > value) {
        if (next == "R") {
          return -1;
        } else if (next == "A") {
          return 1;
        }
        n = next;
      }
      break;
    }
    return 0;
  }
};

void parse_workflow(
    const std::string &line,
    std::unordered_map<std::string, std::vector<instr>> &workflows) {
  const char *YYCURSOR = line.data();
  const char *YYMARKER;
  const char *d0, *d1, *d2, *d3, *d4, *d5;
  int first_val = -1;
  int curr_val = 0;
  instr next;
  /*!stags:re2c format = 'const char *@@;'; */
  std::vector<instr> *wf;
parse_start:
  /*!re2c
@d0 [a-z]+ @d1 {
  auto res = workflows.emplace(std::string(d0,d1-d0), std::vector<instr>());
  wf = &res.first->second;
  goto parse_instrs;
}
[\x00] {
  return;
}
* {
goto parse_start;
}
   */

parse_instrs:
  /*!re2c
@d0 [a-z]+ @d1 "<" @d2 "-"? [0-9]+ @d3 ":" {
  next.attr = std::string(d0, d1-d0);
  next.type = instr::LT;
  next.value = aoc::to_int<int64_t>(d2, d3);
  goto parse_next;
}
@d0 [a-z]+ @d1 ">" @d2 "-"? [0-9]+ @d3 ":" {
  next.attr = std::string(d0, d1-d0);
  next.type = instr::GT;
  next.value = aoc::to_int<int64_t>(d2, d3);
  goto parse_next;
}
@d0 [a-z]+ @d1 [,}] {
  next.attr = "";
  next.type = instr::NONE;
  next.value = 0;
  next.next = std::string(d0,d1-d0);
  wf->push_back(next);
  goto parse_instrs;
}
"R" {
  next.type = instr::NONE;
  next.attr = "";
  next.value = 0;
  next.next = "R";
  wf->push_back(next);
  goto parse_instrs;
}
"A" {
  next.type = instr::NONE;
  next.attr = "";
  next.value = 0;
  next.next = "A";
  wf->push_back(next);
  goto parse_instrs;
}
[\x00] {
  return;
}
* {
  goto parse_instrs;
}
   */
parse_next:
  /*!re2c
@d0 [a-zAR]+ @d1 {
  next.next = std::string(d0,d1-d0);
  wf->push_back(next);
  goto parse_instrs;
}
[\x00] {
  return;
}
* {
  goto parse_next;
}
   */
}

void parse_part(const std::string &line,
                std::vector<std::unordered_map<std::string, int64_t>> &parts) {
  const char *YYCURSOR = line.data();
  const char *YYMARKER;
  const char *d0, *d1, *d2, *d3;
  int first_val = -1;
  int curr_val = 0;
  /*!stags:re2c format = 'const char *@@;'; */
  parts.emplace_back();
parse_start:
  /*!re2c
@d0 [a-zA-Z]+ @d1 "=" @d2 "-"? [0-9]+ @d3 {
  parts.back().emplace(std::string(d0,d1-d0), aoc::to_int<int64_t>(d2, d3));
  goto parse_start;
}
[\x00] {
  return;
}
* {
goto parse_start;
}
   */
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  std::unordered_map<std::string, std::vector<instr>> workflows;
  std::vector<std::unordered_map<std::string, int64_t>> parts;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    if (line.empty()) {
      break;
    }
    parse_workflow(line, workflows);
  }
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    parse_part(line, parts);
  }
  int64_t sum = 0;
  for (auto &p : parts) {
    std::string name = "in";
    std::string next;
    while (true) {
      auto &wf = workflows.at(name);
      for (auto &v : wf) {
        int res = v.execute(p, next);
        switch (res) {
        case 0:
          if (!next.empty()) {
            name = next;
            goto next_wf;
          }
          break;
        case 1:
          for (auto &attr : p) {
            sum += attr.second;
          }
          goto done;
        case -1:
          goto done;
        }
      }
    next_wf:;
    }
  done:;
  }
  std::cout << sum << std::endl;
}
