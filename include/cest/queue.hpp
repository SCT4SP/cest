#ifndef _CEST_QUEUE_HPP_
#define _CEST_QUEUE_HPP_

#include <functional>
#include <vector>
#include <deque>
#include "libstdc++-v3/include/bits/stl_queue.h"

// specialisation on std::uses_allocator for cest::queue and priority_queue
namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  template<typename _Tp, typename _Seq, typename _Alloc>
    struct uses_allocator<cest::queue<_Tp, _Seq>, _Alloc>
    : public uses_allocator<_Seq, _Alloc>::type { };

  template<typename _Tp, typename _Seq, typename _Compare,
	   typename _Alloc>
    struct uses_allocator<cest::priority_queue<_Tp, _Seq, _Compare>, _Alloc>
    : public uses_allocator<_Seq, _Alloc>::type { };

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#endif // _CEST_QUEUE_HPP_
