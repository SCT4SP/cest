#ifndef _CEST_DEQUE_HPP_
#define _CEST_DEQUE_HPP_

#include <algorithm>
#include <type_traits>
#include <initializer_list>
#include <iterator>
#include <memory>
#include "libstdc++-v3/include/bits/stl_deque.h"

// specialisation on std::__is_bitwise_relocatable for cest::deque
#if __cplusplus >= 201103L
  // std::allocator is safe, but it is not the only allocator
  // for which this is valid.
  template<class _Tp>
    struct std::__is_bitwise_relocatable<CEST_NAMESPACE::deque<_Tp>>
    : std::true_type { };
#endif

#endif // _CEST_DEQUE_HPP_
