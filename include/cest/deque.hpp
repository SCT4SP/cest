#ifndef _CEST_DEQUE_HPP_
#define _CEST_DEQUE_HPP_

#include "vector.hpp"
#include <memory>    // std::allocator_traits
//#include <algorithm> // std::rotate

#define CHUNK_SIZE 1024

namespace CEST_NAMESPACE {

template <
  class T,
  class Allocator = std::allocator<T>
>
class deque {
public:

  using value_type            = T;
  using allocator_type        = Allocator;
  using size_type             = std::size_t;
  using difference_type       = std::ptrdiff_t;
  using reference             = value_type&;
  using const_reference       = const value_type&;
  using pointer               = std::allocator_traits<Allocator>::pointer;
  using const_pointer         = std::allocator_traits<Allocator>::const_pointer;
  using iterator              =       T*;
  using const_iterator        = const T*;
  using reverse_iterator      = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  constexpr deque() {
    m_head.push_back(vector<T>());
    m_tail.push_back(vector<T>());
  }

  constexpr void push_front( const T& value )
  {
    if (m_head.back().size() >= CHUNK_SIZE)
      m_head.push_back(vector<T>());

    m_head.back().push_back(value);
  }

  constexpr void push_front( T&& value )
  {
    if (m_head.back().size() >= CHUNK_SIZE)
      m_head.push_back(vector<T>());

    m_head.back().push_back(std::move(value));
  }

  constexpr void push_back( const T& value )
  {
    if (m_tail.back().size() >= CHUNK_SIZE)
      m_tail.push_back(vector<T>());

    m_tail.back().push_back(value);
  }

  constexpr void push_back( T&& value )
  {
    if (m_tail.back().size() >= CHUNK_SIZE)
      m_tail.push_back(vector<T>());

    m_tail.back().push_back(std::move(value));
  }

  constexpr reference       front()       { return m_head.back().back(); }
  constexpr const_reference front() const { return m_head.back().back(); }
  constexpr reference        back()       { return m_tail.back().back(); }
  constexpr const_reference  back() const { return m_tail.back().back(); }
  [[nodiscard]] constexpr bool empty() const noexcept { return size()==0; }

  reference       operator[]( size_type pos )
  {
    size_type chunk, chunk_offset;
    if (pos < part_size(m_head))
    {
      const size_type head_pos = part_size(m_head) - pos - 1;
      chunk                    = head_pos / CHUNK_SIZE;
      chunk_offset             = head_pos % CHUNK_SIZE;
      return m_head[chunk][chunk_offset];
    } else {
      const size_type tail_pos = pos - part_size(m_head);
      chunk                    = tail_pos / CHUNK_SIZE;
      chunk_offset             = tail_pos % CHUNK_SIZE;
      return m_tail[chunk][chunk_offset];
    }
  }
  //const_reference operator[]( size_type pos ) const;

  constexpr size_type size() const noexcept {
    return part_size(m_head) + part_size(m_tail);
  }

private:
  constexpr size_type part_size(const vector<vector<T>>& vv) const noexcept
  {
    return (vv.size() - 1) * CHUNK_SIZE + vv.back().size();
  }

  vector<vector<T>> m_head; // push_front
  vector<vector<T>> m_tail; // push_back
};

} // namespace CEST_NAMESPACE

#endif // _CEST_DEQUE_HPP_
