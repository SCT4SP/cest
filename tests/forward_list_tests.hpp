#ifndef _CEST_FORWARD_LIST_TESTS_HPP_
#define _CEST_FORWARD_LIST_TESTS_HPP_

#include "cest/forward_list.hpp"
#include "cest/mono_block_alloc.hpp"
#include "../tests/tests_util.hpp"
#include <forward_list>
#include <cassert>

namespace fl_tests {

template <typename FL>
constexpr bool forward_list_test1() {
  FL fl;
  return fl.empty();
}

template <typename FL>
constexpr bool forward_list_test2() {
  FL fl;
  fl.push_front(42);
  return 42==fl.front();
}

template <typename FL>
constexpr bool forward_list_test3() {
  FL fl;
  fl.push_front(2);
  fl.push_front(1);
  return 1==fl.front();
}

template <typename FL>
constexpr bool forward_list_test4() {
  FL fl;
  fl.insert_after(fl.before_begin(),123);  // l.begin() here is an error
  fl.insert_after(fl.begin(),1); // 123     -> 123 1
  fl.push_front(2);             // 123 1   -> 2 123 1
  fl.erase_after(fl.begin());    // 2 123 1 -> 2 1
  auto it0 = fl.insert_after(fl.before_begin(),3); // same as l.push_front(3)

  int sum1 = 0, sum2 = 0;
  for (auto it = fl.begin(); it != fl.end(); it++)    // 3 + 2 + 1
    sum1 += *it;

  using cit_t = typename FL::const_iterator;
  cit_t it = fl.cbegin();
  for (; it != fl.cend(); ++it) // 3 + 2 + 1
    sum2 += *it;

  bool b1 = it == fl.end();
  bool b2 = fl.end() == it;
  return 3==fl.front() && 6==sum1 && 6==sum2 && b1 && b2 && 3==*it0;
}

// Problematic for the custom allocator: comparing nodes against the this
// pointer will fail as this is not on the stack; as here we allocate
// each forward_list using the new operator.
// Try not doing this? It occurs within copy assignment, on line 149 of
// forward_list.tcc
// Would std::vector also have this issue?
// cest::forward_list also has a problem here, but only with the new allocator
template <typename FL>
constexpr bool forward_list_test5() {
  FL *p1 = new FL;
  FL *p2 = new FL;

  int a = 17, b = 18;
  p1->push_front(b);
  p1->push_front(a);

  FL l(*p1); // copy ctor
  *p2 = *p1; // copy assignment

  bool b1 = a == p1->front();
  bool b2 = a == p2->front();
  bool b3 = a ==   l.front();

  delete p1;
  delete p2;

  return b1 && b2 && b3;
}

struct Foo { int x; int y; };

template <typename FL>
constexpr bool forward_list_test6() {
  FL fl;

  fl.push_front(Foo{1,2});
  auto it1 = fl.begin();
  bool b1 = 1 == it1->x && 2 == (*it1).y;

  auto it2 = fl.erase_after(fl.before_begin());
  bool b2 = it2 == fl.end() && fl.empty();

  Foo *fp = fl.get_allocator().allocate(1);
  fl.get_allocator().deallocate(fp,1);

  Foo o{3,4};
  fl.push_front(o);
  fl.erase_after(fl.before_begin(), fl.end()); // remove all elements
  bool b3 = fl.empty();

  fl.erase_after(fl.before_begin(), fl.end()); // do it again!
  bool b4 = fl.empty();

  fl.push_front(Foo{1,2});
  fl.clear();
  bool b5 = fl.empty();

  return b1 && b2 && b3 && b4 && b5;
}

template <bool SA, class F1, class F2, class F3,
                   class F4, class F5, class F6, class F7>
constexpr void doit()
{
  using tests_util::push_front_dtor_test;

          assert(forward_list_test1<F1>());
          assert(forward_list_test2<F2>());
          assert(forward_list_test3<F3>());
          assert(forward_list_test4<F4>());
          assert(forward_list_test5<F5>());
          assert(forward_list_test6<F6>());
          assert(push_front_dtor_test<F7>());

  if constexpr (SA) {
    static_assert(forward_list_test1<F1>());
    static_assert(forward_list_test2<F2>());
    static_assert(forward_list_test3<F3>());
    static_assert(forward_list_test4<F4>());
    static_assert(forward_list_test5<F5>());
    static_assert(forward_list_test6<F6>());
    static_assert(push_front_dtor_test<F7>());
  }
}

template <bool SA, template <class...> class TT>
constexpr void tests_helper()
{
  using tests_util::Bar;

  using FL1  = TT<int>;
  using FL2  = TT<int>;
  using FL3  = TT<int>;
  using FL4  = TT<int>;
  using FL5  = TT<int>;
  using FL6  = TT<Foo>;
  using FL7  = TT<Bar>;

  using FLa1 = TT<int, cea::mono_block_alloc<int>>;
  using FLa2 = TT<int, cea::mono_block_alloc<int>>;
  using FLa3 = TT<int, cea::mono_block_alloc<int>>;
  using FLa4 = TT<int, cea::mono_block_alloc<int>>;
  using FLa5 = TT<int, cea::mono_block_alloc<int>>;
  using FLa6 = TT<Foo, cea::mono_block_alloc<Foo>>;
  using FLa7 = TT<Bar, cea::mono_block_alloc<Bar>>;

  doit<SA, FL1,  FL2,  FL3,  FL4,  FL5,  FL6, FL7>();
//  doit<SA, FLa1, FLa2, FLa3, FLa4, FLa5, FLa6, FLa7>(); // valgrind unhappy
}

} // namespace fl_tests

void forward_list_tests()
{
  using namespace fl_tests;

  tests_helper<CONSTEXPR_CEST,cest::forward_list>();
}

#endif // _CEST_FORWARD_LIST_TESTS_HPP_
