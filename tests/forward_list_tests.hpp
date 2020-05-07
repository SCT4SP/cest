#ifndef _CEST_FORWARD_LIST_TESTS_HPP_
#define _CEST_FORWARD_LIST_TESTS_HPP_

#include "cest/forward_list.hpp"
#include <forward_list>
#include <cassert>

template <template <typename...> typename FL>
constexpr auto forward_list_test1() {
  FL<int> m;
  return 0;
}

template <template <typename...> typename FL>
constexpr auto forward_list_test2() {
  FL<int> m;
  m.push_front(42);
  return m.front();
}

template <template <typename...> typename FL>
constexpr auto forward_list_test3() {
  FL<int> m;
  m.push_front(2);
  m.push_front(1);
  return m.front();
}

template <template <typename...> typename FL>
constexpr auto forward_list_test4() {
  FL<int> m;
  m.push_front(1);
  m.push_front(2);
  m.push_front(3);
  int sum1 = 0, sum2 = 0;
  for (auto it = m.begin(); it != m.end(); it++)
    sum1 += *it;
  for (auto it = m.cbegin(); it != m.end(); ++it)
    sum2 += *it;
  return std::tuple{sum1,sum2};
}

template <template <typename...> typename FL>
constexpr auto rt_forward_list_tests() {
         assert(forward_list_test1<FL>() == 0);
         assert(forward_list_test2<FL>() == 42);
         assert(forward_list_test3<FL>() == 1);
         assert(forward_list_test4<FL>() == (std::tuple{6,6}));
}

template <template <typename...> typename FL>
constexpr auto ct_forward_list_tests() {
  static_assert(forward_list_test1<FL>() == 0);
  static_assert(forward_list_test2<FL>() == 42);
  static_assert(forward_list_test3<FL>() == 1);
  static_assert(forward_list_test4<FL>() == std::tuple{6,6});
}

void forward_list_tests()
{
  ct_forward_list_tests<cest::forward_list>();
  rt_forward_list_tests<cest::forward_list>();
  rt_forward_list_tests<std::forward_list>();
}

#endif // _CEST_FORWARD_LIST_TESTS_HPP_
