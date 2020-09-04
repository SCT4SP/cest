#ifndef _CEST_NUMERIC_TESTS_HPP_
#define _CEST_NUMERIC_TESTS_HPP_

#include "cest/numeric.hpp"
#include "cest/deque.hpp"
#include <numeric>
#include <deque>

template <typename Dq>
constexpr bool numeric_test1()
{
  Dq dq = {1,2,3,4,5};
  typename Dq::value_type x;

  if constexpr (std::is_same_v<Dq, cest::deque<int>>) {
    x = cest::accumulate(dq.begin(), dq.end(), 0);
  } else {
    x =  std::accumulate(dq.begin(), dq.end(), 0);
  }

  return 15==x;
}

void numeric_tests()
{
#if CONSTEXPR_CEST == 1
  static_assert(deque_test1<cest::deque<int>>());
#endif

  assert((numeric_test1< std::deque<int>>()));
  assert((numeric_test1<cest::deque<int>>()));
}

#endif // _CEST_NUMERIC_TESTS_HPP_
