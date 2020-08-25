#include "cest/iostream.hpp"
#include "cest/string.hpp"
#include "cest/vector.hpp"
#include "cest/deque.hpp"
#include "cest/set.hpp"
#include "cest/algorithm.hpp"
#include "cest/numeric.hpp"

// clang++ -std=c++2a -I include example.cpp

namespace ns = cest;

constexpr int doit()
{
  using namespace ns;
  string str = "Hello";
  vector<int> v{1,2,3};
  deque<int> dq{2,3,4};
  set<int> s;
  set_intersection(dq.begin(), dq.end(),
                    v.begin(),  v.end(),
                   inserter(s, s.end()));
  auto x = accumulate(s.begin(), s.end(), 0);
  cout << str << " World " << x << endl;
  return 0;
}

int main(int argc, char *argv[])
{
  static_assert(0 == doit());
  return doit();
}
