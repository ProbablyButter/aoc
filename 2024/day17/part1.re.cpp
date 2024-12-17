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
#include "matrix.hpp"
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

struct machine {
  int64_t A, B, C;
  // instruction pointer
  int64_t ip = 0;
  std::vector<int> instrs;
  std::vector<int> output;

  int64_t get_combo() const {
    switch (instrs[ip + 1]) {
    case 0:
    case 1:
    case 2:
    case 3:
      return instrs[ip + 1];
    case 4:
      return A;
    case 5:
      return B;
    case 6:
      return C;
    default:
      throw std::runtime_error("invalid combo");
    }
  }

  // returns whether step was halted or not
  bool step() {
    std::cout << "step " << ip << std::endl;
    if (ip + 1 >= instrs.size()) {
      return true;
    }
    switch (instrs[ip]) {
    case 0:
      std::cout << "adv " << get_combo() << std::endl;
      // adv A = A / 2**combo
      if (get_combo() >= 64) {
        throw std::runtime_error("combo too big");
      }
      A = A / (1ll << get_combo());
      ip += 2;
      break;
    case 1:
      // bxl B = B ^ literal
      std::cout << "bxl " << instrs[ip + 1] << std::endl;
      B = B ^ instrs[ip + 1];
      ip += 2;
      break;
    case 2:
      // bst B = combo % 8
      std::cout << "bst " << get_combo() << std::endl;
      B = get_combo() % 8;
      ip += 2;
      break;
    case 3:
      // jnz: if A != 0, ip = literal
      std::cout << "jnz " << instrs[ip + 1] << std::endl;
      if (A != 0) {
        ip = instrs[ip + 1];
        if (ip % 2 != 0) {
          throw std::runtime_error("ip not even");
        }
      } else {
        ip += 2;
      }
      break;
    case 4:
      // bxc B = B ^ C
      std::cout << "bxc " << B << ", " << C << std::endl;
      B = B ^ C;
      ip += 2;
      break;
    case 5:
      // out outputs combo % 8
      std::cout << "out " << get_combo() << std::endl;
      output.push_back(get_combo() % 8);
      ip += 2;
      break;
    case 6:
      // bdv B = A / 2**combo
      std::cout << "bdv " << get_combo() << std::endl;
      if (get_combo() >= 64) {
        throw std::runtime_error("combo too big");
      }
      B = A / (1 << get_combo());
      ip += 2;
      break;
    case 7:
      // cdv C = A / 2**combo
      std::cout << "cdv " << get_combo() << std::endl;
      if (get_combo() >= 64) {
        throw std::runtime_error("combo too big");
      }
      C = A / (1 << get_combo());
      ip += 2;
      break;
    default:
      throw std::runtime_error("bad instr");
    }
    return false;
  }
};

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);

  machine state;

  std::string line;
  // read registers
  {
    std::getline(in, line);
    auto tmp = aoc::split(line);
    state.A = aoc::to_int<int64_t>(tmp.back());
  }
  {
    std::getline(in, line);
    auto tmp = aoc::split(line);
    state.B = aoc::to_int<int64_t>(tmp.back());
  }
  {
    std::getline(in, line);
    auto tmp = aoc::split(line);
    state.C = aoc::to_int<int64_t>(tmp.back());
  }
  std::getline(in, line);
  // read instructions
  {
    std::getline(in, line);
    auto tmp = aoc::split(line);
    auto tmp2 = aoc::split(tmp[1], ",");
    for (int i = 0; i < tmp2.size(); ++i) {
      state.instrs.emplace_back(aoc::to_int<int>(tmp2[i]));
      std::cout << state.instrs.back() << ", ";
    }
    std::cout << std::endl;
  }
  while (!state.step()) {
  }
  for (auto &v : state.output) {
    std::cout << v << ",";
  }
  std::cout << std::endl;
  std::cout << state.A << ", " << state.B << ", " << state.C << std::endl;
}
