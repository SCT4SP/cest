#ifndef _CEST_STRINGSTREAM_TESTS_HPP_
#define _CEST_STRINGSTREAM_TESTS_HPP_

#include "cest/sstream.hpp"
#include "cest/string.hpp"
#include <sstream>
#include <string>
#include <cassert>

namespace ss_tests {

template <typename Iss, typename S>
constexpr bool istringstream_test1()
{
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
constexpr bool istringstream_test2()
{
  Iss s;
  S str("alphaville");
  s.str(std::move(str));
  char c;
  s.get(c);
  char buf[4];
  s.read(buf,4);
  bool b = 'a'==c && 'l'==buf[0] && 'p'==buf[1] && 'h'==buf[2] && 'a'==buf[3];
  s.get(c);
  return b && 'v'==c;
}

template <typename Ifs, typename S, typename Isbi>
constexpr bool istringstream_test3()
{
  Ifs file(__FILE__);
  //std::istreambuf_iterator<typename S::value_type> ite(file);
  Isbi it(file);
  bool b1 = *it == '#';
  S str(it, {});
  bool b2 = str[0] == '#';
  return b1 && b2;
}

} // namespace ss_tests

void stringstream_tests()
{
  using namespace ss_tests;

  using  std_isbi =  std::istreambuf_iterator<char>;
  using cest_isbi = cest::istreambuf_iterator<char>;

  assert((istringstream_test1< std::istringstream, std::string>()));
  assert((istringstream_test1<cest::istringstream, cest::string>()));
  assert((istringstream_test2< std::istringstream, std::string>()));
  assert((istringstream_test2<cest::istringstream, cest::string>()));
  assert((istringstream_test3< std::ifstream,  std::string,  std_isbi>()));
  assert((istringstream_test3<cest::ifstream, cest::string, cest_isbi>()));

#if CONSTEXPR_CEST == 1
  static_assert(istringstream_test1<cest::istringstream, cest::string>());
  static_assert(istringstream_test2<cest::istringstream, cest::string>());
#endif
}

#endif // _CEST_ISTRINGSTREAM_TESTS_HPP_
