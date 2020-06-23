#ifndef _CEST_ALLOCATOR_TESTS_HPP_
#define _CEST_ALLOCATOR_TESTS_HPP_

#include "cest/mono_block_alloc.hpp"
#include <cassert>
#include <memory>

namespace alloc_tests {

struct Foo { int a; char b; float c; double d; };

template <template <class...> class At>
constexpr bool alloc_test1()
{
  using alloc_int_t    = At<int>;
  using alloc_double_t = At<double>;
  alloc_int_t    alloc_i;
  alloc_double_t alloc_d;
  alloc_int_t    alloc_i2(alloc_d); // double free here?
  static_assert(std::is_same_v<decltype(alloc_i2), alloc_int_t>);

  int *pi = alloc_i.allocate(4);

  // this simply calls the At<Foo> constructor
  typename std::allocator_traits<decltype(alloc_i)>::rebind_alloc<Foo> ralloc_foo;

  Foo *pf = ralloc_foo.allocate(4);
  pf->a = 1;     // std::construct_at is not required for simple types
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

// This is a mystery. It occurs when 3, 2 and 1 are inserted to a std::set
// which uses the constexpr allocator mono_block_alloc
template <template <class...> class At>
constexpr bool alloc_test2()
{
  At<int> alloc;
  int* p1 = alloc.allocate(1);
  int* p2 = alloc.allocate(1);
  int i;

  if (&i == p2)  // compile error!
    return false;

  alloc.deallocate(p2,1);
  alloc.deallocate(p1,1);

  return true;
}

template <bool SA, template <class...> class At>
constexpr void tests_helper()
{
  assert(alloc_test1<At>());
  assert(alloc_test2<At>());
  if constexpr (SA) {
#ifndef NO_STATIC_TESTS
    static_assert(alloc_test1<At>());
    static_assert(alloc_test2<At>());
#endif
  }
}

} // namespae alloc_tests

void allocator_tests()
{
  using namespace alloc_tests;

  tests_helper<true,std::allocator>();  // true: constexpr tests
  tests_helper<false,cea::mono_block_alloc>(); // true fails on test2
}

#endif // _CEST_ALLOCATOR_TESTS_HPP_
