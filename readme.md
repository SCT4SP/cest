# C'est la vie

<p align="left">
  <a href="https://github.com/pkeir/cest/actions"><img alt="GitHub Actions status" src="https://github.com/pkeir/cest/workflows/build-test/badge.svg"></a>
</p>

The **C'est** project aims to provide a non-standard version of the C++
standard library with enhanced support for compile time evaluation
(`constexpr`) for use in research.

GCC 10 and Clang 10 declare support for all 7 C++20 proposals which are part of
"Relaxations of constexpr restrictions": [P1064](https://wg21.link/P1064)
[P1002](https://wg21.link/P1002) [P1327](https://wg21.link/P1327)
[P1330](https://wg21.link/P1330) [P1331](https://wg21.link/P1331)
[P1668](https://wg21.link/P1668) [P0784](https://wg21.link/P0784R7). Of these,
the last, P0784 (More constexpr containers) includes discussion on "constexpr
new-expressions".  Clang's support includes crucial facilities missing in GCC:
for example, equality comparison of pointers at compile-time. Consequently,
**C'est** requires a recent release of the Clang compiler; as included with
Ubuntu 20.04.

The cest library is not standalone, and requires standard library support;
including `std::construct_at`. Version 10 of GCC's libstdc++ is sufficient, and
while not included by default on Ubuntu 20.04, this is available as a package
(libstdc++-10-dev). Clang uses libstdc++ on Ubuntu by default, and will use
version 10 if it is available. C'est also uses code from libstdc++;
as seen in `basic_ios.hpp`, `ios_base.hpp`, `istream.hpp`, `ostream.hpp`,
`sstream.hpp`, `streambuf.hpp` and elsewhere.

Header files in **C'est** are named after those in the standard library; where
you would normally have `#include <vector>`, you should use `#include
"cest/vector.hpp"`. Classes and objects of the **C'est** library reside within
the `cest` namespace; and consequently where you would have had `std::deque`,
you should use `cest::deque` instead when using **C'est**.

In time, it is likely that the C++ standard library will continue to include
more and more `constexpr` support. Most containers will naturally accommodate
this direction. IO and file access will present more choices. For now,
**C'est** does allow some IO commands to compile within constant expressions
(e.g. `cest::cout << "Hello World\n"`). This is primarily to support the
compile-time evaluation of existing code bases.

The **C'est** library has incomplete support for the following class templates: `vector`, `string`, `forward_list`, `list`, `set`, `map`, `queue`, `deque`, `unique_ptr`, `shared_ptr` and `function`. Given a `constexpr` container, most function templates from `algorithm` and `numeric` can now also be used within a constant expression.

The code below provides a basic demonstration of some functionality. Executing the resulting program will output `Hello World 5`:

```cpp
#include "cest/iostream.hpp"
#include "cest/string.hpp"
#include "cest/vector.hpp"
#include "cest/deque.hpp"
#include "cest/set.hpp"
#include "cest/algorithm.hpp"
#include "cest/numeric.hpp"
#include "cest/functional.hpp"

// clang++ -std=c++2a -I include example.cpp

constexpr bool doit()
{
  using namespace cest;

  string str = "Hello";
  vector<int> v{1,2,3};
  deque<int> dq{2,3,4};
  set<int> s;

  set_intersection(dq.begin(), dq.end(), v.begin(),  v.end(), inserter(s, s.end()));
  function<int()> f = [&]() { return accumulate(s.begin(), s.end(), 0); };
  auto x = f();
  cout << str << " World " << x << endl;

  return 5==x;
}

int main(int argc, char *argv[])
{
  static_assert(doit());
  return doit() ? 0 : 1;
}
```

The code above comes from the `example.cpp` file; found in the same
directory as this readme. The command required to build the code above is:

```
clang++ -std=c++2a -I include example.cpp
```

Compile-time and runtime tests are included within the `tests` directory. To
build and run the tests use the following commands. For the runtime component,
expected console output is listed in `cest_tests.cpp`.

```
cd tests
mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=clang++ ..
make
./cest_tests
```

## Projects using C'est

* [ctcheckmm](https://github.com/pkeir/ctcheckmm): a compile-time Metamath proof database verifier
* [poacher](https://github.com/JPenuchot/poacher): a constexpr Brainfuck compiler, with proper AST and code generation (see examples/brainfuck)
