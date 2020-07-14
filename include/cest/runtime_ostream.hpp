#ifndef _CEST_RUNTIME_OSTREAM_HPP_
#define _CEST_RUNTIME_OSTREAM_HPP_

#include "basic_ios.hpp"
#include <iostream>      // std::cout, std::cerr, std::clog

namespace CEST_NAMESPACE {

namespace impl {

template <class CharT, class Traits, class T>
constexpr void runtime_ostream(basic_ostream<CharT,Traits>& os, T x)
{
  if (!std::is_constant_evaluated())
    if      (&cest::cout == &os) std::cout << x;
    else if (&cest::cerr == &os) std::cerr << x;
    else if (&cest::clog == &os) std::clog << x;
}

// madness for endl
template <class CharT, class Traits>
constexpr void runtime_ostream(
       basic_ostream<CharT,Traits>& os,
  std::basic_ostream<CharT,Traits>& (*fp)(std::basic_ostream<CharT,Traits>&)
)
{
  if (!std::is_constant_evaluated())
    if      (&cest::cout == &os) std::cout << fp;
    else if (&cest::cerr == &os) std::cerr << fp;
    else if (&cest::clog == &os) std::clog << fp;
}

} // namespace impl

} // namespace CEST_NAMESPACE

#endif // __CEST_RUNTIME_OSTREAM_HPP__
