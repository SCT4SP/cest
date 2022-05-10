#ifndef _CEST_NUMERIC_TESTS_HPP_
#define _CEST_NUMERIC_TESTS_HPP_

#include <deque>
#include <numeric>

template <typename Dq> constexpr bool numeric_test1() {
  Dq dq = {1, 2, 3, 4, 5};
  typename Dq::value_type x;

  x = std::accumulate(dq.begin(), dq.end(), 0);

  return 15 == x;
}

void numeric_tests() {
#if CONSTEXPR_CEST == 1
  static_assert(numeric_test1<std::deque<int>>());
#endif

  assert((numeric_test1<std::deque<int>>()));
}

#endif // _CEST_NUMERIC_TESTS_HPP_
