#ifndef _CEST_COMPLEX_TESTS_HPP_
#define _CEST_COMPLEX_TESTS_HPP_

#include "cest/complex.hpp"
#include "tests_util.hpp"
#include <cassert>
#include <complex>

template <typename Cx> constexpr bool complex_test1() {
  Cx x{-3, 4};
  Cx y = x + x;
  Cx z = x * x;
  bool b1 = -3 == x.real() && 4 == x.imag();
  bool b2 = y == Cx{-6, 8};
  bool b3 = z == Cx{-7, -24};
  bool b4 = tests_util::comparable(5.0, abs(x));
  return b1 && b2 && b3 && b4;
}

void complex_tests() {
#if CONSTEXPR_CEST == 1
  static_assert(complex_test1<cest::complex<double>>());
#endif

  assert(complex_test1<std::complex<double>>());
  assert(complex_test1<cest::complex<double>>());
}

#endif // _CEST_COMPLEX_TESTS_HPP_
