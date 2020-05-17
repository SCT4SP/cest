#ifndef _CEST_OSTREAM_HPP_
#define _CEST_OSTREAM_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/iostream_tests.hpp

#include "ios.hpp" // cest::basic_ios
#include <string>  // std::char_traits

namespace cest {

template <
  class CharT,
  class Traits = std::char_traits<CharT>
> struct basic_ostream : virtual public basic_ios<CharT, Traits>
{
  using char_type   = CharT;
  using traits_type = Traits;
  using int_type    = Traits::int_type;
  using pos_type    = Traits::pos_type;
  using off_type    = Traits::off_type;

  // for endl
  constexpr basic_ostream& operator<<(basic_ostream& (*pf)(basic_ostream&))  {
    return *this;
  }

  // for hex
  constexpr basic_ostream& operator<<(ios_base&      (*pf)(ios_base&))       {
    return *this;
  }

  constexpr basic_ostream& operator<<(int value)                             {
    return *this;
  }

  constexpr basic_ostream& operator<<(unsigned int value)                    {
    return *this;
  }
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

} // namespace cest

#endif // _CEST_OSTREAM_HPP_
