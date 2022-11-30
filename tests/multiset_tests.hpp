#ifndef _CEST_MULTISET_TESTS_HPP_
#define _CEST_MULTISET_TESTS_HPP_

#include "cest/set.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <tuple>
#include <type_traits>

namespace multiset_tests_ns {

constexpr bool common_static_multiset_tests() {
  auto f = []<template <class...> class MS>() {
    static_assert(sizeof(MS<float>) == sizeof(MS<double>));
#if !defined(_LIBCPP_VERSION) && !defined(__clang__)
    static_assert(std::weakly_incrementable<typename MS<int>::iterator>);
#endif
    using iter_t = typename MS<int>::iterator;
    using const_iter_t = typename MS<int>::const_iterator;
    static_assert(std::is_same_v<typename iter_t::iterator_category,
                                 std::bidirectional_iterator_tag>);
    static_assert(std::is_same_v<iter_t, const_iter_t>);
  };

  f.operator()<std::multiset>();
  f.operator()<cest::multiset>();

  return true;
}

template <typename MS> constexpr bool multiset_test1() {
  MS ms;

  bool bs  = 0 == ms.size();
  auto r1 = ms.insert(3); // multiset size increased: insert done
  bs = bs && 1 == ms.size();
  auto r2 = ms.insert(3); // multiset size increased: insert done
  bs = bs && 2 == ms.size();
  return r1 != ms.end() && 3 == *r1 && r2 != ms.end() && 3 == *r2 && bs;
}

template <typename MS, typename T> constexpr auto multiset_test3(T x, T y, T z) {
  MS ms;

  auto r1 = ms.insert(x);
  auto r2 = ms.insert(y);
  auto r3 = ms.insert(z);

  return std::tuple{ms.size(), *r1, *r2, *r3};
}

template <class MS, class T> constexpr MS &inserts(MS &ms, T x) {
  ms.insert(x);
  return ms;
}

template <class MS, class T, class... Ts>
constexpr MS &inserts(MS &ms, T x, Ts... xs) {
  ms.insert(x);
  return inserts(ms, xs...);
}

template <typename MS, typename T>
constexpr bool multiset_test4(T x1, T x2, T x3, T x4, T x5) {
  MS ms1, ms2, ms3;
  ms1.insert(x1);
  ms1.insert(x2);
  ms1.insert(x3);
  ms1.insert(x4);
  ms1.insert(x5);
  ms2.insert(x5);
  ms2.insert(x4);
  ms2.insert(x3);
  ms2.insert(x2);
  ms2.insert(x1);
  ms3.insert(x5);
  ms3.insert(x1);
  ms3.insert(x4);
  ms3.insert(x2);
  ms3.insert(x3);

  return 5 == ms1.size() && 5 == ms2.size() && 5 == ms3.size();
}

template <typename MS, typename T> constexpr bool multiset_test5(T x) {
  MS ms;

  auto ib0 = ms.begin();
  auto ie0 = ms.end();
  bool b0 = ib0 == ie0;

  ms.insert(x);
  auto ib1 = ms.begin();
  bool b1 = ib1 == ie0;
  T a = *ib1;
  ms.insert(x - 1);
  auto ib2 = ms.begin();
  bool b2 = ib2 == ie0;
  T b = *ib2;
  ms.insert(x + 1);
  auto ib3 = ms.begin();
  bool b3 = ib3 == ie0;
  T c = *ib3;

  return b0 && !b1 && !b2 && !b3 && 42 == a && 41 == b && 41 == c &&
         3 == ms.size();
}

// test pre-increment (and insert, begin and end)
template <typename MS, typename T, typename... Ts>
constexpr bool multiset_test6(T x, Ts... xs) {
  MS ms;

  ms.insert(x);
  auto it = ms.begin();
  ++it;
  bool r0 = it == ms.end();

  inserts(ms, xs...);

  auto it1 = ms.insert(6);
  auto a1 = *it1;
  ++it1; // 6 -> 8
  auto b1 = *it1;

  auto it12 = ms.insert(8);
  auto a12 = *it12;
  ++it12; // 8 -> 11
  auto b12 = *it12;

  auto it2 = ms.insert(15);
  auto a2 = *it2;
  ++it2; // 15 -> 17
  auto b2 = *it2;

  auto it3 = ms.insert(1);
  auto a3 = *it3;
  ++it3; // 1 -> 6
  auto b3 = *it3;

  auto it4 = ms.insert(11);
  auto a4 = *it4;
  ++it4; // 11 -> 13
  auto b4 = *it4;

  auto it5 = ms.insert(27);
  auto a5 = *it5;
  ++it5; // 27 -> end
  auto r6 = it5 == ms.end();

  T sum{0};
  T prev = std::numeric_limits<int>::min();
  bool inc = true; // increasing
  for (auto it = ms.begin(); it != ms.end(); ++it) {
    inc = inc && (*it >= prev);
    prev = *it;
    sum += prev;
  }

  return r0 && 6 == a1 && 8 == b1 && 8 == a12 && 11 == b12 && 15 == a2 &&
         17 == b2 && 1 == a3 && 6 == b3 && 11 == a4 && 13 == b4 && 27 == a5 &&
         r6 && 213 == sum && inc && 16 == ms.size();
}

// tests post-increment
template <typename MS> constexpr bool multiset_test7() {
  MS ms;
  inserts(ms, 1, 5, 4, 2, 3);
  auto it0 = ms.begin();
  auto it1 = it0++;
  return 2 == *it0 && 1 == *it1;
}

namespace test9 {
struct FatKey {
  int x;
  int data[1000];
};
struct LightKey {
  int x;
};
constexpr bool operator<(const FatKey &fk, const LightKey &lk) {
  return fk.x < lk.x;
}
constexpr bool operator<(const LightKey &lk, const FatKey &fk) {
  return lk.x < fk.x;
}
constexpr bool operator<(const FatKey &fk1, const FatKey &fk2) {
  return fk1.x < fk2.x;
}
} // namespace test9

// tests find
template <typename MS> constexpr bool multiset_test8() {
  MS ms;
  inserts(ms, 1, 2, 3, 4);
  auto it = ms.find(2);
  bool ok = it != ms.end();
  return ok;
}

template <typename MS> constexpr bool multiset_test9() {
  using namespace test9;

  LightKey lk = {2};

  MS ms;
  FatKey fk{2, {}};
  inserts(ms, FatKey{1, {}}, FatKey{2, {}}, FatKey{3, {}}, FatKey{4, {}});
  auto it = ms.find(lk);  // The C++14 template version of find
  auto itf = ms.find(fk); // ""
  int x = it->x;
  int xf = itf->x;
  bool ok = it != ms.end();

  auto it2 = ++it;
  auto x2 = it2->x;
  auto it3 = it2++;
  auto x3 = it3->x;

  return ok && 2 == x && 2 == xf && 3 == x2 && 3 == x3;
}

template <typename MS> constexpr bool multiset_test10() {
  MS ms1;
  inserts(ms1, 1, 2, 3);
  const MS ms2 = ms1;
  MS ms3, ms4;
  inserts(ms3, 1, 2, 3, 4, 5);
  ms4 = ms3;
  ms3 = ms1;
  MS ms5 = ms1;
  ms5.clear();

  return 3 == ms1.size() && 3 == ms2.size() && 3 == ms3.size() &&
         5 == ms4.size() && ms5.empty();
}

template <bool SA, class S1, class S2, class S3, class S4, class S5, class S6,
          class S7, class S8, class S9, class S10>
constexpr void doit() {
  constexpr const auto tup3 = std::tuple{3, 3, 2, 1};
  constexpr const auto tup4 = std::tuple{3, 1, 2, 3};
  constexpr const auto tup5 = std::tuple{3, 1, 3, 2};
  constexpr const auto tup6 = std::tuple{3, 1, 2, 2};

  assert(multiset_test1<S1>());
  assert(multiset_test3<S3>(3, 2, 1) == tup3);
  assert(multiset_test3<S3>(1, 2, 3) == tup4);
  assert(multiset_test3<S3>(1, 3, 2) == tup5);
  assert(multiset_test3<S3>(1, 2, 2) == tup6);
  assert(multiset_test4<S4>(1, 2, 3, 4, 5));
  assert(multiset_test5<S5>(42));
  assert(multiset_test6<S6>(1, 6, 8, 11, 13, 15, 17, 22, 25, 27));
  assert(multiset_test6<S6>(27, 25, 22, 17, 15, 13, 11, 8, 6, 1));
  assert(multiset_test6<S6>(1, 27, 6, 25, 8, 22, 11, 17, 13, 15));
  assert(multiset_test7<S7>());
  assert(multiset_test8<S8>());
  assert(multiset_test9<S9>());
  assert(multiset_test10<S10>());

  if constexpr (SA) {
#if CONSTEXPR_CEST == 1
    static_assert(multiset_test1<S1>());
    static_assert(multiset_test3<S3>(3, 2, 1) == tup3);
    static_assert(multiset_test3<S3>(1, 2, 3) == tup4);
    static_assert(multiset_test3<S3>(1, 3, 2) == tup5);
    static_assert(multiset_test3<S3>(1, 2, 2) == tup6);
    static_assert(multiset_test4<S4>(1, 2, 3, 4, 5));
    static_assert(multiset_test5<S5>(42));
    static_assert(multiset_test6<S6>(1, 6, 8, 11, 13, 15, 17, 22, 25, 27));
    static_assert(multiset_test6<S6>(27, 25, 22, 17, 15, 13, 11, 8, 6, 1));
    static_assert(multiset_test6<S6>(1, 27, 6, 25, 8, 22, 11, 17, 13, 15));
    static_assert(multiset_test7<S7>());
    static_assert(multiset_test8<S8>());
    static_assert(multiset_test9<S9>());
    static_assert(multiset_test10<S10>());
#endif
  }
}

template <bool SA, template <class...> class St,
          template <class> class Alloc = std::allocator>
constexpr void tests_helper() {
  using S1 = St<int, std::less<int>, Alloc<int>>;
  using S2 = St<int, std::less<int>, Alloc<int>>;
  using S3 = St<int, std::less<int>, Alloc<int>>;
  using S4 = St<int, std::less<int>, Alloc<int>>;
  using S5 = St<int, std::less<int>, Alloc<int>>;
  using S6 = St<int, std::less<int>, Alloc<int>>;
  using S7 = St<int, std::less<int>, Alloc<int>>;
  using S8 = St<int, std::less<int>, Alloc<int>>;
  using S9 = St<test9::FatKey, std::less<>>;
  using S10 = St<int, std::less<int>, Alloc<int>>;

  doit<SA, S1, S2, S3, S4, S5, S6, S7, S8, S9, S10>();
}

} // namespace multiset_tests_ns

void multiset_tests() {
  multiset_tests_ns::tests_helper<false, std::multiset>();
  multiset_tests_ns::tests_helper<CONSTEXPR_CEST, cest::multiset>();

#if CONSTEXPR_CEST == 1
  static_assert(multiset_tests_ns::common_static_multiset_tests());
#endif
}

#endif // _CEST_MULTISET_TESTS_HPP_
