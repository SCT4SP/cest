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

constexpr bool make_unique_test()
{
  // Virtual polymorphism

  struct a_t {
    constexpr virtual int foo() { return 0; }
    constexpr virtual ~a_t() = default;
  };

  struct b_t : a_t {
    constexpr int foo() override { return 1; }
  };

  cest::unique_ptr<a_t> a_v = cest::make_unique<a_t>();
  cest::unique_ptr<a_t> b_v = cest::make_unique<b_t>();

  if(a_v->foo() != 0 || b_v->foo() != 1)
    return false;

  // Forwarding

  class m_t
  {
    int v_;
  public:
    constexpr m_t(int && v): v_(v) {}
    constexpr int get_v() { return v_; }
  };

  auto m_v = cest::make_unique<m_t>(2);
  if(m_v->get_v() != 2)
    return false;

  return true;
}

template <template <typename ...> typename St>
constexpr bool shared_ptr_test()
{
  St<int> sp1{new int{123}};
  auto sp2 = sp1;
  bool b1 = 123==*sp1 && 123==*sp1.get() && 123==*sp2 && 123==*sp2.get();
  b1 = b1 && 2==sp1.use_count() && 2==sp2.use_count();

  /*St<int[]> spa1{new int[4]{1,2,3,4}};
  auto spa2 = spa1;
  bool b2 = 1==spa1[0] && 2==spa1[1] && 3==spa1[2] && 4==spa1[3];
  bool b3 = 1==spa2[0] && 2==spa2[1] && 3==spa2[2] && 4==spa2[3];
*/
  int i{41};
  int *p = new int{123};
  {
    struct del { constexpr void operator()(int*) { ++i_; } int& i_; };
    del d{i};
    St<int> sp3{p, d};
  }
  bool b4 = 42==i;
  delete p;

  return b1 && /*b2 && b3 &&*/ b4;
}

void
memory_tests()
{
#if CONSTEXPR_CEST == 1
  static_assert(constexpr_mem_test<cest::unique_ptr>(),
                "unique_ptr: Tests failed!");
  static_assert(make_unique_test(), "make_unique: Tests failed!");
  static_assert(shared_ptr_test<cest::shared_ptr>());
#endif

  assert(constexpr_mem_test<std::unique_ptr>());
  assert(constexpr_mem_test<cest::unique_ptr>());
  assert(make_unique_test());

  static_assert(std::is_same_v<std::shared_ptr<int>::element_type, int>);
  static_assert(std::is_same_v<std::shared_ptr<int[]>::element_type, int>);
  static_assert(std::is_same_v<cest::shared_ptr<int>::element_type, int>);
  static_assert(std::is_same_v<cest::shared_ptr<int[]>::element_type, int>);
  assert(shared_ptr_test<std::shared_ptr>());
  assert(shared_ptr_test<cest::shared_ptr>());
}

#endif
