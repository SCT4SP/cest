#ifndef _CEST_COMPLEX_HPP_
#define _CEST_COMPLEX_HPP_

// This is based on the complex header from libstdc++ with the specialisations
// of float, double and long double removed; as they rely on C99's complex; as
// in "__complex__ double" etc. The class is now defined in the cest namespace,
// and explicit usages of std are replaced by cest. Trailing definitions in
// namespace __gnu_cxx are also omitted.

#include <type_traits>
#include "cest/algorithm.hpp"

// Get rid of a macro possibly defined in <complex.h>
#undef complex

#if __cplusplus > 201703L
# define __cpp_lib_constexpr_complex 201711L
#endif

namespace cest
{
  /**
   * @defgroup complex_numbers Complex Numbers
   * @ingroup numerics
   *
   * Classes and functions for complex numbers.
   * @{
   */

  // Forward declarations.
  template<typename _Tp> struct complex;
//  template<> struct complex<float>;
//  template<> struct complex<double>;
//  template<> struct complex<long double>;

  ///  Return magnitude of @a z.
  template<typename _Tp> _Tp constexpr abs(const complex<_Tp>&);
  ///  Return phase angle of @a z.
  template<typename _Tp> _Tp arg(const complex<_Tp>&);
  ///  Return @a z magnitude squared.
  template<typename _Tp> _Tp _GLIBCXX20_CONSTEXPR norm(const complex<_Tp>&);

  ///  Return complex conjugate of @a z.
  template<typename _Tp>
    _GLIBCXX20_CONSTEXPR complex<_Tp> conj(const complex<_Tp>&);
  ///  Return complex with magnitude @a rho and angle @a theta.
  template<typename _Tp> complex<_Tp> polar(const _Tp&, const _Tp& = 0);

  // Transcendentals:
  /// Return complex cosine of @a z.
  template<typename _Tp> complex<_Tp> cos(const complex<_Tp>&);
  /// Return complex hyperbolic cosine of @a z.
  template<typename _Tp> complex<_Tp> cosh(const complex<_Tp>&);
  /// Return complex base e exponential of @a z.
  template<typename _Tp> complex<_Tp> exp(const complex<_Tp>&);
  /// Return complex natural logarithm of @a z.
  template<typename _Tp> complex<_Tp> log(const complex<_Tp>&);
  /// Return complex base 10 logarithm of @a z.
  template<typename _Tp> complex<_Tp> log10(const complex<_Tp>&);
  /// Return @a x to the @a y'th power.
  template<typename _Tp> complex<_Tp> pow(const complex<_Tp>&, int);
  /// Return @a x to the @a y'th power.
  template<typename _Tp> complex<_Tp> pow(const complex<_Tp>&, const _Tp&);
  /// Return @a x to the @a y'th power.
  template<typename _Tp> complex<_Tp> pow(const complex<_Tp>&,
                                          const complex<_Tp>&);
  /// Return @a x to the @a y'th power.
  template<typename _Tp> complex<_Tp> pow(const _Tp&, const complex<_Tp>&);
  /// Return complex sine of @a z.
  template<typename _Tp> complex<_Tp> sin(const complex<_Tp>&);
  /// Return complex hyperbolic sine of @a z.
  template<typename _Tp> complex<_Tp> sinh(const complex<_Tp>&);
  /// Return complex square root of @a z.
  template<typename _Tp> complex<_Tp> sqrt(const complex<_Tp>&);
  /// Return complex tangent of @a z.
  template<typename _Tp> complex<_Tp> tan(const complex<_Tp>&);
  /// Return complex hyperbolic tangent of @a z.
  template<typename _Tp> complex<_Tp> tanh(const complex<_Tp>&);


  // 26.2.2  Primary template class complex
  /**
   *  Template to represent complex numbers.
   *
   *  Specializations for float, double, and long double are part of the
   *  library.  Results with any other type are not guaranteed.
   *
   *  @param  Tp  Type of real and imaginary values.
  */
  template<typename _Tp>
    struct complex
    {
      /// Value typedef.
      typedef _Tp value_type;

      ///  Default constructor.  First parameter is x, second parameter is y.
      ///  Unspecified parameters default to 0.
      _GLIBCXX_CONSTEXPR complex(const _Tp& __r = _Tp(), const _Tp& __i = _Tp())
      : _M_real(__r), _M_imag(__i) { }

      // Let the compiler synthesize the copy constructor
#if __cplusplus >= 201103L
      constexpr complex(const complex&) = default;
#endif

      ///  Converting constructor.
      template<typename _Up>
        _GLIBCXX_CONSTEXPR complex(const complex<_Up>& __z)
	: _M_real(__z.real()), _M_imag(__z.imag()) { }

#if __cplusplus >= 201103L
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // DR 387. std::complex over-encapsulated.
      _GLIBCXX_ABI_TAG_CXX11
      constexpr _Tp
      real() const { return _M_real; }

      _GLIBCXX_ABI_TAG_CXX11
      constexpr _Tp
      imag() const { return _M_imag; }
#else
      ///  Return real part of complex number.
      _Tp&
      real() { return _M_real; }

      ///  Return real part of complex number.
      const _Tp&
      real() const { return _M_real; }

      ///  Return imaginary part of complex number.
      _Tp&
      imag() { return _M_imag; }

      ///  Return imaginary part of complex number.
      const _Tp&
      imag() const { return _M_imag; }
#endif

      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // DR 387. std::complex over-encapsulated.
      _GLIBCXX20_CONSTEXPR void
      real(_Tp __val) { _M_real = __val; }

      _GLIBCXX20_CONSTEXPR void
      imag(_Tp __val) { _M_imag = __val; }

      /// Assign a scalar to this complex number.
      _GLIBCXX20_CONSTEXPR complex<_Tp>& operator=(const _Tp&);

      /// Add a scalar to this complex number.
      // 26.2.5/1
      _GLIBCXX20_CONSTEXPR complex<_Tp>&
      operator+=(const _Tp& __t)
      {
	_M_real += __t;
	return *this;
      }

      /// Subtract a scalar from this complex number.
      // 26.2.5/3
      _GLIBCXX20_CONSTEXPR complex<_Tp>&
      operator-=(const _Tp& __t)
      {
	_M_real -= __t;
	return *this;
      }

      /// Multiply this complex number by a scalar.
      _GLIBCXX20_CONSTEXPR complex<_Tp>& operator*=(const _Tp&);
      /// Divide this complex number by a scalar.
      _GLIBCXX20_CONSTEXPR complex<_Tp>& operator/=(const _Tp&);

      // Let the compiler synthesize the copy assignment operator
#if __cplusplus >= 201103L
      _GLIBCXX20_CONSTEXPR complex& operator=(const complex&) = default;
#endif

      /// Assign another complex number to this one.
      template<typename _Up>
        _GLIBCXX20_CONSTEXPR complex<_Tp>& operator=(const complex<_Up>&);
      /// Add another complex number to this one.
      template<typename _Up>
        _GLIBCXX20_CONSTEXPR complex<_Tp>& operator+=(const complex<_Up>&);
      /// Subtract another complex number from this one.
      template<typename _Up>
        _GLIBCXX20_CONSTEXPR complex<_Tp>& operator-=(const complex<_Up>&);
      /// Multiply this complex number by another.
      template<typename _Up>
        _GLIBCXX20_CONSTEXPR complex<_Tp>& operator*=(const complex<_Up>&);
      /// Divide this complex number by another.
      template<typename _Up>
        _GLIBCXX20_CONSTEXPR complex<_Tp>& operator/=(const complex<_Up>&);

      _GLIBCXX_CONSTEXPR complex __rep() const
      { return *this; }

    private:
      _Tp _M_real;
      _Tp _M_imag;
    };

  template<typename _Tp>
    _GLIBCXX20_CONSTEXPR complex<_Tp>&
    complex<_Tp>::operator=(const _Tp& __t)
    {
     _M_real = __t;
     _M_imag = _Tp();
     return *this;
    }

  // 26.2.5/5
  template<typename _Tp>
    _GLIBCXX20_CONSTEXPR complex<_Tp>&
    complex<_Tp>::operator*=(const _Tp& __t)
    {
      _M_real *= __t;
      _M_imag *= __t;
      return *this;
    }

  // 26.2.5/7
  template<typename _Tp>
    _GLIBCXX20_CONSTEXPR complex<_Tp>&
    complex<_Tp>::operator/=(const _Tp& __t)
    {
      _M_real /= __t;
      _M_imag /= __t;
      return *this;
    }

  template<typename _Tp>
    template<typename _Up>
    _GLIBCXX20_CONSTEXPR complex<_Tp>&
    complex<_Tp>::operator=(const complex<_Up>& __z)
    {
      _M_real = __z.real();
      _M_imag = __z.imag();
      return *this;
    }

  // 26.2.5/9
  template<typename _Tp>
    template<typename _Up>
    _GLIBCXX20_CONSTEXPR complex<_Tp>&
    complex<_Tp>::operator+=(const complex<_Up>& __z)
    {
      _M_real += __z.real();
      _M_imag += __z.imag();
      return *this;
    }

  // 26.2.5/11
  template<typename _Tp>
    template<typename _Up>
    _GLIBCXX20_CONSTEXPR complex<_Tp>&
    complex<_Tp>::operator-=(const complex<_Up>& __z)
    {
      _M_real -= __z.real();
      _M_imag -= __z.imag();
      return *this;
    }

  // 26.2.5/13
  // XXX: This is a grammar school implementation.
  template<typename _Tp>
    template<typename _Up>
    _GLIBCXX20_CONSTEXPR complex<_Tp>&
    complex<_Tp>::operator*=(const complex<_Up>& __z)
    {
      const _Tp __r = _M_real * __z.real() - _M_imag * __z.imag();
      _M_imag = _M_real * __z.imag() + _M_imag * __z.real();
      _M_real = __r;
      return *this;
    }

  // 26.2.5/15
  // XXX: This is a grammar school implementation.
  template<typename _Tp>
    template<typename _Up>
    _GLIBCXX20_CONSTEXPR complex<_Tp>&
    complex<_Tp>::operator/=(const complex<_Up>& __z)
    {
      const _Tp __r =  _M_real * __z.real() + _M_imag * __z.imag();
      const _Tp __n = cest::norm(__z);
      _M_imag = (_M_imag * __z.real() - _M_real * __z.imag()) / __n;
      _M_real = __r / __n;
      return *this;
    }

  // Operators:
  //@{
  ///  Return new complex value @a x plus @a y.
  template<typename _Tp>
    inline _GLIBCXX20_CONSTEXPR complex<_Tp>
    operator+(const complex<_Tp>& __x, const complex<_Tp>& __y)
    {
      complex<_Tp> __r = __x;
      __r += __y;
      return __r;
    }

  template<typename _Tp>
    inline _GLIBCXX20_CONSTEXPR complex<_Tp>
    operator+(const complex<_Tp>& __x, const _Tp& __y)
    {
      complex<_Tp> __r = __x;
      __r += __y;
      return __r;
    }

  template<typename _Tp>
    inline _GLIBCXX20_CONSTEXPR complex<_Tp>
    operator+(const _Tp& __x, const complex<_Tp>& __y)
    {
      complex<_Tp> __r = __y;
      __r += __x;
      return __r;
    }
  //@}

  //@{
  ///  Return new complex value @a x minus @a y.
  template<typename _Tp>
    inline _GLIBCXX20_CONSTEXPR complex<_Tp>
    operator-(const complex<_Tp>& __x, const complex<_Tp>& __y)
    {
      complex<_Tp> __r = __x;
      __r -= __y;
      return __r;
    }

  template<typename _Tp>
    inline _GLIBCXX20_CONSTEXPR complex<_Tp>
    operator-(const complex<_Tp>& __x, const _Tp& __y)
    {
      complex<_Tp> __r = __x;
      __r -= __y;
      return __r;
    }

  template<typename _Tp>
    inline _GLIBCXX20_CONSTEXPR complex<_Tp>
    operator-(const _Tp& __x, const complex<_Tp>& __y)
    {
      complex<_Tp> __r = -__y;
      __r += __x;
      return __r;
    }
  //@}

  //@{
  ///  Return new complex value @a x times @a y.
  template<typename _Tp>
    inline _GLIBCXX20_CONSTEXPR complex<_Tp>
    operator*(const complex<_Tp>& __x, const complex<_Tp>& __y)
    {
      complex<_Tp> __r = __x;
      __r *= __y;
      return __r;
    }

  template<typename _Tp>
    inline _GLIBCXX20_CONSTEXPR complex<_Tp>
    operator*(const complex<_Tp>& __x, const _Tp& __y)
    {
      complex<_Tp> __r = __x;
      __r *= __y;
      return __r;
    }

  template<typename _Tp>
    inline _GLIBCXX20_CONSTEXPR complex<_Tp>
    operator*(const _Tp& __x, const complex<_Tp>& __y)
    {
      complex<_Tp> __r = __y;
      __r *= __x;
      return __r;
    }
  //@}

  //@{
  ///  Return new complex value @a x divided by @a y.
  template<typename _Tp>
    inline _GLIBCXX20_CONSTEXPR complex<_Tp>
    operator/(const complex<_Tp>& __x, const complex<_Tp>& __y)
    {
      complex<_Tp> __r = __x;
      __r /= __y;
      return __r;
    }

  template<typename _Tp>
    inline _GLIBCXX20_CONSTEXPR complex<_Tp>
    operator/(const complex<_Tp>& __x, const _Tp& __y)
    {
      complex<_Tp> __r = __x;
      __r /= __y;
      return __r;
    }

  template<typename _Tp>
    inline _GLIBCXX20_CONSTEXPR complex<_Tp>
    operator/(const _Tp& __x, const complex<_Tp>& __y)
    {
      complex<_Tp> __r = __x;
      __r /= __y;
      return __r;
    }
  //@}

  ///  Return @a x.
  template<typename _Tp>
    inline _GLIBCXX20_CONSTEXPR complex<_Tp>
    operator+(const complex<_Tp>& __x)
    { return __x; }

  ///  Return complex negation of @a x.
  template<typename _Tp>
    inline _GLIBCXX20_CONSTEXPR complex<_Tp>
    operator-(const complex<_Tp>& __x)
    { return complex<_Tp>(-__x.real(), -__x.imag()); }

  //@{
  ///  Return true if @a x is equal to @a y.
  template<typename _Tp>
    inline _GLIBCXX_CONSTEXPR bool
    operator==(const complex<_Tp>& __x, const complex<_Tp>& __y)
    { return __x.real() == __y.real() && __x.imag() == __y.imag(); }

  template<typename _Tp>
    inline _GLIBCXX_CONSTEXPR bool
    operator==(const complex<_Tp>& __x, const _Tp& __y)
    { return __x.real() == __y && __x.imag() == _Tp(); }

#if !(__cpp_impl_three_way_comparison >= 201907L)
  template<typename _Tp>
    inline _GLIBCXX_CONSTEXPR bool
    operator==(const _Tp& __x, const complex<_Tp>& __y)
    { return __x == __y.real() && _Tp() == __y.imag(); }
  //@}

  //@{
  ///  Return false if @a x is equal to @a y.
  template<typename _Tp>
    inline _GLIBCXX_CONSTEXPR bool
    operator!=(const complex<_Tp>& __x, const complex<_Tp>& __y)
    { return __x.real() != __y.real() || __x.imag() != __y.imag(); }

  template<typename _Tp>
    inline _GLIBCXX_CONSTEXPR bool
    operator!=(const complex<_Tp>& __x, const _Tp& __y)
    { return __x.real() != __y || __x.imag() != _Tp(); }

  template<typename _Tp>
    inline _GLIBCXX_CONSTEXPR bool
    operator!=(const _Tp& __x, const complex<_Tp>& __y)
    { return __x != __y.real() || _Tp() != __y.imag(); }
#endif
  //@}

  ///  Extraction operator for complex values.
  template<typename _Tp, typename _CharT, class _Traits>
    basic_istream<_CharT, _Traits>&
    operator>>(basic_istream<_CharT, _Traits>& __is, complex<_Tp>& __x)
    {
      bool __fail = true;
      _CharT __ch;
      if (__is >> __ch)
	{
	  if (_Traits::eq(__ch, __is.widen('(')))
	    {
	      _Tp __u;
	      if (__is >> __u >> __ch)
		{
		  const _CharT __rparen = __is.widen(')');
		  if (_Traits::eq(__ch, __rparen))
		    {
		      __x = __u;
		      __fail = false;
		    }
		  else if (_Traits::eq(__ch, __is.widen(',')))
		    {
		      _Tp __v;
		      if (__is >> __v >> __ch)
			{
			  if (_Traits::eq(__ch, __rparen))
			    {
			      __x = complex<_Tp>(__u, __v);
			      __fail = false;
			    }
			  else
			    __is.putback(__ch);
			}
		    }
		  else
		    __is.putback(__ch);
		}
	    }
	  else
	    {
	      __is.putback(__ch);
	      _Tp __u;
	      if (__is >> __u)
		{
		  __x = __u;
		  __fail = false;
		}
	    }
	}
      if (__fail)
	__is.setstate(ios_base::failbit);
      return __is;
    }

  ///  Insertion operator for complex values.
  // PGK: Commented out as C'est has no basic_ostringstream
  /*template<typename _Tp, typename _CharT, class _Traits>
    basic_ostream<_CharT, _Traits>&
    operator<<(basic_ostream<_CharT, _Traits>& __os, const complex<_Tp>& __x)
    {
      basic_ostringstream<_CharT, _Traits> __s;
      __s.flags(__os.flags());
      __s.imbue(__os.getloc());
      __s.precision(__os.precision());
      __s << '(' << __x.real() << ',' << __x.imag() << ')';
      return __os << __s.str();
    }*/

  // Values
#if __cplusplus >= 201103L
  template<typename _Tp>
    constexpr _Tp
    real(const complex<_Tp>& __z)
    { return __z.real(); }

  template<typename _Tp>
    constexpr _Tp
    imag(const complex<_Tp>& __z)
    { return __z.imag(); }
#else
  template<typename _Tp>
    inline _Tp&
    real(complex<_Tp>& __z)
    { return __z.real(); }

  template<typename _Tp>
    inline const _Tp&
    real(const complex<_Tp>& __z)
    { return __z.real(); }

  template<typename _Tp>
    inline _Tp&
    imag(complex<_Tp>& __z)
    { return __z.imag(); }

  template<typename _Tp>
    inline const _Tp&
    imag(const complex<_Tp>& __z)
    { return __z.imag(); }
#endif

  // 26.2.7/3 abs(__z):  Returns the magnitude of __z.
  template<typename _Tp>
    constexpr _Tp
    __complex_abs(const complex<_Tp>& __z)
    {
      _Tp __x = __z.real();
      _Tp __y = __z.imag();
      const _Tp __s = cest::max(abs(__x), abs(__y));
      if (__s == _Tp())  // well ...
        return __s;
      __x /= __s;
      __y /= __s;
      return __s * sqrt(__x * __x + __y * __y);
    }

#if _GLIBCXX_USE_C99_COMPLEX
  inline float
  __complex_abs(__complex__ float __z) { return __builtin_cabsf(__z); }

  inline double
  __complex_abs(__complex__ double __z) { return __builtin_cabs(__z); }

  inline long double
  __complex_abs(const __complex__ long double& __z)
  { return __builtin_cabsl(__z); }

  template<typename _Tp>
    constexpr _Tp
    abs(const complex<_Tp>& __z) { return __complex_abs(__z.__rep()); }
#else
  template<typename _Tp>
    constexpr _Tp
    abs(const complex<_Tp>& __z) { return __complex_abs(__z); }
#endif


  // 26.2.7/4: arg(__z): Returns the phase angle of __z.
  template<typename _Tp>
    inline _Tp
    __complex_arg(const complex<_Tp>& __z)
    { return  atan2(__z.imag(), __z.real()); }

#if _GLIBCXX_USE_C99_COMPLEX
  inline float
  __complex_arg(__complex__ float __z) { return __builtin_cargf(__z); }

  inline double
  __complex_arg(__complex__ double __z) { return __builtin_carg(__z); }

  inline long double
  __complex_arg(const __complex__ long double& __z)
  { return __builtin_cargl(__z); }

  template<typename _Tp>
    inline _Tp
    arg(const complex<_Tp>& __z) { return __complex_arg(__z.__rep()); }
#else
  template<typename _Tp>
    inline _Tp
    arg(const complex<_Tp>& __z) { return __complex_arg(__z); }
#endif

  // 26.2.7/5: norm(__z) returns the squared magnitude of __z.
  //     As defined, norm() is -not- a norm is the common mathematical
  //     sense used in numerics.  The helper class _Norm_helper<> tries to
  //     distinguish between builtin floating point and the rest, so as
  //     to deliver an answer as close as possible to the real value.
  template<bool>
    struct _Norm_helper
    {
      template<typename _Tp>
        static inline _GLIBCXX20_CONSTEXPR _Tp _S_do_it(const complex<_Tp>& __z)
        {
          const _Tp __x = __z.real();
          const _Tp __y = __z.imag();
          return __x * __x + __y * __y;
        }
    };

  template<>
    struct _Norm_helper<true>
    {
      template<typename _Tp>
        static inline _GLIBCXX20_CONSTEXPR _Tp _S_do_it(const complex<_Tp>& __z)
        {
          //_Tp __res = std::abs(__z);
          //return __res * __res;
          const _Tp __x = __z.real();
          const _Tp __y = __z.imag();
          return __x * __x + __y * __y;
        }
    };

  template<typename _Tp>
    inline _GLIBCXX20_CONSTEXPR _Tp
    norm(const complex<_Tp>& __z)
    {
      return _Norm_helper<std::is_floating_point_v<_Tp>
	&& !_GLIBCXX_FAST_MATH>::_S_do_it(__z);
    }

  template<typename _Tp>
    inline complex<_Tp>
    polar(const _Tp& __rho, const _Tp& __theta)
    {
      __glibcxx_assert( __rho >= 0 );
      return complex<_Tp>(__rho * cos(__theta), __rho * sin(__theta));
    }

  template<typename _Tp>
    inline _GLIBCXX20_CONSTEXPR complex<_Tp>
    conj(const complex<_Tp>& __z)
    { return complex<_Tp>(__z.real(), -__z.imag()); }

  // Transcendentals

  // 26.2.8/1 cos(__z):  Returns the cosine of __z.
  template<typename _Tp>
    inline complex<_Tp>
    __complex_cos(const complex<_Tp>& __z)
    {
      const _Tp __x = __z.real();
      const _Tp __y = __z.imag();
      return complex<_Tp>(cos(__x) * cosh(__y), -sin(__x) * sinh(__y));
    }

#if _GLIBCXX_USE_C99_COMPLEX
  inline __complex__ float
  __complex_cos(__complex__ float __z) { return __builtin_ccosf(__z); }

  inline __complex__ double
  __complex_cos(__complex__ double __z) { return __builtin_ccos(__z); }

  inline __complex__ long double
  __complex_cos(const __complex__ long double& __z)
  { return __builtin_ccosl(__z); }

  template<typename _Tp>
    inline complex<_Tp>
    cos(const complex<_Tp>& __z) { return __complex_cos(__z.__rep()); }
#else
  template<typename _Tp>
    inline complex<_Tp>
    cos(const complex<_Tp>& __z) { return __complex_cos(__z); }
#endif

  // 26.2.8/2 cosh(__z): Returns the hyperbolic cosine of __z.
  template<typename _Tp>
    inline complex<_Tp>
    __complex_cosh(const complex<_Tp>& __z)
    {
      const _Tp __x = __z.real();
      const _Tp __y = __z.imag();
      return complex<_Tp>(cosh(__x) * cos(__y), sinh(__x) * sin(__y));
    }

#if _GLIBCXX_USE_C99_COMPLEX
  inline __complex__ float
  __complex_cosh(__complex__ float __z) { return __builtin_ccoshf(__z); }

  inline __complex__ double
  __complex_cosh(__complex__ double __z) { return __builtin_ccosh(__z); }

  inline __complex__ long double
  __complex_cosh(const __complex__ long double& __z)
  { return __builtin_ccoshl(__z); }

  template<typename _Tp>
    inline complex<_Tp>
    cosh(const complex<_Tp>& __z) { return __complex_cosh(__z.__rep()); }
#else
  template<typename _Tp>
    inline complex<_Tp>
    cosh(const complex<_Tp>& __z) { return __complex_cosh(__z); }
#endif

  // 26.2.8/3 exp(__z): Returns the complex base e exponential of x
  template<typename _Tp>
    inline complex<_Tp>
    __complex_exp(const complex<_Tp>& __z)
    { return cest::polar<_Tp>(exp(__z.real()), __z.imag()); }

#if _GLIBCXX_USE_C99_COMPLEX
  inline __complex__ float
  __complex_exp(__complex__ float __z) { return __builtin_cexpf(__z); }

  inline __complex__ double
  __complex_exp(__complex__ double __z) { return __builtin_cexp(__z); }

  inline __complex__ long double
  __complex_exp(const __complex__ long double& __z)
  { return __builtin_cexpl(__z); }

  template<typename _Tp>
    inline complex<_Tp>
    exp(const complex<_Tp>& __z) { return __complex_exp(__z.__rep()); }
#else
  template<typename _Tp>
    inline complex<_Tp>
    exp(const complex<_Tp>& __z) { return __complex_exp(__z); }
#endif

  // 26.2.8/5 log(__z): Returns the natural complex logarithm of __z.
  //                    The branch cut is along the negative axis.
  template<typename _Tp>
    inline complex<_Tp>
    __complex_log(const complex<_Tp>& __z)
    { return complex<_Tp>(log(cest::abs(__z)), cest::arg(__z)); }

#if _GLIBCXX_USE_C99_COMPLEX
  inline __complex__ float
  __complex_log(__complex__ float __z) { return __builtin_clogf(__z); }

  inline __complex__ double
  __complex_log(__complex__ double __z) { return __builtin_clog(__z); }

  inline __complex__ long double
  __complex_log(const __complex__ long double& __z)
  { return __builtin_clogl(__z); }

  template<typename _Tp>
    inline complex<_Tp>
    log(const complex<_Tp>& __z) { return __complex_log(__z.__rep()); }
#else
  template<typename _Tp>
    inline complex<_Tp>
    log(const complex<_Tp>& __z) { return __complex_log(__z); }
#endif

  template<typename _Tp>
    inline complex<_Tp>
    log10(const complex<_Tp>& __z)
    { return std::log(__z) / log(_Tp(10.0)); }

  // 26.2.8/10 sin(__z): Returns the sine of __z.
  template<typename _Tp>
    inline complex<_Tp>
    __complex_sin(const complex<_Tp>& __z)
    {
      const _Tp __x = __z.real();
      const _Tp __y = __z.imag();
      return complex<_Tp>(sin(__x) * cosh(__y), cos(__x) * sinh(__y));
    }

#if _GLIBCXX_USE_C99_COMPLEX
  inline __complex__ float
  __complex_sin(__complex__ float __z) { return __builtin_csinf(__z); }

  inline __complex__ double
  __complex_sin(__complex__ double __z) { return __builtin_csin(__z); }

  inline __complex__ long double
  __complex_sin(const __complex__ long double& __z)
  { return __builtin_csinl(__z); }

  template<typename _Tp>
    inline complex<_Tp>
    sin(const complex<_Tp>& __z) { return __complex_sin(__z.__rep()); }
#else
  template<typename _Tp>
    inline complex<_Tp>
    sin(const complex<_Tp>& __z) { return __complex_sin(__z); }
#endif

  // 26.2.8/11 sinh(__z): Returns the hyperbolic sine of __z.
  template<typename _Tp>
    inline complex<_Tp>
    __complex_sinh(const complex<_Tp>& __z)
    {
      const _Tp __x = __z.real();
      const _Tp  __y = __z.imag();
      return complex<_Tp>(sinh(__x) * cos(__y), cosh(__x) * sin(__y));
    }

#if _GLIBCXX_USE_C99_COMPLEX
  inline __complex__ float
  __complex_sinh(__complex__ float __z) { return __builtin_csinhf(__z); }

  inline __complex__ double
  __complex_sinh(__complex__ double __z) { return __builtin_csinh(__z); }

  inline __complex__ long double
  __complex_sinh(const __complex__ long double& __z)
  { return __builtin_csinhl(__z); }

  template<typename _Tp>
    inline complex<_Tp>
    sinh(const complex<_Tp>& __z) { return __complex_sinh(__z.__rep()); }
#else
  template<typename _Tp>
    inline complex<_Tp>
    sinh(const complex<_Tp>& __z) { return __complex_sinh(__z); }
#endif

  // 26.2.8/13 sqrt(__z): Returns the complex square root of __z.
  //                     The branch cut is on the negative axis.
  template<typename _Tp>
    complex<_Tp>
    __complex_sqrt(const complex<_Tp>& __z)
    {
      _Tp __x = __z.real();
      _Tp __y = __z.imag();

      if (__x == _Tp())
        {
          _Tp __t = sqrt(abs(__y) / 2);
          return complex<_Tp>(__t, __y < _Tp() ? -__t : __t);
        }
      else
        {
          _Tp __t = sqrt(2 * (std::abs(__z) + abs(__x)));
          _Tp __u = __t / 2;
          return __x > _Tp()
            ? complex<_Tp>(__u, __y / __t)
            : complex<_Tp>(abs(__y) / __t, __y < _Tp() ? -__u : __u);
        }
    }

#if _GLIBCXX_USE_C99_COMPLEX
  inline __complex__ float
  __complex_sqrt(__complex__ float __z) { return __builtin_csqrtf(__z); }

  inline __complex__ double
  __complex_sqrt(__complex__ double __z) { return __builtin_csqrt(__z); }

  inline __complex__ long double
  __complex_sqrt(const __complex__ long double& __z)
  { return __builtin_csqrtl(__z); }

  template<typename _Tp>
    inline complex<_Tp>
    sqrt(const complex<_Tp>& __z) { return __complex_sqrt(__z.__rep()); }
#else
  template<typename _Tp>
    inline complex<_Tp>
    sqrt(const complex<_Tp>& __z) { return __complex_sqrt(__z); }
#endif

  // 26.2.8/14 tan(__z):  Return the complex tangent of __z.

  template<typename _Tp>
    inline complex<_Tp>
    __complex_tan(const complex<_Tp>& __z)
    { return std::sin(__z) / std::cos(__z); }

#if _GLIBCXX_USE_C99_COMPLEX
  inline __complex__ float
  __complex_tan(__complex__ float __z) { return __builtin_ctanf(__z); }

  inline __complex__ double
  __complex_tan(__complex__ double __z) { return __builtin_ctan(__z); }

  inline __complex__ long double
  __complex_tan(const __complex__ long double& __z)
  { return __builtin_ctanl(__z); }

  template<typename _Tp>
    inline complex<_Tp>
    tan(const complex<_Tp>& __z) { return __complex_tan(__z.__rep()); }
#else
  template<typename _Tp>
    inline complex<_Tp>
    tan(const complex<_Tp>& __z) { return __complex_tan(__z); }
#endif


  // 26.2.8/15 tanh(__z):  Returns the hyperbolic tangent of __z.

  template<typename _Tp>
    inline complex<_Tp>
    __complex_tanh(const complex<_Tp>& __z)
    { return std::sinh(__z) / std::cosh(__z); }

#if _GLIBCXX_USE_C99_COMPLEX
  inline __complex__ float
  __complex_tanh(__complex__ float __z) { return __builtin_ctanhf(__z); }

  inline __complex__ double
  __complex_tanh(__complex__ double __z) { return __builtin_ctanh(__z); }

  inline __complex__ long double
  __complex_tanh(const __complex__ long double& __z)
  { return __builtin_ctanhl(__z); }

  template<typename _Tp>
    inline complex<_Tp>
    tanh(const complex<_Tp>& __z) { return __complex_tanh(__z.__rep()); }
#else
  template<typename _Tp>
    inline complex<_Tp>
    tanh(const complex<_Tp>& __z) { return __complex_tanh(__z); }
#endif


  // 26.2.8/9  pow(__x, __y): Returns the complex power base of __x
  //                          raised to the __y-th power.  The branch
  //                          cut is on the negative axis.
  template<typename _Tp>
    complex<_Tp>
    __complex_pow_unsigned(complex<_Tp> __x, unsigned __n)
    {
      complex<_Tp> __y = __n % 2 ? __x : complex<_Tp>(1);

      while (__n >>= 1)
        {
          __x *= __x;
          if (__n % 2)
            __y *= __x;
        }

      return __y;
    }

  // In C++11 mode we used to implement the resolution of
  // DR 844. complex pow return type is ambiguous.
  // thus the following overload was disabled in that mode.  However, doing
  // that causes all sorts of issues, see, for example:
  //   http://gcc.gnu.org/ml/libstdc++/2013-01/msg00058.html
  // and also PR57974.
  template<typename _Tp>
    inline complex<_Tp>
    pow(const complex<_Tp>& __z, int __n)
    {
      return __n < 0
	? complex<_Tp>(1) / cest::__complex_pow_unsigned(__z, -(unsigned)__n)
        : cest::__complex_pow_unsigned(__z, __n);
    }

  template<typename _Tp>
    complex<_Tp>
    pow(const complex<_Tp>& __x, const _Tp& __y)
    {
#if ! _GLIBCXX_USE_C99_COMPLEX
      if (__x == _Tp())
	return _Tp();
#endif
      if (__x.imag() == _Tp() && __x.real() > _Tp())
        return pow(__x.real(), __y);

      complex<_Tp> __t = std::log(__x);
      return cest::polar<_Tp>(exp(__y * __t.real()), __y * __t.imag());
    }

  template<typename _Tp>
    inline complex<_Tp>
    __complex_pow(const complex<_Tp>& __x, const complex<_Tp>& __y)
    { return __x == _Tp() ? _Tp() : std::exp(__y * std::log(__x)); }

#if _GLIBCXX_USE_C99_COMPLEX
  inline __complex__ float
  __complex_pow(__complex__ float __x, __complex__ float __y)
  { return __builtin_cpowf(__x, __y); }

  inline __complex__ double
  __complex_pow(__complex__ double __x, __complex__ double __y)
  { return __builtin_cpow(__x, __y); }

  inline __complex__ long double
  __complex_pow(const __complex__ long double& __x,
		const __complex__ long double& __y)
  { return __builtin_cpowl(__x, __y); }

  template<typename _Tp>
    inline complex<_Tp>
    pow(const complex<_Tp>& __x, const complex<_Tp>& __y)
    { return __complex_pow(__x.__rep(), __y.__rep()); }
#else
  template<typename _Tp>
    inline complex<_Tp>
    pow(const complex<_Tp>& __x, const complex<_Tp>& __y)
    { return __complex_pow(__x, __y); }
#endif

  template<typename _Tp>
    inline complex<_Tp>
    pow(const _Tp& __x, const complex<_Tp>& __y)
    {
      return __x > _Tp() ? cest::polar<_Tp>(pow(__x, __y.real()),
					   __y.imag() * log(__x))
	                 : std::pow(complex<_Tp>(__x), __y);
    }

} // namespace cest

#endif // _CEST_COMPLEX_HPP_
