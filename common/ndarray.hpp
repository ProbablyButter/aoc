#ifndef AOC_NDARRAY_HPP
#define AOC_NDARRAY_HPP

#include <array>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace aoc {
template <class T> struct ndarray {
  std::vector<T> data;
  std::vector<size_t> shape;

  ndarray() = default;

  template <class... Ts> ndarray(Ts... lens) : shape({lens...}) {
    data.resize(size());
  }

  ndarray(const ndarray &o) = default;

  ndarray(ndarray &&o) = default;

  template <class... Ts> T &operator()(Ts... idcs) const {
    std::array<long long, sizeof...(Ts)> idx = {idcs...};
    if (idx.size() != shape.size()) {
      throw std::runtime_error("wrong number of idcs");
    }
    size_t i = 0;
    for (size_t j = 0; j < idx.size(); ++j) {
      i *= shape[j];
      if (idx[j] < 0) {
        i += shape[j] + idx[j];
      } else {
        i += idx[j];
      }
    }
    return const_cast<T &>(data[i]);
  }

  size_t size() const {
    size_t res = 1;
    for (auto v : shape) {
      res *= v;
    }
    return res;
  }

  // TODO: implement subarray feature
};
} // namespace aoc

#endif
