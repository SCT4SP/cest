#ifndef _CEST_IOSTREAM_TESTS_HPP_
#define _CEST_IOSTREAM_TESTS_HPP_

#include "cest/iostream.hpp"
#include <tuple>
#include <cassert>

constexpr auto iostream_test1()
{
  cest::cout << "Hello ";
  cest::cout << "World!" << cest::endl;
  cest::cerr << "Fire and ";
  cest::cerr << "Brimstone!" << cest::endl;
  cest::cout << cest::hex << 42 << ' ' << 42U << cest::endl;
  cest::cerr << cest::hex << 42 << ' ' << 42U << cest::endl;
  return std::tuple{true};
}

void iostream_tests()
{
  constexpr const auto tup1 = std::tuple{true};

#ifndef NO_STATIC_TESTS
  static_assert(iostream_test1() == tup1);
#endif
  
  assert(iostream_test1() == tup1);
}

#endif // _CEST_IOSTREAM_TESTS_HPP_
