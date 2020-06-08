#ifndef _CEST_IOS_HPP_
#define _CEST_IOS_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/ios_tests.hpp

#include <string>  // std::char_traits

namespace cest {

struct ios_base {
  using openmode = unsigned;             // check in libstdc++
  static constexpr openmode in = 1 << 2; // ""
};

constexpr ios_base& dec(ios_base& str) { return str; }
constexpr ios_base& hex(ios_base& str) { return str; }
constexpr ios_base& oct(ios_base& str) { return str; }

template <
  class CharT,
  class Traits = std::char_traits<CharT>
> class basic_ios : public ios_base
{
public:
  explicit constexpr operator bool() const { return true; }
           constexpr bool good()     const { return true; }
           constexpr bool eof()      const { return true; }
           constexpr bool fail()     const { return true; }
};

} // namespace cest

#endif // _CEST_IOS_HPP_
