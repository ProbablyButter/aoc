/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"
#include "json.hpp"

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
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

template <class T, class F>
inline std::pair<const std::type_index, std::function<void(std::any const &)>>
to_any_visitor(F const &f) {
  return {std::type_index(typeid(T)), [g = f](std::any const &a) {
            if constexpr (std::is_void_v<T>)
              g();
            else
              g(*std::any_cast<T>(&a));
          }};
}

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  aoc::json doc;
  doc.parse(in);
  int64_t sum = 0;

  std::unordered_map<std::type_index, std::function<void(std::any const &)>>
      any_visitor{
          to_any_visitor<aoc::json::int_type>(
              [&](const aoc::json::int_type &val) { sum += val; }),
          to_any_visitor<aoc::json::real_type>(
              [&](const aoc::json::real_type &val) {}),
          to_any_visitor<aoc::json::null_type>([&]() {}),
          to_any_visitor<aoc::json::string_type>(
              [&](const aoc::json::string_type &val) {}),
      };
  any_visitor.insert(to_any_visitor<aoc::json::object_type>(
      [&](const aoc::json::object_type &val) {
        for (auto &v : val) {
          if (v.second.type() == typeid(std::string)) {
            if (*std::any_cast<std::string>(&v.second) == "red") {
              // object has a red value, ignore
              return;
            }
          }
        }
        for (auto &v : val) {
          // recurse
          any_visitor.at(std::type_index(v.second.type()))(v.second);
        }
      }));
  any_visitor.insert(to_any_visitor<aoc::json::array_type>(
      [&](const aoc::json::array_type &val) {
        for (auto &v : val) {
          // recurse
          any_visitor.at(std::type_index(v.type()))(v);
        }
      }));

  any_visitor.at(std::type_index(doc.root.type()))(doc.root);
  std::cout << sum << std::endl;
}
