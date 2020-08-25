#include "cest/iostream.hpp"
#include "cest/deque.hpp"
#include "cest/string.hpp"
#include "cest/vector.hpp"

// clang++ -std=c++2a -I include example.cpp

namespace ns = cest;

constexpr int doit()
{
  ns::deque<int> dq;
  dq.push_front(42);
  ns::cout << "Hello World\n";
  return 0;
}

int main(int argc, char *argv[])
{
  static_assert(0 == doit());
  return doit();
}
