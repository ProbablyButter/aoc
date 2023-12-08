#include "ndarray.hpp"

#include <iostream>

int main(int argc, char **argv) {
  aoc::ndarray<int> tmp;
  tmp.shape = {2, 3, 4};
  tmp.data.resize(tmp.size());
  std::cout << tmp(0, 1, 2) << std::endl;
}
