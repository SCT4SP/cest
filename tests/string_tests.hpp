#ifndef _CEST_STRING_TESTS_HPP_
#define _CEST_STRING_TESTS_HPP_

#include "cest/string.hpp"
#include "cest/iostream.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <tuple>
#include <cassert>

template <typename S>
constexpr auto string_test1()
{
  S str;
  bool b1 = str.empty();
  bool b2 = str.capacity() > 0;
  typename S::size_type  s1  = str.size();
  typename S::value_type nt0 = str.c_str()[str.length()];

  str.push_back('q');
  typename S::value_type nt1 = str.c_str()[str.length()];
  bool b3 = str.empty();
  typename S::size_type s2 = str.size();
  auto q = str[0];
  
  return std::tuple{b1,b2,s1,b3,s2,q,nt0,nt1};
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
  return std::tuple{str.size(),str.length(),str[0],str[1],nt,nt2,eq,c};
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
  S str0("");
  bool b0 = str0.capacity() > 0;
  S str("abc");
  bool b1 = (str += 'd') == "abcd";
  bool b2 = str == "abcd";
  str += str;
  bool b3 = str == "abcdabcd";
  typename S::size_type len = str.length();
  auto nt = str.c_str()[str.length()];
  return std::tuple{b0,b1,b2,b3,len,nt};
}

template <typename S>
constexpr auto string_test6(auto &cout, auto &endl) {
  S str("Zod");
  cout << str << endl;
  return std::tuple{true};
}

template <typename S>
constexpr bool string_test7() {
  S str("Zod");
  S str2(str);
  str += '!';
  return str != str2;
}

template <typename S>
constexpr bool string_test8() {
  S str1("Hello\0 World");
  S str2("Hello\0 World", 12);
  return str1.length() != str2.length();
}

void string_tests()
{
  constexpr const auto tup1 = std::tuple{true,true,0,false,1,'q','\0','\0'};
  constexpr const auto tup2 = std::tuple{2,2,'O','k','\0','\0',true,'!'};
  constexpr const auto tup3 = std::tuple{false,false,false,false};
  constexpr const auto tup4 = std::tuple{'b',true,2,true,2,true,true};
  constexpr const auto tup5 = std::tuple{true,true,true,true,8,'\0'};
  constexpr const auto tup6 = std::tuple{true};

  using std_char_type    = decltype(std::cout)::char_type;
  using std_traits_type  = decltype(std::cout)::traits_type;
  auto  &std_endl = std::endl<std_char_type,std_traits_type>;

  using cest_char_type   = decltype(cest::cout)::char_type;
  using cest_traits_type = decltype(cest::cout)::traits_type;
  auto &cest_endl = cest::endl<cest_char_type,cest_traits_type>;

#ifndef NO_STATIC_TESTS
  static_assert((string_test1<cest::string>()) == tup1);
  static_assert((string_test2<cest::string>()) == tup2);
  static_assert((string_test3<cest::string>()) == tup3);
  static_assert((string_test4<cest::string>()) == tup4);
  static_assert((string_test5<cest::string>()) == tup5);
  static_assert(string_test6<cest::string>(cest::cout, cest_endl) == tup6);
#endif
  
  assert(string_test1< std::string>() == tup1);
  assert(string_test1<cest::string>() == tup1);
  assert(string_test2< std::string>() == tup2);
  assert(string_test2<cest::string>() == tup2);
  assert(string_test3< std::string>() == tup3);
  assert(string_test3<cest::string>() == tup3);
  assert(string_test4< std::string>() == tup4);
  assert(string_test4<cest::string>() == tup4);
  assert(string_test5< std::string>() == tup5);
  assert(string_test5<cest::string>() == tup5);
  assert(string_test6< std::string>( std::cout,  std_endl) == tup6);
  assert(string_test6<cest::string>(cest::cout, cest_endl) == tup6);
  assert(string_test7< std::string>());
  assert(string_test7<cest::string>());
  assert(string_test8< std::string>());
  assert(string_test8<cest::string>());
}

#endif // _CEST_ALGORITHM_TESTS_HPP_
