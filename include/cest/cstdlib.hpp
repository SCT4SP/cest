#ifndef _CEST_CSTDLIB_HPP_
#define _CEST_CSTDLIB_HPP_

#include <cstdlib>

namespace cest {

  using std::size_t;

  constexpr int abs(int n) { return (n<0) ? -n : n; }

} // namespace cest

#endif // _CEST_CSTDLIB_HPP_
