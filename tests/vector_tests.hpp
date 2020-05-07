#ifndef _CEST_VECTOR_TESTS_HPP_
#define _CEST_VECTOR_TESTS_HPP_

#include "cest/vector.hpp"
#include <cassert>
#include <vector>

template <template <typename> typename V>
constexpr auto vec_test0() {
  V<double> v;
  return v.size();
}

template <template <typename> typename V>
constexpr auto vec_test1() {
  V<int> v;
  v.push_back(123);
  return std::tuple{*v.begin(),v.empty(),v.size(),v.capacity()};
}

template <template <typename> typename V>
constexpr auto vec_test2() {
  V<double> v;
  double d = 2 * 3.142;
  v.push_back(3.142);
  v.push_back(d);
  return std::tuple{v.size(),v.capacity()};
}

struct Bar {
  constexpr Bar()             : m_x(42)     { }
  constexpr Bar(int x)        : m_x(x)      { }
  constexpr Bar(const Bar &f) : m_x(f.m_x)  { }
  constexpr ~Bar()                          { }
  int m_x;
};

template <template <typename> typename V>
constexpr auto vec_test2b() {
  V<Bar> v;
  Bar f(42);
  v.push_back(f);
  v.push_back(f);
  return std::tuple{v.begin()->m_x,v.size(),v.capacity()};
}

template <template <typename> typename V>
constexpr auto vec_test3() {
  V<float> v;
  v.push_back(3.142f);
  v.push_back(3.142f);
  v.push_back(3.142f);
  return std::tuple{v.size(),v.capacity()};
}

template <template <typename> typename V>
constexpr auto vec_test4() {
  V<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  int sum = 0;
  for (auto it = v.begin(); it != v.end(); it++)
    sum += *it;
  return sum;
}

template <template <typename> typename V>
constexpr auto vec_test5() {
  V<double> v;
  v.push_back(3.142);
  v.push_back(3.142);
  v.pop_back();
  v.pop_back();
  return std::tuple{v.empty(),v.size(),v.capacity()};
}

template <template <typename> typename V>
constexpr auto vec_test6() {
  V<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);
  v.push_back(6);
  auto it_erase = v.erase(v.begin()+2,v.begin()+4); // remove 3 & 4
  int sum1 = 0;
  for (auto it = v.begin(); it != it_erase; it++)  // 1 + 2
    sum1 += *it;
  int sum2 = 0;
  for (auto it = v.cbegin(); it != it_erase; ++it)  // 1 + 2
    sum2 += *it;
  return std::tuple{v.empty(),v.size(),sum1,sum2};
}

// tests reserve
template <template <typename> typename V>
constexpr auto vec_test7() {
  V<int> v;
  auto sz0 = v.size();
  v.reserve(3);
  auto sz1 = v.size();
  v[0] = 1;
  v[1] = 2;
  v[2] = 3;
  auto sz2 = v.size();
  int sum = v[0] + v[1] + v[2];
  return std::tuple{sz0,sz1,sz2,sum};
}

template <template <typename...> typename V>
constexpr void rt_vector_tests() {
  assert(vec_test0<V>()      == 0);
  assert(vec_test1<V>()      == (std::tuple{123,false,1,1}));
  assert(vec_test2<V>()      == (std::tuple{2,2}));
  assert(vec_test2b<V>()     == (std::tuple{42,2,2}));
  assert(vec_test3<V>()      == (std::tuple{3,4}));
  assert(vec_test4<V>()      == 6);
  assert(vec_test5<V>()      == (std::tuple{true,0,2}));
  assert(vec_test6<V>()      == (std::tuple{false,4,3,3}));
  assert(vec_test7<V>()      == (std::tuple{0,0,0,6}));
}

void vector_tests()
{
#ifndef NO_STATIC_TESTS
  static_assert(vec_test0<cest::vector>() == 0);
  static_assert(vec_test1<cest::vector>() == std::tuple{123,false,1,1});
  static_assert(vec_test2<cest::vector>() == std::tuple{2,2});
  static_assert(vec_test2b<cest::vector>() == (std::tuple{42,2,2}));
  static_assert(vec_test3<cest::vector>() == std::tuple{3,4});
  static_assert(vec_test4<cest::vector>() == 6);
  static_assert(vec_test5<cest::vector>() == std::tuple{true,0,2});
  static_assert(vec_test6<cest::vector>() == std::tuple{false,4,3,3});
  static_assert(vec_test7<cest::vector>() == std::tuple{0,0,0,6});
#endif

  rt_vector_tests<cest::vector>();
  rt_vector_tests<std::vector>();
}

#endif // _CEST_VECTOR_TESTS_HPP_
