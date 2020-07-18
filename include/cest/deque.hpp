#ifndef _CEST_DEQUE_HPP_
#define _CEST_DEQUE_HPP_

#include "vector.hpp"
#include "list.hpp"
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

  constexpr deque()
  {
    value_type *p = m_alloc.allocate(CHUNK_SIZE);
    m_back  = p + (CHUNK_SIZE / 2);
    m_front = m_back - 1;
    m_chunks.push_front(p);
    m_front_chunk = m_back_chunk = m_chunks.begin();
  }

  constexpr ~deque()
  {
    /*value_type *p = m_front;
    do {
      std::destroy_n(m_p,m_size);
    } while */
    for (auto p : m_chunks)
      m_alloc.deallocate(p, CHUNK_SIZE);
  }

  constexpr void push_front( const T& value )
  {
/*    if (m_head.back().size() >= CHUNK_SIZE)
      m_head.push_back(vector<T>());

    m_head.back().push_back(value);
*/
  }

  constexpr void push_front( T&& value )
  {
    if (m_front == *m_front_chunk)
    {
      if (m_front_chunk == m_chunks.begin())
      {
        value_type *p = m_alloc.allocate(CHUNK_SIZE);
        m_chunks.push_front(p);
        m_front_chunk = m_chunks.begin();
      }
      else {
        --m_front_chunk;
      }

      m_front = *m_front_chunk + CHUNK_SIZE - 1;
    }
    else
    {
      --m_front;
    }

    std::construct_at(m_front, std::move(value));
    m_size++;
  }

  constexpr void push_back( const T& value )
  {
  }

  constexpr void push_back( T&& value )
  {
    if (m_back == (*m_back_chunk + CHUNK_SIZE - 1))
    {
      if (++m_back_chunk == m_chunks.end())
      {
        value_type *p = m_alloc.allocate(CHUNK_SIZE);
        m_chunks.push_back(p);
        --m_back_chunk;
      }
      
      m_back = *m_back_chunk;
    }
    else
    {
      ++m_back;
    }

    std::construct_at(m_back, std::move(value));
    m_size++;
  }

  constexpr reference       front()       { return *m_front; }
  constexpr const_reference front() const { return *m_front; }
  constexpr reference        back()       { return *m_back;  }
  constexpr const_reference  back() const { return *m_back;  }
  [[nodiscard]] constexpr bool empty() const noexcept { return size()==0; }
  constexpr size_type size() const noexcept { return m_size; }

  reference       operator[]( size_type pos )
  {
/*    size_type chunk, chunk_offset;
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
*/
  }
  //const_reference operator[]( size_type pos ) const;

private:

  allocator_type         m_alloc;
  value_type*            m_front = nullptr;
  value_type*            m_back  = nullptr;
  size_type              m_size  = 0;
  list<value_type*>::iterator m_front_chunk;
  list<value_type*>::iterator m_back_chunk;
  list<value_type*> m_chunks;
};

} // namespace CEST_NAMESPACE

#endif // _CEST_DEQUE_HPP_
