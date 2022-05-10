#ifndef _CEST_UTILITY_TESTS_HPP_
#define _CEST_UTILITY_TESTS_HPP_

#include <cassert>
#include <utility>

template <template <typename, typename> typename P>
constexpr bool pair_test1() {
  P p = {42, true};
  return p.first == 42 && p.second;
}

void utility_tests() {
#if CONSTEXPR_CEST == 1
  static_assert(pair_test1<std::pair>());
#endif

  assert(pair_test1<std::pair>());
}

#endif // _CEST_UTILITY_TESTS_HPP_
