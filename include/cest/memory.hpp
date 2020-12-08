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

  constexpr unique_ptr(unique_ptr const&) = delete;
  constexpr unique_ptr& operator=(unique_ptr const&) = delete;

  constexpr unique_ptr() noexcept
    : ptr_(nullptr)
  {}

  constexpr unique_ptr(T* ptr) noexcept
    : ptr_(ptr)
  {}

  template<typename U>
  constexpr unique_ptr(unique_ptr<U>&& other) noexcept
    : unique_ptr(other.release())
  {}

  constexpr unique_ptr& operator=(unique_ptr&& other) noexcept
  {
    using std::swap;
    swap(other.ptr_, ptr_);
    return *this;
  }

  // Destructor

  constexpr ~unique_ptr() noexcept
  {
    if (ptr_)
      delete ptr_;
  }

  // Getters/Setters

  constexpr const pointer get() const noexcept { return ptr_; }

  constexpr pointer release() noexcept
  {
    auto ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }

  constexpr void reset(pointer n_ptr = pointer()) noexcept
  {
    auto ptr = ptr_;

    ptr_ = n_ptr;

    if (ptr)
      delete ptr;
  }

  constexpr element_type const& operator*() const noexcept
  {
    return *ptr_;
  }
  constexpr element_type& operator*() noexcept { return *ptr_; }
  constexpr pointer operator->() const noexcept { return ptr_; }

  constexpr operator bool() const noexcept { return bool(ptr_); }
};

//------------------------------------------------------------------------------
// Comparison operators

template<typename T>
constexpr bool
operator==(unique_ptr<T> const& a, unique_ptr<T> const& b) noexcept
{
  return a.get() == b.get();
}

template<typename T>
constexpr bool
operator!=(unique_ptr<T> const& a, unique_ptr<T> const& b) noexcept
{
  return a.get() != b.get();
}

template<typename T>
constexpr bool
operator<(unique_ptr<T> const& a, unique_ptr<T> const& b) noexcept
{
  return a.get() < b.get();
}

template<typename T>
constexpr bool
operator<=(unique_ptr<T> const& a, unique_ptr<T> const& b) noexcept
{
  return a.get() <= b.get();
}

template<typename T>
constexpr bool
operator>(unique_ptr<T> const& a, unique_ptr<T> const& b) noexcept
{
  return a.get() > b.get();
}

template<typename T>
constexpr bool
operator>=(unique_ptr<T> const& a, unique_ptr<T> const& b) noexcept
{
  return a.get() >= b.get();
}

template<typename T, typename... Args>
constexpr unique_ptr<T> make_unique( Args&&... args )
{
  return { new T{ std::forward<Args>(args)... } };
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
