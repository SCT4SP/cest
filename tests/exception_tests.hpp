#ifndef _CEST_EXCEPTION_TESTS_HPP_
#define _CEST_EXCEPTION_TESTS_HPP_

#include "cest/exception.hpp"
#include "cest/string.hpp"
#include <exception>
#include <string>

template <typename E, typename Str> constexpr bool exception_test1() {
  E e;
  Str s(e.what());
  bool b = s.find("::exception") != Str::npos;
  return b;
}

void exception_tests() {
#if CONSTEXPR_CEST == 1
  static_assert(exception_test1<cest::exception, cest::string>());
#endif
  assert((exception_test1<std::exception, std::string>()));
}

#endif // _CEST_EXCEPTION_TESTS_HPP_
