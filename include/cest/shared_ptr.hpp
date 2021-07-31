#ifndef _CEST_SHARED_PTR_HPP_
#define _CEST_SHARED_PTR_HPP_

// TODO: Sharing subobjects
// https://www.justsoftwaresolutions.co.uk/cplusplus/shared-ptr-secret-constructor.html

#include <type_traits>

#include "cest/iostream.hpp"
#include "cest/swap.hpp"

namespace cest {

template <class T>
struct default_delete
{
  constexpr default_delete() noexcept = default;

  template <class U>
  constexpr default_delete(const default_delete<U>&) noexcept {}

  constexpr void operator()(T* ptr) const { delete ptr; }
};

template <class T>
struct default_delete<T[]>
{
  constexpr default_delete() noexcept = default;

  template<class U>
  constexpr default_delete(const default_delete<U[]>&) noexcept {}

  template <class U>
  constexpr void operator()(U* ptr) const { delete [] ptr; }
};

template <typename>
  class shared_ptr;

namespace impl {

template <typename T, typename Td>
class shared_ptr_base
{
protected:
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
    virtual constexpr void destroy() override { del_(ptr_); }

    U* ptr_;
    Deleter del_;
  };

  template <typename U>
  struct def_del : default_delete<U> {};

public:
  using element_type = std::remove_extent_t<T>;
  using weak_type = std::weak_ptr<T>;

  constexpr shared_ptr_base() noexcept {}
  constexpr shared_ptr_base(std::nullptr_t) noexcept {}

  template <class Y>
  explicit constexpr shared_ptr_base(Y* ptr)
    : ptr_{ptr}, pctrl_{new ctrl_derived<Y,def_del<Td>>{ptr,def_del<Td>{}}} {}

  template <class Y, class Deleter>
  constexpr shared_ptr_base(Y* ptr, Deleter del)
    : ptr_{ptr}, pctrl_{new ctrl_derived<Y,Deleter>{ptr,del}} {}

  constexpr shared_ptr_base(const shared_ptr_base& r) noexcept
    : ptr_{r.ptr_}, pctrl_{r.pctrl_} { ++pctrl_->count_; }

  template <class Y>
  constexpr shared_ptr_base(const shared_ptr<Y>& r, element_type* ptr) noexcept
    : ptr_{ptr}, pctrl_{r.pctrl_} { ++pctrl_->count_; }

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

  explicit constexpr operator bool() const noexcept { return ptr_; }

  template <class Deleter, class U>
  friend constexpr Deleter* get_deleter(const shared_ptr<U>&) noexcept;

protected:
  T* ptr_{};
  ctrl *pctrl_{};
};

template <class Deleter, class U>
constexpr Deleter* get_deleter(const shared_ptr<U>& p) noexcept
{
  using elem_t = typename shared_ptr<U>::element_type;

  if constexpr (std::is_invocable_v<Deleter,elem_t*>)
  {
    using ctrl_derived_t =
      typename shared_ptr<U>::template ctrl_derived<elem_t,Deleter>;
    auto ptr = dynamic_cast<ctrl_derived_t*>(p.pctrl_);
    return ptr ? &ptr->del_ : nullptr;
  } 
  
  return nullptr;
}

} // namespace impl

template <typename T>
class shared_ptr : public impl::shared_ptr_base<T,T>
{
  using base_t = impl::shared_ptr_base<T,T>;

public:

  using element_type = typename base_t::element_type;
  using weak_type = typename base_t::weak_type;
  using base_t::base_t;
};

template <typename T>
class shared_ptr<T[]> : public impl::shared_ptr_base<T,T[]>
{
  using base_t = impl::shared_ptr_base<T,T[]>;

public:

  using element_type = typename base_t::element_type;
  using weak_type = typename base_t::weak_type;
  using base_t::base_t;

  constexpr element_type& operator[](std::ptrdiff_t i) const {
    return this->get()[i];
  }
};

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
