#include "fraction.hpp"

#include <iostream>

int main(int argc, char **argv) {
  {
    // expect 0/1
    aoc::fraction f{0, 1};
    f.reduce();
    std::cout << f.num << "/" << f.den << std::endl;
  }
  {
    // expect 0/1
    aoc::fraction f{0, 10};
    f.reduce();
    std::cout << f.num << "/" << f.den << std::endl;
  }
  {
    // expect 1/5
    aoc::fraction f{2, 10};
    f.reduce();
    std::cout << f.num << "/" << f.den << std::endl;
  }
  {
    // expect 120/23
    aoc::fraction f{240, 46};
    f.reduce();
    std::cout << f.num << "/" << f.den << std::endl;
  }
  {
    // expect 23/120
    aoc::fraction f{46, 240};
    f.reduce();
    std::cout << f.num << "/" << f.den << std::endl;
  }
  {
    // expect -23/120
    aoc::fraction f{-46, 240};
    f.reduce();
    std::cout << f.num << "/" << f.den << std::endl;
  }
  {
    // expect -23/120
    aoc::fraction f{46, -240};
    f.reduce();
    std::cout << f.num << "/" << f.den << std::endl;
  }
  {
    // expect 23/120
    aoc::fraction f{-46, -240};
    f.reduce();
    std::cout << f.num << "/" << f.den << std::endl;
  }
  {
    // expect 23/60
    aoc::fraction f0{-46, -240};
    aoc::fraction f1{-46, -240};
    f0 += f1;
    std::cout << f0.num << "/" << f0.den << std::endl;
  }
  {
    // expect -23/28680
    aoc::fraction f0{-46, -240};
    aoc::fraction f1{46, -239};
    f0 += f1;
    std::cout << f0.num << "/" << f0.den << std::endl;
  }
  {
    aoc::fraction f0{-46, -240};
    aoc::fraction f1{46, -239};
    f0 += f1;
    f0 -= f1;
    f0 *= f1;
    f0 /= f1;
    // expect: 23/120
    std::cout << f0.num << "/" << f0.den << std::endl;
    // expect: 23/60
    f1 = f0 + f0 * f0 / f0;
    std::cout << f1.num << "/" << f1.den << std::endl;
    f1 += 1;
    f1 -= 1;
    f1 *= 1;
    f1 /= 1;
    f1 = (((f1 + 1) - 1) * 2) / 3;
    // expect: 23/90
    std::cout << f1.num << "/" << f1.den << std::endl;
  }
}
