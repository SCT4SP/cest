#ifndef _CEST_VECTOR_HPP_
#define _CEST_VECTOR_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/vector_tests.hpp

#include <memory>  // std::allocator
#include <cstddef>
#include <algorithm>

namespace CEST_NAMESPACE {

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

  [[nodiscard]]
  constexpr bool            empty() const noexcept { return m_size == 0;  }
  constexpr size_type        size() const noexcept { return m_size;       }
  constexpr size_type    capacity() const noexcept { return m_capacity;   }
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

  constexpr iterator        begin()       noexcept { return {m_p};          }
  constexpr const_iterator  begin() const noexcept { return {m_p};          }
  constexpr const_iterator cbegin() const noexcept { return {m_p};          }
  constexpr iterator          end()       noexcept { return {m_p + m_size}; }
  constexpr const_iterator    end() const noexcept { return {m_p + m_size}; }
  constexpr const_iterator   cend() const noexcept { return {m_p + m_size}; }

  constexpr reverse_iterator
  rbegin()       noexcept { return reverse_iterator(end()); }

  constexpr const_reverse_iterator
  rbegin() const noexcept { return const_reverse_iterator(end()); }

  constexpr reverse_iterator
  rend()         noexcept { return reverse_iterator(begin()); }

  constexpr const_reverse_iterator
  rend()   const noexcept { return const_reverse_iterator(begin()); }

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

template <typename _Tp, typename _Alloc>
constexpr bool
operator==(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
{ return (__x.size() == __y.size()
          && std::equal(__x.begin(), __x.end(), __y.begin())); }

} // namespace cest

#endif // _CEST_VECTOR_HPP_
