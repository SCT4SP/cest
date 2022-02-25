#ifndef _CEST_ALGORITHM_TESTS_HPP_
#define _CEST_ALGORITHM_TESTS_HPP_

#include "cest/array.hpp"
#include "cest/forward_list.hpp"
#include "cest/list.hpp"
#include "cest/set.hpp"
#include "cest/vector.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <forward_list>
#include <iterator>
#include <list>
#include <numeric>
#include <set>
#include <vector>

template <typename V, typename S> constexpr bool algorithm_test1() {
  using namespace std;
  using arr_t = array<int, 5>;
  arr_t a{1, 2, 3, 4, 5};
  arr_t b{3, 4, 5, 6, 7};
  arr_t res;

  auto end =
      set_intersection(a.begin(), a.end(), b.begin(), b.end(), res.begin());
  auto sum = accumulate(res.begin(), end, 0);

  V v1, v2, vres;
  copy(a.begin(), a.end(), back_inserter(v1)); // std::copy works constexpr
  copy(b.begin(), b.end(), back_inserter(v2)); // ""
  auto vend = set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
                               back_inserter(vres));
  auto vsum = accumulate(vres.begin(), vres.end(), 0);

  S s1, s2, sres;
  copy(a.begin(), a.end(), inserter(s1, s1.end()));
  copy(b.begin(), b.end(), inserter(s2, s2.end()));
  auto send = set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(),
                               inserter(sres, sres.end()));
  auto ssum = accumulate(sres.begin(), sres.end(), 0);

  *vend;                                        // no-op
  *send;                                        // no-op
  return sum == 12 && vsum == 12 && ssum == 12; // 3+4+5 == 12
}

template <typename V, typename S> constexpr bool algorithm_test2() {
  using namespace std;
  using arr_t = array<int, 5>;
  arr_t a{1, 2, 3, 4, 5};

  auto e = *find(a.begin(), a.end(), 4);            //     found
  bool b = find(a.begin(), a.end(), 42) == a.end(); // not found

  V v;
  copy(a.begin(), a.end(), back_inserter(v));
  auto ve = *find(v.begin(), v.end(), 4);            //     found
  bool vb = find(v.begin(), v.end(), 42) == v.end(); // not found

  char sz[] = "abcdefg";
  auto sze = *find(begin(sz), end(sz), 'c');           //     found
  auto szb = find(begin(sz), end(sz), '&') == end(sz); // not found

  return e == 4 && b && 4 == ve && vb && sze == 'c' && szb;
}

template <typename V, typename S, typename L, typename FL>
constexpr bool algorithm_test3() {
  using namespace std;
  using arr_t = array<int, 5>;
  arr_t a{1, 2, 3, 4, 5};
  auto sz1 = distance(a.begin(), a.end());
  for_each(a.begin(), a.end(), [](auto &x) { x *= 2; });
  auto asum_ok = 30 == accumulate(a.begin(), a.end(), 0);

  V v;
  copy(a.begin(), a.end(), back_inserter(v));
  auto sz2 = distance(v.begin(), v.end());
  for_each(v.begin(), v.end(), [](auto &x) { x *= 2; });
  auto vsum_ok = 60 == accumulate(v.begin(), v.end(), 0);

  L l;
  copy(a.begin(), a.end(), back_inserter(l));
  auto sz3 = distance(l.begin(), l.end());

  FL fl;
  copy(a.begin(), a.end(), front_inserter(fl));
  auto sz4 = distance(fl.begin(), fl.end());

  return sz1 == 5 && sz2 == 5 && sz3 == 5 && sz4 == 5 && asum_ok && vsum_ok;
}

template <template <typename...> typename S, template <typename...> typename V,
          template <typename...> typename L, template <typename...> typename FL>
void rt_algorithm_tests() {
  assert((algorithm_test1<S<int>, V<int>>()));
  assert((algorithm_test2<S<int>, V<int>>()));
  assert((algorithm_test3<S<int>, V<int>, L<int>, FL<int>>()));
}

void algorithm_tests() {
#if CONSTEXPR_CEST == 1
  static_assert((algorithm_test1<cest::vector<int>, cest::set<int>>()));
  static_assert((algorithm_test2<cest::vector<int>, cest::set<int>>()));
  static_assert((algorithm_test3<cest::vector<int>, cest::set<int>,
                                 cest::list<int>, cest::forward_list<int>>()));
#endif

  rt_algorithm_tests<std::vector, std::set, std::list, std::forward_list>();
  rt_algorithm_tests<cest::vector, cest::set, cest::list, cest::forward_list>();
}

#endif // _CEST_ALGORITHM_TESTS_HPP_
