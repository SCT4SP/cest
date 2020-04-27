#ifndef _CEST_MAP_TESTS_HPP_
#define _CEST_MAP_TESTS_HPP_

#include "cest/map.hpp"
#include <map>
#include <cassert>

template <template <class...> class M, class T, class U>
constexpr auto map_test1()
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

  return std::tuple{p1.second,p2.second,p3.second,p4.second};
}

template <template <class...> class M, class T, class U>
constexpr auto map_test2()
{
  M<T,U> m;

  using value_type = typename M<T,U>::value_type;

  const value_type v1 = std::make_pair('a',1);
  auto p1 = m.insert(v1);

  auto i1 = m.find('a');
  bool b1 = i1 != m.end();

  auto i2 = m.find('q');
  bool b2 = i2 != m.end();

  return std::tuple{i1->first,i1->second,b1,b2};
}

void map_tests()
{
  constexpr const auto tup1 = std::tuple{true,false,false,true};
  constexpr const auto tup2 = std::tuple{'a',1,true,false};

#ifndef NO_STATIC_TESTS
//  static_assert((map_test1<cest::map,char,int>()) == tup1);
#endif

//  assert((map_test1<cest::map,char,int>()) == tup1);
  assert((map_test1<std::map,char,int>()) == tup1);
  assert((map_test2<std::map,char,int>()) == tup2);
}

#endif // _CEST_MAP_TESTS_HPP_
