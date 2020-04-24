#ifndef _CE_VECTOR_HPP_
#define _CE_VECTOR_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/vector_tests.hpp

#include <memory>
#include <cstddef>

namespace cest {

template <
  class T,
  class Allocator = std::allocator<T>
>
class vector {
public:
  using value_type      = T;
  using allocator_type  = Allocator;
  using size_type       = std::size_t;
  using iterator        = T*;
  using const_iterator  = const iterator;
  using reference       = value_type&;
  using const_reference = const value_type&;

  constexpr vector() : m_p(nullptr), m_size(0), m_capacity(0) {}
  constexpr size_type        size() const { return m_size;       }
  constexpr size_type    capacity() const { return m_capacity;   }
  constexpr iterator        begin()       { return m_p;          }
  constexpr const_iterator  begin() const { return m_p;          }
  constexpr const_iterator cbegin() const { return m_p;          }
  constexpr iterator          end()       { return m_p + m_size; }
  constexpr bool            empty() const { return m_size == 0;  }
  constexpr void         pop_back()       { m_size--; }
  constexpr reference       operator[](size_type pos)       { return m_p[pos]; }
  constexpr const_reference operator[](size_type pos) const { return m_p[pos]; }
	
  constexpr void push_back(const T &value) {
    if (0 == m_capacity) {
      reserve(1);
    } else if (m_capacity == m_size) {
      reserve(m_capacity * 2);
    }
    std::construct_at(&m_p[m_size++],value);
  }
  constexpr void push_back(T &&value) {
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
      T *p = m_alloc.allocate(new_cap);
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

  size_type m_size, m_capacity;
  allocator_type m_alloc;
  T *m_p;
};

} // namespace cest

#endif // _CE_VECTOR_HPP_
