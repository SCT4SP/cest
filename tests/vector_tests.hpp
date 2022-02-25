#ifndef _CEST_VECTOR_TESTS_HPP_
#define _CEST_VECTOR_TESTS_HPP_

#include "../tests/tests_util.hpp"
#include "cest/memory.hpp"
#include "cest/vector.hpp"
#include <cassert>
#include <memory>
#include <vector>

namespace v_tests {

template <typename V> constexpr bool vec_test0() {
  V v;
  return 0 == v.size();
}

template <typename V> constexpr bool vec_test1() {
  V v;
  v.push_back(123);
  return 123 == *v.begin() && !v.empty() && 1 == v.size() && 1 == v.capacity();
}

template <typename V> constexpr bool vec_test2() {
  V v;
  typename V::value_type d = 2 * 3.142;
  v.push_back(3.142);
  v.push_back(d);
  return 2 == v.size() && 2 == v.capacity();
}

template <typename V> constexpr bool vec_test4() {
  V v;
  v.push_back(3.142f);
  v.push_back(3.142f);
  v.push_back(3.142f);
  bool b = 3 == v.size();
  auto c1 = v.capacity();
  v.clear();
  auto c2 = v.capacity();
  return b && v.empty() && c2 == c1;
}

template <typename V> constexpr bool vec_test5() {
  V v;
  v.clear();
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  using int_t = typename V::value_type;
  int_t sum = 0;
  for (auto it = v.begin(); it != v.end(); it++)
    sum += *it;
  int_t sum2 = v.at(0) + v.at(1) + v.at(2);
  return 6 == sum && 6 == sum2;
}

template <typename V> constexpr bool vec_test6() {
  V v;
  v.push_back(3.142);
  v.push_back(3.142);
  v.pop_back();
  v.pop_back();
  return v.empty() && 0 == v.size() && 2 == v.capacity();
}

template <typename V> constexpr bool vec_test7() {
  V v = {1, 2, 3, 4, 5, 6};
  auto it_erase = v.erase(v.begin() + 2, v.begin() + 4); // remove 3 & 4
  using int_t = typename V::value_type;
  int_t sum1 = 0;
  for (auto it = v.begin(); it != it_erase; it++) // 1 + 2
    sum1 += *it;
  int_t sum2 = 0;
  using cit_t = typename V::const_iterator;
  for (cit_t it = v.cbegin(); it != it_erase; ++it) // 1 + 2
    sum2 += *it;
  return !v.empty() && 4 == v.size() && 3 == sum1 && 3 == sum2;
}

// tests reserve
template <typename V> constexpr bool vec_test8() {
  V v;
  auto sz1 = v.size();
  auto c1 = v.capacity();
  v.reserve(3);
  auto sz2 = v.size();
  auto c2 = v.capacity();
  return 0 == sz1 && 0 == sz2 && c2 > c1;
}

template <typename V> constexpr bool vec_test9() {
  V v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  bool b1 = 3 == *v.rbegin() && 1 == *(v.rend() - 1);
  bool b2 = 1 == v.front() && 3 == v.back();
  bool b3 = v == v;
  return b1 && b2 && b3;
}

// copy ctor and operator=
template <typename V> constexpr bool vec_test10() {
  V v1, v2;
  v1.push_back(42);
  v2 = v1;
  V v3 = v1;
  bool b1 = v2[0] == v1[0];
  bool b2 = v3[0] == v1[0];
  v1.push_back(43);
  v1.push_back(44);
  v2[0] = 123;
  bool b3 = 3 == v1.size();
  v1 = v2;
  bool b4 = 1 == v1.size() && 123 == v1[0];
  return b1 && b2 && b3 && b4;
}

template <typename V> constexpr bool vec_test11() {
  V v;
  tests_util::Bar<> f(42);
  v.push_back(f);
  v.push_back(f);
  v.erase(v.begin(), v.begin() + 1);
  bool b1 = 1 == v.size();

  V v2;
  v2.resize(16);
  bool b2 = 16 == v2.size();

  v2.resize(7);
  bool b3 = 7 == v2.size();

  return b1 && b2 && b3;
}

template <typename T> struct my_allocator : std::allocator<T> {};

template <typename V> constexpr bool vec_test12() {
  V v(16);
  return 16 == v.size();
}

template <bool SA, class V0, class V1, class V2, class V3, class V4, class V5,
          class V6, class V7, class V8, class V9, class V10, class V11,
          class V12>
constexpr void doit() {
  using namespace tests_util;

  assert(vec_test0<V0>());
  assert(vec_test1<V1>());
  assert(vec_test2<V2>());
  assert(push_back_dtor_test<V3>());
  assert(vec_test4<V4>());
  assert(vec_test5<V5>());
  assert(vec_test6<V6>());
  assert(vec_test7<V7>());
  assert(vec_test8<V8>());
  assert(vec_test9<V9>());
  assert(vec_test10<V10>());
  assert(vec_test11<V11>());
  assert(vec_test12<V12>());

  if constexpr (SA) {
    static_assert(vec_test0<V0>());
    static_assert(vec_test1<V1>());
    static_assert(vec_test2<V2>());
    static_assert(push_back_dtor_test<V3>());
    static_assert(vec_test4<V4>());
    static_assert(vec_test5<V5>());
    static_assert(vec_test6<V6>());
    static_assert(vec_test7<V7>());
    static_assert(vec_test8<V8>());
    static_assert(vec_test9<V9>());
    static_assert(vec_test10<V10>());
    static_assert(vec_test11<V11>());
    static_assert(vec_test12<V12>());
  }
}

template <bool SA, template <class...> class Vt, typename T>
constexpr void tests_helper() {
  using tests_util::Bar;

  using V0 = Vt<double>;
  using V1 = Vt<int>;
  using V2 = Vt<double>;
  using V3 = Vt<Bar<false>>;
  using V4 = Vt<float>;
  using V5 = Vt<int>;
  using V6 = Vt<double>;
  using V7 = Vt<int>;
  using V8 = Vt<T>;
  using V9 = Vt<int>;
  using V10 = Vt<int>;
  using V11 = Vt<Bar<>>;
  using V12 = Vt<Bar<>, my_allocator<Bar<>>>;

  doit<SA, V0, V1, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12>();
}

} // namespace v_tests

void vector_tests() {
  using namespace v_tests;

  // true: constexpr tests        false: no constexpr tests
  tests_helper<false, std::vector, std::unique_ptr<int>>();
  tests_helper<CONSTEXPR_CEST, cest::vector, cest::unique_ptr<int>>();
}

#endif // _CEST_VECTOR_TESTS_HPP_
