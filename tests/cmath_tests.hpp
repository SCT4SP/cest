#ifndef _CEST_CMATH_TESTS_HPP_
#define _CEST_CMATH_TESTS_HPP_

#include "tests_util.hpp"
#include <cest/cmath.hpp>
#include <cmath>
#include <cassert>

template <typename T, typename F1, typename F2, typename F3>
constexpr bool cmath_test1(F1 abs, F2 atan, F3 sqrt)
{
  const T tol{0.01};

  const T reference_pi{3.14159};
  const T pi = atan(1) * 4;
  bool b1 = abs(reference_pi-pi) < tol;
  bool b2 = tests_util::comparable(T{5},sqrt(T{25}));
  return b1 && b2;
}

void cmath_tests()
{
  // function pointers
  auto std_abs_d = static_cast<double(*)(double)>(std::abs);
  auto std_atan_d = static_cast<double(*)(double)>(std::atan);
  auto std_sqrt_d = static_cast<double(*)(double)>(std::sqrt);
  constexpr auto cest_abs_d = static_cast<double(*)(double)>(cest::abs);
  constexpr auto cest_atan_d = static_cast<double(*)(double)>(cest::atan);
  constexpr auto cest_sqrt_d = static_cast<double(*)(double)>(cest::sqrt);

#if CONSTEXPR_CEST == 1
  static_assert(cmath_test1<double>(cest_abs_d,cest_atan_d,cest_sqrt_d));
#endif

  assert(cmath_test1<double>(std_abs_d,std_atan_d,std_sqrt_d));
  assert(cmath_test1<double>(cest_abs_d,cest_atan_d,cest_sqrt_d));
}


#endif // _CEST_CMATH_TESTS_HPP_
