#ifndef _CEST_CCTYPE_HPP_
#define _CEST_CCTYPE_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/cctype_tests.hpp

#include <algorithm>

namespace cest {

constexpr int isalnum(int ch) {
  const char sz[] = "0123456789"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                    "abcdefghijklmnopqrstuvwxyz";
  return std::find(std::cbegin(sz), std::cend(sz) ,ch) != std::cend(sz);
}

} // namespace cest

#endif // _CEST_CCTYPE_HPP_
