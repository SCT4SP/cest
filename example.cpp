#include "cest/algorithm.hpp"
#include "cest/deque.hpp"
#include "cest/functional.hpp"
#include "cest/iostream.hpp"
#include "cest/numeric.hpp"
#include "cest/set.hpp"
#include "cest/string.hpp"
#include "cest/vector.hpp"

// Copyright (c) 2020-2022 Paul Keir, University of the West of Scotland.

// g++-12  -std=c++20 -I include example.cpp
//   ... or:
// clang++ -std=c++20 -I include example.cpp

constexpr bool doit() {
  using namespace cest;

  string str = "Hello";
  vector<int> v{1, 2, 3};
  deque<int> dq{2, 3, 4};
  set<int> s;

  set_intersection(dq.begin(), dq.end(), v.begin(), v.end(),
                   inserter(s, s.end()));
  function<int()> f = [&]() { return accumulate(s.begin(), s.end(), 0); };
  auto x = f();
  cout << str << " World " << x << endl;

  return 5 == x;
}

int main(int argc, char *argv[]) {
  static_assert(doit());
  return doit() ? 0 : 1;
}
