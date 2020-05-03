#ifndef _CEST_STRING_TESTS_HPP_
#define _CEST_STRING_TESTS_HPP_

#include "cest/string.hpp"
#include <string>
#include <tuple>
#include <cassert>

template <typename S>
constexpr auto string_test1()
{
  S str;
  bool b1 = str.empty();
  typename S::size_type s1 = str.size();

  str.push_back('q');
  bool b2 = str.empty();
  typename S::size_type s2 = str.size();
  auto q = str[0];
  
  return std::tuple{b1,s1,b2,s2,q};
}

void string_tests()
{
  constexpr const auto tup1 = std::tuple{true,0,false,1,'q'};

#ifndef NO_STATIC_TESTS
  static_assert((string_test1<cest::string>()) == tup1);
#endif
  
  assert(string_test1<cest::string>() == tup1);
  assert(string_test1<std::string>()  == tup1);
}

#endif // _CEST_ALGORITHM_TESTS_HPP_
