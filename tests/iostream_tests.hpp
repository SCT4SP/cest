#ifndef _CEST_IOSTREAM_TESTS_HPP_
#define _CEST_IOSTREAM_TESTS_HPP_

#include "cest/iostream.hpp"
#include "cest/fstream.hpp"
#include <iostream>
#include <fstream>
#include <cassert>

constexpr bool iostream_test1(auto &cout, auto &cerr, auto &hex, auto &endl)
{
  cout << "Hello ";
  cout << "World!" << endl;
  cerr << "Fire and ";
  cerr << "Brimstone!" << endl;
  cout << hex << 66 << ' ' << 66U << endl; // 42
  cerr << hex << 66 << ' ' << 66U << endl;
  return true;
}

template <typename Str, typename Ifs, typename Is>
constexpr bool iostream_test2()
{
  Str filename(__FILE__);
  Ifs ifs(filename.c_str());
  if (!ifs)
    return false;
  Is& is = ifs;
  char c;
  is.get(c);
  return '#' == c;
}

void iostream_tests()
{
  constexpr const auto tup1 = std::tuple{true};

  using std_char_type    = decltype(std::cout)::char_type;
  using std_traits_type  = decltype(std::cout)::traits_type;
  auto  &std_endl = std::endl<std_char_type,std_traits_type>;

  using cest_char_type   = decltype(cest::cout)::char_type;
  using cest_traits_type = decltype(cest::cout)::traits_type;
  auto &cest_endl = cest::endl<cest_char_type,cest_traits_type>;

#if RUN_STATIC_TESTS == 1
  static_assert(iostream_test1(cest::cout, cest::cerr, cest::hex, cest_endl));
  static_assert(iostream_test2<std::string,std::ifstream,std::istream>());
#endif

  assert(iostream_test1( std::cout,  std::cerr,  std::hex,  std_endl));
  assert(iostream_test1(cest::cout, cest::cerr, cest::hex, cest_endl));
  assert((iostream_test2<std::string,std::ifstream,std::istream>()));
}

#endif // _CEST_IOSTREAM_TESTS_HPP_
