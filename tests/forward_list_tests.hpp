#ifndef _CEST_FORWARD_LIST_TESTS_HPP_
#define _CEST_FORWARD_LIST_TESTS_HPP_

#include "cest/forward_list.hpp"
#include <forward_list>
#include <cassert>

template <template <typename...> typename FL>
constexpr auto forward_list_test1() {
  FL<int> l;
  return 0;
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
  l.push_front(1);
  l.push_front(2);
  l.push_front(3);
  int sum1 = 0, sum2 = 0;
  for (auto it = l.begin(); it != l.end(); it++)
    sum1 += *it;
// todo
//  using cit_t = typename FL<int>::const_iterator;
//  for (cit_t it = l.cbegin(); it != l.end(); ++it)  // 1 + 2 + 3
//    sum2 += *it;
  return std::tuple{sum1,6};
}

template <template <typename...> typename FL>
constexpr void rt_forward_list_tests() {
         assert(forward_list_test1<FL>() == 0);
         assert(forward_list_test2<FL>() == 42);
         assert(forward_list_test3<FL>() == 1);
         assert(forward_list_test4<FL>() == (std::tuple{6,6}));
}

template <template <typename...> typename FL>
constexpr void ct_forward_list_tests() {
#ifndef NO_STATIC_TESTS
  static_assert(forward_list_test1<FL>() == 0);
  static_assert(forward_list_test2<FL>() == 42);
  static_assert(forward_list_test3<FL>() == 1);
  static_assert(forward_list_test4<FL>() == std::tuple{6,6});
#endif
}

void forward_list_tests()
{
  ct_forward_list_tests<cest::forward_list>();
  rt_forward_list_tests<cest::forward_list>();
  rt_forward_list_tests<std::forward_list>();
}

#endif // _CEST_FORWARD_LIST_TESTS_HPP_
