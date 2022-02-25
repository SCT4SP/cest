#ifndef _CEST_CMATH_HPP_
#define _CEST_CMATH_HPP_

#include <type_traits>

#include "cest/bits/abs.hpp"

namespace cest {

constexpr float fabs(float x) { return abs(x); }
constexpr float fabsf(float x) { return abs(x); }
constexpr double fabs(double x) { return abs(x); }
constexpr long double fabs(long double x) { return abs(x); }
constexpr long double fabsl(long double x) { return abs(x); }
template <typename T>
constexpr std::enable_if_t<std::is_integral_v<T>, double> fabs(T x) {
  return static_cast<double>(abs(x));
}

#define CEST_M_PI 3.141592653589793
#define CEST_M_PI_2 1.570796326794897
#define CEST_M_E 2.718281828459045

// some forward declarations so the function ordering below doesn't matter
// mainly so they can be put in alphabetical order or shuffled as required
constexpr double pow(double, int);
constexpr double sqrt(double);
constexpr double sin(double);
constexpr double sinh(double);

namespace {
constexpr double tol = 0.001;

constexpr double square(const double x) { return x * x; }

constexpr double cube(const double x) { return x * x * x; }

// https://www.cse.wustl.edu/~ychen/131/Notes/SquareRoot/sqrt.html
constexpr double sqrt_helper(const double x, const double g) {
  return abs(g - x / g) < (tol * g) ? g : sqrt_helper(x, (g + x / g) / 2.0);
}

constexpr double sin_helper(const double x) {
  return x < tol ? x
                 : 3 * (sin_helper(x / 3.0)) - 4 * cube(sin_helper(x / 3.0));
}

constexpr double sinh_helper(const double x) {
  return x < tol ? x
                 : 3 * (sinh_helper(x / 3.0)) + 4 * cube(sinh_helper(x / 3.0));
}

// atan formulae from http://mathonweb.com/algebra_e-book.htm
// x - x^3/3 + x^5/5 - x^7/7+x^9/9  etc.
constexpr double atan_poly_helper(const double res, const double num1,
                                  const double den1, const double delta) {
  return res < tol
             ? res
             : res +
                   atan_poly_helper((num1 * delta) / (den1 + 2.) - num1 / den1,
                                    num1 * delta * delta, den1 + 4., delta);
}

constexpr double atan_poly(const double x) {
  return x + atan_poly_helper(pow(x, 5) / 5. - pow(x, 3) / 3., pow(x, 7), 7.,
                              x * x);
}

constexpr double atan_identity(const double x) {
  return x <= (2. - sqrt(3.))
             ? atan_poly(x)
             : (CEST_M_PI_2 / 3.) +
                   atan_poly((sqrt(3.) * x - 1) / (sqrt(3.) + x));
}

constexpr double atan_cmplmntry(const double x) {
  return (x < 1) ? atan_identity(x) : CEST_M_PI_2 - atan_identity(1 / x);
}

constexpr double exp_helper(const double r) {
  return 1.0 + r + pow(r, 2) / 2.0 + pow(r, 3) / 6.0 + pow(r, 4) / 24.0 +
         pow(r, 5) / 120.0 + pow(r, 6) / 720.0 + pow(r, 7) / 5040.0;
}

constexpr int exponent_helper(const double x, const int e) {
  return x >= 10.0 ? exponent_helper(x * 0.1, e + 1)
         : x < 1.0 ? exponent_helper(x * 10.0, e - 1)
                   : e;
}

constexpr int exponent(const double x) { return exponent_helper(x, 0); }

constexpr double mantissa(const double x) {
  return x >= 10.0 ? mantissa(x * 0.1) : x < 1.0 ? mantissa(x * 10.0) : x;
}

constexpr double log_helper2(const double y) {
  return 2.0 * (y + pow(y, 3) / 3.0 + pow(y, 5) / 5.0 + pow(y, 7) / 7.0 +
                pow(y, 9) / 9.0 + pow(y, 11) / 11.0);
}

// log in the range 1-sqrt(10)
constexpr double log_helper(const double x) {
  return log_helper2((x - 1.0) / (x + 1.0));
}

constexpr double nearest(double x) {
  return (x - 0.5) > (int)x ? (int)(x + 0.5) : (int)x;
}

constexpr double fraction(double x) {
  return (x - 0.5) > (int)x ? -(((double)(int)(x + 0.5)) - x)
                            : x - ((double)(int)(x));
}

} // anonymous namespace

constexpr double atan(double x) {
  return (x >= 0) ? atan_cmplmntry(x) : -atan_cmplmntry(-x);
}

constexpr double atan2(double y, double x) {
  return x > 0             ? atan(y / x)
         : y >= 0 && x < 0 ? atan(y / x) + CEST_M_PI
         : y < 0 && x < 0  ? atan(y / x) - CEST_M_PI
         : y > 0 && x == 0 ? CEST_M_PI_2
         : y < 0 && x == 0 ? -CEST_M_PI_2
                           : 0; // 0 == undefined
}

constexpr int ceil(double v) {
  return ((double)(int)v != v && v > 0) ? v + 1 : v;
}

constexpr double cos(double x) { return sin(CEST_M_PI_2 - x); }

constexpr double cosh(double x) { return sqrt(1.0 + square(sinh(x))); }

constexpr int floor(double v) {
  return ((double)(int)v != v && v < 0) ? v - 1 : v;
}

constexpr double pow(double base, int exponent) {
  return exponent < 0    ? 1.0 / pow(base, -exponent)
         : exponent == 0 ? 1.
         : exponent == 1 ? base
                         : base * pow(base, exponent - 1);
}

constexpr double sqrt(double x) { return sqrt_helper(x, 1.0); }

constexpr double sin(double x) {
  return sin_helper(x < 0 ? -x + CEST_M_PI : x);
}

constexpr double sinh(double x) {
  return x < 0 ? -sinh_helper(-x) : sinh_helper(x);
}

// exp(x) = e^n . e^r (where n is an integer, and -0.5 > r < 0.5
// exp(r) = e^r = 1 + r + r^2/2 + r^3/6 + r^4/24 + r^5/120
constexpr double exp(double x) {
  return pow(CEST_M_E, nearest(x)) * exp_helper(fraction(x));
}

constexpr double log(double x) {
  return x == 0 ? -std::numeric_limits<double>::infinity()
         : x < 0
             ? std::numeric_limits<double>::quiet_NaN()
             : 2.0 * log_helper(sqrt(mantissa(x))) + 2.3025851 * exponent(x);
}

#undef CEST_M_PI
#undef CEST_M_PI_2
#undef CEST_M_E

} // namespace cest

#endif // _CEST_CMATH_HPP_
