#ifndef _CEST_ISTREAM_HPP_
#define _CEST_ISTREAM_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/iostream_tests.hpp

#include "ios.hpp" // cest::basic_ios
#include <string>  // std::char_traits

namespace cest {

template <
  class _CharT,
  class _Traits = std::char_traits<_CharT>
> class basic_istream : /*virtual*/ public basic_ios<_CharT, _Traits>
{
public:
  using char_type   = _CharT;
  using traits_type = _Traits;
  using int_type    = typename traits_type::int_type;
  using pos_type    = typename traits_type::pos_type;
  using off_type    = typename traits_type::off_type;

  virtual constexpr ~basic_istream() { }

  constexpr basic_istream& get(char_type& ch);
//  { return *this; }

  constexpr basic_istream& unget() {
    return *this;
  }

  protected:
  constexpr basic_istream() { }
};

using  istream = basic_istream<char>;
using wistream = basic_istream<wchar_t>;

  // from istream.cc
  template<typename _CharT, typename _Traits>
    constexpr basic_istream<_CharT, _Traits>&
    basic_istream<_CharT, _Traits>::
    get(char_type& __c)
    {
      /*_M_gcount = 0;
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
      return *this;*/
      return *this;
    }

} // namespace cest

#endif // _CEST_ISTREAM_HPP_
