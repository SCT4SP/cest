#ifndef _CEST_IOSTREAM_HPP_
#define _CEST_IOSTREAM_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/ostream_tests.hpp

#include "string.hpp"
#include "ostream.hpp"
#include "sstream.hpp"

namespace cest {

namespace impl {
  basic_stringbuf cout(string("cout"));
  basic_stringbuf cerr(string("cerr"));
}

ostream cout(&impl::cout);
ostream cerr(&impl::cerr);

} // namespace cest

#endif // _CEST_IOSTREAM_HPP_
