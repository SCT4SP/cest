#ifndef _CEST_LIST_TESTS_HPP_
#define _CEST_LIST_TESTS_HPP_

#include "cest/list.hpp"
//#include <list>
#include <cassert>

void list_tests()
{
}

#endif // _CEST_LIST_TESTS_HPP_

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
