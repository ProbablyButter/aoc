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

  void reset(long long iA, long long iB, long long iC) {
    A = iA;
    B = iB;
    C = iC;
    ip = 0;
    output.clear();
  }

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

  void print_combo(int i) {
    switch (i) {
    case 0:
    case 1:
    case 2:
    case 3:
      std::cout << i;
      break;
    case 4:
      std::cout << "A";
      break;
    case 5:
      std::cout << "B";
      break;
    case 6:
      std::cout << "C";
      break;
    }
  }

  void dump_prog() {
    for (size_t i = 0; i < instrs.size(); i += 2) {
      switch (instrs[i]) {
      case 0:
        std::cout << "A /= 2**";
        print_combo(instrs[i + 1]);
        std::cout << std::endl;
        break;
      case 1:
        std::cout << "B ^= " << instrs[i + 1] << std::endl;
        break;
      case 2:
        std::cout << "B = ";
        print_combo(instrs[i + 1]);
        std::cout << " % 8" << std::endl;
        break;
      case 3:
        std::cout << "jnz " << instrs[i + 1] << std::endl;
        break;
      case 4:
        std::cout << "B ^= C" << std::endl;
        break;
      case 5:
        std::cout << "out ";
        print_combo(instrs[i + 1]);
        std::cout << " % 8" << std::endl;
        break;
      case 6:
        std::cout << "B = A / 2**";
        print_combo(instrs[i + 1]);
        std::cout << std::endl;
        break;
      case 7:
        std::cout << "C = A / 2**";
        print_combo(instrs[i + 1]);
        std::cout << std::endl;
        break;
      }
    }
  }

  // returns whether step was halted or not
  bool step() {
    if (ip + 1 >= instrs.size()) {
      return true;
    }
    switch (instrs[ip]) {
    case 0:
      // adv A = A / 2**combo
      if (get_combo() >= 64) {
        throw std::runtime_error("combo too big");
      }
      A = A / (1ll << get_combo());
      ip += 2;
      break;
    case 1:
      // bxl B = B ^ literal
      B = B ^ instrs[ip + 1];
      ip += 2;
      break;
    case 2:
      // bst B = combo % 8
      B = get_combo() % 8;
      ip += 2;
      break;
    case 3:
      // jnz: if A != 0, ip = literal
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
      B = B ^ C;
      ip += 2;
      break;
    case 5:
      // out outputs combo % 8
      output.push_back(get_combo() % 8);
      ip += 2;
      break;
    case 6:
      // bdv B = A / 2**combo
      if (get_combo() >= 64) {
        throw std::runtime_error("combo too big");
      }
      B = A / (1ll << get_combo());
      ip += 2;
      break;
    case 7:
      // cdv C = A / 2**combo
      if (get_combo() >= 64) {
        throw std::runtime_error("combo too big");
      }
      C = A / (1ll << get_combo());
      ip += 2;
      break;
    default:
      throw std::runtime_error("bad instr");
    }
    return false;
  }
};

int main(int argc, char **argv) {
  // my program:
  // B = (A & 7) ^ 3
  // C = A >> B
  // B ^= C
  // A >>= 3
  // B ^= 5
  // out B % 8
  // jnz 0
  // this solution only works for this program!

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
    }
  }

  long long init_B = state.B, init_C = state.C;

  // figure out what the MSB must be to get the last output
  auto msb_min = 1ll << (3 * state.instrs.size() - 3);
  auto msb_max = 1ll << (3 * state.instrs.size());
  auto incr = msb_min;

  long long i = msb_min;
  int pos = state.instrs.size() - 1;
  while (pos >= 0) {
    while (true) {
      state.reset(i, init_B, init_C);
      while (!state.step()) {
      }
      bool valid = true;
      for (int j = pos; j < state.instrs.size(); ++j) {
        if (state.output.at(j) != state.instrs.at(j)) {
          valid = false;
        }
      }
      if (valid) {
        break;
      }
      i += incr;
    }
    std::cout << pos << std::endl;
    --pos;
    incr >>= 3;
  }
  // verify our solution
  state.reset(i, init_B, init_C);
  while (!state.step()) {
  }
  if (state.output == state.instrs) {
    std::cout << i << std::endl;
  }
}
