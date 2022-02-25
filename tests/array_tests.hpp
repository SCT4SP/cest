#ifndef _CEST_ARRAY_TESTS_HPP_
#define _CEST_ARRAY_TESTS_HPP_

#include "cest/array.hpp"
#include <array>
#include <cassert>

template <typename A> constexpr bool array_test1() {
  A a{1, 2, 3, 4, 5};
  return 5 == a[4] && 5 == a.size();
}

void array_tests() {
#if CONSTEXPR_CEST == 1
  static_assert(array_test1<cest::array<int, 5>>());
#endif

  assert((array_test1<std::array<int, 5>>()));
  assert((array_test1<cest::array<int, 5>>()));
}

#endif // _CEST_ARRAY_TESTS_HPP_
