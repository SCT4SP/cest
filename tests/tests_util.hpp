#ifndef _CEST_TESTS_UTIL_HPP_
#define _CEST_TESTS_UTIL_HPP_

namespace tests_util {

struct Bar {
  constexpr Bar()             : m_p(new int(42))      { }
  constexpr Bar(int x)        : m_p(new int(x))       { }
  constexpr Bar(const Bar &f) : m_p(new int(*f.m_p))  { }
  constexpr ~Bar() { delete m_p; }
  int* m_p;
};

} // namespace tests_util

#endif // _CEST_TESTS_UTIL_HPP_
