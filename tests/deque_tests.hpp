#ifndef _CEST_DEQUE_TESTS_HPP_
#define _CEST_DEQUE_TESTS_HPP_

#include "cest/deque.hpp"
#include <deque>
#include <cassert>

template <typename D>
constexpr bool deque_test1()
{
  D d;
  bool b1 = d.size() == 0 &&  d.empty();
  d.push_front(3);
  d.push_front(2);
  d.push_front(1);
  bool b2 = d.size() == 3 && !d.empty();
  d.push_back(4);
  d.push_back(5);
  bool b3 = d.size() == 5 && !d.empty();
  bool b4 = d[0]==1 && d[1]==2 && d[2]==3 && d[3]==4 && d[4]==5;
  bool b5 = d.front()==1 && d.back()==5;
  d.pop_back();
  d.pop_front();
  bool b6 = d[0]==2 && d[2]==4 && d.size()==3;
  d.pop_back();
  d.pop_back();
  bool b7 = d[0]==2 && d.size()==1;
  d.pop_back();
  bool b8 = d.empty();
  return b1 && b2 && b3 && b4 && b5 && b6 && b7 && b8;
}

template <typename D>
constexpr bool deque_test2()
{
  D d;
  d.push_front(2);
  d.push_front(1);
  d.push_back(3);
  auto it = d.begin();
  bool b1 = *it==1;
  ++it;
  bool b2 = *it==2;
  ++it;
  bool b3 = *it==3;
  return b1 && b2 && b3;
}

void deque_tests()
{
#if CONSTEXPR_CEST == 1
  static_assert(deque_test1<cest::deque<int>>());
#endif

  assert((deque_test1< std::deque<int>>()));
  assert((deque_test1<cest::deque<int>>()));
  assert((deque_test2< std::deque<int>>()));
//  assert((deque_test2<cest::deque<int>>()));
}

#endif //  _CEST_DEQUE_TESTS_HPP_
