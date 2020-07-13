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
  Iss s1("abc");
  Iss s2(str);
  char c1, c2;
  s1.get(c1);
  s2.get(c2);
  bool b1 = c1 == 'a' && c2 == 'd';
  s1.get(c1);
  s2.get(c2);
  bool b2 = c1 == 'b' && c2 == 'e';
  bool good = s1.good() && s2.good();
  s1.unget();
  s2.unget();
  s1.get(c1);
  s2.get(c2);
  bool b3 = c1 == 'b' && c2 == 'e';
  bool beof1 = !s1.eof() && !s1.fail();
  s1.get(c1);
  s1.get(c1);
  bool beof2 = s1.eof() && s1.fail();
  return b1 && b2 && b3 && good && beof1 && beof2;
}

template <typename Iss, typename S>
constexpr bool istringstream_test2() {
  Iss s;
  S str("alphaville");
  s.str(std::move(str));
  char c;
  s.get(c);
  return 'a' == c;
}

} // namespace ss_tests

void stringstream_tests()
{
  using namespace ss_tests;

  assert((istringstream_test1<std::istringstream, std::string>()));
  assert((istringstream_test1<cest::istringstream, cest::string>()));
  assert((istringstream_test2<std::istringstream, std::string>()));
  assert((istringstream_test2<cest::istringstream, cest::string>()));

#if RUN_STATIC_TESTS == 1
  static_assert(istringstream_test1<cest::istringstream, cest::string>());
//  static_assert(istringstream_test1<std::istringstream, std::string>());
  static_assert((istringstream_test2<cest::istringstream, cest::string>()));
#endif
}

#endif // _CEST_ISTRINGSTREAM_TESTS_HPP_
