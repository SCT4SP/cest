#ifndef _CEST_OPTIONAL_TESTS_HPP_
#define _CEST_OPTIONAL_TESTS_HPP_

#include "cest/optional.hpp"
#include <optional>
#include <cassert>

template <typename O> constexpr bool optional_test1() {
  O o1{42}, o2;
  return o1.value() == 42 && !o2;
}

void optional_tests() {
#if CONSTEXPR_CEST == 1
  static_assert(optional_test1<cest::optional<int>>());
#endif

  assert((optional_test1<std::optional<int>>()));
  assert((optional_test1<cest::optional<int>>()));
}

#endif // _CEST_OPTIONAL_TESTS_HPP_
