#ifndef _CEST_IOSTREAM_HPP_
#define _CEST_IOSTREAM_HPP_

#include "ostream.hpp"
#include "sstream.hpp"
#include "string.hpp"

namespace cest {

namespace impl {
basic_stringbuf cout(string("cout"));
basic_stringbuf cerr(string("cerr"));
basic_stringbuf clog(string("clog"));
basic_stringbuf cin(string("cin"));
} // namespace impl

ostream cout(&impl::cout);
ostream cerr(&impl::cerr);
ostream clog(&impl::clog);
istream cin(&impl::cin);

using iostream = basic_iostream<char>;
using wiostream = basic_iostream<wchar_t>;

} // namespace cest

#endif // _CEST_IOSTREAM_HPP_
