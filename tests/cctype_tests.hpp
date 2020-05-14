#ifndef _CEST_CCTYPE_TESTS_HPP_
#define _CEST_CCTYPE_TESTS_HPP_

#include "cest/cctype.hpp"
#include <cctype>
#include <tuple>
#include <cassert>

void cctype_tests()
{
#ifndef NO_STATIC_TESTS
  static_assert(cest::isalnum('q') != 0);
  static_assert(cest::isalnum('!') == 0);
#endif

  assert(std::isalnum('q') != 0);
  assert(cest::isalnum('q') != 0);
  assert(std::isalnum('!') == 0);
  assert(cest::isalnum('!') == 0);
}

#endif // _CEST_CCTYPE_TESTS_HPP_
