#ifndef _CEST_VECTOR_HPP_
#define _CEST_VECTOR_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/vector_tests.hpp

#include <memory>  // std::allocator
#include <cstddef>

namespace cest {

template <
  class T,
  class Allocator = std::allocator<T>
>
class vector {
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

  constexpr vector()  = default;
  constexpr size_type        size() const { return m_size;       }
  constexpr size_type    capacity() const { return m_capacity;   }
  constexpr iterator        begin()       { return {m_p};          }
  constexpr const_iterator  begin() const { return {m_p};          }
  constexpr const_iterator cbegin() const { return {m_p};          }
  constexpr iterator          end()       { return {m_p + m_size}; }
  constexpr bool            empty() const { return m_size == 0;  }
  constexpr void         pop_back()       { m_size--; }
  constexpr reference       operator[](size_type pos)       { return m_p[pos]; }
  constexpr const_reference operator[](size_type pos) const { return m_p[pos]; }
	
  constexpr void push_back(const value_type &value) {
    if (0 == m_capacity) {
      reserve(1);
    } else if (m_capacity == m_size) {
      reserve(m_capacity * 2);
    }
    std::construct_at(&m_p[m_size++],value);
  }
  constexpr void push_back(value_type &&value) {
    if (0 == m_capacity) {
      reserve(1);
    } else if (m_capacity == m_size) {
      reserve(m_capacity * 2);
    }
    std::construct_at(&m_p[m_size++],std::forward<T>(value));
  }
  constexpr void reserve(size_type new_cap)
  {
    if (new_cap > m_capacity)
    {
      value_type *p = m_alloc.allocate(new_cap);
      std::copy_n(m_p, m_size, p);
      if (0 != m_capacity) {
        std::destroy_n(m_p,m_size);
        m_alloc.deallocate(m_p,m_capacity);
      }
      m_p = p;
      m_capacity = new_cap;
    }
  }
  constexpr iterator erase(iterator first, iterator last) {
    iterator it1 = first, it2 = last;
    for (; it2 != end(); it2++)
      *it1++ = *it2;
    m_size -= last - first;
    return first;
  }
  constexpr ~vector() {
    std::destroy_n(m_p,m_size);
    if (0 != m_capacity) m_alloc.deallocate(m_p,m_capacity);
  }

  allocator_type  m_alloc;
  size_type       m_size     = 0;
  size_type       m_capacity = 0;
  value_type     *m_p        = nullptr;
};

} // namespace cest

#endif // _CEST_VECTOR_HPP_
