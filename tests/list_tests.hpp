#ifndef _CEST_LIST_TESTS_HPP_
#define _CEST_LIST_TESTS_HPP_

#include "cest/list.hpp"
#include <list>
#include <tuple>
#include <cassert>

template <typename L>
constexpr auto list_test1()
{
  L l;
  l.push_front(42);
  auto x = l.front();
  l.push_front(43);
  return std::tuple{l.front(), x};
}

void list_tests()
{
  constexpr const auto tup1 = std::tuple{43,42};

#ifndef NO_STATIC_TESTS
  static_assert(list_test1<cest::list<int>>() == tup1);
#endif

  assert(list_test1<cest::list<int>>() == tup1);
  assert(list_test1<std::list<int>>()  == tup1);
}

//list<_Tp, _Alloc>::emplace_back(_Args&&... __args)
//{
//    __node_allocator& __na = base::__node_alloc();
//    __hold_pointer __hold = __allocate_node(__na);
//    __node_alloc_traits::construct(__na, _VSTD::addressof(__hold->__value_), _VSTD::forward<_Args>(__args)...);
//    __link_pointer __nl = __hold->__as_link();
//    __link_nodes_at_back(__nl, __nl);
//    ++base::__sz();
//#if _LIBCPP_STD_VER > 14
//    return __hold.release()->__value_;
//#else
//    __hold.release();
//#endif
//}

//template <class _Tp, class _Alloc>
//void
//list<_Tp, _Alloc>::push_back(value_type&& __x)
//{
//    __node_allocator& __na = base::__node_alloc();
//    __hold_pointer __hold = __allocate_node(__na);
//    __node_alloc_traits::construct(__na, _VSTD::addressof(__hold->__value_), _VSTD::move(__x));
//    __link_nodes_at_back(__hold.get()->__as_link(), __hold.get()->__as_link());
//    ++base::__sz();
//    __hold.release();
//}

#endif // _CEST_LIST_TESTS_HPP_
