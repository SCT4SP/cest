#ifndef _CEST_ABS_HPP_
#define _CEST_ABS_HPP_

namespace cest {

constexpr int abs(int x) { return x < 0 ? -x : x; }
constexpr long abs(long x) { return x < 0 ? -x : x; }
constexpr long long abs(long long x) { return x < 0 ? -x : x; }

constexpr float abs(float x) { return x < 0.0f ? -x : x; }
constexpr double abs(double x) { return x < 0.0 ? -x : x; }
constexpr long double abs(long double x) { return x < 0.0l ? -x : x; }

} // namespace cest

#endif // _CEST_ABS_HPP_
