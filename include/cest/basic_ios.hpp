#ifndef _CEST_BASIC_IOS_HPP_
#define _CEST_BASIC_IOS_HPP_

#include "streambuf.hpp"
#include "ios_base.hpp"
#include <string>  // std::char_traits

namespace CEST_NAMESPACE {

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

  explicit constexpr operator bool() const
  { return !this->fail(); }

  constexpr bool
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

  constexpr void init(basic_streambuf<_CharT, _Traits>* __sb);
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
    if (this->exceptions() & this->rdstate())                
      std::__throw_ios_failure(__N("basic_ios::clear"));
  }

  template<typename _CharT, typename _Traits>
  constexpr void
  basic_ios<_CharT, _Traits>::init(basic_streambuf<_CharT, _Traits>* __sb)
  {
    ios_base::_M_init();

//    _M_cache_locale(_M_ios_locale);

    _M_fill = _CharT();
    _M_fill_init = false;

    _M_tie = 0;
    _M_exception = goodbit;
    _M_streambuf = __sb;
    _M_streambuf_state = __sb ? goodbit : badbit;
  }

} // namespace CEST_NAMESPACE

#endif // _CEST_BASIC_IOS_HPP_
