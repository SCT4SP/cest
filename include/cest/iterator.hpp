#ifndef _CEST_ITERATOR_HPP_
#define _CEST_ITERATOR_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/iterator_tests.hpp

#include <iterator> // std::iterator_traits

namespace CEST_NAMESPACE {

  using std::inserter;

// Based on GNU libstdc++ class __normal_iterator from stl_iterator.h
template <typename I, typename C>
class iter {
protected:
  I curr;

public:
  using iterator_category = std::iterator_traits<I>::iterator_category;
  using value_type        = std::iterator_traits<I>::value_type;
  using difference_type   = std::iterator_traits<I>::difference_type;
  using reference         = std::iterator_traits<I>::reference;
  using pointer           = std::iterator_traits<I>::pointer;

  constexpr iter() noexcept : curr(I()) { }
  explicit constexpr iter(const I &i) noexcept : curr(i) { }

  // Allow iterator to const_iterator conversion
  template <typename I2>
  constexpr iter(const iter<
                         I2,
                         std::enable_if_t<
                           std::is_same_v<I2, typename C::pointer>,
                           C
                         >
                       > &i) noexcept : curr(i.base()) { }

  // Forward iterator requirements
  constexpr reference  operator *() const noexcept { return *curr;         }
  constexpr   pointer  operator->() const noexcept { return curr;          }
  constexpr      iter &operator++()       noexcept { ++curr; return *this; }
  constexpr      iter  operator++(int)    noexcept { return iter(curr++);  }

  // Bidirectional iterator requirements
  constexpr      iter &operator--()       noexcept { --curr; return *this; }
  constexpr      iter  operator--(int)    noexcept { return iter(curr--);  }

  // Random access iterator requirements
  constexpr  reference operator[](difference_type n) const noexcept {
    return curr[n];
  }

  constexpr      iter &operator+=(difference_type n)       noexcept {
    curr += n; return *this;
  }

  constexpr      iter  operator +(difference_type n) const noexcept {
    return iter(curr + n);
  }

  constexpr      iter &operator-=(difference_type n)       noexcept {
    curr -= n; return *this;
  }

  constexpr       iter operator -(difference_type n) const noexcept {
    return iter(curr - n);
  }

  constexpr const I &base() const noexcept { return curr; }
};

// The two iterators are permitted to vary in type by cv-qualification
template <typename IL, typename IR, typename C>
constexpr bool operator==(const iter<IL, C> &l, const iter<IR, C> &r) noexcept {
  return l.base() == r.base();
}

template<typename IL, typename IR, typename C>
constexpr auto  operator-(const iter<IL, C> &l, const iter<IR, C> &r) noexcept
  -> decltype(l.base() - r.base()) {
  return l.base() - r.base();
}

} // namespace CEST_NAMESPACE

#endif // _CEST_ITERATOR_HPP_
