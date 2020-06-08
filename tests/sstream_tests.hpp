#ifndef _CEST_SSTREAM_TESTS_HPP_
#define _CEST_SSTREAM_TESTS_HPP_

#include "cest/sstream.hpp"
#include <sstream>
#include <cassert>

template <typename S>
constexpr bool sstream_test1()
{
  S s;
  return true;
}

void sstream_tests()
{
#ifndef NO_STATIC_TESTS
//  static_assert(sstream_test1<cest::stringstream>());
#endif

  assert((sstream_test1< std::stringstream>()));
//  assert((sstream_test1<cest::stringstream>()));
}

#endif //  _CEST_SSTREAM_TESTS_HPP_
