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

struct instr {
  std::string attr;
  enum instr_type { NONE, LT, GT };
  instr_type type;
  int64_t value;
  std::string next;
  aoc::hslab<int64_t, 4> execute(
      aoc::hslab<int64_t, 4> p, std::vector<aoc::hslab<int64_t, 4>> &accept,
      std::vector<std::pair<std::string, aoc::hslab<int64_t, 4>>> &to_process) {
    aoc::hslab<int64_t, 4> res{};
    switch (type) {
    case NONE:
      if (next == "R") {
        // reject all
      } else if (next == "A") {
        accept.emplace_back(p);
      } else {
        to_process.emplace_back(next, p);
      }
      break;
    case LT: {
      res = p;
      size_t idx;
      switch (attr[0]) {
      case 'x':
        idx = 0;
        break;
      case 'm':
        idx = 1;
        break;
      case 'a':
        idx = 2;
        break;
      case 's':
        idx = 3;
        break;
      default:
        throw std::runtime_error("unknown attr");
      }
      // keep in p everything which satisfies condition
      p.upper[idx] = std::max<int64_t>(std::min<int64_t>(p.upper[idx], value),
                                       p.lower[idx]);
      // remove from res everything which satisfies the condition
      res.lower[idx] = std::min<int64_t>(
          std::max<int64_t>(res.lower[idx], value), res.upper[idx]);
      if (p.volume()) {
        if (next == "R") {
        } else if (next == "A") {
          accept.emplace_back(p);
        } else {
          to_process.emplace_back(next, p);
        }
      }
    } break;
    case GT:
      res = p;
      size_t idx;
      switch (attr[0]) {
      case 'x':
        idx = 0;
        break;
      case 'm':
        idx = 1;
        break;
      case 'a':
        idx = 2;
        break;
      case 's':
        idx = 3;
        break;
      default:
        throw std::runtime_error("unknown attr");
      }
      // keep in p everything which satisfies condition
      p.lower[idx] = std::min<int64_t>(
          std::max<int64_t>(p.lower[idx], value + 1), p.upper[idx]);
      // remove from res everything which satisfies the condition
      res.upper[idx] = std::max<int64_t>(
          std::min<int64_t>(res.upper[idx], value + 1), res.lower[idx]);
      if (p.volume()) {
        if (next == "R") {
        } else if (next == "A") {
          accept.emplace_back(p);
        } else {
          to_process.emplace_back(next, p);
        }
      }
      break;
    }
    return res;
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

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  std::unordered_map<std::string, std::vector<instr>> workflows;
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
  using part_type = aoc::hslab<int64_t, 4>;
  std::vector<part_type> accept;
  std::vector<std::pair<std::string, part_type>> to_process;
  {
    part_type p;
    for (size_t i = 0; i < 4; ++i) {
      p.lower[i] = 1;
      p.upper[i] = 4001;
    }
    to_process.emplace_back("in", p);
  }
  while (to_process.size()) {
    auto value = to_process.back();
    to_process.pop_back();
    auto &wf = workflows.at(value.first);

    for (auto &wf_instr : wf) {
      value.second = wf_instr.execute(value.second, accept, to_process);
      if (value.second.volume() == 0) {
        // nothing left
        break;
      }
    }
  }
  int64_t sum = 0;
  for (auto &v : accept) {
    sum += v.volume();
  }
  std::cout << sum << std::endl;
}
