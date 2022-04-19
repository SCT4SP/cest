#ifndef _CEST_TESTS_UTIL_HPP_
#define _CEST_TESTS_UTIL_HPP_

#include <algorithm>
#include <cassert>
#include <cmath>

namespace tests_util {

template <typename T>
constexpr bool comparable(const T &x, const T &y, const T tolerance = 0.01) {
  return std::abs(x - y) < x * tolerance;
}

template <bool TestAssert = true> struct Bar {
  constexpr Bar() : m_p(new int(42)) {}
  constexpr Bar(int x) : m_p(new int(x)) {}
  constexpr Bar(const Bar &f) noexcept : m_p(new int(*f.m_p)) {}
  constexpr Bar(Bar &&other) : m_p(other.m_p) {
    assert(TestAssert);
    other.m_p = nullptr;
  }
  constexpr ~Bar() { delete m_p; }
  constexpr Bar &operator=(const Bar &other) {
    delete m_p;
    m_p = new int(*other.m_p);
    return *this;
  }
  constexpr Bar &operator=(Bar &&other) {
    std::swap(m_p, other.m_p);
    return *this;
  }
  int *m_p;
};

template <typename C> constexpr bool push_back_dtor_test() {
  C c;
  typename C::value_type f(42);
  c.push_back(f);
  c.push_back(f); // ~Bar() (Bar destructor) called here (by reserve w' vector)
  bool b1 = 42 == *c.begin()->m_p && 2 == std::distance(c.begin(), c.end());
  c.pop_back(); // ~Bar() (Bar destructor) called here
  bool b2 = 1 == std::distance(c.begin(), c.end());
  return b1 && b2;
}

template <typename C> constexpr bool push_front_dtor_test() {
  C c;
  typename C::value_type f(42);
  c.push_front(f);
  c.push_front(f); // ~Bar() (Bar destructor) called here
  bool b1 = 42 == *c.begin()->m_p && 2 == std::distance(c.begin(), c.end());
  c.pop_front(); // ~Bar() (Bar destructor) called here
  bool b2 = 1 == std::distance(c.begin(), c.end());
  return b1 && b2;
}

template <typename C> constexpr bool push_dtor_test() {
  C c;
  typename C::value_type f(42);
  c.push(f);
  c.push(f); // ~Bar() (Bar destructor) called here
  bool b1 = 42 == *c.front().m_p && 2 == c.size();
  c.pop(); // ~Bar() (Bar destructor) called here
  bool b2 = 1 == c.size();
  return b1 && b2;
}

} // namespace tests_util

#endif // _CEST_TESTS_UTIL_HPP_
