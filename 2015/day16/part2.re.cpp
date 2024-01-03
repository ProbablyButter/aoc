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
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;

  std::unordered_map<size_t, int64_t> children;
  std::unordered_map<size_t, int64_t> cats;
  std::unordered_map<size_t, int64_t> samoyeds;
  std::unordered_map<size_t, int64_t> pomeranians;
  std::unordered_map<size_t, int64_t> akitas;
  std::unordered_map<size_t, int64_t> vizslas;
  std::unordered_map<size_t, int64_t> goldfish;
  std::unordered_map<size_t, int64_t> trees;
  std::unordered_map<size_t, int64_t> cars;
  std::unordered_map<size_t, int64_t> perfumes;

  while (true) {
    std::getline(in, line);
    if (in.eof()) {
      break;
    }
    auto tmp = aoc::split(line, "");
    size_t sue_num = aoc::to_int<size_t>(tmp[1].substr(0, tmp[1].size() - 1));
    for (size_t i = 2; i < tmp.size(); i += 2) {
      auto type = tmp[i].substr(0, tmp[i].size() - 1);
      size_t count;
      if (tmp[i + 1].back() == ',') {
        count =
            aoc::to_int<size_t>(tmp[i + 1].substr(0, tmp[i + 1].size() - 1));
      } else {
        count = aoc::to_int<size_t>(tmp[i + 1]);
      }
      if (type == "children") {
        children[sue_num] = count;
      } else if (type == "cats") {
        cats[sue_num] = count;
      } else if (type == "samoyeds") {
        samoyeds[sue_num] = count;
      } else if (type == "pomeranians") {
        pomeranians[sue_num] = count;
      } else if (type == "akitas") {
        akitas[sue_num] = count;
      } else if (type == "vizslas") {
        vizslas[sue_num] = count;
      } else if (type == "goldfish") {
        goldfish[sue_num] = count;
      } else if (type == "trees") {
        trees[sue_num] = count;
      } else if (type == "cars") {
        cars[sue_num] = count;
      } else if (type == "perfumes") {
        perfumes[sue_num] = count;
      } else {
        std::cout << type << std::endl;
        throw std::runtime_error("unknown type");
      }
    }
  }
  std::unordered_set<size_t> eliminated;
  int64_t echildren = 3;
  int64_t ecats = 7;
  int64_t esamoyeds = 2;
  int64_t epomeranians = 3;
  int64_t eakitas = 0;
  int64_t evizslas = 0;
  int64_t egoldfish = 5;
  int64_t etrees = 3;
  int64_t ecars = 2;
  int64_t eperfumes = 1;
#define EXPAND_EQ(SUB)                                                         \
  for (auto &v : SUB) {                                                        \
    if (v.second != e##SUB) {                                                  \
      eliminated.emplace(v.first);                                             \
    }                                                                          \
  }
#define EXPAND_GT(SUB)                                                         \
  for (auto &v : SUB) {                                                        \
    if (v.second <= e##SUB) {                                                  \
      eliminated.emplace(v.first);                                             \
    }                                                                          \
  }
#define EXPAND_LT(SUB)                                                         \
  for (auto &v : SUB) {                                                        \
    if (v.second >= e##SUB) {                                                  \
      eliminated.emplace(v.first);                                             \
    }                                                                          \
  }
  EXPAND_EQ(children);
  EXPAND_GT(cats);
  EXPAND_EQ(samoyeds);
  EXPAND_LT(pomeranians);
  EXPAND_EQ(akitas);
  EXPAND_EQ(vizslas);
  EXPAND_LT(goldfish);
  EXPAND_GT(trees);
  EXPAND_EQ(cars);
  EXPAND_EQ(perfumes);

  for (size_t i = 1; i <= 500; ++i) {
    if (eliminated.find(i) == eliminated.end()) {
      std::cout << i << std::endl;
    }
  }
}
