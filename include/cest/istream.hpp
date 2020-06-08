#ifndef _CEST_ISTREAM_HPP_
#define _CEST_ISTREAM_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/iostream_tests.hpp

#include "ios.hpp" // cest::basic_ios
#include <string>  // std::char_traits

namespace cest {

template <
  class CharT,
  class Traits = std::char_traits<CharT>
> class basic_istream : virtual public basic_ios<CharT, Traits>
{
public:
  using char_type   = CharT;
  using traits_type = Traits;
  using int_type    = typename Traits::int_type;
  using pos_type    = typename Traits::pos_type;
  using off_type    = typename Traits::off_type;

  constexpr basic_istream& get(char_type& ch);
  constexpr basic_istream& unget();
};

using  istream = basic_istream<char>;
using wistream = basic_istream<wchar_t>;

} // namespace cest

#endif // _CEST_ISTREAM_HPP_
