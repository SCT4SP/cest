#ifndef _CEST_CCTYPE_HPP_
#define _CEST_CCTYPE_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/cctype_tests.hpp

#include "cest/string.hpp"
#include <algorithm>

namespace cest {

constexpr int isalnum(int ch) {
//  const char sz[] = "0123456789"
  string str("0123456789"
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"); // a raw string array includes \0
  return std::find(str.cbegin(), str.cend(), ch) != str.cend();
}

constexpr int isupper(int ch) {
  string str("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  return std::find(str.cbegin(), str.cend(), ch) != str.cend();
}

} // namespace cest

#endif // _CEST_CCTYPE_HPP_
