#ifndef _CEST_ITERATOR_TESTS_HPP_
#define _CEST_ITERATOR_TESTS_HPP_

#include "cest/iterator.hpp"
#include "cest/vector.hpp"
#include "cest/forward_list.hpp"
#include "cest/list.hpp"
#include "cest/set.hpp"
#include "cest/map.hpp"
#include "cest/deque.hpp"
#include "cest/fstream.hpp"
#include <cassert>
#include <iterator>
#include <vector>
#include <forward_list>
#include <list>
#include <set>
#include <map>
#include <deque>
#include <fstream>

template <template <typename...> typename V>
constexpr auto iterator_test1()
{
  V<int> v;
  using  it_t = typename V<int>::iterator;
  using cit_t = typename V<int>::const_iterator;
   it_t it1 = v.begin();
  cit_t it2 = v.cbegin();
  return it1==it2;
}

template <typename It1, typename It2>
constexpr bool iterator_test_helper()
{
  using dt1_t = typename std::iterator_traits<It1>::difference_type;
  using dt2_t = typename std::iterator_traits<It2>::difference_type;
  using vt1_t = typename std::iterator_traits<It1>::value_type;
  using vt2_t = typename std::iterator_traits<It2>::value_type;
  using p1_t  = typename std::iterator_traits<It1>::pointer;
  using p2_t  = typename std::iterator_traits<It2>::pointer;
  using r1_t  = typename std::iterator_traits<It1>::reference;
  using r2_t  = typename std::iterator_traits<It2>::reference;
  using ic1_t = typename std::iterator_traits<It1>::iterator_category;
  using ic2_t = typename std::iterator_traits<It2>::iterator_category;
  bool b1 = std::is_same_v<dt1_t,dt2_t>;
  bool b2 = std::is_same_v<vt1_t,vt2_t>;
  bool b3 = std::is_same_v< p1_t, p2_t>;
  bool b4 = std::is_same_v< r1_t, r2_t>;
  bool b5 = std::is_same_v<ic1_t,ic2_t>;
  return b1&&b2&&b3&&b4&&b5;
}

template <typename T1, typename T2>
constexpr bool static_iterator_test()
{
  using it1_t  = typename T1::iterator;
  using it2_t  = typename T2::iterator;
  using cit1_t = typename T1::const_iterator;
  using cit2_t = typename T2::const_iterator;
  bool b1 = iterator_test_helper< it1_t, it2_t>();
  bool b2 = iterator_test_helper<cit1_t,cit2_t>();
  return b1&&b2;
}

template <typename Ifs, typename Str, typename Iter>
constexpr auto iterator_test2()
{
  Ifs file(__FILE__);
  Str str(Iter(file), {});
  return !str.empty();
}

void iterator_tests()
{
  using namespace cest;
#if CONSTEXPR_CEST == 1
  static_assert(iterator_test1<vector>());
  static_assert(iterator_test1<deque>());
  static_assert(static_iterator_test<std::vector<int>,vector<int>>());
  static_assert(static_iterator_test<std::forward_list<int>,forward_list<int>>());
  static_assert(static_iterator_test<std::set<int>,set<int>>());
  static_assert(static_iterator_test<std::map<int,char>,map<int,char>>());
  static_assert(static_iterator_test<std::list<int>,list<int>>());
  static_assert(static_iterator_test<std::deque<int>,deque<int>>());
#endif

  assert(iterator_test1<std::vector>());
  assert(iterator_test1<vector>());
  assert(iterator_test1<std::deque>());
  assert(iterator_test1<deque>());

  using ifs  = std::ifstream;  using str  = std::string;
  using ifs_ = cest::ifstream; using str_ = cest::string;
  assert((iterator_test2<ifs,  str,   std::istreambuf_iterator<char>>()));
  assert((iterator_test2<ifs_, str_, cest::istreambuf_iterator<char>>()));
}

#endif // _CEST_ITERATOR_TESTS_HPP_
