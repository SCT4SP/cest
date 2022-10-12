#ifndef _CEST_TYPEINFO_TESTS_HPP_
#define _CEST_TYPEINFO_TESTS_HPP_

#include "cest/typeinfo.hpp"
#include <cassert>

struct Animal { virtual ~Animal() = default; };
struct Archaeopteryx : Animal {
#if defined(__clang__)
  ~Archaeopteryx()  = default;
#else
  constexpr ~Archaeopteryx() {};
#endif
};

constexpr bool typeinfo_test1() {
  Archaeopteryx x;
  bool b1 = typeid(x) == typeid(x);
  bool b2 = typeid(x) == typeid(Archaeopteryx);
  bool b3 = typeid(Animal) != typeid(Archaeopteryx);
  bool b4 = typeid(int) != typeid(float);
  bool b5 = typeid(cest::type_info) == typeid(std::type_info);
  return b1 && b2 && b3 && b4 && b5;
}

void typeinfo_tests() {
#if CONSTEXPR_CEST == 1
  static_assert(typeinfo_test1());
#endif

  assert((typeinfo_test1()));
}

#endif // _CEST_TYPEINFO_TESTS_HPP_
