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

using arg_type = std::variant<std::string, uint16_t>;

struct instr {
  virtual ~instr() = default;

  virtual bool operator()(std::unordered_map<std::string, uint16_t> &state) = 0;

  virtual std::string to_string() = 0;
};

struct assign_instr : instr {
  arg_type a0;
  std::string dst;
  assign_instr(arg_type a0_, std::string d_) : a0(a0_), dst(d_) {}
  assign_instr(uint16_t a0_, std::string d_) : a0(a0_), dst(d_) {}
  assign_instr(std::string a0_, std::string d_) : a0(a0_), dst(d_) {}

  std::string to_string() override {
    std::stringstream out;
    if (std::holds_alternative<std::string>(a0)) {
      out << std::get<0>(a0);
    } else {
      out << std::get<1>(a0);
    }
    out << " -> " << dst;
    return out.str();
  }

  bool operator()(std::unordered_map<std::string, uint16_t> &state) override {
    uint16_t v0;
    if (std::holds_alternative<std::string>(a0)) {
      auto i0 = state.find(std::get<0>(a0));
      if (i0 == state.end()) {
        return false;
      }
      v0 = i0->second;
    } else {
      v0 = std::get<1>(a0);
    }
    state[dst] = v0;
    return true;
  }
  virtual ~assign_instr() = default;
};

struct and_instr : instr {
  arg_type a0;
  arg_type a1;
  std::string dst;
  and_instr(arg_type a0_, arg_type a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  and_instr(std::string a0_, std::string a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  and_instr(std::string a0_, uint16_t a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  and_instr(uint16_t a0_, std::string a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  and_instr(uint16_t a0_, uint16_t a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}

  std::string to_string() override {
    std::stringstream out;
    if (std::holds_alternative<std::string>(a0)) {
      out << std::get<0>(a0);
    } else {
      out << std::get<1>(a0);
    }
    out << " AND ";
    if (std::holds_alternative<std::string>(a1)) {
      out << std::get<0>(a1);
    } else {
      out << std::get<1>(a1);
    }
    out << " -> " << dst;
    return out.str();
  }

  bool operator()(std::unordered_map<std::string, uint16_t> &state) override {
    uint16_t v0;
    uint16_t v1;
    if (std::holds_alternative<std::string>(a0)) {
      auto i0 = state.find(std::get<0>(a0));
      if (i0 == state.end()) {
        return false;
      }
      v0 = i0->second;
    } else {
      v0 = std::get<1>(a0);
    }

    if (std::holds_alternative<std::string>(a1)) {
      auto i0 = state.find(std::get<0>(a1));
      if (i0 == state.end()) {
        return false;
      }
      v1 = i0->second;
    } else {
      v1 = std::get<1>(a1);
    }
    state[dst] = v0 & v1;
    return true;
  }

  virtual ~and_instr() = default;
};

struct or_instr : instr {
  arg_type a0;
  arg_type a1;
  std::string dst;
  or_instr(arg_type a0_, arg_type a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  or_instr(std::string a0_, std::string a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  or_instr(uint16_t a0_, std::string a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  or_instr(std::string a0_, uint16_t a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  or_instr(uint16_t a0_, uint16_t a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  std::string to_string() override {
    std::stringstream out;
    if (std::holds_alternative<std::string>(a0)) {
      out << std::get<0>(a0);
    } else {
      out << std::get<1>(a0);
    }
    out << " OR ";
    if (std::holds_alternative<std::string>(a1)) {
      out << std::get<0>(a1);
    } else {
      out << std::get<1>(a1);
    }
    out << " -> " << dst;
    return out.str();
  }
  bool operator()(std::unordered_map<std::string, uint16_t> &state) override {
    uint16_t v0;
    uint16_t v1;
    if (std::holds_alternative<std::string>(a0)) {
      auto i0 = state.find(std::get<0>(a0));
      if (i0 == state.end()) {
        return false;
      }
      v0 = i0->second;
    } else {
      v0 = std::get<1>(a0);
    }

    if (std::holds_alternative<std::string>(a1)) {
      auto i0 = state.find(std::get<0>(a1));
      if (i0 == state.end()) {
        return false;
      }
      v1 = i0->second;
    } else {
      v1 = std::get<1>(a1);
    }
    state[dst] = v0 | v1;
    return true;
  }
  virtual ~or_instr() = default;
};

struct lshift_instr : instr {
  arg_type a0;
  arg_type a1;
  std::string dst;
  lshift_instr(arg_type a0_, arg_type a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  lshift_instr(std::string a0_, std::string a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  lshift_instr(uint16_t a0_, std::string a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  lshift_instr(std::string a0_, uint16_t a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  lshift_instr(uint16_t a0_, uint16_t a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  std::string to_string() override {
    std::stringstream out;
    if (std::holds_alternative<std::string>(a0)) {
      out << std::get<0>(a0);
    } else {
      out << std::get<1>(a0);
    }
    out << " LSHIFT ";
    if (std::holds_alternative<std::string>(a1)) {
      out << std::get<0>(a1);
    } else {
      out << std::get<1>(a1);
    }
    out << " -> " << dst;
    return out.str();
  }
  bool operator()(std::unordered_map<std::string, uint16_t> &state) override {
    uint16_t v0;
    uint16_t v1;
    if (std::holds_alternative<std::string>(a0)) {
      auto i0 = state.find(std::get<0>(a0));
      if (i0 == state.end()) {
        return false;
      }
      v0 = i0->second;
    } else {
      v0 = std::get<1>(a0);
    }

    if (std::holds_alternative<std::string>(a1)) {
      auto i0 = state.find(std::get<0>(a1));
      if (i0 == state.end()) {
        return false;
      }
      v1 = i0->second;
    } else {
      v1 = std::get<1>(a1);
    }
    state[dst] = v0 << v1;
    return true;
  }
  virtual ~lshift_instr() = default;
};

struct rshift_instr : instr {
  arg_type a0;
  arg_type a1;
  std::string dst;
  rshift_instr(arg_type a0_, arg_type a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  rshift_instr(std::string a0_, std::string a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  rshift_instr(uint16_t a0_, std::string a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  rshift_instr(std::string a0_, uint16_t a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  rshift_instr(uint16_t a0_, uint16_t a1_, std::string d_)
      : a0(a0_), a1(a1_), dst(d_) {}
  std::string to_string() override {
    std::stringstream out;
    if (std::holds_alternative<std::string>(a0)) {
      out << std::get<0>(a0);
    } else {
      out << std::get<1>(a0);
    }
    out << " RSHIFT ";
    if (std::holds_alternative<std::string>(a1)) {
      out << std::get<0>(a1);
    } else {
      out << std::get<1>(a1);
    }
    out << " -> " << dst;
    return out.str();
  }
  bool operator()(std::unordered_map<std::string, uint16_t> &state) override {
    uint16_t v0;
    uint16_t v1;
    if (std::holds_alternative<std::string>(a0)) {
      auto i0 = state.find(std::get<0>(a0));
      if (i0 == state.end()) {
        return false;
      }
      v0 = i0->second;
    } else {
      v0 = std::get<1>(a0);
    }

    if (std::holds_alternative<std::string>(a1)) {
      auto i0 = state.find(std::get<0>(a1));
      if (i0 == state.end()) {
        return false;
      }
      v1 = i0->second;
    } else {
      v1 = std::get<1>(a1);
    }
    state[dst] = v0 >> v1;
    return true;
  }
  virtual ~rshift_instr() = default;
};

struct not_instr : instr {
  arg_type a0;
  std::string dst;
  not_instr(arg_type a0_, std::string d_) : a0(a0_), dst(d_) {}
  not_instr(std::string a0_, std::string d_) : a0(a0_), dst(d_) {}
  not_instr(uint16_t a0_, std::string d_) : a0(a0_), dst(d_) {}
  virtual ~not_instr() = default;
  std::string to_string() override {
    std::stringstream out;
    out << "NOT ";
    if (std::holds_alternative<std::string>(a0)) {
      out << std::get<0>(a0);
    } else {
      out << std::get<1>(a0);
    }
    out << " -> " << dst;
    return out.str();
  }
  bool operator()(std::unordered_map<std::string, uint16_t> &state) override {
    uint16_t v0;
    if (std::holds_alternative<std::string>(a0)) {
      auto i0 = state.find(std::get<0>(a0));
      if (i0 == state.end()) {
        return false;
      }
      v0 = i0->second;
    } else {
      v0 = std::get<1>(a0);
    }
    state[dst] = ~v0;
    return true;
  }
};

enum instr_type { NONE, NOT, AND, OR, LSHIFT, RSHIFT };

void parse_instr(const std::string &line,
                 std::vector<std::unique_ptr<instr>> &instrs) {
  const char *YYCURSOR = line.data();
  const char *YYMARKER;
  const char *d0, *d1, *d2, *d3, *d4, *d5;
  instr_type type = NONE;
  arg_type a0;
  arg_type a1;
  std::string dst;
  /*!stags:re2c format = 'const char *@@;'; */
parse_start:
  /*!re2c
@d0 [0-9]+ @d1 {
  a0 = aoc::to_int<uint16_t>(d0, d1);
  goto parse_a1;
}
@d0 [a-z]+ @d1 {
  a0 = std::string(d0, d1-d0);
  goto parse_a1;
}
"NOT" {
  type = NOT;
  goto parse_start;
}
[\x00] {
  std::cout << "missing " << line << std::endl;
  return;
}
* {
  goto parse_start;
}
   */
parse_a1:
  /*!re2c
@d0 [0-9]+ @d1 {
  a1 = aoc::to_int<uint16_t>(d0, d1);
  goto parse_a1;
}
@d0 [a-z]+ @d1 {
  a1 = std::string(d0, d1-d0);
  goto parse_a1;
}
"OR" {
  type = OR;
  goto parse_a1;
}
"AND" {
  type = AND;
  goto parse_a1;
}
"LSHIFT" {
  type = LSHIFT;
  goto parse_a1;
}
"RSHIFT" {
  type = RSHIFT;
  goto parse_a1;
}
"->" {
  goto parse_dst;
}
[\x00] {
  std::cout << "missing " << line << std::endl;
  return;
}
* {
  goto parse_a1;
}
   */

parse_dst:
  /*!re2c
@d4 [a-z]+ @d5 {
  dst = std::string(d4, d5-d4);
  goto done;
}
[\x00] {
  std::cout << "missing " << line << std::endl;
  return;
}
* {
  goto parse_dst;
}
   */
done:
  switch (type) {
  case NOT:
    instrs.emplace_back(new not_instr(a0, dst));
    break;
  case AND:
    instrs.emplace_back(new and_instr(a0, a1, dst));
    break;
  case OR:
    instrs.emplace_back(new or_instr(a0, a1, dst));
    break;
  case LSHIFT:
    instrs.emplace_back(new lshift_instr(a0, a1, dst));
    break;
  case RSHIFT:
    instrs.emplace_back(new rshift_instr(a0, a1, dst));
    break;
  case NONE:
    instrs.emplace_back(new assign_instr(a0, dst));
    break;
  }
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  std::unordered_map<std::string, uint16_t> vars;
  std::vector<std::unique_ptr<instr>> instrs;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    parse_instr(line, instrs);
  }

  size_t curr_size = instrs.size();

  // first get the value to overwrite b with
  while (true) {
    auto prev_size = curr_size;
    auto pos = std::partition(instrs.begin(), instrs.begin() + curr_size,
                              [&](auto &v) {
                                bool res = v->operator()(vars);
                                return !res;
                              });
    curr_size = std::distance(instrs.begin(), pos);
    if (curr_size == 0) {
      break;
    }
    if (curr_size == prev_size) {
      std::cout << "early terminate" << std::endl;
      for (auto &v : instrs) {
        std::cout << v->to_string() << std::endl;
      }
      break;
    }
  }
  uint16_t b_val = vars.at("a");
  // remove the instruction which sets b
  std::remove_if(instrs.begin(), instrs.end(), [](auto &v) {
    auto str = v->to_string();
    if (str[str.size() - 2] == ' ' && str[str.size() - 1] == 'b') {
      return true;
    }
    return false;
  });
  instrs.pop_back();
  vars.clear();
  vars["b"] = b_val;

  // rerun circuit
  curr_size = instrs.size();
  while (true) {
    auto prev_size = curr_size;
    auto pos = std::partition(instrs.begin(), instrs.begin() + curr_size,
                              [&](auto &v) {
                                bool res = v->operator()(vars);
                                return !res;
                              });
    curr_size = std::distance(instrs.begin(), pos);
    if (curr_size == 0) {
      break;
    }
    if (curr_size == prev_size) {
      std::cout << "early terminate" << std::endl;
      for (auto &v : instrs) {
        std::cout << v->to_string() << std::endl;
      }
      break;
    }
  }
  std::cout << vars.at("a") << std::endl;
}
