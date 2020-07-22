#ifndef _CEST_LIST_TESTS_HPP_
#define _CEST_LIST_TESTS_HPP_

#include "cest/list.hpp"
#include <list>
#include <cassert>

template <typename L>
constexpr auto list_test1()
{
  L l;
  auto s0 = l.size();
  auto b0 = l.empty();
  const int x = 123;
  auto e0 = *l.insert(l.begin(), x);
  auto e1 = *l.insert(l.begin(), 42);
  auto e2 = *l.insert(l.end(), 43);
  auto ft  = l.front();
  auto bk  = l.back();
  auto s3  = l.size();
  auto b3  = l.empty();
  l.clear();
  auto s4  = l.size();
  auto b4  = l.empty();

  l.push_front(x);
  l.push_front(42);
  l.push_back(x);
  l.push_back(43);

  auto y0 = l.front();
  auto y1 = l.back();
  auto s  = l.size();

  bool res = s0==0 && b0 && e0==x && e1==42 && e2==43;
       res = res && ft==42 && bk==43 && s3==3 && b3==false;
       res = res && s4==0  && b4;
       res = res && y0==42 && y1==43 && s==4;
  return res;
}

template <typename L>
constexpr auto list_test2()
{
  L l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);
  l.push_back(4);
  l.push_back(5);

  int sum1 = 0;
  for (auto it = l.begin(); it != l.end(); )
  {
    sum1 += *it;
    if (*it % 2 == 0) { // delete even numbers
      it = l.erase(it);
    } else {
      ++it;
    }
  }
  
  int sum2 = 0;
  using cit_t = typename L::const_iterator;
  for (cit_t it = l.cbegin(); it != l.cend(); ++it)
    sum2 += *it;

  return sum1==15 && sum2==9 && l.size()==3;
}

template <template <typename ...> typename L>
constexpr auto list_test3()
{
  struct Foo { int i; short s; double *p; };

  L<Foo> l;
  const int i{42};
  const short s{329};
  double d{42};
  Foo f{43,43,&d};
  auto it = l.emplace(l.begin(), f);
  auto e1 = *it;
  l.emplace(it, i, s, &d); // at the front
  auto e2 = l.front();
  auto sz = l.size();
  l.emplace_front(17,1000,&d);
  l.emplace_back(99,2000,&d);
  auto e3i = l.front().i;
  auto e4i = l.back().i;
  auto sz2 = l.size();
  l.pop_back();
  l.pop_front();
  auto e5  = l.front();
  auto sz3 = l.size();

  return e1.i==f.i && e1.s==f.s && e1.p==f.p &&
         e2.i==i   && e2.s==s   && e2.p==&d  && sz==2 &&
         e3i==17   && e4i==99   && sz2==4    &&
         e5.i==i   && e5.s==s   && e5.p==&d  && sz3==2;
}

void list_tests()
{
#if CONSTEXPR_CEST == 1
  static_assert(list_test1<cest::list<int>>());
  static_assert(list_test2<cest::list<int>>());
  static_assert(list_test3<cest::list>());
#endif

  assert(list_test1< std::list<int>>());
  assert(list_test1<cest::list<int>>());
  assert(list_test2< std::list<int>>());
  assert(list_test2<cest::list<int>>());
  assert(list_test3< std::list>());
  assert(list_test3<cest::list>());
}

#endif // _CEST_LIST_TESTS_HPP_
