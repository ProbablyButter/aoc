#include "aoc.hpp"

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char** argv)
{
  std::filesystem::path in_path = get_resource_path("input.txt");
  std::ifstream in(in_path);
  int lower, upper;
  char c;
  int valid = 0;
  std::string line;
  while (!in.eof()) {
    in >> lower;
    // ignore -
    in.get(c);
    in >> upper;
    // ignore space
    in.get(c);
    in.get(c);
    std::getline(in, line);
    int count = 0;
    for (auto v : line) {
      if (v == c) {
        ++count;
      }
    }
    if (count >= lower && count <= upper) {
      ++valid;
    }
  }
  std::cout << valid << std::endl;
}
