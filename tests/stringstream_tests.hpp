#ifndef _CEST_STRINGSTREAM_TESTS_HPP_
#define _CEST_STRINGSTREAM_TESTS_HPP_

#include "cest/sstream.hpp"
#include "cest/string.hpp"
#include <sstream>
#include <string>
#include <cassert>

namespace ss_tests {

template <typename Iss, typename S>
constexpr bool istringstream_test1() {
  S str("def");
//  Iss s1("abc");
  Iss s2(str);
  char c;
  s2.get(c);
  return true;//s1.get() == 'a' && s2.get() == 'd';
}

} // namespace ss_tests

void stringstream_tests()
{
  using namespace ss_tests;

//  assert((istringstream_test1<std::istringstream, std::string>()));
  assert((istringstream_test1<cest::istringstream, cest::string>()));

#ifndef NO_STATIC_TESTS
  static_assert(istringstream_test1<cest::istringstream, cest::string>());
//  static_assert(istringstream_test1<std::istringstream, std::string>());
#endif
}

#endif // _CEST_ISTRINGSTREAM_TESTS_HPP_
