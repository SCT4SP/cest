#ifndef _CEST_OSTREAM_HPP_
#define _CEST_OSTREAM_HPP_

#include "ios.hpp" // cest::basic_ios
#include "streambuf.hpp"
#include <string>  // std::char_traits

namespace cest {

template <
  class _CharT,
  class _Traits = std::char_traits<_CharT>
> class basic_ostream : /*virtual*/ public basic_ios<_CharT, _Traits>
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
  typedef std::num_get<_CharT, std::ostreambuf_iterator<_CharT, _Traits> >
          __num_get_type;
  typedef std::ctype<_CharT>             __ctype_type;

  explicit constexpr
  basic_ostream(__streambuf_type* __sb)
  { /*this->init(__sb);*/ }

  // for endl
  constexpr basic_ostream& operator<<(basic_ostream& (*pf)(basic_ostream&))  {
    return pf(*this);
  }

  // for hex
  constexpr basic_ostream& operator<<(ios_base&      (*pf)(ios_base&))       {
    pf(*this);
    return *this;
  }

  constexpr basic_ostream& operator<<(int value)                             {
    return *this;
  }

  constexpr basic_ostream& operator<<(unsigned int value)                    {
    return *this;
  }

  constexpr __ostream_type& flush();

  protected:
  constexpr basic_ostream()
  { /*this->init(0);*/ }
};

template< class CharT, class Traits>
constexpr basic_ostream<CharT,Traits>&
operator<<(basic_ostream<CharT,Traits>& os, CharT ch) {
  return os;
}

template <class CharT, class Traits>
constexpr basic_ostream<CharT,Traits>&
operator<<(basic_ostream<CharT,Traits>& os, const CharT* s) {
  return os;
}

template <class CharT, class Traits >
constexpr basic_ostream<CharT,Traits> &
endl(basic_ostream<CharT, Traits> &os) {
  return os;
}

using ostream = basic_ostream<char>;

// from ostream.tcc
  template<typename _CharT, typename _Traits>
    constexpr basic_ostream<_CharT, _Traits>&
    basic_ostream<_CharT, _Traits>::
    flush()
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // DR 60. What is a formatted input function?
      // basic_ostream::flush() is *not* an unformatted output function.
/*      ios_base::iostate __err = ios_base::goodbit;
      __try
  {
    if (this->rdbuf() && this->rdbuf()->pubsync() == -1)
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
*/
      return *this;
    }

} // namespace cest

#endif // _CEST_OSTREAM_HPP_
