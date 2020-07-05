#ifndef _CEST_IOS_HPP_
#define _CEST_IOS_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/ios_tests.hpp

#include "streambuf.hpp"
#include <string>  // std::char_traits
#include <ios>     // std::streamsize

namespace cest {

  enum _Ios_Fmtflags
  {
    _S_boolalpha  = 1L << 0,
    _S_dec    = 1L << 1,
    _S_fixed    = 1L << 2,
    _S_hex    = 1L << 3,
    _S_internal   = 1L << 4,
    _S_left     = 1L << 5,
    _S_oct    = 1L << 6,
    _S_right    = 1L << 7,
    _S_scientific   = 1L << 8,
    _S_showbase   = 1L << 9,
    _S_showpoint  = 1L << 10,
    _S_showpos  = 1L << 11,
    _S_skipws   = 1L << 12,
    _S_unitbuf  = 1L << 13,
    _S_uppercase  = 1L << 14,
    _S_adjustfield  = _S_left | _S_right | _S_internal,
    _S_basefield  = _S_dec | _S_oct | _S_hex,
    _S_floatfield   = _S_scientific | _S_fixed,
    _S_ios_fmtflags_end = 1L << 16,
    _S_ios_fmtflags_max = __INT_MAX__,
    _S_ios_fmtflags_min = ~__INT_MAX__
  };

  constexpr _Ios_Fmtflags
  operator&(_Ios_Fmtflags __a, _Ios_Fmtflags __b)
  { return _Ios_Fmtflags(static_cast<int>(__a) & static_cast<int>(__b)); }

  constexpr _Ios_Fmtflags
  operator|(_Ios_Fmtflags __a, _Ios_Fmtflags __b)
  { return _Ios_Fmtflags(static_cast<int>(__a) | static_cast<int>(__b)); }

  constexpr _Ios_Fmtflags
  operator^(_Ios_Fmtflags __a, _Ios_Fmtflags __b)
  { return _Ios_Fmtflags(static_cast<int>(__a) ^ static_cast<int>(__b)); }

  constexpr _Ios_Fmtflags
  operator~(_Ios_Fmtflags __a)
  { return _Ios_Fmtflags(~static_cast<int>(__a)); }

  constexpr const _Ios_Fmtflags&
  operator|=(_Ios_Fmtflags& __a, _Ios_Fmtflags __b)
  { return __a = __a | __b; }

  constexpr const _Ios_Fmtflags&
  operator&=(_Ios_Fmtflags& __a, _Ios_Fmtflags __b)
  { return __a = __a & __b; }

  constexpr const _Ios_Fmtflags&
  operator^=(_Ios_Fmtflags& __a, _Ios_Fmtflags __b)
  { return __a = __a ^ __b; }

  enum _Ios_Openmode
  {
    _S_app    = 1L << 0,
    _S_ate    = 1L << 1,
    _S_bin    = 1L << 2,
    _S_in     = 1L << 3,
    _S_out    = 1L << 4,
    _S_trunc  = 1L << 5,
    _S_ios_openmode_end = 1L << 16,
    _S_ios_openmode_max = __INT_MAX__,
    _S_ios_openmode_min = ~__INT_MAX__
  };

  constexpr _Ios_Openmode
  operator&(_Ios_Openmode __a, _Ios_Openmode __b)
  { return _Ios_Openmode(static_cast<int>(__a) & static_cast<int>(__b)); }

  constexpr _Ios_Openmode
  operator|(_Ios_Openmode __a, _Ios_Openmode __b)
  { return _Ios_Openmode(static_cast<int>(__a) | static_cast<int>(__b)); }

  constexpr _Ios_Openmode
  operator^(_Ios_Openmode __a, _Ios_Openmode __b)
  { return _Ios_Openmode(static_cast<int>(__a) ^ static_cast<int>(__b)); }

  constexpr _Ios_Openmode
  operator~(_Ios_Openmode __a)
  { return _Ios_Openmode(~static_cast<int>(__a)); }

  constexpr const _Ios_Openmode&
  operator|=(_Ios_Openmode& __a, _Ios_Openmode __b)
  { return __a = __a | __b; }

  constexpr const _Ios_Openmode&
  operator&=(_Ios_Openmode& __a, _Ios_Openmode __b)
  { return __a = __a & __b; }

  constexpr const _Ios_Openmode&
  operator^=(_Ios_Openmode& __a, _Ios_Openmode __b)
  { return __a = __a ^ __b; }

  enum _Ios_Iostate
  {
    _S_goodbit    = 0,
    _S_badbit     = 1L << 0,
    _S_eofbit     = 1L << 1,
    _S_failbit    = 1L << 2,
    _S_ios_iostate_end = 1L << 16,
    _S_ios_iostate_max = __INT_MAX__,
    _S_ios_iostate_min = ~__INT_MAX__
  };

  constexpr _Ios_Iostate
  operator&(_Ios_Iostate __a, _Ios_Iostate __b)
  { return _Ios_Iostate(static_cast<int>(__a) & static_cast<int>(__b)); }

  constexpr _Ios_Iostate
  operator|(_Ios_Iostate __a, _Ios_Iostate __b)
  { return _Ios_Iostate(static_cast<int>(__a) | static_cast<int>(__b)); }

  constexpr _Ios_Iostate
  operator^(_Ios_Iostate __a, _Ios_Iostate __b)
  { return _Ios_Iostate(static_cast<int>(__a) ^ static_cast<int>(__b)); }

  constexpr _Ios_Iostate
  operator~(_Ios_Iostate __a)
  { return _Ios_Iostate(~static_cast<int>(__a)); }

  constexpr const _Ios_Iostate&
  operator|=(_Ios_Iostate& __a, _Ios_Iostate __b)
  { return __a = __a | __b; }

  constexpr const _Ios_Iostate&
  operator&=(_Ios_Iostate& __a, _Ios_Iostate __b)
  { return __a = __a & __b; }

  constexpr const  _Ios_Iostate&
  operator^=(_Ios_Iostate& __a, _Ios_Iostate __b)
  { return __a = __a ^ __b; }

  using streamsize = std::streamsize;

struct ios_base {

  typedef _Ios_Fmtflags fmtflags;
  static const fmtflags boolalpha =   _S_boolalpha;
  static const fmtflags dec =         _S_dec;
  static const fmtflags fixed =       _S_fixed;
  static const fmtflags hex =         _S_hex;
  static const fmtflags internal =    _S_internal;
  static const fmtflags left =        _S_left;
  static const fmtflags oct =         _S_oct;
  static const fmtflags right =       _S_right;
  static const fmtflags scientific =  _S_scientific;
  static const fmtflags showbase =    _S_showbase;
  static const fmtflags showpoint =   _S_showpoint;
  static const fmtflags showpos =     _S_showpos;
  static const fmtflags skipws =      _S_skipws;
  static const fmtflags unitbuf =     _S_unitbuf;
  static const fmtflags uppercase =   _S_uppercase;
  static const fmtflags adjustfield = _S_adjustfield;
  static const fmtflags basefield =   _S_basefield;
  static const fmtflags floatfield =  _S_floatfield;

  typedef _Ios_Iostate iostate;
  static constexpr iostate badbit  = _S_badbit;
  static constexpr iostate eofbit  = _S_eofbit;
  static constexpr iostate failbit = _S_failbit;
  static constexpr iostate goodbit = _S_goodbit;

  typedef _Ios_Openmode openmode;
  static constexpr openmode app    = _S_app;
  static constexpr openmode ate    = _S_ate;
  static constexpr openmode binary = _S_bin;
  static constexpr openmode in     = _S_in;
  static constexpr openmode out    = _S_out;
  static constexpr openmode trunc  = _S_trunc;

protected:
  streamsize _M_precision;
  streamsize _M_width;
  fmtflags   _M_flags;
  iostate    _M_exception;
  iostate    _M_streambuf_state;

public:
  constexpr fmtflags
  flags() const
  { return _M_flags; }
};

constexpr ios_base& dec(ios_base& str) { return str; }
constexpr ios_base& hex(ios_base& str) { return str; }
constexpr ios_base& oct(ios_base& str) { return str; }

template <class, class> class basic_ostream; // forward declaration
template <class, class> class basic_istream; // forward declaration

template <
  class _CharT,
  class _Traits = std::char_traits<_CharT>
> class basic_ios : public ios_base
{
public:
  typedef _CharT                                 char_type;
  typedef typename _Traits::int_type             int_type;
  typedef typename _Traits::pos_type             pos_type;
  typedef typename _Traits::off_type             off_type;
  typedef _Traits                                traits_type;

  typedef std::ctype<_CharT>                          __ctype_type;
  typedef std::num_put<_CharT, std::ostreambuf_iterator<_CharT, _Traits> >
             __num_put_type;
  typedef std::num_get<_CharT, std::istreambuf_iterator<_CharT, _Traits> >
             __num_get_type;

protected:
  basic_ostream<_CharT, _Traits>*                _M_tie;
  mutable char_type                              _M_fill;
  mutable bool                                   _M_fill_init;
  basic_streambuf<_CharT, _Traits>*              _M_streambuf;

  const __ctype_type*                            _M_ctype;
  const __num_put_type*                          _M_num_put;
  const __num_get_type*                          _M_num_get;

public:

  explicit operator bool() const
  { return !this->fail(); }

  bool
  operator!() const
  { return this->fail(); }

  constexpr iostate
  rdstate() const
  { return _M_streambuf_state; }

  constexpr void clear(iostate __state = goodbit); 

  constexpr void
  setstate(iostate __state)
  { this->clear(this->rdstate() | __state); }

  constexpr void                                             
  _M_setstate(iostate __state)
  {
    _M_streambuf_state |= __state;
    if (this->exceptions() & __state)
      __throw_exception_again;
  }

  constexpr bool
  good() const
  { return this->rdstate() == 0; }

  constexpr bool
  eof() const
  { return (this->rdstate() & eofbit) != 0; }

  constexpr bool
  fail() const
  { return (this->rdstate() & (badbit | failbit)) != 0; }

  constexpr iostate
  exceptions() const
  { return _M_exception; }

  constexpr void
  exceptions(iostate __except)
  {
    _M_exception = __except;
    this->clear(_M_streambuf_state);
  }

  explicit constexpr
  basic_ios(basic_streambuf<_CharT, _Traits>* __sb)
  : ios_base(), _M_tie(0), _M_fill(), _M_fill_init(false), _M_streambuf(0),
_M_ctype(0), _M_num_put(0), _M_num_get(0)
  { this->init(__sb); }

  virtual constexpr
  ~basic_ios() { }

  constexpr basic_ostream<_CharT, _Traits>*                              
  tie() const                        
  { return _M_tie; }

  constexpr basic_streambuf<_CharT, _Traits>*
  rdbuf() const
  { return _M_streambuf; }

  constexpr basic_ios()
  : ios_base(), _M_tie(0), _M_fill(char_type()), _M_fill_init(false),
_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
  { }
};

  // from basic_ios.tcc
  template<typename _CharT, typename _Traits>
  constexpr void                                                           
  basic_ios<_CharT, _Traits>::clear(iostate __state)       
  {                 
    if (this->rdbuf())
      _M_streambuf_state = __state;   
    else
      _M_streambuf_state = __state | badbit;
    //if (this->exceptions() & this->rdstate())                
      //__throw_ios_failure(__N("basic_ios::clear"));
  }

} // namespace cest

#endif // _CEST_IOS_HPP_
