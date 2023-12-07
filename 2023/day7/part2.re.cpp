/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"
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

int get_power(const std::array<int, 5> &hand) {
  std::unordered_map<int, int> tmp;
  int jacks = 0;
  for (auto &v : hand) {
    if (v == 1) {
      ++jacks;
      continue;
    }
    auto iter = tmp.find(v);
    if (iter == tmp.end()) {
      tmp.emplace(v, 1);
    } else {
      ++iter->second;
    }
  }

  for (auto &v : tmp) {
    if (v.second + jacks == 5) {
      // 5 if a kind
      return 6;
    }
  }
  if (jacks == 5) {
    return 6;
  }
  for (auto &v : tmp) {
    if (v.second + jacks == 4) {
      // 4 of a kind
      return 5;
    }
  }
  for (auto &v : tmp) {
    if (v.second + jacks == 3) {
      // either 3 of a kind or full house
      for (auto &v2 : tmp) {
        if (v2.first != v.first) {
          if (v2.second == 2) {
            // full house
            return 4;
          }
        }
      }
      // 3 of a kind
      return 3;
    }
  }

  for (auto &v : tmp) {
    if (v.second + jacks == 2) {
      // either 1 or 2 pair
      for (auto &v2 : tmp) {
        if (v2.first != v.first) {
          if (v2.second == 2) {
            // 2 pair
            return 2;
          }
        }
      }
      // 1 pair
      return 1;
    }
  }

  // high card
  return 0;
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  std::vector<std::array<int, 5>> hands;
  std::vector<int64_t> bets;
  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line, "", -1);
    hands.emplace_back();
    for (int i = 0; i < 5; ++i) {
      switch (tmp[0][i]) {
      case 'A':
        hands.back()[i] = 14;
        break;
      case 'K':
        hands.back()[i] = 13;
        break;
      case 'Q':
        hands.back()[i] = 12;
        break;
      case 'J':
        hands.back()[i] = 1;
        break;
      case 'T':
        hands.back()[i] = 10;
        break;
      case '9':
        hands.back()[i] = 9;
        break;
      case '8':
        hands.back()[i] = 8;
        break;
      case '7':
        hands.back()[i] = 7;
        break;
      case '6':
        hands.back()[i] = 6;
        break;
      case '5':
        hands.back()[i] = 5;
        break;
      case '4':
        hands.back()[i] = 4;
        break;
      case '3':
        hands.back()[i] = 3;
        break;
      case '2':
        hands.back()[i] = 2;
        break;
      default:
        throw std::runtime_error("invalid card");
      }
    }
    bets.emplace_back(0);
    std::from_chars(tmp[1].data(), tmp[1].data() + tmp[1].size(), bets.back());
  }
  std::vector<size_t> permute;
  permute.reserve(bets.size());
  for (size_t i = 0; i < bets.size(); ++i) {
    permute.emplace_back(i);
  }
  std::sort(permute.begin(), permute.end(), [&](auto &a, auto &b) {
    // compute power of each hand
    auto power_a = get_power(hands[a]);
    auto power_b = get_power(hands[b]);
    if (power_a != power_b) {
      return power_a < power_b;
    }
    // compare card magnitude
    return std::lexicographical_compare(hands[a].begin(), hands[a].end(),
                                        hands[b].begin(), hands[b].end(),
                                        [](auto &c, auto &d) { return c < d; });
  });
  aoc::apply_permutation_in_place(hands, permute);
  aoc::apply_permutation_in_place(bets, permute);

  std::vector<char> card_face = {' ', 'J', '2', '3', '4', '5', '6', '7',
                                 '8', '9', 'T', ' ', 'Q', 'K', 'A'};

  int64_t res = 0;
  for (int64_t i = 0; i < hands.size(); ++i) {
    res += (i + 1) * bets[i];
  }
  std::cout << res << std::endl;
}
