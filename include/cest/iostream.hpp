#ifndef _CEST_IOSTREAM_HPP_
#define _CEST_IOSTREAM_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/iostream_tests.hpp

namespace cest {

template<
  class CharT,
  class Traits = std::char_traits<CharT>
> struct basic_ostream : virtual public std::basic_ios<CharT, Traits>
{
  template <typename T>
  constexpr basic_ostream &operator<<(const T&) { return *this; }
  constexpr basic_ostream &operator<<(basic_ostream &(* const fp)(basic_ostream &)) {
    return *this;
  }
};

template< class CharT, class Traits >
constexpr basic_ostream<CharT, Traits> &endl(basic_ostream<CharT, Traits> &os) {
  return os;
}

using ostream = basic_ostream<char>;

ostream cout;
ostream cerr;

} // namespace cest

#endif // _CEST_IOSTREAM_HPP_
