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
  typename S::size_type  s1  = str.size();
  typename S::size_type  c1  = str.capacity();
  typename S::value_type nt0 = str.c_str()[str.length()];

  str.push_back('q');
  typename S::value_type nt1 = str.c_str()[str.length()];
  bool b2 = str.empty();
  typename S::size_type s2 = str.size();
  auto q = str[0];
  
  return std::tuple{b1,s1,b2,s2,q,nt1};
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

template <typename S>
constexpr auto string_test4()
{
  S str("abcde,abcde");
  S cd("cd"), xxx("xxx");
  
  typename S::size_type i1 = str.find('b');
  typename S::size_type i2 = str.find('x');
  typename S::size_type i3 = str.find("cd");
  typename S::size_type i4 = str.find("xxx");
  typename S::size_type i5 = str.find(cd);
  typename S::size_type i6 = str.find(xxx);
  cd.clear();
  bool e = cd.empty();
  return std::tuple{str[i1],i2==S::npos,i3,i4==S::npos,i5,i6==S::npos,e};
}

template <typename S>
constexpr auto string_test5()
{
  S str("abc");
  bool b1 = (str += 'd') == "abcd";
  bool b2 = str == "abcd";
  str += str;
  bool b3 = str == "abcdabcd";
  typename S::size_type len = str.length();
  auto nt = str.c_str()[str.length()];
  return std::tuple{b1,b2,b3,len,nt};
}

void string_tests()
{
  constexpr const auto tup1 = std::tuple{true,0,false,1,'q','\0'};
  constexpr const auto tup2 = std::tuple{true,2,2,'O','k','\0','\0',true,'!'};
  constexpr const auto tup3 = std::tuple{false,false,false,false};
  constexpr const auto tup4 = std::tuple{'b',true,2,true,2,true,true};
  constexpr const auto tup5 = std::tuple{true,true,true,8,'\0'};

#ifndef NO_STATIC_TESTS
//  static_assert((string_test1<cest::string>()) == tup1);
//  static_assert((string_test2<cest::string>()) == tup2);
//  static_assert((string_test3<cest::string>()) == tup3);
//  static_assert((string_test4<cest::string>()) == tup4);
//  static_assert((string_test5<cest::string>()) == tup5);
#endif
  
  assert(string_test1<std::string>()  == tup1);
  assert(string_test1<cest::string>() == tup1);
  assert(string_test2<std::string>()  == tup2);
  assert(string_test2<cest::string>() == tup2);
  assert(string_test3<std::string>()  == tup3);
  assert(string_test3<cest::string>() == tup3);
  assert(string_test4<std::string>()  == tup4);
  assert(string_test4<cest::string>() == tup4);
  assert(string_test5<std::string>()  == tup5);
  assert(string_test5<cest::string>() == tup5);
}

#endif // _CEST_ALGORITHM_TESTS_HPP_
