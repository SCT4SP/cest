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

constexpr void int_array_deleter(int* p) { delete [] p; }

template <template <typename ...> typename St>
constexpr bool shared_ptr_test()
{
  St<int> sp1{new int{123}};
  auto sp2 = sp1;
  bool b1 = 123==*sp1 && 123==*sp1.get() && 123==*sp2 && 123==*sp2.get();
  b1 = b1 && 2==sp1.use_count() && 2==sp2.use_count();
  b1 = b1 && nullptr==get_deleter<void>(sp1);

  St<int[]> spa1{new int[4]{1,2,3,4}};
  auto spa2 = spa1;
  bool b2 = 1==spa1[0] && 2==spa1[1] && 3==spa1[2] && 4==spa1[3];
  bool b3 = 1==spa2[0] && 2==spa2[1] && 3==spa2[2] && 4==spa2[3];

  int i{41};
  bool b4{false};
  int *p = new int{123};
  {
    struct del { constexpr void operator()(int*) { ++i_; } int& i_; };
    del d1{i};
    St<int> sp3{p, d1};
    auto d2 = get_deleter<del>(sp3);
    b4 = d1.i_==d2->i_;
    auto d3 = [](int* p) { delete p; };
    auto d3b = [](int* p) { delete p; };
    St<int>   sp4{new int, d3};
    b4 = b4 && nullptr!=get_deleter<decltype(d3)>(sp4);
    b4 = b4 && nullptr==get_deleter<decltype(d3b)>(sp4);
    St<int[]> sp5{new int[4], int_array_deleter};
    b4 = b4 && nullptr!=get_deleter<void(*)(int*)>(sp5);
  }
  bool b5 = 42==i;
  delete p;

  return b1 && b2 && b3 && b4 && b5;
}

// test the aliasing constructor
template <template <typename ...> typename St>
constexpr bool shared_ptr_test2()
{
  bool b1{false}, b2{false};
  int i{41};
  {
    St<int[]> sp8{new int[8]{1,2,3,4,5,6,7,8}};
    St<int[]> sp4{sp8, sp8.get() + 4};
    b1 = 1==sp8[0] && 5==sp4[0];

    struct del { constexpr void operator()(char*) { ++i_; } int& i_; };
    del d{i};
    char *p = new char[9]{"abcdefgh"};
    St<char[]> sp8b{p,d};
    St<char[]> sp4b{sp8b, sp8b.get() + 4};
    b2 = 'a'==sp8b[0] && 'e'==sp4b[0];
    delete [] p;
  }
  return b1 && b2 && 42==i;
}

template <typename T, typename P>
constexpr bool shared_ptr_compare_test()
{
  bool b = true;
  P p(nullptr), q(nullptr);
  b = b && p==q && !(p!=q) && p<=q && p>=q && !(p<q) && !(p>q);

  auto test_arr2 = [&](auto p, auto q) {
    b = b && !(p==q) && p!=q && p<=q && !(p>=q) && p<q && !(p>q);
  };

  {
    T arr[2];
    if constexpr (std::is_pointer_v<P>) {  // a raw pointer
      test_arr2(&arr[0], &arr[1]);
    } else {
      P p(&arr[0],[](int*){}), q(p, p.get()+1); // null deleter
      test_arr2(p, q);
    }
  }

  {
    T* arr = new T[2];
    if constexpr (std::is_pointer_v<P>) {  // a raw pointer
      test_arr2(&arr[0], &arr[1]);
      delete [] arr;
    } else {
      P p(&arr[0]), q(p, p.get()+1); // non-null deleter
      test_arr2(p, q);
    }
  }

  return b;
}

void
memory_tests()
{
#if CONSTEXPR_CEST == 1
  static_assert(constexpr_mem_test<cest::unique_ptr>(),
                "unique_ptr: Tests failed!");
  static_assert(make_unique_test(), "make_unique: Tests failed!");
  static_assert(shared_ptr_test<cest::shared_ptr>());
  static_assert(shared_ptr_test2<cest::shared_ptr>());
  static_assert(shared_ptr_compare_test<int, int*>());
  static_assert(shared_ptr_compare_test<int,cest::shared_ptr<int[]>>());
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
  assert(shared_ptr_test2<std::shared_ptr>());
  assert(shared_ptr_test2<cest::shared_ptr>());

  assert((shared_ptr_compare_test<int, int*>()));
  assert((shared_ptr_compare_test<int, std::shared_ptr<int[]>>()));
}

#endif
