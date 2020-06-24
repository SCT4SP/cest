#ifndef _MONO_BLOCK_STACK_ALLOC_HPP_
#define _MONO_BLOCK_STACK_ALLOC_HPP_

#include <cstddef>  // size_t
#include <memory>   // allocator
#include <typeinfo> // typeid::name()
#include <iostream> // <<

namespace cea {

template <typename T, std::size_t C = 1024>
struct mbsa_base
{
  constexpr mbsa_base() : m_size()
  {
#ifdef CE_DEBUG
    if (!std::is_constant_evaluated())
      std::cout << "mbsa_base<" <<
         typeid(T).name() << ">::mbsa_base(" << cap << ")\n";
#endif
    static_assert(C <= std::numeric_limits<std::size_t>::max() / sizeof(T));
  }

  constexpr ~mbsa_base() { }

  std::size_t m_size;
  T m_p[C];
};

template <class T, std::size_t C = 1024>
struct mbsa : mbsa_base<T>
{
  typedef T value_type;

  // Added as std::allocator_traits<IntAlloc>::rebind_alloc needs some help
  // due to the presence of a second (nontype?) template parameter.
  template <typename U>
  struct rebind {
    using other = mbsa<U>;
  };
 
  constexpr mbsa() : mbsa_base<T,C>() {}

  // marked as noexcept, but it can throw
  constexpr mbsa(const mbsa &a) noexcept : mbsa_base<T,C>()
  {
#ifdef CE_DEBUG
    assert(); // when is this really called?
    std::cout << "mbsa<" << typeid(T).name() << ">::mbsa(const mbsa&)\n";
#endif
  }

  // this needs more thought:
  template <class U>
  constexpr mbsa(const mbsa <U>& a) noexcept : mbsa_base<T,C>() {

#ifdef CE_DEBUG
    assert(); // when is this really called?
    std::cout << "template <class U> mbsa<" <<
       typeid(T).name() << ">::mbsa(const mbsa <" <<
       typeid(U).name() << ">&)\n";
#endif
  }
 
  [[nodiscard]]
  constexpr T* allocate(std::size_t n)
  {
    if (mbsa_base<T>::m_size + n > C)
      throw std::bad_alloc();

    T* ret = &mbsa_base<T>::m_p[mbsa_base<T>::m_size];
    mbsa_base<T>::m_size += n;

#ifdef CE_DEBUG
    if (!std::is_constant_evaluated())
      std::cout << "mbsa<" << typeid(T).name() <<
        ">::allocate(" << n << ") " << ret << ' ' << n*sizeof(T) << " bytes\n";
#endif

    return ret;
  }

  constexpr void deallocate(T* p, std::size_t n) noexcept {}
};

template <class T, class U>
bool operator==(const mbsa <T>&, const mbsa <U>&) { return true;  }
template <class T, class U>
bool operator!=(const mbsa <T>&, const mbsa <U>&) { return false; }

} // namespace cea

#endif // _MONO_BLOCK_STACK_ALLOC_HPP_
