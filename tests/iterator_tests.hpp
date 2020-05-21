#ifndef _CEST_ITERATOR_TESTS_HPP_
#define _CEST_ITERATOR_TESTS_HPP_

#include "cest/iterator.hpp"
#include "cest/vector.hpp"
#include "cest/forward_list.hpp"
#include "cest/set.hpp"
#include "cest/map.hpp"
#include <cassert>
#include <iterator>
#include <vector>
#include <forward_list>
#include <set>
#include <map>

template <template <typename...> typename V>
constexpr auto iterator_test1() {
  V<int> v;
  using  it_t = typename V<int>::iterator;
  using cit_t = typename V<int>::const_iterator;
   it_t it1 = v.begin();
  cit_t it2 = v.cbegin();
  return it1==it2;
}

template <typename T1, typename T2>
constexpr auto iterator_test2() {
  using it1_t = typename T1::iterator;
  using it2_t = typename T2::iterator;
  using dt1_t = typename std::iterator_traits<it1_t>::difference_type;
  using dt2_t = typename std::iterator_traits<it2_t>::difference_type;
  using vt1_t = typename std::iterator_traits<it1_t>::value_type;
  using vt2_t = typename std::iterator_traits<it2_t>::value_type;
  using p1_t  = typename std::iterator_traits<it1_t>::pointer;
  using p2_t  = typename std::iterator_traits<it2_t>::pointer;
  using r1_t  = typename std::iterator_traits<it1_t>::reference;
  using r2_t  = typename std::iterator_traits<it2_t>::reference;
  using ic1_t = typename std::iterator_traits<it1_t>::iterator_category;
  using ic2_t = typename std::iterator_traits<it2_t>::iterator_category;
  static_assert(std::is_same_v<dt1_t,dt2_t>);
  static_assert(std::is_same_v<vt1_t,vt2_t>);
  static_assert(std::is_same_v< p1_t, p2_t>);
  static_assert(std::is_same_v< r1_t, r2_t>);
  static_assert(std::is_same_v<ic1_t,ic2_t>);
  return true;
}

void iterator_tests()
{
#ifndef NO_STATIC_TESTS
  using namespace std;
  static_assert(iterator_test1<cest::vector>());
  static_assert(iterator_test2<vector<int>,      cest::vector<int>>());
  static_assert(iterator_test2<forward_list<int>,cest::forward_list<int>>());
//  static_assert(iterator_test2<std::set,cest::set>());
//  static_assert(iterator_test2<std::map<int,char>,cest::map<int,char>>());
#endif

  assert(iterator_test1<std::vector>());
  assert(iterator_test1<cest::vector>());
}

#endif // _CEST_ITERATOR_TESTS_HPP_
