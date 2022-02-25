#ifndef _CEST_CSTDLIB_HPP_
#define _CEST_CSTDLIB_HPP_

#include "cest/bits/abs.hpp"
#include <cstdlib>

namespace cest {

using std::size_t;

constexpr long labs(long x) { return x < 0 ? -x : x; }
constexpr long long llabs(long long x) { return x < 0 ? -x : x; }

} // namespace cest

#endif // _CEST_CSTDLIB_HPP_
