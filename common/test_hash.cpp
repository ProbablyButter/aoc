#include "hash.hpp"
//#include "md5.hpp"

#include <iostream>
#include <string>

int main(int argc, char **argv) {
  {
    // expected: d41d8cd98f00b204e9800998ecf8427e
    aoc::md5 hash;
    hash.init();
    hash.finalize();
    std::cout << hash.str() << std::endl;
  }
  {
    // expected: 9e107d9d372bb6826bd81d3542a419d6
    std::string data = "The quick brown fox jumps over the lazy dog";
    aoc::md5 hash;
    hash.init();
    hash.append(reinterpret_cast<const uint8_t *>(data.data()), data.size());
    hash.finalize();
    std::cout << hash.str() << std::endl;
#if 0
    MD5 hash_base;
    hash_base.update(data.data(), data.size());
    hash_base.finalize();
    std::cout << hash_base.hexdigest() << std::endl;
#endif
  }
  {
    // expected: c3725f9ff3277e098c83525a7aba8dd5
    std::string data =
        "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcde";
    aoc::md5 hash;
    hash.init();
    hash.append(reinterpret_cast<const uint8_t *>(data.data()), data.size());
    hash.finalize();
    std::cout << hash.str() << std::endl;
#if 0
    MD5 hash_base;
    hash_base.update(data.data(), data.size());
    hash_base.finalize();
    std::cout << hash_base.hexdigest() << std::endl;
#endif
  }
}
