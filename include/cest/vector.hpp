#ifndef _CEST_VECTOR_HPP_
#define _CEST_VECTOR_HPP_

#include <memory>  // std::allocator
#include <cstddef>
#include <algorithm>
#include <initializer_list>

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
  using pointer               = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer         = typename std::allocator_traits<Allocator>::const_pointer;
  using iterator              =       T*;
  using const_iterator        = const T*;
  using reverse_iterator      = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  constexpr vector() : m_size{}, m_capacity{}, m_p{} {}

  constexpr vector(const vector& other) : vector()
  {
    reserve(other.capacity());
    m_size = other.size();
    for (size_type i = 0; i < m_size; i++)
      std::construct_at(&m_p[i], other.m_p[i]);
  }

  constexpr vector(size_type count,
                   const Allocator& alloc = Allocator()) : vector()
  {
    reserve(count);
    m_size = count;
    for (size_type i = 0; i < m_size; i++)
      std::construct_at(&m_p[i]);
  }

  constexpr vector(std::initializer_list<T> init,
                   const Allocator& alloc = Allocator()) : vector()
  {
    for (const auto &x : init) { push_back(x); }
  }

  constexpr ~vector()
  {
    clear();
    if (0 != m_capacity) m_alloc.deallocate(m_p,m_capacity);
  }

  constexpr vector& operator=(const vector& other)
  {
    reserve(other.capacity());

    size_type i = 0;
    if (other.size() >= m_size) {
      for (; i < m_size; i++)
        m_p[i] = other.m_p[i];
      for (; i < other.size(); i++)
        std::construct_at(&m_p[i], other.m_p[i]);
    } else {
      for (; i < other.size(); i++)
        m_p[i] = other.m_p[i];
      std::destroy_n(&m_p[i], m_size - other.size());
    }

    m_size = other.size();
    return *this;
  }

  [[nodiscard]]
  constexpr bool            empty() const noexcept { return m_size == 0;  }
  constexpr size_type        size() const noexcept { return m_size;       }
  constexpr size_type    capacity() const noexcept { return m_capacity;   }
  constexpr void reserve(size_type new_cap)
  {
    if (new_cap > m_capacity)
    {
      value_type *p = m_alloc.allocate(new_cap);
      for (size_type i = 0; i < m_size; i++)
        std::construct_at(&p[i], m_p[i]);
      if (0 != m_capacity) {
        std::destroy_n(m_p,m_size);
        m_alloc.deallocate(m_p,m_capacity);
      }
      m_p = p;
      m_capacity = new_cap;
    }
  }

  constexpr void resize(size_type sz)
  {
    // likely not the most optimal implementation
    if (m_size < sz) {
      reserve(sz);
      // try to default fill the remaining elements that reserve has allocated
      for (size_type i = m_size; i < m_capacity; i++)
        std::construct_at(&m_p[i], value_type{});
      m_size = sz;
    } else if (m_size > sz) {
      for (int i = m_size; i > sz; i--)
        pop_back();
    }
  }

  constexpr iterator        begin()       noexcept { return {m_p};          }
  constexpr const_iterator  begin() const noexcept { return {m_p};          }
  constexpr const_iterator cbegin() const noexcept { return {m_p};          }
  constexpr iterator          end()       noexcept { return {m_p + m_size}; }
  constexpr const_iterator    end() const noexcept { return {m_p + m_size}; }
  constexpr const_iterator   cend() const noexcept { return {m_p + m_size}; }
  constexpr reference       front()                { return *begin();       }
  constexpr const_reference front() const          { return *begin();       }
  constexpr reference        back()                { return *(end()-1);     }
  constexpr const_reference  back() const          { return *(end()-1);     }

  constexpr reverse_iterator
  rbegin()       noexcept { return reverse_iterator(end()); }

  constexpr const_reverse_iterator
  rbegin() const noexcept { return const_reverse_iterator(end()); }

  constexpr reverse_iterator
  rend()         noexcept { return reverse_iterator(begin()); }

  constexpr const_reverse_iterator
  rend()   const noexcept { return const_reverse_iterator(begin()); }

  constexpr void         pop_back()       {
    m_size--;
    std::destroy_n(&m_p[m_size],1);
  }

  constexpr reference at(size_type pos) {
    if (pos >= m_size) {
      throw std::out_of_range("error: index out of range in vector::at");
    }
    return m_p[pos];
  }

  constexpr const_reference at(size_type pos) const {
    if (pos >= m_size) {
      throw std::out_of_range("error: index out of range in vector::at const");
    }
    return m_p[pos];
  }

  constexpr reference       operator[](size_type pos)       { return m_p[pos]; }
  constexpr const_reference operator[](size_type pos) const { return m_p[pos]; }

  constexpr void clear() noexcept
  {
    std::destroy_n(m_p, m_size);
    m_size = 0;
  }

  constexpr void push_back(const value_type &value)
  {
    if (0 == m_capacity) {
      reserve(1);
    } else if (m_capacity == m_size) {
      reserve(m_capacity * 2);
    }
    std::construct_at(&m_p[m_size++],value);
  }

  constexpr void push_back(value_type &&value)
  {
    if (0 == m_capacity) {
      reserve(1);
    } else if (m_capacity == m_size) {
      reserve(m_capacity * 2);
    }
    std::construct_at(&m_p[m_size++],std::forward<T>(value));
  }

  constexpr iterator erase(iterator first, iterator last)
  {
    iterator it1 = first, it2 = last;
    for (; it2 != end(); it2++) {
      *it1++ = *it2;
    }
    const auto num_to_erase = last - first;
    std::destroy_n(end() - num_to_erase, num_to_erase);
    m_size -= num_to_erase;
    return first;
  }

  allocator_type  m_alloc;
  size_type       m_size;
  size_type       m_capacity;
  value_type     *m_p;
};

template <typename _Tp, typename _Alloc>
constexpr bool
operator==(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y)
{ return (__x.size() == __y.size()
          && std::equal(__x.begin(), __x.end(), __y.begin())); }

} // namespace cest

#endif // _CEST_VECTOR_HPP_
