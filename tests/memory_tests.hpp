#ifndef _CEST_MEMORY_TESTS_HPP_
#define _CEST_MEMORY_TESTS_HPP_

#include <cest/iostream.hpp>
#include <cest/memory.hpp>

constexpr bool
constexpr_mem_test()
{
  // Construction
  cest::unique_ptr<int> v_int(new int(2));
  if (!v_int || *v_int != 2)
    return false;

  // Assign
  v_int = cest::unique_ptr<int>(new int(5));
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
  static_assert(constexpr_mem_test(), "unique_ptr: Tests failed!");
  return;
}

#endif
