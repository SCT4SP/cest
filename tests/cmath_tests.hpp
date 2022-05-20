#ifndef _CEST_CMATH_TESTS_HPP_
#define _CEST_CMATH_TESTS_HPP_

#include "tests_util.hpp"
#include <cassert>
#include <cmath>

template <typename T>
constexpr bool cmath_test1() {
  const T tol{0.01};

  const T reference_pi{3.14159};
  const T pi = std::atan(1) * 4;
  bool b1 = std::abs(reference_pi - pi) < tol;
  bool b2 = tests_util::comparable(T{5}, std::sqrt(T{25}));
  return b1 && b2;
}

void cmath_tests() {
#if CONSTEXPR_CEST == 1
  static_assert(cmath_test1<double>());
#endif

  assert(cmath_test1<double>());
}

#endif // _CEST_CMATH_TESTS_HPP_
