#ifndef _CEST_MEMORY_HPP_
#define _CEST_MEMORY_HPP_

#include <type_traits>

#include <cest/iostream.hpp>
#include <cest/swap.hpp>

namespace cest {

//------------------------------------------------------------------------------
// Class definitions

template<typename T>
class unique_ptr
{
  T* ptr_;

public:
  // Type aliases

  using pointer = T*;
  using element_type = T;

  // Constructors

  inline constexpr unique_ptr(unique_ptr const&) = delete;
  inline constexpr unique_ptr& operator=(unique_ptr const&) = delete;

  inline constexpr unique_ptr() noexcept
    : ptr_(nullptr)
  {}

  inline constexpr unique_ptr(T* ptr) noexcept
    : ptr_(ptr)
  {}

  inline constexpr unique_ptr(unique_ptr&& other) noexcept
    : unique_ptr()
  {
    using std::swap;
    swap(other.ptr_, ptr_);
  };

  inline constexpr unique_ptr& operator=(unique_ptr&& other) noexcept
  {
    using std::swap;
    swap(other.ptr_, ptr_);
    return *this;
  };

  // Destructor

  inline constexpr ~unique_ptr() noexcept
  {
    if (ptr_)
      delete ptr_;
  }

  // Getters/Setters

  inline constexpr const pointer get() const noexcept { return ptr_; }

  inline constexpr pointer release() noexcept
  {
    auto ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }

  inline constexpr void reset(pointer n_ptr = pointer()) noexcept
  {
    auto ptr = ptr_;

    ptr_ = n_ptr;

    if (ptr)
      delete ptr;
  }

  inline constexpr element_type const& operator*() const noexcept
  {
    return *ptr_;
  }
  inline constexpr element_type& operator*() noexcept { return *ptr_; }

  inline constexpr operator bool() const noexcept { return bool(ptr_); }
};

//------------------------------------------------------------------------------
// Comparison operators

template<typename T>
inline constexpr bool
operator==(unique_ptr<T> const& a, unique_ptr<T> const& b) noexcept
{
  return a.get() == b.get();
}

template<typename T>
inline constexpr bool
operator!=(unique_ptr<T> const& a, unique_ptr<T> const& b) noexcept
{
  return a.get() != b.get();
}

template<typename T>
inline constexpr bool
operator<(unique_ptr<T> const& a, unique_ptr<T> const& b) noexcept
{
  return a.get() < b.get();
}

template<typename T>
inline constexpr bool
operator<=(unique_ptr<T> const& a, unique_ptr<T> const& b) noexcept
{
  return a.get() <= b.get();
}

template<typename T>
inline constexpr bool
operator>(unique_ptr<T> const& a, unique_ptr<T> const& b) noexcept
{
  return a.get() > b.get();
}

template<typename T>
inline constexpr bool
operator>=(unique_ptr<T> const& a, unique_ptr<T> const& b) noexcept
{
  return a.get() >= b.get();
}

//------------------------------------------------------------------------------
// Stream operator

template<typename T>
constexpr cest::ostream&
operator<<(cest::ostream& o, unique_ptr<T> const& p)
{
  if (!std::is_constant_evaluated())
    return o << p.get();
  return o;
}

} // namespace cest

#endif // _CEST_MEMORY_HPP_
