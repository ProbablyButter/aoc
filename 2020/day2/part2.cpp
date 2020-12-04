#include "aoc.hpp"

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  int lower, upper;
  char c;
  char skip;
  int valid = 0;
  std::string line;
  while (true) {
    in >> lower;
    if (in.eof()) {
      break;
    }
    // ignore -
    in.get(skip);
    in >> upper;
    // ignore space
    in.get(skip);
    in.get(c);

    in.get(skip);

    // line should have an extra space at the beginning, this skips having decrement lower/upper
    std::getline(in, line);

    if ((line.at(lower) == c) ^ (line.at(upper) == c)) {
      ++valid;
    }
  }
  std::cout << valid << std::endl;
}
