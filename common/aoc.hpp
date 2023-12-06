#ifndef AOC_HPP
#define AOC_HPP

#include <algorithm>
#include <array>
#include <filesystem>
#include <iterator>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

std::filesystem::path get_resource_path(const std::string &name);

namespace aoc {
///
/// @brief A range which infinitely pads another with a given value. Useful for
/// using re2c on std::string_view.
///
template <class Iter> struct padded_range {
  Iter src_beg, src_end;
  using value_type =
      std::decay_t<std::remove_reference_t<decltype(*std::declval<Iter>())>>;
  value_type pad_value;

  class Iterator {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type =
        std::decay_t<std::remove_reference_t<decltype(*std::declval<Iter>())>>;
    using difference_type = long long;
    using pointer = typename std::add_pointer<value_type>::type;
    using reference = typename std::add_lvalue_reference<value_type>::type;

  private:
    Iter pos, src_end;
    value_type pad_;

    Iterator(Iter beg, Iter stop, decltype(pad_) pad)
        : pos(beg), src_end(stop), pad_(pad) {}

  public:
    Iterator(const Iterator &o) = default;

    decltype(*pos) operator*() {
      if (pos < src_end) {
        return *pos;
      }
      return pad_;
    }

    std::add_const_t<decltype(*pos)> operator*() const {
      if (pos < src_end) {
        return *pos;
      }
      return pad_;
    }

    std::add_const_t<decltype(pos[0])> operator[](long long i) const {
      if (pos + i < src_end) {
        return pos[i];
      }
      return pad_;
    }

    decltype(pos[0]) operator[](long long i) {
      if (pos + i < src_end) {
        return pos[i];
      }
      return pad_;
    }

    bool operator==(const Iterator &o) const {
      return o.pos == pos && o.src_end == src_end && pad_ == o.pad_;
    }

    bool operator!=(const Iterator &o) const { return !(*this == o); }

    bool operator<(const Iterator &o) const { return pos < o.pos; }

    bool operator<=(const Iterator &o) const { return pos <= o.pos; }

    bool operator>(const Iterator &o) const { return pos > o.pos; }

    bool operator>=(const Iterator &o) const { return pos >= o.pos; }

    Iterator &operator++() {
      ++pos;
      return *this;
    }

    Iterator &operator--() {
      --pos;
      return *this;
    }

    Iterator operator++(int) {
      Iterator res(*this);
      ++pos;
      return res;
    }

    Iterator operator--(int) {
      Iterator res(*this);
      --pos;
      return res;
    }

    Iterator &operator+=(long long i) {
      pos += i;
      return *this;
    }

    Iterator &operator-=(long long i) {
      pos -= i;
      return *this;
    }

    friend class padded_range;
  };

  Iterator begin() const { return Iterator(src_beg, src_end, pad_value); }

  Iterator end() const { return Iterator(src_end, src_end, pad_value); }
};

///
/// @param base what base to assume the input is in, or 0 to try and auto-detect
///
long long svtoll(const std::string_view &str, size_t base = 0);

template <typename T, class U>
std::vector<T> apply_permutation(const std::vector<T> &vec,
                                 const std::vector<U> &p) {
  std::vector<T> sorted_vec(vec.size());
  std::transform(p.begin(), p.end(), sorted_vec.begin(),
                 [&](auto i) { return vec[i]; });
  return sorted_vec;
}

template <typename T, class U>
void apply_permutation_in_place(std::vector<T> &vec, const std::vector<U> &p) {
  std::vector<bool> done(vec.size());
  for (U i = 0; i < vec.size(); ++i) {
    if (done[i]) {
      continue;
    }
    done[i] = true;
    U prev_j = i;
    U j = p[i];
    while (i != j) {
      std::swap(vec[prev_j], vec[j]);
      done[j] = true;
      prev_j = j;
      j = p[j];
    }
  }
}
} // namespace aoc

#endif
