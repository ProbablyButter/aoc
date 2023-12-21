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

enum gate_type { BROADCAST, FLIPFLOP, CONJUNCTION };

void parse_gate(
    const std::string &line,
    std::unordered_map<std::string, std::vector<std::string>> &inputs,
    std::unordered_map<std::string, std::vector<std::string>> &outputs,
    std::unordered_map<std::string, gate_type> &types) {
  const char *YYCURSOR = line.data();
  const char *YYMARKER;
  const char *d0, *d1, *d2, *d3, *d4, *d5;
  /*!stags:re2c format = 'const char *@@;'; */
  std::string curr_gate;
parse_start:
  /*!re2c
"%" @d0 [a-z]+ @d1 {
  curr_gate = std::string(d0,d1-d0);
  outputs.emplace(curr_gate, std::vector<std::string>());
  types.emplace(curr_gate, FLIPFLOP);
  goto parse_outputs;
}
"&" @d0 [a-z]+ @d1 {
  curr_gate = std::string(d0,d1-d0);
  outputs.emplace(curr_gate, std::vector<std::string>());
  types.emplace(curr_gate, CONJUNCTION);
  goto parse_outputs;
}
"broadcaster" {
  curr_gate = "broadcaster";
  outputs.emplace(curr_gate, std::vector<std::string>());
  types.emplace(curr_gate, BROADCAST);
  goto parse_outputs;
}
[\x00] {
  return;
}
* {
goto parse_start;
}
   */

parse_outputs:
  /*!re2c
@d0 [a-z]+ @d1 {
  std::string next(d0,d1-d0);
  inputs[next].push_back(curr_gate);
  outputs.at(curr_gate).emplace_back(next);
  goto parse_outputs;
}
[\x00] {
  return;
}
* {
  goto parse_outputs;
}
   */
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  // maps a gate to its outputs
  std::unordered_map<std::string, std::vector<std::string>> outputs;
  std::unordered_map<std::string, std::vector<std::string>> inputs;
  std::unordered_map<std::string, gate_type> types;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    parse_gate(line, inputs, outputs, types);
  }
  std::unordered_map<std::string, bool> flipflop_states;
  std::unordered_map<std::string, std::unordered_map<std::string, bool>>
      conjunction_states;

  std::vector<std::tuple<std::string, bool, std::string>> to_process;
  int64_t low_count = 0;
  int64_t hi_count = 0;

  for (size_t button_count = 0; button_count < 1000; ++button_count) {
    to_process.clear();
    to_process.emplace_back("broadcaster", false, "button");
    size_t i = 0;
    for (; i < to_process.size(); ++i) {
      auto val = to_process[i];
      // std::cout << std::get<2>(val) << " -" << std::get<1>(val) << "-> "
      //<< std::get<0>(val) << std::endl;
      auto titer = types.find(std::get<0>(val));
      if(titer == types.end())
      {
        continue;
      }
      switch (titer->second) {
      case BROADCAST: {
        for (auto &outs : outputs.at(std::get<0>(val))) {
          to_process.emplace_back(outs, std::get<1>(val), std::get<0>(val));
        }
      } break;
      case FLIPFLOP: {
        if (std::get<1>(val)) {
          // high, do nothing
        } else {
          auto iter = flipflop_states.emplace(std::get<0>(val), true);
          if (!iter.second) {
            iter.first->second = !iter.first->second;
          }
          if (iter.first->second) {
            // flipped on, send high
            for (auto &outs : outputs.at(std::get<0>(val))) {
              to_process.emplace_back(outs, true, std::get<0>(val));
            }
          } else {
            // flipped off, send low
            for (auto &outs : outputs.at(std::get<0>(val))) {
              to_process.emplace_back(outs, false, std::get<0>(val));
            }
          }
        }
      } break;
      case CONJUNCTION: {
        conjunction_states[std::get<0>(val)][std::get<2>(val)] =
            std::get<1>(val);
        bool all_high = true;
        if (conjunction_states.at(std::get<0>(val)).size() ==
            inputs.at(std::get<0>(val)).size()) {
          for (auto &ins : conjunction_states.at(std::get<0>(val))) {
            if (!ins.second) {
              all_high = false;
              break;
            }
          }
        } else {
          all_high = false;
        }
        if (all_high) {
          // send low
          for (auto &outs : outputs.at(std::get<0>(val))) {
            to_process.emplace_back(outs, false, std::get<0>(val));
          }
        } else {
          // send high
          for (auto &outs : outputs.at(std::get<0>(val))) {
            to_process.emplace_back(outs, true, std::get<0>(val));
          }
        }

      } break;
      }
    }
    for (auto &v : to_process) {
      if (std::get<1>(v)) {
        ++hi_count;
      } else {
        ++low_count;
      }
    }
    //std::cout << low_count << ", " << hi_count << std::endl;
  }
  std::cout << low_count * hi_count << std::endl;
}
