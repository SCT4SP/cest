#ifndef _CEST_FSTREAM_HPP_
#define _CEST_FSTREAM_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/iostream_tests.hpp

#include "istream.hpp" // cest::basic_istream
#include <string>      // std::char_traits

namespace cest {

template <
  class CharT,
  class Traits = std::char_traits<CharT>
> class basic_ifstream : public basic_istream<CharT, Traits>
{
public:
  using char_type   = CharT;
  using traits_type = Traits;
  using int_type    = typename Traits::int_type;
  using pos_type    = typename Traits::pos_type;
  using off_type    = typename Traits::off_type;

  explicit constexpr basic_ifstream( const char* filename,
                                     ios_base::openmode mode = ios_base::in ) {
  }
};

using  ifstream = basic_ifstream<char>;
using wifstream = basic_ifstream<wchar_t>;

} // namespace cest

#endif // _CEST_FSTREAM_HPP_
