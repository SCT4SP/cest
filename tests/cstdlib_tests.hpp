#ifndef _CEST_CSTDLIB_TESTS_HPP_
#define _CEST_CSTDLIB_TESTS_HPP_

#include "cest/cstdlib.hpp"
#include <cassert>

constexpr bool cstdlib_test1(auto abs)
{
  int x =  42;
  int y = -42;

  return abs(x)==abs(y);
}

void cstdlib_tests()
{
#if CONSTEXPR_CEST == 1
  static_assert(cstdlib_test1(cest::abs));
#endif
  
  assert(cstdlib_test1(cest::abs));
}

#endif // _CEST_CSTDLIB_TESTS_HPP_
