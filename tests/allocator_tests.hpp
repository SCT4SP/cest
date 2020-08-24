#ifndef _CEST_ALLOCATOR_TESTS_HPP_
#define _CEST_ALLOCATOR_TESTS_HPP_

#include <cassert>
#include <memory>

namespace alloc_tests {

struct Foo { int a; char b; float c; double d; };

template <class IntAlloc>
constexpr bool alloc_test1()
{
  using alloc_int_t    = IntAlloc;
  using alloc_double_t = typename std::allocator_traits<IntAlloc>::
                           template rebind_alloc<double>;
  alloc_int_t    alloc_i;
  alloc_double_t alloc_d;
  alloc_int_t    alloc_i2(alloc_d); // double free here?
  static_assert(std::is_same_v<decltype(alloc_i2), alloc_int_t>);

  int *pi = alloc_i.allocate(4);

  // this simply calls the At<Foo> constructor
  typename std::allocator_traits<decltype(alloc_i)>::
    template rebind_alloc<Foo> ralloc_foo;

  Foo *pf = ralloc_foo.allocate(4);
  std::construct_at(pf);
  pf->a = 1;
  pf->b = '2';
  pf->c = 3.0f;
  pf->d = 4.0;

  ralloc_foo.deallocate(pf, 4);

  // Alternative way to do the deallocate above - perhaps not, valgrind unhappy
  // At ralloc_foo2(ralloc_foo);
  // ralloc_foo2.deallocate(pf, 4);

  alloc_i.deallocate(pi, 4);
  return true;
}

template <typename IntAlloc>
constexpr bool alloc_test2()
{
  IntAlloc alloc;
  int* p1 = alloc.allocate(1);
  int* p2 = alloc.allocate(1);
  int i;

  if (&i == p2)
    return false;

  if (p1 == p2)
    return false;

  alloc.deallocate(p2,1);
  alloc.deallocate(p1,1);

  return true;
}

template <bool SA, class IntAlloc>
constexpr void tests_helper()
{
  assert(alloc_test1<IntAlloc>());
  assert(alloc_test2<IntAlloc>());
  if constexpr (SA) {
#if CONSTEXPR_CEST == 1
    static_assert(alloc_test1<IntAlloc>());
    static_assert(alloc_test2<IntAlloc>());
#endif
  }
}

} // namespae alloc_tests

void allocator_tests()
{
  using namespace alloc_tests;

  tests_helper<CONSTEXPR_CEST,std::allocator<int>>();  // true: constexpr tests
}

#endif // _CEST_ALLOCATOR_TESTS_HPP_
