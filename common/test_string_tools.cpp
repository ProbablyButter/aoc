#include "string_tools.hpp"

#include <iostream>

int main(int argc, char **argv) {
  {
    std::string str = "a b   c  d  ";
    {
      auto tmp = aoc::split(str, "", -1);
      for (auto &v : tmp) {
        std::cout << "\"" << v << "\"" << std::endl;
      }
    }

    std::cout << std::endl;
    {
      auto tmp = aoc::split(str, "", 1);
      for (auto &v : tmp) {
        std::cout << "\"" << v << "\"" << std::endl;
      }
    }
    std::cout << std::endl;

    {
      auto tmp = aoc::split(str, "", 2);
      for (auto &v : tmp) {
        std::cout << "\"" << v << "\"" << std::endl;
      }
    }
  }
  std::cout << std::endl;

  {
    std::string str = "a,;b,;,;,;c,;,;d,;,;";
    {
      auto tmp = aoc::split(str, ",;", -1);
      for (auto &v : tmp) {
        std::cout << "\"" << v << "\"" << std::endl;
      }
    }

    std::cout << std::endl;
    {
      auto tmp = aoc::split(str, ",;", 1);
      for (auto &v : tmp) {
        std::cout << "\"" << v << "\"" << std::endl;
      }
    }
    std::cout << std::endl;

    {
      auto tmp = aoc::split(str, ",;", 2);
      for (auto &v : tmp) {
        std::cout << "\"" << v << "\"" << std::endl;
      }
    }
  }
  std::cout << std::endl;
  {
    std::string str = "a,;b,;,;,;c,;,;d,;,;e";
    {
      auto tmp = aoc::split(str, ",;", -1);
      for (auto &v : tmp) {
        std::cout << "\"" << v << "\"" << std::endl;
      }
    }
  }
  std::cout << std::endl;
  {
    std::string str = "    hello   ";
    auto tmp = aoc::trim(str);
    std::cout << "\"" << tmp << "\"" << std::endl;
  }
}
