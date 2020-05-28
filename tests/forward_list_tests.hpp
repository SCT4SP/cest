#ifndef _CEST_FORWARD_LIST_TESTS_HPP_
#define _CEST_FORWARD_LIST_TESTS_HPP_

#include "cest/forward_list.hpp"
#include <forward_list>
#include <cassert>

template <template <typename...> typename FL>
constexpr auto forward_list_test1() {
  FL<int> l;
  return l.empty();
}

template <template <typename...> typename FL>
constexpr auto forward_list_test2() {
  FL<int> l;
  l.push_front(42);
  return l.front();
}

template <template <typename...> typename FL>
constexpr auto forward_list_test3() {
  FL<int> l;
  l.push_front(2);
  l.push_front(1);
  return l.front();
}

template <template <typename...> typename FL>
constexpr auto forward_list_test4() {
  FL<int> l;
  l.insert_after(l.before_begin(),123);  // l.begin() here is an error
  l.insert_after(l.begin(),1); // 123     -> 123 1
  l.push_front(2);             // 123 1   -> 2 123 1
  l.erase_after(l.begin());    // 2 123 1 -> 2 1
  l.insert_after(l.before_begin(),3); // same as l.push_front(3)

  int sum1 = 0, sum2 = 0;
  for (auto it = l.begin(); it != l.end(); it++)    // 3 + 2 + 1
    sum1 += *it;

  using cit_t = typename FL<int>::const_iterator;
  cit_t it = l.cbegin();
  for (; it != l.cend(); ++it) // 3 + 2 + 1
    sum2 += *it;

  bool b1 = it == l.end();
  bool b2 = l.end() == it;
  return std::tuple{l.front(),sum1,sum2,b1,b2};
}

template <template <typename...> typename FL>
constexpr auto forward_list_test5() {
  FL<int> *p1 = new FL<int>;
  FL<int> *p2 = new FL<int>;

  int a = 17, b = 18;
  p1->push_front(b);
  p1->push_front(a);

  FL<int> l(*p1); // copy ctor
  *p2 = *p1;      // copy assignment

  bool b1 = a == p1->front();
  bool b2 = a == p2->front();
  bool b3 = a ==   l.front();

  delete p1;
  delete p2;

  return std::tuple{b1&&b2&&b3};
}

template <template <typename...> typename FL>
constexpr void rt_forward_list_tests() {
         assert(forward_list_test1<FL>() == true);
         assert(forward_list_test2<FL>() == 42);
         assert(forward_list_test3<FL>() == 1);
         assert(forward_list_test4<FL>() == (std::tuple{3,6,6,true,true}));
         assert(forward_list_test5<FL>() == (std::tuple{true}));
}

template <template <typename...> typename FL>
constexpr void ct_forward_list_tests() {
#ifndef NO_STATIC_TESTS
  static_assert(forward_list_test1<FL>() == true);
  static_assert(forward_list_test2<FL>() == 42);
  static_assert(forward_list_test3<FL>() == 1);
  static_assert(forward_list_test4<FL>() == std::tuple{3,6,6,true,true});
  static_assert(forward_list_test5<FL>() == std::tuple{true});
#endif
}

void forward_list_tests()
{
  ct_forward_list_tests<cest::forward_list>();
  rt_forward_list_tests<std::forward_list>();
  rt_forward_list_tests<cest::forward_list>();
}

#endif // _CEST_FORWARD_LIST_TESTS_HPP_
