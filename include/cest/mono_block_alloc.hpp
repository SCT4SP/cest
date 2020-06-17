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
  constexpr mba_base(const std::size_t cap) : m_cap(cap), m_size(), m_p()
  {
#ifndef CE
    std::cout << "mba_base<" <<
       typeid(T).name() << ">::mba_base()\n";
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

  template <class U>
  constexpr mono_block_alloc(const mono_block_alloc <U>&) noexcept {}
 
  [[nodiscard]]
  constexpr T* allocate(std::size_t n)
  {
    if (mba_base<T>::m_size + n > mba_base<T>::m_cap)
      throw std::bad_alloc();

    T* ret = mba_base<T>::m_p + mba_base<T>::m_size;
    mba_base<T>::m_size += n;

#ifndef CE
    std::cout << "mono_block_alloc<" << typeid(T).name() <<
      ">::allocate(" << n << ") " << ret << ' ' << sizeof(T) << '\n';
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
