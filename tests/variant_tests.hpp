#ifndef _CEST_VARIANT_TESTS_HPP_
#define _CEST_VARIANT_TESTS_HPP_

#include "cest/variant.hpp"
#include <variant>
#include <cassert>

template <typename V> constexpr bool variant_test1() {
  V vd{3.142};
  V vc{'q'};
  return vd.index() == 1 && vc.index() == 2;
}

void variant_tests() {
#if CONSTEXPR_CEST == 1
  static_assert(variant_test1<cest::variant<int, double, char>>());
#endif

  assert((variant_test1<std::variant<int, double, char>>()));
  assert((variant_test1<cest::variant<int, double, char>>()));
}

#endif // _CEST_VARIANT_TESTS_HPP_
