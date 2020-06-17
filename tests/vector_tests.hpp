#ifndef _CEST_VECTOR_TESTS_HPP_
#define _CEST_VECTOR_TESTS_HPP_

#include "cest/vector.hpp"
#include <cassert>
#include <vector>

template <template <typename> typename V>
constexpr bool vec_test0() {
  V<double> v;
  return 0==v.size();
}

template <template <typename> typename V>
constexpr bool vec_test1() {
  V<int> v;
  v.push_back(123);
  return 123==*v.begin() && !v.empty() && 1==v.size() && 1==v.capacity();
}

template <template <typename> typename V>
constexpr bool vec_test2() {
  V<double> v;
  double d = 2 * 3.142;
  v.push_back(3.142);
  v.push_back(d);
  return 2==v.size() && 2==v.capacity();
}

struct Bar {
  constexpr Bar()             : m_x(42)     { }
  constexpr Bar(int x)        : m_x(x)      { }
  constexpr Bar(const Bar &f) : m_x(f.m_x)  { }
  constexpr ~Bar()                          { }
  int m_x;
};

template <template <typename> typename V>
constexpr bool vec_test2b() {
  V<Bar> v;
  Bar f(42);
  v.push_back(f);
  v.push_back(f);
  return 42==v.begin()->m_x && 2==v.size() && 2==v.capacity();
}

template <template <typename> typename V>
constexpr bool vec_test3() {
  V<float> v;
  v.push_back(3.142f);
  v.push_back(3.142f);
  v.push_back(3.142f);
  return 3==v.size() && 4==v.capacity();
}

template <template <typename> typename V>
constexpr bool vec_test4() {
  V<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  int sum = 0;
  for (auto it = v.begin(); it != v.end(); it++)
    sum += *it;
  return 6==sum;
}

template <template <typename> typename V>
constexpr bool vec_test5() {
  V<double> v;
  v.push_back(3.142);
  v.push_back(3.142);
  v.pop_back();
  v.pop_back();
  return v.empty() && 0==v.size() && 2==v.capacity();
}

template <template <typename> typename V>
constexpr bool vec_test6() {
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
  using cit_t = typename V<int>::const_iterator;
  for (cit_t it = v.cbegin(); it != it_erase; ++it)  // 1 + 2
    sum2 += *it;
  return !v.empty() && 4==v.size() && 3==sum1 && 3==sum2;
}

// tests reserve
template <template <typename> typename V>
constexpr bool vec_test7() {
  V<int> v;
  auto sz0 = v.size();
  v.reserve(3);
  auto sz1 = v.size();
  v[0] = 1;
  v[1] = 2;
  v[2] = 3;
  auto sz2 = v.size();
  int sum = v[0] + v[1] + v[2];
  return 0==sz0 && 0==sz1 && 0==sz2 && 6==sum;
}

template <template <typename...> typename V>
constexpr void rt_vector_tests() {
  assert(vec_test0<V>());
  assert(vec_test1<V>());
  assert(vec_test2<V>());
  assert(vec_test2b<V>());
  assert(vec_test3<V>());
  assert(vec_test4<V>());
  assert(vec_test5<V>());
  assert(vec_test6<V>());
  assert(vec_test7<V>());
}

template <template <class...> class Cv, template <class...> class Sv>
void vector_tests_helper()
{
  using v0_t  = Cv<double>;
  using v1_t  = Cv<int>;
  using v2_t  = Cv<double>;
  using v2b_t = Cv<Bar>;
  using v3_t  = Cv<float>;
  using v4_t  = Cv<int>;
  using v5_t  = Cv<double>;
  using v6_t  = Cv<int>;

#ifndef NO_STATIC_TESTS
  static_assert(vec_test0<Cv>());
  static_assert(vec_test1<Cv>());
  static_assert(vec_test2<Cv>());
  static_assert(vec_test2b<Cv>());
  static_assert(vec_test3<Cv>());
  static_assert(vec_test4<Cv>());
  static_assert(vec_test5<Cv>());
  static_assert(vec_test6<Cv>());
  static_assert(vec_test7<Cv>());
#endif

  rt_vector_tests<Cv>();
  rt_vector_tests<Sv>();
}

void vector_tests() { vector_tests_helper<cest::vector, std::vector>(); }

#endif // _CEST_VECTOR_TESTS_HPP_
