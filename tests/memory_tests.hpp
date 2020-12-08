#ifndef _CEST_MEMORY_TESTS_HPP_
#define _CEST_MEMORY_TESTS_HPP_

#include <cest/memory.hpp>
#include <memory>
#include <cassert>

template <template<typename...>typename U>
constexpr bool
constexpr_mem_test()
{
  // Construction
  U<int> v_int(new int(2));
  if (!v_int || *v_int != 2)
    return false;

  // Assign
  v_int = U<int>(new int(5));
  if (!v_int || *v_int != 5)
    return false;

  // Move
  auto v_int_moved = std::move(v_int);
  if (v_int || !v_int_moved || *v_int_moved != 5)
    return false;

  // Destruction
  bool some_bool = true;
  {
    struct destroy_me_t
    {
      bool& ref;
      constexpr ~destroy_me_t() { ref = false; }
    };
    destroy_me_t o{ some_bool };
  }
  if (some_bool)
    return false;

  // operator->
  {
    struct dummy_t
    {
      int val;
      constexpr int foo() const { return val; }
    };

    U<dummy_t> dummy_ptr(new dummy_t{42});
    if(dummy_ptr->foo() != 42)
      return false;
  }

  // TODO:
  // - const pointer get() const noexcept { return ptr_; }
  // - pointer release() noexcept
  // - void reset(pointer n_ptr = pointer()) noexcept
  // - element_type const& operator*() const noexcept
  // - element_type& operator*() noexcept { return *ptr_; }

  return true;
}

void
memory_tests()
{
#if CONSTEXPR_CEST == 1
  static_assert(constexpr_mem_test<cest::unique_ptr>(),
                "unique_ptr: Tests failed!");
#endif

  assert(constexpr_mem_test<std::unique_ptr>());
  assert(constexpr_mem_test<cest::unique_ptr>());
}

#endif
