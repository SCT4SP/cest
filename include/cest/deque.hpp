#ifndef _CEST_DEQUE_HPP_
#define _CEST_DEQUE_HPP_

#include "vector.hpp"
#include <memory>    // std::allocator_traits
#include <algorithm> // std::rotate

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
    m_front = CHUNK_SIZE / 2;
    m_back  = CHUNK_SIZE / 2 - 1;
    m_chunks.push_back(p);
    m_front_chunk = m_back_chunk = 0;
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
    push_front_helper();
    std::construct_at(&m_chunks[m_front_chunk][m_front],           value);
  }

  constexpr void push_front( T&& value )
  {
    push_front_helper();
    std::construct_at(&m_chunks[m_front_chunk][m_front], std::move(value));
  }

  constexpr void push_back( const T& value )
  {
    push_back_helper();
    std::construct_at(&m_chunks[m_back_chunk][m_back],             value);
  }

  constexpr void push_back( T&& value )
  {
    push_back_helper();
    std::construct_at(&m_chunks[m_back_chunk][m_back],   std::move(value));
  }

  constexpr reference       front() {
    return m_chunks[m_front_chunk][m_front];
  }
  constexpr const_reference front() const {
    return m_chunks[m_front_chunk][m_front];
  }
  constexpr reference        back()       {
    return m_chunks[m_back_chunk][m_back];
  }
  constexpr const_reference  back() const {
    return m_chunks[m_back_chunk][m_back];
  }
  [[nodiscard]] constexpr bool empty() const noexcept { return size()==0; }
  constexpr size_type size() const noexcept { return m_size; }

  constexpr      reference operator[]( size_type pos )
  {
    const size_type chunk        = (m_front + pos) % CHUNK_SIZE;
    const size_type chunk_offset = (m_front + pos) / CHUNK_SIZE;
    return m_chunks[m_front_chunk + chunk_offset][chunk];
  }
  constexpr const_reference operator[]( size_type pos ) const
  {
    const size_type chunk        = (m_front + pos) % CHUNK_SIZE;
    const size_type chunk_offset = (m_front + pos) / CHUNK_SIZE;
    return m_chunks[m_front_chunk + chunk_offset][chunk];
  }

private:
  constexpr void push_front_helper()
  {
    if (m_front == 0)
    {
      if (m_front_chunk == 0)
      {
        value_type *p = m_alloc.allocate(CHUNK_SIZE);
        m_chunks.push_back(p);
        std::rotate(m_chunks.rbegin(), m_chunks.rbegin() + 1, m_chunks.rend());
      }
      else {
        --m_front_chunk;
      }

      m_front = CHUNK_SIZE - 1;
    }
    else
    {
      --m_front;
    }

    m_size++;
  }

  constexpr void push_back_helper()
  {
    if (m_back == CHUNK_SIZE - 1)
    {
      if (m_back_chunk == m_chunks.size() - 1)
      {
        value_type *p = m_alloc.allocate(CHUNK_SIZE);
        m_chunks.push_back(p);
        ++m_back_chunk;
      }
      
      m_back = 0;
    }
    else
    {
      ++m_back;
    }

    m_size++;
  }

  allocator_type      m_alloc;
  size_type           m_front;
  size_type           m_back;
  size_type           m_size  = 0;
  size_type           m_front_chunk;
  size_type           m_back_chunk;
  vector<value_type*> m_chunks;
};

} // namespace CEST_NAMESPACE

#endif // _CEST_DEQUE_HPP_
