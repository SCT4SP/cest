#ifndef _CEST_STRING_HPP_
#define _CEST_STRING_HPP_

#include <string> // __cpp_lib_constexpr_string is defined in here

#include "ostream.hpp" // basic_ostream needed for ADL by operator<< below
#include "runtime_ostream.hpp"

#if defined(__cpp_lib_constexpr_string) && !defined(USE_CEST_VECTOR_ETC) && !defined(__clang__) && __GLIBCXX__ >= 20220000 && __GNUC__ >= 13

// 20220000  ? https://gcc.gnu.org/bugzilla/show_bug.cgi?id=103295 is resolved.
// __clang__ ? Bug 103295 still seems to bite.

namespace cest {

using std::basic_string;
using std::string;

} // namespace cest

#else // __cpp_lib_constexpr_string

#include <iterator>    // std::reverse_iterator
#include <limits>      // std::numeric_limits
#include <memory>      // std::allocator
#include <string>      // std::char_traits
#include <type_traits> // std::is_same_v

namespace cest {

template <class CharT, class Traits = std::char_traits<CharT>,
          class Allocator = std::allocator<CharT>>
class basic_string {
public:
  using traits_type = Traits;
  using value_type = CharT;
  using allocator_type = Allocator;
  using size_type = typename std::allocator_traits<Allocator>::size_type;
  using difference_type =
      typename std::allocator_traits<Allocator>::difference_type;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer =
      typename std::allocator_traits<Allocator>::const_pointer;
  using iterator = CharT *;
  using const_iterator = const CharT *;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  static const size_type npos = static_cast<size_type>(-1);

  constexpr basic_string() noexcept(noexcept(Allocator()))
      : basic_string(Allocator()) {}

  explicit constexpr basic_string(const Allocator &alloc) noexcept
      : m_alloc(alloc) {
    m_capacity = 1;                         // Unlike 0, a scalable value
    m_p = m_alloc.allocate(m_capacity + 1); // +1 for the null terminator
    for (size_type i = 0; i < m_capacity + 1; i++)
      std::construct_at(&m_p[i]);
    traits_type::assign(this->data()[0], CharT()); // m_p[0] = \0;
  }

  constexpr basic_string(const CharT *s, size_type count,
                         const Allocator &alloc = Allocator())
      : m_alloc(alloc) {
    m_capacity = count + 1;                 // ensure m_capacity is not 0
    m_p = m_alloc.allocate(m_capacity + 1); // +1 for the null terminator
    for (size_type i = 0; i < m_capacity + 1; i++)
      std::construct_at(&m_p[i]);
    traits_type::copy(m_p, s, count); // traits_type supports user customisation
    traits_type::assign(this->data()[m_size = count], CharT());
  }

  constexpr basic_string(const CharT *s, const Allocator &alloc = Allocator())
      : basic_string(s, traits_type::length(s), alloc) {}

  constexpr basic_string(const basic_string &str) : basic_string(str.c_str()) {}

  template <class InputIt>
  constexpr basic_string(InputIt __beg, InputIt __end,
                         const Allocator &alloc = Allocator())
      : m_alloc(alloc) {
    static_assert(std::is_same_v<
                  std::input_iterator_tag,
                  typename std::iterator_traits<InputIt>::iterator_category>);

    size_type __len = 0;
    size_type __capacity = size_type(m_capacity);

    while (__beg != __end && __len < __capacity) {
      data()[__len++] = *__beg;
      ++__beg;
    }

    while (__beg != __end) {
      if (__len == __capacity) {
        // Allocate more space.
        __capacity = (__len + 1) * 2;
        pointer __another = m_alloc.allocate(__capacity + 1);
        for (size_type i = 0; i < __capacity + 1; i++)
          std::construct_at(&__another[i]);
        traits_type::copy(__another, m_p, __len);
        m_alloc.deallocate(m_p, m_capacity + 1);
        m_p = __another;
        m_capacity = __capacity;
      }
      data()[__len++] = *__beg;
      ++__beg;
    }

    traits_type::assign(this->data()[m_size = __len], CharT());
  }

  constexpr ~basic_string() { m_alloc.deallocate(m_p, m_capacity + 1); }

  constexpr allocator_type get_allocator() const { return m_alloc; }

  constexpr size_type size() const { return m_size; }
  constexpr size_type length() const { return m_size; }
  constexpr size_type capacity() const { return m_capacity; }
  constexpr iterator begin() { return m_p; }
  constexpr const_iterator begin() const { return m_p; }
  constexpr const_iterator cbegin() const { return m_p; }
  constexpr iterator end() noexcept { return m_p + m_size; }
  constexpr const_iterator end() const noexcept { return m_p + m_size; }
  constexpr const_iterator cend() const noexcept { return m_p + m_size; }
  constexpr bool empty() const { return m_size == 0; }
  constexpr const CharT *c_str() const noexcept { return m_p; }
  constexpr const CharT *data() const noexcept { return m_p; }
  constexpr CharT *data() noexcept { return m_p; }
  constexpr CharT &front() { return m_p[0]; }
  constexpr const CharT &front() const { return m_p[0]; }
  constexpr CharT &back() { return m_p[m_size - 1]; }
  constexpr const CharT &back() const { return m_p[m_size - 1]; }

  constexpr void clear() noexcept {
    traits_type::assign(this->data()[m_size = 0], CharT());
  }
  constexpr void pop_back() {
    traits_type::assign(this->data()[--m_size], CharT());
  }
  constexpr reference operator[](size_type pos) { return m_p[pos]; }
  constexpr const_reference operator[](size_type pos) const { return m_p[pos]; }

  constexpr size_type find(const basic_string &str,
                           size_type pos = 0) const noexcept {
    return this->find(str.data(), pos);
  }

  constexpr size_type find(CharT ch, size_type /*pos*/ = 0) const noexcept {
    auto *p = traits_type::find(this->data(), this->size(), ch);
    return p ? (p - this->data()) : npos;
  }

  constexpr size_type find(const CharT *s, size_type pos,
                           size_type count) const noexcept {
    const size_type size = this->size();

    if (count == 0)
      return pos <= size ? pos : npos;
    if (pos >= size)
      return npos;

    const CharT elem0 = s[0];
    const CharT *const data = this->data();
    const CharT *first = data + pos;
    const CharT *const last = data + size;
    size_type len = size - pos;

    while (len >= count) {
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
    traits_type::assign(this->data()[m_size], value);
    traits_type::assign(this->data()[++m_size], CharT()); // null terminator
  }

  constexpr basic_string &append(const CharT *s, size_type count) {
    const size_type len = count + this->size();

    if (len <= this->capacity()) {
      if (count)
        traits_type::copy(this->data() + this->size(), s, count);
    } else {
      value_type *p = m_alloc.allocate(len + 1);
      for (size_type i = 0; i < len + 1; i++)
        std::construct_at(&p[i]);
      traits_type::copy(p, m_p, m_size);
      traits_type::copy(p + m_size, s, count);
      m_alloc.deallocate(m_p, m_capacity + 1);
      m_p = p;
      m_size = len;
      m_capacity = len;
    }

    traits_type::assign(this->data()[m_size = len], CharT());
    return *this;
  }
  constexpr basic_string &append(const basic_string &str) {
    return this->append(str.data(), str.size());
  }

  constexpr basic_string &operator+=(const basic_string &str) {
    return this->append(str);
  }
  constexpr basic_string &operator+=(CharT ch) {
    push_back(ch);
    return *this;
  }

  constexpr basic_string &operator=(const CharT *s) {
    // const CharT* s  = str.c_str();
    size_type count = traits_type::length(s);

    if (m_capacity < count + 1) {
      m_alloc.deallocate(m_p, m_capacity + 1);
      m_capacity = count + 1;                 // ensure m_capacity is not 0
      m_p = m_alloc.allocate(m_capacity + 1); // +1 for the null terminator
      for (size_type i = 0; i < m_capacity + 1; i++)
        std::construct_at(&m_p[i]);
    }

    traits_type::copy(m_p, s, count + 1);
    m_size = count;
    return *this;
  }

  constexpr basic_string &operator=(const basic_string &str) {
    return this->operator=(str.c_str());
  }

  constexpr void reserve(size_type new_cap) {
    if (new_cap > m_capacity) {
      value_type *p = m_alloc.allocate(new_cap + 1); // for the null terminator
      for (size_type i = 0; i < new_cap + 1; i++)
        std::construct_at(&p[i]);
      traits_type::copy(p, m_p, m_size + 1);
      m_alloc.deallocate(m_p, m_capacity + 1);
      m_p = p;
      m_capacity = new_cap;
    }
  }

  constexpr int compare(const CharT *s) const noexcept {
    const size_type size = this->size();
    const size_type osize = traits_type::length(s);
    const size_type len = std::min(size, osize);

    int r = traits_type::compare(this->data(), s, len);
    if (!r)
      r = _S_compare(size, osize);
    return r;
  }

  constexpr int compare(const basic_string &str) const {
    const size_type size = this->size();
    const size_type osize = str.size();
    const size_type len = std::min(size, osize);

    int r = traits_type::compare(this->data(), str.data(), len);
    if (!r) // traits_type::compare says equal, but possibly inequal length
      r = _S_compare(size, osize);
    return r;
  }

private:
  static constexpr int _S_compare(size_type n1, size_type n2) noexcept {
    const difference_type d = difference_type(n1 - n2);

    if (d > std::numeric_limits<int>::max())
      return std::numeric_limits<int>::max();
    else if (d < std::numeric_limits<int>::min())
      return std::numeric_limits<int>::min();
    else
      return int(d);
  }

  allocator_type m_alloc;
  size_type m_size = 0;
  size_type m_capacity = 0;
  value_type *m_p = nullptr;
};

template <typename _CharT, typename _Traits, typename _Alloc>
inline bool
operator==(const basic_string<_CharT, _Traits, _Alloc> &__lhs,
           const basic_string<_CharT, _Traits, _Alloc> &__rhs) noexcept {
  return __lhs.compare(__rhs) == 0;
}

template <typename _CharT>
inline constexpr std::enable_if_t<std::is_same_v<_CharT, char>, bool>
operator==(const basic_string<_CharT> &__lhs,
           const basic_string<_CharT> &__rhs) noexcept {
  return (__lhs.size() == __rhs.size() &&
          !std::char_traits<_CharT>::compare(__lhs.data(), __rhs.data(),
                                             __lhs.size()));
}
template <typename _CharT, typename _Traits, typename _Alloc>
inline constexpr bool
operator==(const basic_string<_CharT, _Traits, _Alloc> &__lhs,
           const _CharT *__rhs) {
  return __lhs.compare(__rhs) == 0;
}

template <typename _CharT, typename _Traits, typename _Alloc>
inline constexpr bool
operator==(const _CharT *__lhs,
           const basic_string<_CharT, _Traits, _Alloc> &__rhs) {
  return __rhs.compare(__lhs) == 0;
}

template <typename _CharT, typename _Traits, typename _Alloc>
inline constexpr bool
operator!=(const basic_string<_CharT, _Traits, _Alloc> &__lhs,
           const basic_string<_CharT, _Traits, _Alloc> &__rhs) noexcept {
  return !(__lhs == __rhs);
}

template <typename _CharT, typename _Traits, typename _Alloc>
inline constexpr bool
operator!=(const _CharT *__lhs,
           const basic_string<_CharT, _Traits, _Alloc> &__rhs) {
  return !(__lhs == __rhs);
}

template <typename _CharT, typename _Traits, typename _Alloc>
inline constexpr bool
operator!=(const basic_string<_CharT, _Traits, _Alloc> &__lhs,
           const _CharT *__rhs) {
  return !(__lhs == __rhs);
}

template <typename _CharT, typename _Traits, typename _Alloc>
inline constexpr bool
operator<(const basic_string<_CharT, _Traits, _Alloc> &__lhs,
          const basic_string<_CharT, _Traits, _Alloc> &__rhs) noexcept {
  return __lhs.compare(__rhs) < 0;
}

template <typename _CharT, typename _Traits, typename _Alloc>
inline constexpr bool
operator<(const basic_string<_CharT, _Traits, _Alloc> &__lhs,
          const _CharT *__rhs) {
  return __lhs.compare(__rhs) < 0;
}

template <typename _CharT, typename _Traits, typename _Alloc>
inline constexpr bool
operator<(const _CharT *__lhs,
          const basic_string<_CharT, _Traits, _Alloc> &__rhs) {
  return __rhs.compare(__lhs) > 0;
}

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;
using u8string = basic_string<char8_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;

} // namespace cest

#endif // __cpp_lib_constexpr_string

namespace cest {

// This operator will also be used when __cpp_lib_constexpr_string is defined.
// e.g. The first argument could be cest::cout. This has type cest::ostream,
// which is defined in the cest namespace, and so ADL will find this operator.
// So cest::cout << cest::string{"ok"} works, even when the alias definition of
// cest::string (above) means this is really cest::cout << std::string{"ok"}
template <class CharT, class Traits, class Allocator>
constexpr basic_ostream<CharT, Traits> &
operator<<(basic_ostream<CharT, Traits> &os,
           const basic_string<CharT, Traits, Allocator> &str) {
  impl::runtime_ostream(os, str.c_str());
  return os;
}

}

#endif // _CEST_STRING_HPP_
