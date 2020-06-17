#ifndef _CEST_VECTOR_TESTS_HPP_
#define _CEST_VECTOR_TESTS_HPP_

#include "cest/vector.hpp"
#include "cest/mono_block_alloc.hpp"
#include <cassert>
#include <vector>

template <typename V>
constexpr bool vec_test0() {
  V v;
  return 0==v.size();
}

template <typename V>
constexpr bool vec_test1() {
  V v;
  v.push_back(123);
  return 123==*v.begin() && !v.empty() && 1==v.size() && 1==v.capacity();
}

template <typename V>
constexpr bool vec_test2() {
  V v;
  typename V::value_type d = 2 * 3.142;
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

template <typename V>
constexpr bool vec_test3() {
  V v;
  Bar f(42);
  v.push_back(f);
  v.push_back(f);
  return 42==v.begin()->m_x && 2==v.size() && 2==v.capacity();
}

template <typename V>
constexpr bool vec_test4() {
  V v;
  v.push_back(3.142f);
  v.push_back(3.142f);
  v.push_back(3.142f);
  return 3==v.size() && 4==v.capacity();
}

template <typename V>
constexpr bool vec_test5() {
  V v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  using int_t = typename V::value_type;
  int_t sum = 0;
  for (auto it = v.begin(); it != v.end(); it++)
    sum += *it;
  return 6==sum;
}

template <typename V>
constexpr bool vec_test6() {
  V v;
  v.push_back(3.142);
  v.push_back(3.142);
  v.pop_back();
  v.pop_back();
  return v.empty() && 0==v.size() && 2==v.capacity();
}

template <typename V>
constexpr bool vec_test7() {
  V v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);
  v.push_back(6);
  auto it_erase = v.erase(v.begin()+2,v.begin()+4); // remove 3 & 4
  using int_t = typename V::value_type;
  int_t sum1 = 0;
  for (auto it = v.begin(); it != it_erase; it++)  // 1 + 2
    sum1 += *it;
  int_t sum2 = 0;
  using cit_t = typename V::const_iterator;
  for (cit_t it = v.cbegin(); it != it_erase; ++it)  // 1 + 2
    sum2 += *it;
  return !v.empty() && 4==v.size() && 3==sum1 && 3==sum2;
}

// tests reserve
template <typename V>
constexpr bool vec_test8() {
  V v;
  auto sz0 = v.size();
  v.reserve(3);
  auto sz1 = v.size();
  v[0] = 1;
  v[1] = 2;
  v[2] = 3;
  auto sz2 = v.size();
  auto sum = v[0] + v[1] + v[2];
  return 0==sz0 && 0==sz1 && 0==sz2 && 6==sum;
}

template <bool SA, class V0, class V1, class V2, class V3,
                   class V4, class V5, class V6, class V7, class V8>
constexpr bool doit()
{
  assert(vec_test0<V0>());
  assert(vec_test1<V1>());
  assert(vec_test2<V2>());
  assert(vec_test3<V3>());
  assert(vec_test4<V4>());
  assert(vec_test5<V5>());
  assert(vec_test6<V6>());
  assert(vec_test7<V7>());
  assert(vec_test8<V8>());

  if constexpr (SA) {
#ifndef NO_STATIC_TESTS
    static_assert(vec_test0<V0>());
    static_assert(vec_test1<V1>());
    static_assert(vec_test1<V2>());
    static_assert(vec_test3<V3>());
    static_assert(vec_test4<V4>());
    static_assert(vec_test5<V5>());
    static_assert(vec_test6<V6>());
    static_assert(vec_test7<V7>());
    static_assert(vec_test8<V8>());
#endif
  }

  return true;
}

template <bool SA, template <class...> class Vt>
constexpr void vector_tests_helper()
{
  using V0  = Vt<double>;
  using V1  = Vt<int>;
  using V2  = Vt<double>;
  using V3  = Vt<Bar>;
  using V4  = Vt<float>;
  using V5  = Vt<int>;
  using V6  = Vt<double>;
  using V7  = Vt<int>;
  using V8  = Vt<int>;

  using Va0 = Vt<double, cea::mono_block_alloc<double>>;
  using Va1 = Vt<int,    cea::mono_block_alloc<int>>;
  using Va2 = Vt<double, cea::mono_block_alloc<double>>;
  using Va3 = Vt<Bar,    cea::mono_block_alloc<Bar>>;
  using Va4 = Vt<float,  cea::mono_block_alloc<float>>;
  using Va5 = Vt<int,    cea::mono_block_alloc<int>>;
  using Va6 = Vt<double, cea::mono_block_alloc<double>>;
  using Va7 = Vt<int,    cea::mono_block_alloc<int>>;
  using Va8 = Vt<int,    cea::mono_block_alloc<int>>;

  doit<SA, V0,  V1,  V2,  V3,  V4,  V5,  V6,  V7,  V8>();
  doit<SA, Va0, Va1, Va2, Va3, Va4, Va5, Va6, Va7, Va8>();
}

void vector_tests() {
  vector_tests_helper<true,cest::vector>();
#ifdef USE_CONSTEXPR_STDLIB
  vector_tests_helper<true,std::vector>();  // true: constexpr tests
#else
  vector_tests_helper<false,std::vector>(); // false: no constexpr tests
#endif
}

#endif // _CEST_VECTOR_TESTS_HPP_
