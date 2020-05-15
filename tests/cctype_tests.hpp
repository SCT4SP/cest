#ifndef _CEST_CCTYPE_TESTS_HPP_
#define _CEST_CCTYPE_TESTS_HPP_

#include "cest/cctype.hpp"
#include <cctype>
#include <cassert>
#include <limits>

void cctype_tests()
{
#ifndef NO_STATIC_TESTS
  static_assert(cest::isalnum('q') != 0);
  static_assert(cest::isalnum('!') == 0);

  static_assert(cest::isupper('W') != 0);
  static_assert(cest::isupper('a') == 0);
  static_assert(cest::isupper('!') == 0);
#endif

  assert( std::isalnum('q') != 0);
  assert(cest::isalnum('q') != 0);
  assert( std::isalnum('!') == 0);
  assert(cest::isalnum('!') == 0);

  assert( std::isupper('W') != 0);
  assert(cest::isupper('W') != 0);
  assert( std::isupper('a') == 0);
  assert(cest::isupper('a') == 0);
  assert( std::isupper('!') == 0);
  assert(cest::isupper('!') == 0);

  for (auto c = std::numeric_limits<char>::lowest(); ; ++c) {
    assert(std::isalnum(c) == cest::isalnum(c));
    if (c == std::numeric_limits<char>::max()) break;
  }
}

#endif // _CEST_CCTYPE_TESTS_HPP_
