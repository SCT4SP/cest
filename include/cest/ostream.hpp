#ifndef _CEST_OSTREAM_HPP_
#define _CEST_OSTREAM_HPP_

#include "ios.hpp"             // cest::basic_ios
#include "streambuf.hpp"       // cest::basic_streambuf
#include "runtime_ostream.hpp" // cest::runtime_ostream 
#include <string>              // std::char_traits
#include <iostream>            // std::cout, std::cerr, std::clog
#include <locale>              // std::ctype, std::num_put
#include <iterator>            // std::ostreambuf_iterator
#include <exception>           // std::uncaught_exception

namespace cest {

template <class _CharT, class _Traits>
  class basic_ostream : /*virtual*/ public basic_ios<_CharT, _Traits>
{
public:
  typedef _CharT          char_type;
  typedef typename _Traits::int_type    int_type;
  typedef typename _Traits::pos_type    pos_type;
  typedef typename _Traits::off_type    off_type;
  typedef _Traits         traits_type;

  typedef basic_streambuf<_CharT, _Traits>    __streambuf_type;
  typedef basic_ios<_CharT, _Traits>    __ios_type;
  typedef basic_ostream<_CharT, _Traits>    __ostream_type;
  typedef std::num_put<_CharT, std::ostreambuf_iterator<_CharT, _Traits> >
          __num_put_type;
  typedef std::ctype<_CharT>             __ctype_type;

  explicit constexpr
  basic_ostream(__streambuf_type* __sb)
  { this->init(__sb); }

  virtual constexpr
  ~basic_ostream() { }

  class sentry;
  friend class sentry;

  // for endl
  constexpr __ostream_type&
  operator<<(__ostream_type& (*__pf)(__ostream_type&)) {
    return __pf(*this);
  }

  constexpr __ostream_type&
  operator<<(__ios_type&     (*__pf)(__ios_type&))     {
    __pf(*this);
    return *this;
  }

  // for hex
  constexpr __ostream_type&
  operator<<(ios_base&       (*__pf)(ios_base&))       {
    __pf(*this);
    return *this;
  }

  constexpr __ostream_type&
  operator<<(long __n)
  { return _M_insert(__n); }

  constexpr __ostream_type& operator<<(int value)                             {
    impl::runtime_ostream(*this, value);
    return *this;
  }

  constexpr __ostream_type& operator<<(unsigned int value)                    {
    impl::runtime_ostream(*this, value);
    return *this;
  }

  constexpr __ostream_type& flush();

  protected:
  constexpr basic_ostream()
  { this->init(0); }

  template<typename _ValueT>
  constexpr __ostream_type&
  _M_insert(_ValueT __v);
};

using  ostream = basic_ostream<char>;
using wostream = basic_ostream<wchar_t>;

template< class CharT, class Traits>
constexpr basic_ostream<CharT,Traits>&
operator<<(basic_ostream<CharT,Traits>& os, CharT ch)
{
  impl::runtime_ostream(os, ch);
  return os;
}

template <class CharT, class Traits>
constexpr basic_ostream<CharT,Traits>&
operator<<(basic_ostream<CharT,Traits>& os, const CharT* s)
{
  impl::runtime_ostream(os, s);
  return os;
}

template <class CharT, class Traits >
constexpr basic_ostream<CharT,Traits> &
endl(basic_ostream<CharT, Traits> &os)
{
  impl::runtime_ostream(os, std::endl);
  return os;
}

  template <typename _CharT, typename _Traits>
    class basic_ostream<_CharT, _Traits>::sentry
    {
      // Data Members.
      bool        _M_ok;
      basic_ostream<_CharT, _Traits>&   _M_os;

    public:
      explicit                                          
      sentry(basic_ostream<_CharT, _Traits>& __os);
                                                                          
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
      ~sentry()
      {                                                       
  // XXX MT                                                                
  if (bool(_M_os.flags() & ios_base::unitbuf) && !std::uncaught_exception())
    {   
      // Can't call flush directly or else will get into recursive lock.
      if (_M_os.rdbuf() && _M_os.rdbuf()->pubsync() == -1)
        _M_os.setstate(ios_base::badbit);
    }                                                                  
      }
#pragma GCC diagnostic pop
                                                          
      explicit
      operator bool() const                                       
      { return _M_ok; }
    };

  // from ostream.tcc
  template<typename _CharT, typename _Traits>
    constexpr basic_ostream<_CharT, _Traits>&
    basic_ostream<_CharT, _Traits>::
    flush()
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // DR 60. What is a formatted input function?
      // basic_ostream::flush() is *not* an unformatted output function.
      ios_base::iostate __err = ios_base::goodbit;
      __try
  {
    if (this->rdbuf() && this->rdbuf()->pubsync() == -1)
      __err |= ios_base::badbit;
  }
#if !defined(_LIBCPP_VERSION) // __forced_unwind is a libstdc++/gcc thing
      __catch(__cxxabiv1::__forced_unwind&)
  {
    this->_M_setstate(ios_base::badbit);
    __throw_exception_again;
  }
#endif
      __catch(...)
  { this->_M_setstate(ios_base::badbit); }
      if (__err)
  this->setstate(__err);
      return *this;
    }

  // from ostream.tcc
  template<typename _CharT, typename _Traits>
    template<typename _ValueT>
      constexpr basic_ostream<_CharT, _Traits>&
      basic_ostream<_CharT, _Traits>::
      _M_insert(_ValueT __v)
      {
  sentry __cerb(*this);
  if (__cerb)
    {
      ios_base::iostate __err = ios_base::goodbit;
      __try
        {
    const __num_put_type& __np = __check_facet(this->_M_num_put);
    if (__np.put(*this, *this, this->fill(), __v).failed())
      __err |= ios_base::badbit;
        }
#if !defined(_LIBCPP_VERSION) // __forced_unwind is a libstdc++/gcc thing
      __catch(__cxxabiv1::__forced_unwind&)
        {
    this->_M_setstate(ios_base::badbit);
    __throw_exception_again;
        }
#endif
      __catch(...)
        { this->_M_setstate(ios_base::badbit); }
      if (__err)
        this->setstate(__err);
    }
  return *this;
      }

} // namespace cest

#endif // _CEST_OSTREAM_HPP_
