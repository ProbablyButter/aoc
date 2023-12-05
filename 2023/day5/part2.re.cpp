/*!re2c re2c:flags:utf-8 = 1;*/
/*!max:re2c*/
/*!re2c
re2c:define:YYCTYPE = char;
re2c:yyfill:enable = 0;
re2c:flags:tags = 1;
re2c:yyfill:check = 1;
*/

#include "aoc.hpp"

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
struct discont_range {
  std::vector<std::pair<long long, long long>> ranges;

  void sort() {
    std::sort(ranges.begin(), ranges.end(), [](auto &a, auto &b) {
      if (a.first != b.first) {
        return a.first < b.first;
      }
      return a.second < b.second;
    });
  }

  discont_range intersect(discont_range &o) {
    discont_range res;
    if (ranges.empty() || o.ranges.empty()) {
      // no intersection
      return res;
    }
    sort();
    o.sort();

    size_t i = 0;
    size_t j = 0;
    long long curr = ranges[0].first;

    while (true) {
      if (i >= ranges.size()) {
        // nothing left to compare
        break;
      }
      if (j >= o.ranges.size()) {
        // nothing left to compare
        break;
      }
      if (curr >= ranges[i].second) {
        ++i;
        continue;
      }
      if (curr >= o.ranges[j].second) {
        ++j;
        continue;
      }
      // some chance for an intersection
      long long a = std::max(ranges[i].first, o.ranges[j].first);
      long long b = std::min(ranges[i].second, o.ranges[j].second);
      if (a < b) {
        // some intersection
        res.ranges.emplace_back(a, b);
        curr = b;

      } else {
        curr = a;
      }
    }

    return res;
  }

  // everything in this not in o
  discont_range subtract(discont_range &o) {
    if (o.ranges.empty()) {
      return *this;
    }
    discont_range res;
    if (ranges.empty()) {
      return res;
    }
    sort();
    o.sort();

    size_t i = 0;
    size_t j = 0;
    long long curr = ranges[0].first;

    while (true) {
      if (i >= ranges.size()) {
        // nothing left to compare
        break;
      }
      if (j >= o.ranges.size()) {
        // nothing left to compare, add everything else
        if (curr < ranges[i].second) {
          res.ranges.emplace_back(curr, ranges[i].second);
          ++i;
        }
        for (; i < ranges.size(); ++i) {
          res.ranges.push_back(ranges[i]);
        }
        break;
      }
      if (curr >= ranges[i].second) {
        ++i;
        continue;
      }
      if (curr >= o.ranges[j].second) {
        ++j;
        continue;
      }
      if (curr < ranges[i].first) {
        curr = ranges[i].first;
      }
      if (curr < o.ranges[j].first) {
        // add segment
        if (ranges[i].second < o.ranges[j].first) {
          res.ranges.emplace_back(curr, ranges[i].second);
          ++i;
          if (i < ranges.size()) {
            curr = ranges[i].first;
          } else {
            break;
          }
        } else {
          res.ranges.emplace_back(curr, o.ranges[j].first);
          curr = o.ranges[j].second;
        }
      } else {
        curr = o.ranges[j].second;
      }
    }

    return res;
  }
};

struct range_map {
  discont_range srcs;
  discont_range dsts;

  void sort() {
    std::vector<size_t> idcs;
    idcs.reserve(srcs.ranges.size());
    for (size_t i = 0; i < srcs.ranges.size(); ++i) {
      idcs.push_back(i);
    }
    std::sort(idcs.begin(), idcs.end(), [&](auto &a, auto &b) {
      if (srcs.ranges[a].first != srcs.ranges[b].first) {
        return srcs.ranges[a].first < srcs.ranges[b].first;
      }
      return srcs.ranges[a].second < srcs.ranges[b].second;
    });
    aoc::apply_permutation_in_place(srcs.ranges, idcs);
    aoc::apply_permutation_in_place(dsts.ranges, idcs);
  }

  discont_range map(discont_range &val) {
    auto tmp_srcs = srcs;
    discont_range res = val.subtract(tmp_srcs);
    discont_range inter = val.intersect(tmp_srcs);

    // for everything in inter, apply map to dsts
    // i indexes into inter
    // j indexes into srcs/dsts
    size_t i = 0;
    size_t j = 0;
    while (true) {
      if (i >= inter.ranges.size()) {
        // nothing left
        break;
      }
      // find j where i belongs to
      long long a = std::max(inter.ranges[i].first, srcs.ranges[j].first);
      long long b = std::min(inter.ranges[i].second, srcs.ranges[j].second);
      if (a < b) {
        // TODO: add mapped range
        res.ranges.emplace_back(
            (a - srcs.ranges[j].first) + dsts.ranges[j].first,
            (b - srcs.ranges[j].first) + dsts.ranges[j].first);
        ++i;
      } else {
        // either advance i, or advance j
        if (a == inter.ranges[i].first) {
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
    long long a, b, c;
    str >> a;
    str >> b;
    str >> c;
    // dst start, src start, len
    res.srcs.ranges.emplace_back(b, b + c);
    res.dsts.ranges.emplace_back(a, a + c);
  }
  res.sort();
}

#define PRIVATE_CONCAT(a, b) PRIVATE_CONCAT2(a, b)
#define PRIVATE_CONCAT2(a, b) b##_to_##a

#define apply_map(a, b) auto a = PRIVATE_CONCAT(a, b).map(b)

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  std::vector<discont_range> seeds;

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
      long long a, b;
      str >> a;
      str >> b;
      seeds.emplace_back();
      seeds.back().ranges.emplace_back(a, a + b);
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
  long long min_loc = 0xFFFFFFFFFFFFll;
  for (auto &seed : seeds) {
    apply_map(soil, seed);

    apply_map(fertilizer, soil);

    apply_map(water, fertilizer);

    apply_map(light, water);

    apply_map(temperature, light);

    apply_map(humidity, temperature);

    apply_map(location, humidity);

    location.sort();
    min_loc = std::min(location.ranges[0].first, min_loc);
  }
  std::cout << min_loc << std::endl;
}
