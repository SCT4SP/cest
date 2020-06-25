#ifndef _CEST_IFSTREAM_TESTS_HPP_
#define _CEST_IFSTREAM_TESTS_HPP_

#include "cest/fstream.hpp"
#include <fstream>
#include <cassert>

namespace ifs_tests {

template <typename Ifs>
constexpr bool ifstream_test1() {
  Ifs f("not real");
  return true;
}

} // namespace ifs_tests

void ifstream_tests()
{
  using namespace ifs_tests;

  assert(ifstream_test1<cest::ifstream>());

#ifndef NO_STATIC_TESTS
  static_assert(ifstream_test1<cest::ifstream>());
#endif
}

#endif // _CEST_IFSTREAM_TESTS_HPP_
