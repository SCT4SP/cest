#ifndef _MONO_BLOCK_ALLOC_HPP_
#define _MONO_BLOCK_ALLOC_HPP_

#include <cstddef>  // size_t
#include <memory>   // allocator
#include <typeinfo> // typeid::name()
#include <iostream> // <<

namespace cea {

template <typename T>
struct mba_base
{
  constexpr mba_base() : m_cap(0), m_size(), m_p() {} // no
  constexpr mba_base(const std::size_t cap) : m_cap(cap), m_size(), m_p()
  {
#ifdef CE_DEBUG
    if (!std::is_constant_evaluated())
      std::cout << "mba_base<" <<
         typeid(T).name() << ">::mba_base(" << cap << ")\n";
#endif
    if (cap > std::numeric_limits<std::size_t>::max() / sizeof(T))
      throw std::bad_alloc();

    std::allocator<T> alloc;
    m_p = alloc.allocate(m_cap);
 
    if (!m_p) throw std::bad_alloc();
  }

  constexpr ~mba_base()
  {
    std::allocator<T> alloc;
    alloc.deallocate(m_p, m_cap);
  }

  const std::size_t m_cap;
        std::size_t m_size;
  T*                m_p;
};

template <class T>
struct mono_block_alloc : mba_base<T>
{
  typedef T value_type;
 
  constexpr
  mono_block_alloc(const std::size_t capacity = 1024) : mba_base<T>(capacity) {}

  // this needs more thought:
  template <class U>
  constexpr mono_block_alloc(const mono_block_alloc <U>&) noexcept :
    mba_base<T>() {
#ifdef CE_DEBUG
      std::cout << "mono_block_alloc<" <<
         typeid(T).name() << ">::mono_block_alloc(const mono_block_alloc <" <<
         typeid(U).name() << ">&)\n";
#endif
  }
 
  [[nodiscard]]
  constexpr T* allocate(std::size_t n)
  {
    if (mba_base<T>::m_size + n > mba_base<T>::m_cap)
      throw std::bad_alloc();

    T* ret = mba_base<T>::m_p + mba_base<T>::m_size;
    mba_base<T>::m_size += n;

#ifdef CE_DEBUG
    if (!std::is_constant_evaluated())
      std::cout << "mono_block_alloc<" << typeid(T).name() <<
        ">::allocate(" << n << ") " << ret << ' ' << n*sizeof(T) << " bytes\n";
#endif

    return ret;
  }

  constexpr void deallocate(T* p, std::size_t n) noexcept {}
};

template <class T, class U>
bool operator==(const mono_block_alloc <T>&,
                const mono_block_alloc <U>&) { return true;  }
template <class T, class U>
bool operator!=(const mono_block_alloc <T>&,
                const mono_block_alloc <U>&) { return false; }

} // namespace cea

#endif // _MONO_BLOCK_ALLOC_HPP_
