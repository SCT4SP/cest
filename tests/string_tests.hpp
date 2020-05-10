#ifndef _CEST_STRING_TESTS_HPP_
#define _CEST_STRING_TESTS_HPP_

#include "cest/string.hpp"
#include <string>
#include <vector>
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

template <typename S>
constexpr auto string_test2()
{
  S str("ok");
  return std::tuple{true};
}

template <typename S, template <typename...> typename V>
constexpr auto string_test3()
{

//  S const token(tokens.front());
  /*S token(tokens.front());
  tokens.pop();

  bool b = token == "Zod";*/
//  return std::tuple{b};
  return std::tuple{true};
}

void string_tests()
{
  constexpr const auto tup1 = std::tuple{true,0,false,1,'q'};
  constexpr const auto tup2 = std::tuple{true};
  constexpr const auto tup3 = std::tuple{true};

#ifndef NO_STATIC_TESTS
  static_assert((string_test1<cest::string>()) == tup1);
  static_assert((string_test2<cest::string>()) == tup2);
#endif
  
  assert(string_test1<cest::string>() == tup1);
  assert(string_test1<std::string>()  == tup1);
  assert(string_test2<cest::string>() == tup2);
  assert(string_test2<std::string>()  == tup2);
//  assert((string_test3<std::string,std::vector>()) == tup2);
}

#endif // _CEST_ALGORITHM_TESTS_HPP_
