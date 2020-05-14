#ifndef _CEST_IOSTREAM_TESTS_HPP_
#define _CEST_IOSTREAM_TESTS_HPP_

#include "cest/iostream.hpp"
#include <tuple>
#include <cassert>

constexpr auto iostream_test1()
{
  return std::tuple{true};
}

void iostream_tests()
{
  constexpr const auto tup1 = std::tuple{true};

#ifndef NO_STATIC_TESTS
//  static_assert((iostream_test1<cest::string>()) == tup1);
#endif
  
  assert(iostream_test1() == tup1);
}

#endif // _CEST_IOSTREAM_TESTS_HPP_
