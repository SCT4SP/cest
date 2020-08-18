#ifndef _CEST_CCTYPE_HPP_
#define _CEST_CCTYPE_HPP_

#include "cest/string.hpp"
#include <algorithm>

namespace cest {

// returns 8 (non-zero) on a successful find; as do libstdc++ and libc++
constexpr int isalnum(int ch) {
  string str("0123456789"
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"); // a raw string array includes \0
  return std::find(str.cbegin(), str.cend(), ch) != str.cend() ? 8 : 0;
}

// returns 256 (non-zero) on a successful find; as do libstdc++ and libc++
constexpr int isupper(int ch) {
  string str("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  return std::find(str.cbegin(), str.cend(), ch) != str.cend() ? 256 : 0;
}

} // namespace cest

#endif // _CEST_CCTYPE_HPP_
