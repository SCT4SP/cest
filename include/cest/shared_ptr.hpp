#ifndef _CEST_SHARED_PTR_HPP_
#define _CEST_SHARED_PTR_HPP_

#include <type_traits>

#include "cest/iostream.hpp"
#include "cest/swap.hpp"

namespace cest {

template <class T>
struct default_delete
{
  constexpr default_delete() noexcept = default;

  template <class U>
  constexpr default_delete(const default_delete<U>& d) noexcept {}

  constexpr void operator()(T* ptr) const { delete ptr; }
};

template <class T>
struct default_delete<T[]>
{
  constexpr default_delete() noexcept = default;

  template<class U>
  constexpr default_delete(const default_delete<U[]>& d) noexcept {}

  template <class U>
  constexpr void operator()(U* ptr) const { delete [] ptr; }
};

namespace impl {

template <typename T>
class shared_ptr_base
{
  struct ctrl
  {
    constexpr ctrl() : count_(1) {}
    virtual constexpr void destroy() = 0;
    virtual constexpr ~ctrl() {}

    long count_;
  };

  template <class U, class Deleter>
  struct ctrl_derived : public ctrl
  {
    constexpr ctrl_derived(U* ptr, Deleter del) : ptr_{ptr}, del_{del} {}
    virtual constexpr void destroy() { del_(ptr_); }

    U* ptr_;
    Deleter del_;
  };

public:
  using element_type = std::remove_extent_t<T>;
  using weak_type = std::weak_ptr<T>;

  constexpr shared_ptr_base() noexcept {}
  constexpr shared_ptr_base(std::nullptr_t) noexcept {}

  template <class Y>
  explicit constexpr shared_ptr_base(Y* ptr)
    : ptr_{ptr}, pctrl_{new ctrl_derived<Y,default_delete<T>>{ptr,default_delete<T>{}}} {}

  template <class Y, class Deleter>
  constexpr shared_ptr_base(Y* ptr, Deleter del)
    : ptr_{ptr}, pctrl_{new ctrl_derived<Y,Deleter>{ptr,del}} {}

  constexpr shared_ptr_base(const shared_ptr_base& other) noexcept
    : ptr_{other.ptr_}, pctrl_{other.pctrl_} { ++pctrl_->count_; }

  constexpr ~shared_ptr_base()
  {
    if (pctrl_ && !--pctrl_->count_)
    {
      pctrl_->destroy();
      delete pctrl_;
    }
  }

  constexpr long use_count() const noexcept {
    return pctrl_ ? pctrl_->count_ : 0;
  }

  constexpr element_type* get() const noexcept { return ptr_; }

  constexpr T& operator*() const noexcept { return *ptr_; }
  constexpr T* operator->() const noexcept { return ptr_; }

  constexpr operator bool() const noexcept { return ptr_; }

  template <class Deleter, class U>
  friend constexpr Deleter* get_deleter(const shared_ptr_base<U>&) noexcept;

private:
  T* ptr_{};
  ctrl *pctrl_{};
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
