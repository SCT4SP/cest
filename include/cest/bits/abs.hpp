#ifndef _CEST_ABS_HPP_
#define _CEST_ABS_HPP_

namespace cest {

constexpr double abs(double x) { return x < 0.0 ? -x : x; }
constexpr double fabs(double x) { return abs(x); }

} // namespace cest

#endif // _CEST_ABS_HPP_
