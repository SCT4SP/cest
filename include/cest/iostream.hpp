#ifndef _CEST_IOSTREAM_HPP_
#define _CEST_IOSTREAM_HPP_

#include "ostream.hpp"
#include "sstream.hpp"
#include "string.hpp"

namespace cest {

namespace impl {
inline basic_stringbuf cout(string("cout"));
inline basic_stringbuf cerr(string("cerr"));
inline basic_stringbuf clog(string("clog"));
inline basic_stringbuf cin(string("cin"));
} // namespace impl

inline ostream cout(&impl::cout);
inline ostream cerr(&impl::cerr);
inline ostream clog(&impl::clog);
inline istream cin(&impl::cin);

using iostream = basic_iostream<char>;
using wiostream = basic_iostream<wchar_t>;

} // namespace cest

#endif // _CEST_IOSTREAM_HPP_
