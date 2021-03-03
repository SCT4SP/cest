#ifndef _CEST_FUNCTIONAL_TESTS_HPP_
#define _CEST_FUNCTIONAL_TESTS_HPP_

#include "cest/functional.hpp"
#include <functional>
#include <cassert>

namespace f_tests {

constexpr int forty_two(double *, char) { return 42; }

template <template <typename, typename...> typename F>
constexpr bool function_test1()
{
  bool b0 = true;
  double d{0};

  F<bool()> f1 = [&]{ return b0; };

  F<bool(double*,char)> f2 = forty_two;

  bool b = f1() && f2(&d,'q');
  return b;
}

} // namespace f_tests

void functional_tests()
{
  using namespace f_tests;

#if CONSTEXPR_CEST == 1
  static_assert(function_test1<cest::function>());
#endif

  assert((function_test1<std::function>()));
}


#endif // _CEST_FUNCTIONAL_TESTS_HPP_
