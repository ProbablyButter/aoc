/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"
#include "segmented_range.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// assume no overlap
struct range_map {
  aoc::segmented_range srcs;
  aoc::segmented_range dsts;
  aoc::segmented_range norm_srcs;

  void sort() {
    auto idcs = srcs.get_sort_permute();
    aoc::apply_permutation_in_place(srcs.segments, idcs);
    aoc::apply_permutation_in_place(dsts.segments, idcs);
  }

  aoc::segmented_range map(const aoc::segmented_range &val) {
    auto res = val.subtract(norm_srcs);
    auto inter = val.intersect(norm_srcs);

    // for everything in inter, apply map to dsts
    // i indexes into inter
    // j indexes into srcs/dsts
    size_t i = 0;
    size_t j = 0;
    while (true) {
      if (i >= inter.segments.size()) {
        // nothing left
        break;
      }
      // find j where i belongs to
      int64_t a = std::max(inter.segments[i].first, srcs.segments[j].first);
      int64_t b = std::min(inter.segments[i].second, srcs.segments[j].second);
      if (a < b) {
        // TODO: add mapped range
        res.segments.emplace_back(
            (a - srcs.segments[j].first) + dsts.segments[j].first,
            (b - srcs.segments[j].first) + dsts.segments[j].first);
        ++i;
      } else {
        // either advance i, or advance j
        if (a == inter.segments[i].first) {
          ++j;
        } else {
          ++i;
        }
      }
    }

    return res;
  }
};

void read_map(std::istream &in, range_map &res) {
  std::string line;
  while (true) {
    std::getline(in, line);
    if (line.size() == 0) {
      break;
    }
    if (in.eof()) {
      break;
    }
    // split into 3 ints
    std::istringstream str(line);
    int64_t a, b, c;
    str >> a;
    str >> b;
    str >> c;
    // dst start, src start, len
    res.srcs.add(b, b + c);
    res.dsts.add(a, a + c);
  }
  res.sort();
  res.norm_srcs = res.srcs;
  res.norm_srcs.normalize();
}

#define PRIVATE_CONCAT(a, b) PRIVATE_CONCAT2(a, b)
#define PRIVATE_CONCAT2(a, b) b##_to_##a

#define apply_map(a, b)                                                        \
  auto a = PRIVATE_CONCAT(a, b).map(b);                                        \
  a.normalize()

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  aoc::segmented_range seed;

  range_map seed_to_soil;
  range_map soil_to_fertilizer;
  range_map fertilizer_to_water;
  range_map water_to_light;
  range_map light_to_temperature;
  range_map temperature_to_humidity;
  range_map humidity_to_location;

  // read seeds
  {
    std::getline(in, line);
    std::istringstream str(line);
    std::string tmp;
    str >> tmp;
    while (true) {
      int64_t a, b;
      str >> a;
      str >> b;
      seed.add(a, a + b);
      if (str.eof()) {
        break;
      }
    }
  }
  std::getline(in, line);
  std::getline(in, line);
  read_map(in, seed_to_soil);

  std::getline(in, line);
  read_map(in, soil_to_fertilizer);

  std::getline(in, line);
  read_map(in, fertilizer_to_water);

  std::getline(in, line);
  read_map(in, water_to_light);

  std::getline(in, line);
  read_map(in, light_to_temperature);

  std::getline(in, line);
  read_map(in, temperature_to_humidity);

  std::getline(in, line);
  read_map(in, humidity_to_location);

  // map seed to location

  seed.normalize();
  int64_t min_loc = 0xFFFFFFFFFFFFll;
  apply_map(soil, seed);

  apply_map(fertilizer, soil);

  apply_map(water, fertilizer);

  apply_map(light, water);

  apply_map(temperature, light);

  apply_map(humidity, temperature);

  apply_map(location, humidity);

  min_loc = std::min(location.segments[0].first, min_loc);
  std::cout << min_loc << std::endl;
}
