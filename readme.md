# C'est la vie

<p align="left">
  <a href="https://github.com/pkeir/cest/actions"><img alt="GitHub Actions status" src="https://github.com/pkeir/cest/workflows/build-test/badge.svg"></a>
</p>

The **C'est** project aims to provide a non-standard version of the C++
standard library with enhanced support for compile time evaluation
(`constexpr`) for use in research.

GCC and Clang support all 7 C++20 proposals which are part of
"Relaxations of constexpr restrictions": [P1064](https://wg21.link/P1064)
[P1002](https://wg21.link/P1002) [P1327](https://wg21.link/P1327)
[P1330](https://wg21.link/P1330) [P1331](https://wg21.link/P1331)
[P1668](https://wg21.link/P1668) [P0784](https://wg21.link/P0784R7). Of these,
the last, P0784 (More constexpr containers) includes discussion on "constexpr
new-expressions".

**C'est** supports both Clang and GCC. To pass all the tests provided, C++23
is used (either via flag `-std=c++23` or `-std=c++2b`). A recent version of the
GCC compiler is also required (e.g. GCC 12; `sudo apt-get install g++-12`).

The C'est library is not standalone, and requires a recent standard library;
such as GCC's libstdc++. C'est also includes code from
libstdc++; as seen in `basic_ios.hpp`, `ios_base.hpp`, `istream.hpp`,
`ostream.hpp`, `sstream.hpp`, `streambuf.hpp` and elsewhere.

Header files in **C'est** are named after those in the standard library; where
you would normally have `#include <vector>`, you should use `#include
"cest/vector.hpp"`. Classes and objects of the **C'est** library reside within
the `cest` namespace; and consequently where you would have had `std::deque`,
you should use `cest::deque` instead when using **C'est**.

In time, it is likely that the C++ standard library will continue to include
more and more `constexpr` support. Most containers will naturally accommodate
this direction. IO and file access will raise more questions. For now,
**C'est** does allow some IO commands to compile within constant expressions
(e.g. `cest::cout << "Hello World\n"`). This is primarily to support the
compile-time evaluation of existing code bases.

The **C'est** library has incomplete support for the following class templates: `vector`, `string`, `forward_list`, `list`, `set`, `map`, `queue`, `deque`, `unique_ptr`, `shared_ptr` and `function`. Given a `constexpr` container, most function templates from `algorithm` and `numeric` can now also be used within a constant expression.

The code below provides a basic demonstration of some functionality. Executing the resulting program will output `Hello World 5`:

```cpp
#include "cest/algorithm.hpp"
#include "cest/deque.hpp"
#include "cest/functional.hpp"
#include "cest/iostream.hpp"
#include "cest/numeric.hpp"
#include "cest/set.hpp"
#include "cest/string.hpp"
#include "cest/vector.hpp"

// Copyright (c) 2020-2022 Paul Keir, University of the West of Scotland.

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
```

The code above comes from the `example.cpp` file; found in the same
directory as this readme. The command required to build the code above is:

```
clang++ -std=c++20 -I include example.cpp
```

Compile-time and runtime tests are included within the `tests` directory. To
ensure runtime and compile-time parity, each `static_assert` therein is
accompanied by a runtime `assert`. To build and run the tests use the following
commands (the `Debug` CMake flag ensures the runtime `assert` calls remain
active). For the runtime component, expected console output is listed in
`cest_tests.cpp`.

```
cd tests
mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug ..
make
make test
```

## Projects using C'est

* [ctcheckmm](https://github.com/pkeir/ctcheckmm): a compile-time Metamath proof database verifier
* [poacher](https://github.com/JPenuchot/poacher): a constexpr Brainfuck compiler, with proper AST and code generation (see examples/brainfuck)
