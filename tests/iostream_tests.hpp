#ifndef _CEST_IOSTREAM_TESTS_HPP_
#define _CEST_IOSTREAM_TESTS_HPP_

#include "cest/iostream.hpp"
#include <iostream>
#include <cassert>

constexpr auto iostream_test1(auto &cout, auto &cerr, auto &hex, auto &endl)
{
  cout << "Hello ";
  cout << "World!" << endl;
  cerr << "Fire and ";
  cerr << "Brimstone!" << endl;
  cout << hex << 66 << ' ' << 66U << endl; // 42
  cerr << hex << 66 << ' ' << 66U << endl;
  return true;
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

#ifndef NO_STATIC_TESTS
  static_assert(iostream_test1(cest::cout, cest::cerr, cest::hex, cest_endl));
#endif

  assert(iostream_test1( std::cout,  std::cout,  std::hex,  std_endl));
  assert(iostream_test1(cest::cout, cest::cout, cest::hex, cest_endl));
}

#endif // _CEST_IOSTREAM_TESTS_HPP_
