#ifndef _CEST_SET_TESTS_HPP_
#define _CEST_SET_TESTS_HPP_

#include "cest/set.hpp"
#include <set>
#include <cassert>
#include <tuple>
#include <type_traits>
#include <algorithm>
#include <iostream>

// This is the style of output from implementation of Okasaki's Haskell RB tree
template <template <typename...> typename S, typename ...Ts>
constexpr void debug_print_set(S<Ts...> &s) {
#ifdef NO_STATIC_TESTS
  using namespace std;
  if constexpr (is_same_v<S<Ts...>,cest::set<Ts...>>) {
    using node = typename cest::set<Ts...>::node_type;
    auto show = [&](node *n, auto &show_rec) -> std::string {
      auto paren = [&](node *p){ return string("(") + show_rec(p,show_rec) + ")"; };
      string c = n->c == S<Ts...>::RED ? "R " : "B ";
      string l = n->l        ? paren(n->l) : "E";
      string r = n->r        ? paren(n->r) : "E";
      return string("T ") + c + l + " " + to_string(n->x) + " " + r;
    };
    cout  << show(s.m_root,show) << endl;
  }
#endif // NO_STATIC_TESTS
}

template <template <typename...> typename S>
constexpr auto set_test1()
{
  S<int> set;

  auto r1 = set.insert(3);     // set size increased: insert done
  auto r2 = set.insert(3);     // set size unchanged: insert not done
  return std::tuple{r1.first != set.end(), *r1.first, r1.second,
                    r2.first == r1.first,  *r2.first, r2.second};
}

template <template <typename...> typename S>
constexpr auto set_test2()
{
  S<int> set;

  auto r1 = set.insert(1);
  auto r2 = set.insert(2);
  return std::tuple{set.size(),*r1.first,r1.second,*r2.first,r2.second};
}

template <template <typename...> typename S, typename T>
constexpr auto set_test3(T x, T y, T z)
{
  S<T> s;

  auto r1 = s.insert(x);
  auto r2 = s.insert(y);
  debug_print_set(s);

  auto r3 = s.insert(z);
  debug_print_set(s);

  return std::tuple{s.size(),*r1.first,*r2.first,*r3.first};
}

template <class T, class U>
constexpr T &inserts(T &s, U x) { s.insert(x); return s; }

template <class T, class U, class ...Us>
constexpr T &inserts(T &s, U x, Us ...xs)
{
  s.insert(x);
  return inserts(s, xs...);
}

template <template <typename...> typename S, typename T>
constexpr auto set_test4(T x1, T x2, T x3, T x4, T x5)
{
  S<T> s1, s2, s3;
  s1.insert(x1); s1.insert(x2); s1.insert(x3); s1.insert(x4); s1.insert(x5);
  s2.insert(x5); s2.insert(x4); s2.insert(x3); s2.insert(x2); s2.insert(x1);
  s3.insert(x5); s3.insert(x1); s3.insert(x4); s3.insert(x2); s3.insert(x3);

  debug_print_set(s1);
  debug_print_set(s2);
  debug_print_set(s3); // s1 != s2 | s3, but all are balanced

  return std::tuple{s1.size(), s2.size(), s3.size()};
}

template <template <typename...> typename S, typename T>
constexpr auto set_test5(T x)
{
  S<T> s;

  auto ib0 = s.begin();
  auto ie0 = s.end();
  bool  b0 = ib0==ie0;

  s.insert(x);   auto ib1 = s.begin(); bool b1 = ib1==ie0; T a = *ib1;
  s.insert(x-1); auto ib2 = s.begin(); bool b2 = ib2==ie0; T b = *ib2;
  s.insert(x+1); auto ib3 = s.begin(); bool b3 = ib3==ie0; T c = *ib3;

  return std::tuple{b0,b1,b2,b3,a,b,c,s.size()};
}

// test pre-increment (and insert, begin and end)
template <template <typename...> typename S, typename T, typename ...Ts>
constexpr auto set_test6(T x, Ts ...xs)
{
  S<T> s;

  s.insert(x);
  auto it = s.begin();
  ++it;
  bool r0 = it==s.end();

  // wikipedia - a different structure, but still fine (a valid rb tree)
  inserts(s,x,xs...);

  auto ip1 = s.insert(6);
  bool r1  = ip1.second;
  auto it1 = ip1.first;
  auto a1  = *it1;
  ++it1; // 6 -> 8
  auto b1  = *it1;

  auto ip12 = s.insert(8);
  bool r12  = ip12.second;
  auto it12 = ip12.first;
  auto a12  = *it12;
  ++it12; // 8 -> 11 
  auto b12  = *it12;

  auto ip2 = s.insert(15);
  bool r2  = ip2.second;
  auto it2 = ip2.first;
  auto a2  = *it2;
  ++it2; // 15 -> 17
  auto b2  = *it2;

  auto ip3 = s.insert(1);
  bool r3  = ip3.second;
  auto it3 = ip3.first;
  auto a3  = *it3;
  ++it3; // 1 -> 6
  auto b3  = *it3;

  auto ip4 = s.insert(11);
  bool r4  = ip4.second;
  auto it4 = ip4.first;
  auto a4  = *it4;
  ++it4; // 11 -> 13
  auto b4  = *it4;

  auto ip5 = s.insert(27);
  bool r5  = ip5.second;
  auto it5 = ip5.first;
  auto a5  = *it5;
  ++it5; // 27 -> end
  auto r6  = it5==s.end();

  bool r = !r1&&!r12&&!r2&&!r3&&!r4&&!r5; // no changes

  T sum{0};
  T prev = std::numeric_limits<int>::min();
  bool inc = true; // increasing
  for (auto it = s.begin(); it != s.end(); ++it) {
    inc  = inc && (*it > prev);
    prev = *it;
    sum += prev;
  }

  using std::tuple;
  return tuple{r0,r,a1,b1,a12,b12,a2,b2,a3,b3,a4,b4,a5,r6,sum,inc,s.size()};
}

// tests post-increment
template <template <typename...> typename S>
constexpr auto set_test7() {
  S<int> s;
  inserts(s,1,5,4,2,3);
  auto it0 = s.begin();
  auto it1 = it0++;
  return std::tuple{*it0,*it1};
}

namespace test8 {
  struct FatKey   { int x; int data[1000]; };
  struct LightKey { int x; };
  constexpr
  bool operator<(const FatKey& fk, const LightKey& lk) { return fk.x < lk.x; }
  constexpr
  bool operator<(const LightKey& lk, const FatKey& fk) { return lk.x < fk.x; }
  constexpr
  bool operator<(const FatKey& fk1, const FatKey& fk2) { return fk1.x < fk2.x; }
}

// tests find
template <template <typename...> typename S>
constexpr auto set_test8() {

  using namespace test8;
  S<int> s1;
  inserts(s1,1,2,3,4);
  auto it1 = s1.find(2);
  bool ok1 = it1 != s1.end();

  LightKey lk = {2};

  S<FatKey, std::less<>> s2;
  FatKey fk{42,{}};
  inserts(s2, FatKey{1,{}}, FatKey{2,{}}, FatKey{3,{}}, FatKey{4,{}});
  auto it2 = s2.find(lk);    // The C++14 template version of find
  int   x2 = it2->x;
  bool ok2 = it2 != s2.end();

  auto it3 = ++it2;
  auto  x3 = it3->x;
  auto it4 = it3++;
  auto  x4 = it4->x;

  return std::tuple{ok1,*it1,ok2,x2,x3,x4};
}

void set_tests()
{
  using std::tuple;
  using cest::set;
  constexpr const auto tup1 = tuple{true,3,true,true,3,false};
  constexpr const auto tup2 = tuple{2,1,true,2,true};
  constexpr const auto tup3 = tuple{3,3,2,1};
  constexpr const auto tup4 = tuple{3,1,2,3};
  constexpr const auto tup5 = tuple{3,1,3,2};
  constexpr const auto tup6 = tuple{2,1,2,2};
  constexpr const auto tup7 = tuple{5,5,5};
  constexpr const auto tup8 = tuple{true,false,false,false,42,41,41,3};
  constexpr const auto tup9 = tuple{true,true,6,8,8,11,15,17,1,6,11,13,27,true,145,true,10};
  constexpr const auto tup10 = tuple{2,1};
  constexpr const auto tup11 = tuple{true,2,true,2,3,3};
  constexpr const auto tup12 = tuple{true};

#ifndef NO_STATIC_TESTS
  static_assert(sizeof(set<float>)==sizeof(set<double>));
  static_assert(std::weakly_incrementable<set<int>::iterator>);
  static_assert(set_test1<set>() == tup1);
  static_assert(set_test2<set>() == tup2);
  static_assert(set_test3<set>(3,2,1) == tup3);
  static_assert(set_test3<set>(1,2,3) == tup4);
  static_assert(set_test3<set>(1,3,2) == tup5);
  static_assert(set_test3<set>(1,2,2) == tup6);
  static_assert(set_test4<set>(1,2,3,4,5) == tup7);
  static_assert(set_test5<set>(42)        == tup8);
  static_assert(set_test6<set>(1,6,8,11,13,15,17,22,25,27) == tup9);
  static_assert(set_test6<set>(27,25,22,17,15,13,11,8,6,1) == tup9);
  static_assert(set_test6<set>(1,27,6,25,8,22,11,17,13,15) == tup9);
  static_assert(set_test6<set>(1,27,6,25,8,22,11,17,13,15) == tup9);
  static_assert(set_test7<set>()                           == tup10);
  static_assert(set_test8<set>()                           == tup11);
#endif
         assert(set_test1<set>() == tup1);
         assert(set_test2<set>() == tup2);
         assert(set_test3<set>(3,2,1) == tup3);
         assert(set_test3<set>(1,2,3) == tup4);
         assert(set_test3<set>(1,3,2) == tup5);
         assert(set_test3<set>(1,2,2) == tup6);
         assert(set_test4<set>(1,2,3,4,5) == tup7);
         assert(set_test5<set>(42)        == tup8);
         assert(set_test6<set>(1,6,8,11,13,15,17,22,25,27) == tup9);
         assert(set_test6<set>(27,25,22,17,15,13,11,8,6,1) == tup9);
         assert(set_test6<set>(1,27,6,25,8,22,11,17,13,15) == tup9);
         assert(set_test7<set>()                           == tup10);
         assert(set_test8<set>()                           == tup11);
  {
         using std::set;
#ifndef NO_STATIC_TESTS
         static_assert(sizeof(set<float>)==sizeof(set<double>));
         static_assert(std::weakly_incrementable<set<int>::iterator>);
#endif
         assert(set_test1<set>() == tup1);
         assert(set_test2<set>() == tup2);
         assert(set_test3<set>(3,2,1) == tup3);
         assert(set_test3<set>(1,2,3) == tup4);
         assert(set_test3<set>(1,3,2) == tup5);
         assert(set_test3<set>(1,2,2) == tup6);
         assert(set_test4<set>(1,2,3,4,5) == tup7);
         assert(set_test5<set>(42)        == tup8);
         assert(set_test6<set>(1,6,8,11,13,15,17,22,25,27) == tup9);
         assert(set_test6<set>(27,25,22,17,15,13,11,8,6,1) == tup9);
         assert(set_test6<set>(1,27,6,25,8,22,11,17,13,15) == tup9);
         assert(set_test7<set>()                           == tup10);
         assert(set_test8<set>()                           == tup11);
  }
}

#endif // _CEST_SET_TESTS_HPP_
