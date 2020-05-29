#ifndef _CEST_FORWARD_LIST_TESTS_HPP_
#define _CEST_FORWARD_LIST_TESTS_HPP_

#include "cest/forward_list.hpp"
#include <forward_list>
#include <cassert>

template <template <typename...> typename FL>
constexpr auto forward_list_test1() {
  FL<int> fl;
  return fl.empty();
}

template <template <typename...> typename FL>
constexpr auto forward_list_test2() {
  FL<int> fl;
  fl.push_front(42);
  return fl.front();
}

template <template <typename...> typename FL>
constexpr auto forward_list_test3() {
  FL<int> fl;
  fl.push_front(2);
  fl.push_front(1);
  return fl.front();
}

template <template <typename...> typename FL>
constexpr auto forward_list_test4() {
  FL<int> fl;
  fl.insert_after(fl.before_begin(),123);  // l.begin() here is an error
  fl.insert_after(fl.begin(),1); // 123     -> 123 1
  fl.push_front(2);             // 123 1   -> 2 123 1
  fl.erase_after(fl.begin());    // 2 123 1 -> 2 1
  fl.insert_after(fl.before_begin(),3); // same as l.push_front(3)

  int sum1 = 0, sum2 = 0;
  for (auto it = fl.begin(); it != fl.end(); it++)    // 3 + 2 + 1
    sum1 += *it;

  using cit_t = typename FL<int>::const_iterator;
  cit_t it = fl.cbegin();
  for (; it != fl.cend(); ++it) // 3 + 2 + 1
    sum2 += *it;

  bool b1 = it == fl.end();
  bool b2 = fl.end() == it;
  return std::tuple{fl.front(),sum1,sum2,b1,b2};
}

template <template <typename...> typename FL>
constexpr auto forward_list_test5() {
  FL<int> *p1 = new FL<int>;
  FL<int> *p2 = new FL<int>;

  int a = 17, b = 18;
  p1->push_front(b);
  p1->push_front(a);

  FL<int> l(*p1); // copy ctor
  *p2 = *p1;      // copy assignment

  bool b1 = a == p1->front();
  bool b2 = a == p2->front();
  bool b3 = a ==   l.front();

  delete p1;
  delete p2;

  return std::tuple{b1&&b2&&b3};
}

template <template <typename...> typename FL>
constexpr auto forward_list_test6() {
  struct Foo { int x; int y; };
  FL<Foo> fl;

  fl.push_front(Foo{1,2});
  auto it1 = fl.begin();
  bool b1 = 1 == it1->x && 2 == (*it1).y;

  auto it2 = fl.erase_after(fl.before_begin());
  bool b2 = it2 == fl.end() && fl.empty();

  Foo *fp = fl.get_allocator().allocate(1);
  fl.get_allocator().deallocate(fp,1);

/*  Foo o{3,4};
  fl.push_front(o);
  fl.erase_after(fl.before_begin(), fl.end()); // remove all elements
  bool b3 = fl.empty();
  fl.erase_after(fl.before_begin(), fl.end()); // do it again!
  bool b4 = fl.empty();

  fl.push_front(Foo{1,2});
  fl.clear();
  bool b5 = fl.empty();
*/
  return b1 && b2;// && b3 && b4 && b5;
}

template <template <typename...> typename FL>
constexpr void rt_forward_list_tests() {
         assert(forward_list_test1<FL>() == true);
         assert(forward_list_test2<FL>() == 42);
         assert(forward_list_test3<FL>() == 1);
         assert(forward_list_test4<FL>() == (std::tuple{3,6,6,true,true}));
         assert(forward_list_test5<FL>() == (std::tuple{true}));
         assert(forward_list_test6<FL>());
}

template <template <typename...> typename FL>
constexpr void ct_forward_list_tests() {
#ifndef NO_STATIC_TESTS
  static_assert(forward_list_test1<FL>() == true);
  static_assert(forward_list_test2<FL>() == 42);
  static_assert(forward_list_test3<FL>() == 1);
  static_assert(forward_list_test4<FL>() == std::tuple{3,6,6,true,true});
  static_assert(forward_list_test5<FL>() == std::tuple{true});
  static_assert(forward_list_test6<FL>());
#endif
}

void forward_list_tests()
{
  ct_forward_list_tests<cest::forward_list>();
  rt_forward_list_tests<std::forward_list>();
  rt_forward_list_tests<cest::forward_list>();
}

#endif // _CEST_FORWARD_LIST_TESTS_HPP_
