#ifndef _CEST_IOSTREAM_HPP_
#define _CEST_IOSTREAM_HPP_

#include "string.hpp"
#include "ostream.hpp"
#include "sstream.hpp"

namespace cest {

namespace impl {
  basic_stringbuf cout(string("cout"));
  basic_stringbuf cerr(string("cerr"));
  basic_stringbuf clog(string("clog"));
}

ostream cout(&impl::cout);
ostream cerr(&impl::cerr);
ostream clog(&impl::clog);

using iostream  = basic_iostream<char>;
using wiostream = basic_iostream<wchar_t>;

} // namespace cest

#endif // _CEST_IOSTREAM_HPP_
