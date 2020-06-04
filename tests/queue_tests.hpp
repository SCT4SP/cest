#ifndef _CEST_QUEUE_TESTS_HPP_
#define _CEST_QUEUE_TESTS_HPP_

#include "cest/queue.hpp"
#include "cest/list.hpp"
//#include <deque>
#include <queue>
#include <cassert>

template <typename Q>
constexpr bool queue_test1()
{
  Q q;
  bool b0  = q.empty();
  auto sz0 = q.size();
  const int i{42};

  q.push(i);  // add 42 to the back
  auto ef1 = q.front();
  auto eb1 = q.back();

  q.push(43); // add 43 to the back
  auto ef2 = q.front();
  auto eb2 = q.back();
  auto sz2 = q.size();

  q.pop();    // remove 42 from the front
  auto ef3 = q.front();
  auto eb3 = q.back();

  q.pop();    // remove 43 from the front
  bool b4  = q.empty();

  return b0 && sz0==0 && ef1==42 && eb1==42 && ef2==42 && eb2==43 && sz2==2 &&
                                               ef3==43 && eb3==43 && b4;
}

void queue_tests()
{
#ifndef NO_STATIC_TESTS
  static_assert(queue_test1<cest::queue<int,cest::list<int>>>());
#endif

  assert((queue_test1< std::queue<int,cest::list<int>>>()));
  assert((queue_test1<cest::queue<int,cest::list<int>>>()));
}
#endif // _CEST_QUEUE_TESTS_HPP_
