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

  struct       iter;
  struct const_iter;

  using value_type            = T;
  using allocator_type        = Allocator;
  using size_type             = std::size_t;
  using difference_type       = std::ptrdiff_t;
  using reference             = value_type&;
  using const_reference       = const value_type&;
  using pointer               = std::allocator_traits<Allocator>::pointer;
  using const_pointer         = std::allocator_traits<Allocator>::const_pointer;
  using iterator              =       iter;
  using const_iterator        = const_iter;
  using reverse_iterator      = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  struct iter
  {
    using difference_type   = std::ptrdiff_t;
    using value_type        = deque::value_type;
    using reference         = value_type&;
    using pointer           = value_type*;
    using iterator_category = std::random_access_iterator_tag;

    constexpr reference operator*()           const noexcept { return *m_p; }
    constexpr pointer   operator->()          const noexcept { return  m_p; }
    constexpr auto&     operator++()                noexcept { // pre-incr
      if (m_p == *m_ppchunk + CHUNK_SIZE - 1) {
        ++m_ppchunk;
        m_p = *m_ppchunk;
      } else {
        ++m_p;
      }

      return *this;
    }

    constexpr auto      operator++(int)             noexcept { // post-incr
      auto tmp{m_p, m_ppchunk}; ++(*this); return tmp; 
    }

    constexpr auto&     operator--()                noexcept { // pre-decr
      if (m_p == *m_ppchunk) {
        --m_ppchunk;
        m_p = *m_ppchunk;
      } else {
        --m_p;
      }

      return *this;
    }

    constexpr auto      operator--(int)             noexcept { // post-decr
      auto tmp{m_p, m_ppchunk}; --(*this); return tmp; 
    }

    constexpr bool      operator==(const iter& rhs) noexcept {
      return m_p == rhs.m_p;
    }

    value_type*  m_p       = nullptr;
    value_type** m_ppchunk = nullptr; // address of the vector element
  };

  struct const_iter
  {
    using difference_type   = std::ptrdiff_t;
    using value_type        = deque::value_type;
    using reference         = const value_type&;
    using pointer           = const value_type*;
    using iterator_category = std::random_access_iterator_tag;

    constexpr const_iter(const value_type* p, value_type* const* ppchunk)
      : m_p(p),      m_ppchunk(ppchunk)      {}
    constexpr const_iter(const iter&     it)
      : m_p(it.m_p), m_ppchunk(it.m_ppchunk) {}

    constexpr reference operator*()           const noexcept { return *m_p; }
    constexpr pointer   operator->()          const noexcept { return  m_p; }
    constexpr auto&     operator++()                noexcept { // pre-incr
      if (m_p == *m_ppchunk + CHUNK_SIZE - 1) {
        ++m_ppchunk;
        m_p = *m_ppchunk;
      } else {
        ++m_p;
      }

      return *this;
    }

    constexpr auto      operator++(int)             noexcept { // post-incr
      auto tmp{m_p, m_ppchunk}; ++(*this); return tmp; 
    }

    constexpr auto&     operator--()                noexcept { // pre-decr
      if (m_p == *m_ppchunk) {
        --m_ppchunk;
        m_p = *m_ppchunk;
      } else {
        --m_p;
      }

      return *this;
    }

    constexpr auto      operator--(int)             noexcept { // post-decr
      auto tmp{m_p, m_ppchunk}; --(*this); return tmp; 
    }

    constexpr bool      operator==(const const_iter& rhs) noexcept {
      return m_p == rhs.m_p;
    }

    const value_type*  m_p       = nullptr;
    value_type* const* m_ppchunk = nullptr; // address of the vector element
  };

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
    for (size_type i = 0; i < size(); ++i)
      std::destroy_n(&(*this)[i], 1);
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

  constexpr void pop_back()
  {
    if (!empty()) {
      std::destroy_n(&m_chunks[m_back_chunk][m_back], 1);
      if (m_back == 0) {
        m_back = CHUNK_SIZE - 1;
        --m_back_chunk;
      } else {
        --m_back;
      }
      --m_size;
    }
  }

  constexpr void pop_front()
  {
    if (!empty()) {
      std::destroy_n(&m_chunks[m_front_chunk][m_front], 1);
      if (m_front == CHUNK_SIZE - 1) {
        m_front = 0;
        ++m_front_chunk;
      } else {
        ++m_front;
      }
      --m_size;
    }
  }

  constexpr       iterator  begin()       noexcept {
    return {&m_chunks[m_front_chunk][m_front], &m_chunks[m_front_chunk]};
  }
  constexpr const_iterator  begin() const noexcept {
    return {&m_chunks[m_front_chunk][m_front], &m_chunks[m_front_chunk]};
  }
  constexpr const_iterator cbegin() const noexcept {
    return {&m_chunks[m_front_chunk][m_front], &m_chunks[m_front_chunk]};
  }
  constexpr iterator          end()       noexcept {
    return {&m_chunks[m_back_chunk][m_back+1], &m_chunks[m_back_chunk]};
  }
  constexpr const_iterator    end() const noexcept {
    return {&m_chunks[m_back_chunk][m_back+1], &m_chunks[m_back_chunk]};
  }
  constexpr const_iterator   cend() const noexcept {
    return {&m_chunks[m_back_chunk][m_back+1], &m_chunks[m_back_chunk]};
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
        ++m_back_chunk;
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
  size_type           m_front_chunk;
  size_type           m_back_chunk;
  size_type           m_size  = 0;
  vector<value_type*> m_chunks;
};

} // namespace CEST_NAMESPACE

#endif // _CEST_DEQUE_HPP_
