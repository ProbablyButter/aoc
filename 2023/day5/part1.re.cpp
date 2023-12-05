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

struct range_map {
  std::vector<long long> src_starts;
  std::vector<long long> dst_starts;
  std::vector<long long> lengths;

  long long map(long long val) {
    for (size_t i = 0; i < src_starts.size(); ++i) {
      auto v = src_starts[i];
      if (val >= v && val < v + lengths[i]) {
        // in this range
        return dst_starts[i] + (val - v);
      }
    }
    return val;
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
    res.src_starts.push_back(b);
    res.dst_starts.push_back(a);
    res.lengths.push_back(c);
  }
}

#define PRIVATE_CONCAT(a, b) PRIVATE_CONCAT2(a, b)
#define PRIVATE_CONCAT2(a, b) b##_to_##a

#define apply_map(a, b) auto a = PRIVATE_CONCAT(a, b).map(b)

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  std::string line;
  std::vector<long long> seeds;

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
    long long val;
    while (true) {
      str >> val;
      seeds.push_back(val);
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
    min_loc = std::min(min_loc, location);

  }
  std::cout << min_loc << std::endl;
}
