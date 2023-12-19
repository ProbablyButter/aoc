#include "segmented_range.hpp"

#include <iostream>

int main(int argc, char **argv) {
  {
    aoc::segmented_range segs;
    segs.add(0, 10);
    segs.add(0, 20);
    segs.add(1, 2);
    segs.add(5, 6);
    segs.add(8, 10);
    segs.add(9, 10);
    segs.add(25, 30);
    segs.add(27, 35);
    segs.add(35, 36);
    segs.add(40, 50);

    segs.normalize();

    aoc::segmented_range expected;
    expected.add(0, 1);
    expected.add(1, 2);
    expected.add(2, 5);
    expected.add(5, 6);
    expected.add(8, 9);
    expected.add(9, 10);
    expected.add(10, 20);
    expected.add(25, 27);
    expected.add(27, 30);
    expected.add(30, 35);

    segs.merge_adjacent();
    for (auto &v : segs.segments) {
      std::cout << v.first << ", " << v.second << std::endl;
    }
    std::cout << std::endl;
  }
  {
    aoc::segmented_range segs;
    segs.add(1,4001);
    aoc::segmented_range seg2;
    seg2.add(0,5000);
    segs = segs.subtract(seg2);
    // expect empty range
    for (auto &v : segs.segments) {
      std::cout << v.first << ", " << v.second << std::endl;
    }
    std::cout << std::endl;
  }
  {
    aoc::segmented_range segs;
    segs.add(1,4001);
    segs = segs.subtract(std::pair<int64_t, int64_t>(0, 5000));
    // expect empty range
    for (auto &v : segs.segments) {
      std::cout << v.first << ", " << v.second << std::endl;
    }
    std::cout << std::endl;
  }
}
