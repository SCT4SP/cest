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
  S str("Ok");
  auto nt = str.c_str()[str.length()];
  S str2("_k!");
  str2.front() = 'O';
  auto c = str2.back();
  str2.pop_back();
  auto nt2 = str2.c_str()[str2.length()];
  bool eq = str == str2 && str == "Ok";
  return std::tuple{true,str.size(),str.length(),str[0],str[1],nt,nt2,eq,c};
}

template <typename S>
constexpr auto string_test3()
{
  S strA("abc"), strB("abcZZZ");
  bool b1 = strA == "abcZZZ";
  bool b2 = "abcZZZ" == strA;
  bool b3 = strB == "abc";
  bool b4 = "abc" == strB;
  return std::tuple{b1,b2,b3,b4};
}

void string_tests()
{
  constexpr const auto tup1 = std::tuple{true,0,false,1,'q'};
  constexpr const auto tup2 = std::tuple{true,2,2,'O','k','\0','\0',true,'!'};
  constexpr const auto tup3 = std::tuple{false,false,false,false};

#ifndef NO_STATIC_TESTS
  static_assert((string_test1<cest::string>()) == tup1);
  static_assert((string_test2<cest::string>()) == tup2);
  static_assert((string_test3<cest::string>()) == tup3);
#endif
  
  assert(string_test1<cest::string>() == tup1);
  assert(string_test1<std::string>()  == tup1);
  assert(string_test2<cest::string>() == tup2);
  assert(string_test2<std::string>()  == tup2);
  assert(string_test3<cest::string>() == tup3);
  assert(string_test3<std::string>()  == tup3);
}

#endif // _CEST_ALGORITHM_TESTS_HPP_
