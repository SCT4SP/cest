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

  streamsize _M_gcount;

  virtual constexpr ~basic_istream() { /* _M_gcount = streamsize(0); */ }

  protected:
  constexpr basic_istream() : _M_gcount(streamsize(0))
  { /*this->init(0);*/ }

  constexpr basic_istream& get(char_type& ch) {
    return *this;
  }

  constexpr basic_istream& unget() {
    return *this;
  }
};

using  istream = basic_istream<char>;
using wistream = basic_istream<wchar_t>;

} // namespace cest

#endif // _CEST_ISTREAM_HPP_
