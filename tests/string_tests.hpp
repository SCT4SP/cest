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
constexpr bool string_test1()
{
  S str;
  bool b1 = str.empty();
  bool b2 = str.capacity() > 0;
  typename S::size_type  s1  = str.size();
  typename S::value_type nt0 = str.c_str()[str.length()];

  str.push_back('q');
/*
  typename S::value_type nt1 = str.c_str()[str.length()];
  bool b3 = !str.empty();
  typename S::size_type s2 = str.size();
  auto q = str[0];
*/

  return b1 && b2;// && s1==0 && b3 && s2==1 && q=='q' && nt0=='\0' && nt1=='\0';
}

template <typename S>
constexpr bool string_test2()
{
  S str("Ok");
  auto nt = str.c_str()[str.length()];
  S str2("_k!");
  str2.front() = 'O';
  auto c = str2.back();
  str2.pop_back();
  auto nt2 = str2.c_str()[str2.length()];
  bool eq = str == str2 && str == "Ok";

  bool b1 = 2==str.size() && 2==str.length() && 'O'==str[0] && 'k'==str[1];
  bool b2 = '\0'==nt      && '\0'==nt2       && eq          && '!'==c;
  return b1 && b2;
}

template <typename S>
constexpr bool string_test3()
{
  S strA("abc"), strB("abcZZZ");
  bool b1 = strA == "abcZZZ";
  bool b2 = "abcZZZ" == strA;
  bool b3 = strB == "abc";
  bool b4 = "abc" == strB;

  return !b1 && !b2 && !b3 && !b4;
}

template <typename S>
constexpr bool string_test4()
{
  S str("abcde,abcde");
  S cd("cd"), xxx("xxx");
  S one_dollar("$1");
  
  typename S::size_type i1 = str.find('b');
  typename S::size_type i2 = str.find('x');
  typename S::size_type i3 = str.find("cd");
  typename S::size_type i4 = str.find("xxx");
  typename S::size_type i5 = str.find(cd);
  typename S::size_type i6 = str.find(xxx);
  cd.clear();
  bool b1 = str[i1]=='b' && i2==S::npos && i3==2 && i4==S::npos;
  bool b2 = i5==2 && i6==S::npos && cd.empty();
  bool b3 = !(one_dollar.find('$')==S::npos);
  return b1 && b2 && b3;
}

template <typename S>
constexpr bool string_test5()
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

  return b0 && b1 && b2 && b3 && 8==len && '\0'==nt;
}

template <typename S, typename T, typename U>
constexpr bool string_test6(T &cout, U &endl) {
  S str("Zod");
  cout << str << endl;
  return true;
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

template <typename S>
constexpr bool string_test9() {
  S str1("abc");
  S str2("abcd");
  int cmp = str1.compare(str2);
  bool b1 = cmp < 0;
  bool b2 = str1 < str2;
  return b1 && b2;
}

template <typename S>
constexpr bool string_test10() {
  S str("abc"), str2;
  using Traits = std::char_traits<typename S::value_type>;
  auto len1 = Traits::length(str.c_str());
  str.reserve(32);
  auto len2 = Traits::length(str.c_str());
  str2 = str;
  bool b = str[0]==str2[0] && str[1]==str2[1] && str[2]==str2[2];
  auto len3 = Traits::length(str2.c_str());
  return len1==len2 && len1==len3;
}

void string_tests()
{
  using std_char_type    = decltype(std::cout)::char_type;
  using std_traits_type  = decltype(std::cout)::traits_type;
  auto  &std_endl = std::endl<std_char_type,std_traits_type>;

  using cest_char_type   = decltype(cest::cout)::char_type;
  using cest_traits_type = decltype(cest::cout)::traits_type;
  auto &cest_endl = cest::endl<cest_char_type,cest_traits_type>;

#if CONSTEXPR_CEST == 1
  static_assert(string_test1<cest::string>());
  static_assert(string_test2<cest::string>());
  static_assert(string_test3<cest::string>());
  static_assert(string_test4<cest::string>());
  static_assert(string_test5<cest::string>());
  static_assert(string_test6<cest::string>(cest::cout, cest_endl));
  static_assert(string_test9<cest::string>());
  static_assert(string_test10<cest::string>());
#endif
  
  assert(string_test1< std::string>());
  assert(string_test1<cest::string>());
  assert(string_test2< std::string>());
  assert(string_test2<cest::string>());
  assert(string_test3< std::string>());
  assert(string_test3<cest::string>());

  assert(string_test4<cest::string>());
  assert(string_test4< std::string>());
  assert(string_test4<cest::string>());

  assert(string_test5< std::string>());
  assert(string_test5<cest::string>());
  assert(string_test6< std::string>( std::cout,  std_endl));
  assert(string_test6<cest::string>(cest::cout, cest_endl));
  assert(string_test7< std::string>());
  assert(string_test7<cest::string>());
  assert(string_test8< std::string>());
  assert(string_test8<cest::string>());
  assert(string_test9< std::string>());
  assert(string_test9<cest::string>());
  assert(string_test10<std::string>());
  assert(string_test10<cest::string>());
}

#endif // _CEST_ALGORITHM_TESTS_HPP_
