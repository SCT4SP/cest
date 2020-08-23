#ifndef _CEST_TESTS_UTIL_HPP_
#define _CEST_TESTS_UTIL_HPP_

#include <algorithm>

namespace tests_util {

struct Bar {
  constexpr Bar()             : m_p(new int(42))      { }
  constexpr Bar(int x)        : m_p(new int(x))       { }
  constexpr Bar(const Bar &f) : m_p(new int(*f.m_p))  { }
  constexpr ~Bar() { delete m_p; }
  int* m_p;
};

template <typename C>
constexpr bool push_back_dtor_test() {
  C c;
  tests_util::Bar f(42);
  c.push_back(f);
  c.push_back(f); // ~Bar() (Bar destructor) called here (by reserve w' vector)
  bool b1 = 42==*c.begin()->m_p && 2==std::distance(c.begin(), c.end());
  c.pop_back();   // ~Bar() (Bar destructor) called here
  bool b2 = 1==std::distance(c.begin(), c.end());
  return b1 && b2;
}

template <typename C>
constexpr bool push_front_dtor_test() {
  C c;
  tests_util::Bar f(42);
  c.push_front(f);
  c.push_front(f); // ~Bar() (Bar destructor) called here
  bool b1 = 42==*c.begin()->m_p && 2==std::distance(c.begin(), c.end());
  c.pop_front();    // ~Bar() (Bar destructor) called here
  bool b2 = 1==std::distance(c.begin(), c.end());
  return b1 && b2;
}

template <typename C>
constexpr bool push_dtor_test() {
  C c;
  tests_util::Bar f(42);
  c.push(f);
  c.push(f); // ~Bar() (Bar destructor) called here
  bool b1 = 42==*c.front().m_p && 2==c.size();
  c.pop();    // ~Bar() (Bar destructor) called here
  bool b2 = 1==c.size();
  return b1 && b2;
}

} // namespace tests_util

#endif // _CEST_TESTS_UTIL_HPP_
