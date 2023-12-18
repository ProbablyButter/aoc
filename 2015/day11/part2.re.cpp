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

int main(int argc, char **argv) {
  std::string password = "hepxcrrq";
  //password = "abcdefgh";
  auto inc_password = [](std::string &str) {
    for (auto iter = str.rbegin(); iter != str.rend(); ++iter) {
      ++(*iter);
      if (*iter > 'z') {
        if (iter + 1 == str.rend()) {
          return false;
        }
        *iter = 'a';
      } else {
        break;
      }
    }
    return true;
  };
  auto check_pw = [](const std::string &str) {
    for (auto &v : str) {
      switch (v) {
      case 'i':
      case 'o':
      case 'l':
        return false;
      default:
        break;
      }
    }
    
    bool valid = true;
    for (size_t i = 0; i + 2 < str.size(); ++i) {
      valid = true;
      for (size_t j = 1; j < 3; ++j) {
        if (str[i] + j != str[i + j]) {
          valid = false;
          break;
        }
      }
      if (valid) {
        break;
      }
    }
    if (!valid) {
      return false;
    }
    int64_t first_pair = -1;
    int64_t second_pair = -1;
    for (size_t i = 0; i + 1 < str.size(); ++i) {
      if (str[i] == str[i + 1]) {
        if (first_pair < 0) {
          first_pair = i;
        } else {
          second_pair = i;
        }
      }
    }
    return second_pair > (first_pair + 1);
  };
  while (true) {
    if (!inc_password(password)) {
      break;
    }
    if (check_pw(password)) {
      std::cout << password << std::endl;
      break;
    }
  }
  while (true) {
    if (!inc_password(password)) {
      break;
    }
    if (check_pw(password)) {
      std::cout << password << std::endl;
      break;
    }
  }
}
