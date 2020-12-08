#ifndef _CEST_SHARED_PTR_HPP_
#define _CEST_SHARED_PTR_HPP_

#include <type_traits>

#include "cest/iostream.hpp"
#include "cest/swap.hpp"

namespace cest {

namespace impl {

template <typename T>
class shared_ptr_base
{
public:
  using element_type = std::remove_extent_t<T>;
  using weak_type = std::weak_ptr<T>;

  constexpr shared_ptr_base() noexcept {}
  constexpr shared_ptr_base(std::nullptr_t) noexcept {}

  template <class Y>
  explicit constexpr shared_ptr_base(Y* ptr)
    : ptr_{ptr}, p_use_count_{new long{1}} {}

  template <class Y, class Deleter>
  constexpr shared_ptr_base(Y* ptr, Deleter del)
    : ptr_{ptr}, del_{del} {}

  constexpr shared_ptr_base(const shared_ptr_base& other) noexcept
    : ptr_{other.ptr_}, p_use_count_{&++*other.p_use_count_}, del_{other.del_}{}

  constexpr ~shared_ptr_base()
  {
    if (--*p_use_count_) return; // return: other references still exist
    delete p_use_count_;
    del_ ? del_(ptr_) : delete ptr_;
  }

  constexpr long use_count() const noexcept { return *p_use_count_; }

  constexpr element_type* get() const noexcept { return ptr_; }

  constexpr T& operator*() const noexcept { return *ptr_; }
  constexpr T* operator->() const noexcept { return ptr_; }

  constexpr operator bool() const noexcept { return ptr_; }

  template <class Deleter, class U>
  friend constexpr Deleter* get_deleter(const shared_ptr_base<U>&) noexcept;

private:
  T* ptr_{};
  long* p_use_count_{};
  void(*del_)(T*){}; // todo
};

} // namespace impl

template <typename T>
class shared_ptr : public impl::shared_ptr_base<std::remove_extent_t<T>>
{
  using base_t = impl::shared_ptr_base<std::remove_extent_t<T>>;

public:

  using element_type = typename base_t::element_type;
  using base_t::base_t;
};

template <typename T>
class shared_ptr<T[]> : public impl::shared_ptr_base<T>
{
  using base_t = impl::shared_ptr_base<std::remove_extent_t<T>>;

public:

  using element_type = typename base_t::element_type;
  using base_t::base_t;

  constexpr element_type& operator[](std::ptrdiff_t i) const {
    return this->get()[i];
  }

  template <class Deleter, class U>
  friend constexpr Deleter* get_deleter(const shared_ptr<U>&) noexcept;
};

template <class Deleter, class U>
constexpr Deleter* get_deleter(const shared_ptr<U>& p) noexcept {
  return p.del_;
}

template<typename T>
constexpr cest::ostream&
operator<<(cest::ostream& o, shared_ptr<T> const& p)
{
  if (!std::is_constant_evaluated())
    return o << p.get();
  return o;
}

} // namespace cest

#endif // _CEST_SHARED_PTR_HPP_
