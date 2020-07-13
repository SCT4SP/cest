#ifndef _CEST_ISTREAM_HPP_
#define _CEST_ISTREAM_HPP_

#include "ios.hpp" // cest::basic_ios
#include "streambuf.hpp"
#include <string>  // std::char_traits

namespace CEST_NAMESPACE {

template <
  class _CharT,
  class _Traits = std::char_traits<_CharT>
> class basic_istream : /*virtual*/ public basic_ios<_CharT, _Traits>
{
public:
  typedef _CharT          char_type;
  typedef typename _Traits::int_type    int_type;
  typedef typename _Traits::pos_type    pos_type;
  typedef typename _Traits::off_type    off_type;
  typedef _Traits         traits_type;

  typedef basic_streambuf<_CharT, _Traits>    __streambuf_type;
  typedef basic_ios<_CharT, _Traits>    __ios_type;
  typedef basic_istream<_CharT, _Traits>    __istream_type;
  typedef std::num_get<_CharT, std::istreambuf_iterator<_CharT, _Traits> >
          __num_get_type;
  typedef std::ctype<_CharT>             __ctype_type;

protected:
  streamsize    _M_gcount;

public:
  virtual constexpr
  ~basic_istream()
  { _M_gcount = streamsize(0); }

  class sentry;
  friend class sentry;

  constexpr
  __istream_type&
  get(char_type& ch);

  constexpr
  __istream_type&
  unget();

protected:
  constexpr basic_istream() { }
};

using  istream = basic_istream<char>;
using wistream = basic_istream<wchar_t>;

  template<typename _CharT, typename _Traits>
  class basic_istream<_CharT, _Traits>::sentry
  {
    bool _M_ok;

  public:
    typedef _Traits           traits_type;
    typedef basic_streambuf<_CharT, _Traits>    __streambuf_type;
    typedef basic_istream<_CharT, _Traits>    __istream_type;
    typedef typename __istream_type::__ctype_type   __ctype_type;
    typedef typename _Traits::int_type    __int_type;

    explicit constexpr
    sentry(basic_istream<_CharT, _Traits>& __is, bool __noskipws = false);

    explicit constexpr
    operator bool() const
    { return _M_ok; }
  };

  // from istream.tcc
  template<typename _CharT, typename _Traits>
    constexpr basic_istream<_CharT, _Traits>::sentry::
    sentry(basic_istream<_CharT, _Traits>& __in, bool __noskip) : _M_ok(false)
    {
      ios_base::iostate __err = ios_base::goodbit;
      if (__in.good())
  __try
    {
      if (__in.tie())
        __in.tie()->flush();
      if (!__noskip && bool(__in.flags() & ios_base::skipws))
        {
    const __int_type __eof = traits_type::eof();
    __streambuf_type* __sb = __in.rdbuf();
    __int_type __c = __sb->sgetc();

    const __ctype_type& __ct = __check_facet(__in._M_ctype);
    while (!traits_type::eq_int_type(__c, __eof)
           && __ct.is(std::ctype_base::space,
          traits_type::to_char_type(__c)))
      __c = __sb->snextc();

    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // 195. Should basic_istream::sentry's constructor ever
    // set eofbit?
    if (traits_type::eq_int_type(__c, __eof))
      __err |= ios_base::eofbit;
        }
    }
  __catch(__cxxabiv1::__forced_unwind&)
    {
      __in._M_setstate(ios_base::badbit);
      __throw_exception_again;
    }
  __catch(...)
    { __in._M_setstate(ios_base::badbit); }

      if (__in.good() && __err == ios_base::goodbit)
  _M_ok = true;
      else
  {
    __err |= ios_base::failbit;
    __in.setstate(__err);
  }
    }

  // from istream.tcc
  template<typename _CharT, typename _Traits>
    constexpr basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    get(char_type& __c)
    {
      _M_gcount = 0;
      ios_base::iostate __err = ios_base::goodbit;
      sentry __cerb(*this, true);
      if (__cerb)
  {
    __try
      {
        const int_type __cb = this->rdbuf()->sbumpc();
        // 27.6.1.1 paragraph 3
        if (!traits_type::eq_int_type(__cb, traits_type::eof()))
    {
      _M_gcount = 1;
      __c = traits_type::to_char_type(__cb);
    }
        else
    __err |= ios_base::eofbit;
      }
    __catch(__cxxabiv1::__forced_unwind&)
      {
        this->_M_setstate(ios_base::badbit);
        __throw_exception_again;
      }
    __catch(...)
      { this->_M_setstate(ios_base::badbit); }
  }
      if (!_M_gcount)
  __err |= ios_base::failbit;
      if (__err)
  this->setstate(__err);
      return *this;
    }

  // from istream.tcc
  template<typename _CharT, typename _Traits>
    constexpr basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    unget(void)
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 60. What is a formatted input function?
      _M_gcount = 0;
      // Clear eofbit per N3168.
      this->clear(this->rdstate() & ~ios_base::eofbit);
      sentry __cerb(*this, true);
      if (__cerb)
  {
    ios_base::iostate __err = ios_base::goodbit;
    __try
      {
        const int_type __eof = traits_type::eof();
        __streambuf_type* __sb = this->rdbuf();
        if (!__sb
      || traits_type::eq_int_type(__sb->sungetc(), __eof))
    __err |= ios_base::badbit;
      }
    __catch(__cxxabiv1::__forced_unwind&)
      {
        this->_M_setstate(ios_base::badbit);
        __throw_exception_again;
      }
    __catch(...)
      { this->_M_setstate(ios_base::badbit); }
    if (__err)
      this->setstate(__err);
  }
      return *this;
    }

  template<typename _CharT, typename _Traits>
    class basic_iostream
    : public basic_istream<_CharT, _Traits>//,
//      public basic_ostream<_CharT, _Traits>
    {
    };

} // namespace CEST_NAMESPACE

#endif // _CEST_ISTREAM_HPP_
