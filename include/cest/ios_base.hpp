#ifndef _CEST_IOS_BASE_HPP_
#define _CEST_IOS_BASE_HPP_

#include <iosfwd> // std::streamsize

namespace cest {

enum _Ios_Fmtflags {
  _S_boolalpha = 1L << 0,
  _S_dec = 1L << 1,
  _S_fixed = 1L << 2,
  _S_hex = 1L << 3,
  _S_internal = 1L << 4,
  _S_left = 1L << 5,
  _S_oct = 1L << 6,
  _S_right = 1L << 7,
  _S_scientific = 1L << 8,
  _S_showbase = 1L << 9,
  _S_showpoint = 1L << 10,
  _S_showpos = 1L << 11,
  _S_skipws = 1L << 12,
  _S_unitbuf = 1L << 13,
  _S_uppercase = 1L << 14,
  _S_adjustfield = _S_left | _S_right | _S_internal,
  _S_basefield = _S_dec | _S_oct | _S_hex,
  _S_floatfield = _S_scientific | _S_fixed,
  _S_ios_fmtflags_end = 1L << 16,
  _S_ios_fmtflags_max = __INT_MAX__,
  _S_ios_fmtflags_min = ~__INT_MAX__
};

constexpr _Ios_Fmtflags operator&(_Ios_Fmtflags __a, _Ios_Fmtflags __b) {
  return _Ios_Fmtflags(static_cast<int>(__a) & static_cast<int>(__b));
}

constexpr _Ios_Fmtflags operator|(_Ios_Fmtflags __a, _Ios_Fmtflags __b) {
  return _Ios_Fmtflags(static_cast<int>(__a) | static_cast<int>(__b));
}

constexpr _Ios_Fmtflags operator^(_Ios_Fmtflags __a, _Ios_Fmtflags __b) {
  return _Ios_Fmtflags(static_cast<int>(__a) ^ static_cast<int>(__b));
}

constexpr _Ios_Fmtflags operator~(_Ios_Fmtflags __a) {
  return _Ios_Fmtflags(~static_cast<int>(__a));
}

constexpr const _Ios_Fmtflags &operator|=(_Ios_Fmtflags &__a,
                                          _Ios_Fmtflags __b) {
  return __a = __a | __b;
}

constexpr const _Ios_Fmtflags &operator&=(_Ios_Fmtflags &__a,
                                          _Ios_Fmtflags __b) {
  return __a = __a & __b;
}

constexpr const _Ios_Fmtflags &operator^=(_Ios_Fmtflags &__a,
                                          _Ios_Fmtflags __b) {
  return __a = __a ^ __b;
}

enum _Ios_Openmode {
  _S_app = 1L << 0,
  _S_ate = 1L << 1,
  _S_bin = 1L << 2,
  _S_in = 1L << 3,
  _S_out = 1L << 4,
  _S_trunc = 1L << 5,
  _S_ios_openmode_end = 1L << 16,
  _S_ios_openmode_max = __INT_MAX__,
  _S_ios_openmode_min = ~__INT_MAX__
};

constexpr _Ios_Openmode operator&(_Ios_Openmode __a, _Ios_Openmode __b) {
  return _Ios_Openmode(static_cast<int>(__a) & static_cast<int>(__b));
}

constexpr _Ios_Openmode operator|(_Ios_Openmode __a, _Ios_Openmode __b) {
  return _Ios_Openmode(static_cast<int>(__a) | static_cast<int>(__b));
}

constexpr _Ios_Openmode operator^(_Ios_Openmode __a, _Ios_Openmode __b) {
  return _Ios_Openmode(static_cast<int>(__a) ^ static_cast<int>(__b));
}

constexpr _Ios_Openmode operator~(_Ios_Openmode __a) {
  return _Ios_Openmode(~static_cast<int>(__a));
}

constexpr const _Ios_Openmode &operator|=(_Ios_Openmode &__a,
                                          _Ios_Openmode __b) {
  return __a = __a | __b;
}

constexpr const _Ios_Openmode &operator&=(_Ios_Openmode &__a,
                                          _Ios_Openmode __b) {
  return __a = __a & __b;
}

constexpr const _Ios_Openmode &operator^=(_Ios_Openmode &__a,
                                          _Ios_Openmode __b) {
  return __a = __a ^ __b;
}

enum _Ios_Iostate {
  _S_goodbit = 0,
  _S_badbit = 1L << 0,
  _S_eofbit = 1L << 1,
  _S_failbit = 1L << 2,
  _S_ios_iostate_end = 1L << 16,
  _S_ios_iostate_max = __INT_MAX__,
  _S_ios_iostate_min = ~__INT_MAX__
};

constexpr _Ios_Iostate operator&(_Ios_Iostate __a, _Ios_Iostate __b) {
  return _Ios_Iostate(static_cast<int>(__a) & static_cast<int>(__b));
}

constexpr _Ios_Iostate operator|(_Ios_Iostate __a, _Ios_Iostate __b) {
  return _Ios_Iostate(static_cast<int>(__a) | static_cast<int>(__b));
}

constexpr _Ios_Iostate operator^(_Ios_Iostate __a, _Ios_Iostate __b) {
  return _Ios_Iostate(static_cast<int>(__a) ^ static_cast<int>(__b));
}

constexpr _Ios_Iostate operator~(_Ios_Iostate __a) {
  return _Ios_Iostate(~static_cast<int>(__a));
}

constexpr const _Ios_Iostate &operator|=(_Ios_Iostate &__a, _Ios_Iostate __b) {
  return __a = __a | __b;
}

constexpr const _Ios_Iostate &operator&=(_Ios_Iostate &__a, _Ios_Iostate __b) {
  return __a = __a & __b;
}

constexpr const _Ios_Iostate &operator^=(_Ios_Iostate &__a, _Ios_Iostate __b) {
  return __a = __a ^ __b;
}

using streamsize = std::streamsize;

class ios_base {
public:
  typedef _Ios_Fmtflags fmtflags;
  static const fmtflags boolalpha = _S_boolalpha;
  static const fmtflags dec = _S_dec;
  static const fmtflags fixed = _S_fixed;
  static const fmtflags hex = _S_hex;
  static const fmtflags internal = _S_internal;
  static const fmtflags left = _S_left;
  static const fmtflags oct = _S_oct;
  static const fmtflags right = _S_right;
  static const fmtflags scientific = _S_scientific;
  static const fmtflags showbase = _S_showbase;
  static const fmtflags showpoint = _S_showpoint;
  static const fmtflags showpos = _S_showpos;
  static const fmtflags skipws = _S_skipws;
  static const fmtflags unitbuf = _S_unitbuf;
  static const fmtflags uppercase = _S_uppercase;
  static const fmtflags adjustfield = _S_adjustfield;
  static const fmtflags basefield = _S_basefield;
  static const fmtflags floatfield = _S_floatfield;

  typedef _Ios_Iostate iostate;
  static constexpr iostate badbit = _S_badbit;
  static constexpr iostate eofbit = _S_eofbit;
  static constexpr iostate failbit = _S_failbit;
  static constexpr iostate goodbit = _S_goodbit;

  typedef _Ios_Openmode openmode;
  static constexpr openmode app = _S_app;
  static constexpr openmode ate = _S_ate;
  static constexpr openmode binary = _S_bin;
  static constexpr openmode in = _S_in;
  static constexpr openmode out = _S_out;
  static constexpr openmode trunc = _S_trunc;

protected:
  streamsize _M_precision;
  streamsize _M_width;
  fmtflags _M_flags;
  iostate _M_exception;
  iostate _M_streambuf_state;

  // locale     _M_ios_locale;

  // from src/c++98/ios_locale.cc
  constexpr void _M_init() throw() {
    _M_precision = 6;
    _M_width = 0;
    _M_flags = skipws | dec;
    //    _M_ios_locale = locale(); // std::locale is not constexpr
  }

public:
  constexpr fmtflags flags() const { return _M_flags; }
};

constexpr ios_base &dec(ios_base &str) { return str; }
constexpr ios_base &hex(ios_base &str) { return str; }
constexpr ios_base &oct(ios_base &str) { return str; }

} // namespace cest

#endif // _CEST_IOS_BASE_HPP_
