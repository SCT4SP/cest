#ifndef _CEST_MAP_TESTS_HPP_
#define _CEST_MAP_TESTS_HPP_

#include "cest/map.hpp"
#include <map>
#include <cassert>

constexpr bool common_static_map_tests()
{
  auto f = []<template <class ...> class M>() {
    static_assert(sizeof(M<float,int>)==sizeof(M<double,int>));
    static_assert(sizeof(M<int,float>)==sizeof(M<int,double>));
    using iter_t       = typename M<char,int>::iterator;
    using const_iter_t = typename M<char,int>::const_iterator;
    static_assert(std::weakly_incrementable<iter_t>);
    static_assert(std::weakly_incrementable<const_iter_t>);
    static_assert(std::is_same_v<
                    typename iter_t::iterator_category,
                    std::bidirectional_iterator_tag
                  >);
    static_assert(!std::is_same_v<iter_t, const_iter_t>);
  };

  f.operator()<std::map>();
  f.operator()<cest::map>();

  return true;
}

template <template <class...> class M, class T, class U>
constexpr bool map_test1()
{
  M<T,U> m;

  using value_type = typename M<T,U>::value_type;
  static_assert(std::is_same_v<value_type,std::pair<const T,U>>);

  const value_type v1 = std::make_pair('a',1);
  auto p1 = m.insert(v1);

  auto p2 = m.insert(v1);

  const value_type v2 = std::make_pair('a',2);
  auto p3 = m.insert(v2);

  const value_type v3 = std::make_pair('b',1);
  auto p4 = m.insert(v3);

  return p1.second && !p2.second && !p3.second && p4.second;
}

template <template <class...> class M, class T, class U>
constexpr bool map_test2()
{
  M<T,U> m;
  m.clear();

  using value_type = typename M<T,U>::value_type;

  const value_type v1 = std::make_pair('a',1);
  auto p1 = m.insert(v1);

  auto i1 = m.find('a');
  bool b1 = i1 != m.end();

  auto i2 = m.find('q');
  bool b2 = i2 != m.end();

  bool b3 = 'a'==i1->first && 1==i1->second;

  return b1 && !b2 && b3;
}

template <template <class...> class M, class T, class U>
constexpr bool map_test3()
{
  M<T,U> m1;
  m1.insert({'a',1});
  m1.insert({'b',2});
  m1.insert({'c',3});
  M<T,U> m2 = m1;
  bool b1 = 3==m1.size() && 3==m2.size();

  M<T,U> m3;
  m3.insert({'d',4});
  m1 = m3;
  m3.clear();
  bool b2 = 1==m1.size() && 'd'==m1.begin()->first && m3.empty();

  return b1 && b2;
}

void map_tests()
{
#if CONSTEXPR_CEST == 1
  static_assert(common_static_map_tests());
  static_assert(map_test1<cest::map,char,int>());
  static_assert(map_test2<cest::map,char,int>());
  static_assert(map_test3<cest::map,char,int>());
#endif

  assert((map_test1< std::map,char,int>()));
  assert((map_test1<cest::map,char,int>()));
  assert((map_test2< std::map,char,int>()));
  assert((map_test2<cest::map,char,int>()));
  assert((map_test3< std::map,char,int>()));
  assert((map_test3<cest::map,char,int>()));
}

#endif // _CEST_MAP_TESTS_HPP_
