#ifndef AOC_HPP
#define AOC_HPP

#include <filesystem>
#include <iterator>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

std::filesystem::path get_resource_path(const std::string &name);

namespace aoc
{
  ///
  /// @brief A range which infinitely pads another with a given value. Useful for using re2c on
  /// std::string_view.
  ///
  template <class Iter> struct padded_range
  {
    Iter src_beg, src_end;
    using value_type = std::remove_reference_t<decltype(*std::declval<Iter>())>;
    std::decay_t<value_type> pad_value;

    class Iterator : public std::iterator<std::random_access_iterator_tag, decltype(pad_value)>
    {
      Iter pos, src_end;
      value_type pad_;

      Iterator(Iter beg, Iter stop, decltype(pad_) pad) : pos(beg), src_end(stop), pad_(pad) {}

    public:
      Iterator(const Iterator& o) = default;
      
      decltype(*pos) operator*()
      {
        if (pos < src_end)
        {
          return *pos;
        }
        return pad_;
      }

      std::add_const_t<decltype(*pos)> operator*() const
      {
        if (pos < src_end)
        {
          return *pos;
        }
        return pad_;
      }

      std::add_const_t<decltype(pos[0])> operator[](long long i) const
      {
        if (pos + i < src_end)
        {
          return pos[i];
        }
        return pad_;
      }

      decltype(pos[0]) operator[](long long i)
      {
        if (pos + i < src_end)
        {
          return pos[i];
        }
        return pad_;
      }

      bool operator==(const Iterator &o) const
      {
        return o.pos == pos && o.src_end == src_end && pad_ == o.pad_;
      }

      bool operator!=(const Iterator &o) const { return !(*this == o); }

      Iterator &operator++()
      {
        ++pos;
        return *this;
      }

      Iterator &operator--()
      {
        --pos;
        return *this;
      }

      Iterator &operator+=(long long i)
      {
        pos += i;
        return *this;
      }

      Iterator &operator-=(long long i)
      {
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
} // namespace aoc

#endif
