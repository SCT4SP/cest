#ifndef _CEST_DEQUE_TESTS_HPP_
#define _CEST_DEQUE_TESTS_HPP_

#include "cest/deque.hpp"
#include <deque>
#include <cassert>

template <typename D>
constexpr bool deque_test1()
{
  D d;
  return true;
}

void deque_tests()
{
#ifndef NO_STATIC_TESTS
  static_assert(deque_test1<cest::deque<int>>());
#endif

  assert((deque_test1< std::deque<int>>()));
  assert((deque_test1<cest::deque<int>>()));
}

#endif //  _CEST_DEQUE_TESTS_HPP_
