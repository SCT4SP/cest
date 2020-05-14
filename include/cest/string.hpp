#ifndef _CEST_STRING_HPP_
#define _CEST_STRING_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/string_tests.hpp

#include <string>   // std::char_traits
#include <memory>   // std::allocator
#include <iterator> // std::reverse_iterator

namespace cest {

template <
  class CharT,
  class Traits    = std::char_traits<CharT>,
  class Allocator = std::allocator<CharT>
> class basic_string {
public:

  using traits_type         = Traits;
  using value_type          = CharT;
  using allocator_type      = Allocator;
  using size_type           = std::allocator_traits<Allocator>::size_type;
  using difference_type     = std::allocator_traits<Allocator>::difference_type;
  using reference           = value_type&;
  using const_reference     = const value_type&;
  using pointer             = std::allocator_traits<Allocator>::pointer;
  using const_pointer       = std::allocator_traits<Allocator>::const_pointer;
  using iterator            =       CharT*;
  using const_iterator      = const CharT*;
  using reverse_iterator    = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  static const size_type npos = static_cast<size_type>(-1);

  constexpr basic_string() noexcept(noexcept( Allocator() )) :
    basic_string( Allocator() ) {}

  explicit constexpr
  basic_string(const Allocator& alloc) noexcept : m_alloc(alloc) {
    m_capacity = 1;                                // Unlike 0, a scalable value
    m_p = m_alloc.allocate(m_capacity+1);          // +1 for the null terminator
    traits_type::assign(this->data()[0], CharT()); // m_p[0] = \0;
  }

  constexpr basic_string(const CharT *s, const Allocator &alloc = Allocator()) {
    const size_type sz = traits_type::length(s);
    m_capacity = sz+1;                    // ensure m_capacity is not 0
    m_p = m_alloc.allocate(m_capacity+1); // +1 for the null terminator
    traits_type::copy(m_p, s, sz); // traits_type supports user customisation
    traits_type::assign(this->data()[m_size = sz], CharT());
  }

  constexpr ~basic_string() { m_alloc.deallocate(m_p,m_capacity+1); }

  constexpr size_type        size() const          { return m_size;        }
  constexpr size_type      length() const          { return m_size;        }
  constexpr size_type    capacity() const          { return m_capacity;    }
  constexpr iterator        begin()                { return m_p;           }
  constexpr const_iterator  begin() const          { return m_p;           }
  constexpr const_iterator cbegin() const          { return m_p;           }
  constexpr iterator          end()       noexcept { return m_p + m_size;  }
  constexpr const_iterator    end() const noexcept { return m_p + m_size;  }
  constexpr const_iterator   cend() const noexcept { return m_p + m_size;  }
  constexpr bool            empty() const          { return m_size == 0;   }
  constexpr const CharT*    c_str() const noexcept { return m_p;           }
  constexpr const CharT*     data() const noexcept { return m_p;           }
  constexpr       CharT*     data()       noexcept { return m_p;           }
  constexpr       CharT&    front()                { return m_p[0];        }
  constexpr const CharT&    front() const          { return m_p[0];        }
  constexpr       CharT&     back()                { return m_p[m_size-1]; }
  constexpr const CharT&     back() const          { return m_p[m_size-1]; }

  constexpr void            clear()       noexcept {
    traits_type::assign(this->data()[m_size = 0], CharT());
  }
  constexpr void         pop_back()                {
    traits_type::assign(this->data()[--m_size],   CharT());
  }
  constexpr reference       operator[](size_type pos)       { return m_p[pos]; }
  constexpr const_reference operator[](size_type pos) const { return m_p[pos]; }

  constexpr
  size_type find(const basic_string &str, size_type pos = 0) const noexcept {
    return this->find(str.data(), pos);
  }

  constexpr size_type find(CharT ch, size_type pos = 0) const noexcept {
    auto *p = traits_type::find(this->data(), this->size(), ch);
    return p ? (p-this->data()) : npos;
  }

  constexpr
  size_type find(const CharT *s, size_type pos, size_type count) const noexcept
  {
    const size_type size = this->size();

    if (count == 0)
      return pos <= size ? pos : npos;
    if (pos >= size)
      return npos;

    const CharT       elem0 = s[0];
    const CharT* const data = this->data();
    const CharT*      first = data + pos;
    const CharT* const last = data + size;
    size_type           len = size - pos;

    while (len >= count)
    {
      first = traits_type::find(first, len - count + 1, elem0);
      if (!first)
        return npos;
      if (traits_type::compare(first, s, count) == 0)
        return first - data;
      len = last - ++first;
    }

    return npos;
  }

  constexpr size_type find(const CharT *s, size_type pos = 0) const noexcept {
     return this->find(s, pos, traits_type::length(s));
  }

  constexpr void push_back(const value_type &value) {
    if (m_size + 1 > m_capacity)
      reserve(m_capacity * 2);
    traits_type::assign(this->data()[  m_size], value);
    traits_type::assign(this->data()[++m_size], CharT()); // null terminator
  }

  constexpr basic_string& append(const CharT* s, size_type count) {
    const size_type len = count + this->size();

    if (len <= this->capacity())
    {
      if (count)
        traits_type::copy(this->data() + this->size(), s, count);
    }
    else {
      value_type *p = m_alloc.allocate(len+1);
      traits_type::copy(p, m_p, m_size);
      traits_type::copy(p + m_size, s, count);
      m_alloc.deallocate(m_p, m_capacity+1);
      m_p = p;
      m_size = len;
      m_capacity = len;
    }

    traits_type::assign(this->data()[m_size = len], CharT());
    return *this;
  }
  constexpr basic_string& append(const basic_string& str) {
    return this->append(str.data(), str.size());
  }

  constexpr basic_string& operator+=(const basic_string& str) {
    return this->append(str);
  }
  constexpr basic_string& operator+=(CharT ch) { push_back(ch); return *this; }

  constexpr void reserve(size_type new_cap)
  {
    if (new_cap > m_capacity)
    {
      value_type *p = m_alloc.allocate(new_cap+1); // for the null terminator
      traits_type::copy(p, m_p, m_size);
      m_alloc.deallocate(m_p, m_capacity+1);
      m_p = p;
      m_capacity = new_cap;
    }
  }

  friend constexpr bool operator==(const basic_string &lhs,
                                   const basic_string &rhs) noexcept {
    return lhs.length() == rhs.length() &&
           !traits_type::compare(lhs.data(), rhs.data(), lhs.length());
  }

  friend constexpr bool operator==(const CharT *lhs,
                                   const basic_string &rhs) noexcept {
    return traits_type::length(lhs) == rhs.length() &&
           !traits_type::compare(lhs, rhs.data(), traits_type::length(lhs));
  }

  allocator_type  m_alloc;
  size_type       m_size     = 0;
  size_type       m_capacity = 0;
  value_type     *m_p        = nullptr;
};

using string    = basic_string<char>;
using wstring   = basic_string<wchar_t>;
using u8string  = basic_string<char8_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;

} // namespace cest

#endif // _CEST_STRING_HPP_
