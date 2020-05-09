#ifndef _CEST_VECTOR_HPP_
#define _CEST_VECTOR_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/vector_tests.hpp

#include <memory>  // std::allocator
#include <cstddef>

namespace cest {

// Based on GNU libstdc++ class __normal_iterator from stl_iterator.h
template <typename I, typename C>
class iter {
protected:
  I curr;

public:
  using iterator_category = std::iterator_traits<I>::iterator_category;
  using value_type        = std::iterator_traits<I>::value_type;
  using difference_type   = std::iterator_traits<I>::difference_type;
  using reference         = std::iterator_traits<I>::reference;
  using pointer           = std::iterator_traits<I>::pointer;

  constexpr iter() noexcept : curr(I()) { }
  explicit constexpr iter(const I &i) noexcept : curr(i) { }

  // Allow iterator to const_iterator conversion
  template <typename I2>
  constexpr iter(const iter<
                         I2,
                         std::enable_if_t<
                           std::is_same_v<I2, typename C::pointer>,
                           C
                         >
                       > &i) noexcept : curr(i.base()) { }

  // Forward iterator requirements
  constexpr reference  operator *() const noexcept { return *curr;         }
  constexpr   pointer  operator->() const noexcept { return curr;          }
  constexpr      iter &operator++()       noexcept { ++curr; return *this; }
  constexpr      iter  operator++(int)    noexcept { return iter(curr++);  }

  // Bidirectional iterator requirements
  constexpr      iter &operator--()       noexcept { --curr; return *this; }
  constexpr      iter  operator--(int)    noexcept { return iter(curr--);  }

  // Random access iterator requirements
  constexpr  reference operator[](difference_type n) const noexcept {
    return curr[n];
  }

  constexpr      iter &operator+=(difference_type n)       noexcept {
    curr += n; return *this;
  }

  constexpr      iter  operator +(difference_type n) const noexcept {
    return iter(curr + n);
  }

  constexpr      iter &operator-=(difference_type n)       noexcept {
    curr -= n; return *this;
  }

  constexpr       iter operator -(difference_type n) const noexcept {
    return iter(curr - n);
  }

  constexpr const I &base() const noexcept { return curr; }
};

// The two iterators are permitted to vary in type by cv-qualification
template <typename IL, typename IR, typename C>
constexpr bool operator==(const iter<IL, C> &l, const iter<IR, C> &r) noexcept {
  return l.base() == r.base();
}

template<typename IL, typename IR, typename C>
constexpr auto  operator-(const iter<IL, C> &l, const iter<IR, C> &r) noexcept
  -> decltype(l.base() - r.base()) {
  return l.base() - r.base();
}

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
  using iterator              = cest::iter<pointer,vector>; // T*;
  using const_iterator        = cest::iter<const_pointer,vector>; // const T*;
  using reverse_iterator      = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  constexpr vector() : m_p(nullptr), m_size(0), m_capacity(0) {}
  constexpr size_type        size() const { return m_size;       }
  constexpr size_type    capacity() const { return m_capacity;   }
  constexpr iterator        begin()       { return iterator(m_p);        }
  constexpr const_iterator  begin() const { return const_iterator(m_p);          }
  constexpr const_iterator cbegin() const { return const_iterator(m_p);       }
  constexpr iterator          end()       { return iterator(m_p + m_size); }
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

  size_type m_size, m_capacity;
  allocator_type m_alloc;
  value_type *m_p;
};

} // namespace cest

#endif // _CEST_VECTOR_HPP_
