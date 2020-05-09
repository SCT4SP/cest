#ifndef _CEST_ITERATOR_TESTS_HPP_
#define _CEST_ITERATOR_TESTS_HPP_

#include "cest/iterator.hpp"
#include "cest/vector.hpp"

template <template <typename...> typename V>
constexpr auto iterator_test1() {
  V<int> v;
  using  it_t = typename V<int>::iterator;
  using cit_t = typename V<int>::const_iterator;
   it_t it1 = v.begin();
  cit_t it2 = v.cbegin();
  return it1==it2;
}

void iterator_tests()
{
#ifndef NO_STATIC_TESTS
  static_assert(iterator_test1<cest::vector>());
#endif

  assert(iterator_test1<std::vector>());
  assert(iterator_test1<cest::vector>());
}

#endif // _CEST_ITERATOR_TESTS_HPP_
