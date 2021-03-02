#ifndef _CEST_FUNCTIONAL_HPP_
#define _CEST_FUNCTIONAL_HPP_

#include "memory.hpp"

#include <type_traits>

namespace cest 
{
  // Based on the 2nd answer: 
  // https://stackoverflow.com/questions/53977787/constexpr-version-of-stdfunction  
  // very useful, thank you!
  template <typename R, typename... Args>
  class virt_impl_f {
    public: 
      constexpr virtual R operator()(Args... args) = 0;
      constexpr virtual ~virt_impl_f() = default;
  };

  template <typename F, typename R, typename... Args> 
  class impl_f : public virt_impl_f<R , Args...> {
    public: 
      constexpr impl_f(F&& f) : f_(f) {}
      
      constexpr virtual R operator()(Args... args) override { return f_(std::forward<Args>(args)...); }
      
      F f_;
  };
  
  template <typename F, typename... Args> 
  class impl_f<F, void, Args...> : public virt_impl_f<void, Args...> {
    public: 
      constexpr impl_f(F&& f) : f_(f) {}
      
      constexpr virtual void operator()(Args... args) override { f_(std::forward<Args>(args)...); }
      
      F f_;
  };
  

  template <typename>
  class function;

  template <typename R, typename... Args>
  class function<R (Args...)> {
      cest::unique_ptr<virt_impl_f<R, Args...>> f_;
      
    public:
      constexpr R operator()(Args... args) {
        return f_->operator()(std::forward<Args>(args)...);
      }

      template<typename F>
      constexpr function(F&& f) : 
        f_(cest::make_unique<impl_f<std::decay_t<F>, 
                             R, Args...>>(std::move(f))){}
  };

  template <typename... Args>
  class function<void (Args...)> {
      cest::unique_ptr<virt_impl_f<void, Args...>> f_;

    public:
      constexpr void operator()(Args... args) {
        return f_->operator()(std::forward<Args>(args)...);
      }

      template<typename F>
      constexpr function(F&& f) : 
        f_(cest::make_unique<impl_f<std::decay_t<F>, 
                             void, Args...>>(std::move(f))){}
  };
  
  // Deduction guide borrowed from libcxx
  template<class R, class ...Args>
  function(R(*)(Args...)) -> function<R(Args...)>; // since C++17
}

#endif
