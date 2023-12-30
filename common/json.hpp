#ifndef AOC_JSON_HPP
#define AOC_JSON_HPP

#include <any>
#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace aoc {
struct json {
  using int_type = int64_t;
  using real_type = double;
  using null_type = void;
  using string_type = std::string;
  using object_type = std::unordered_multimap<std::string, std::any>;
  using array_type = std::vector<std::any>;

  std::any root;

  void parse(std::istream &in);

  void dump(std::ostream &out) const;
};
} // namespace aoc

#endif
