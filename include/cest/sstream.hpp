#ifndef _CEST_SSTREAM_HPP_
#define _CEST_SSTREAM_HPP_

#include "istream.hpp"
#include "string.hpp"

namespace cest {

template <
  class _CharT,
  class _Traits = std::char_traits<_CharT>,
  class _Alloc = std::allocator<_CharT>
> class basic_istringstream : public basic_istream<_CharT, _Traits>
{
public:
  using char_type   = _CharT;
  using traits_type = _Traits;
  using int_type    = typename traits_type::int_type;
  using pos_type    = typename traits_type::pos_type;
  using off_type    = typename traits_type::off_type;
  using allocator_type = _Alloc;

  typedef basic_string<_CharT, _Traits, _Alloc>  __string_type;
  // typedef basic_stringbuf<_CharT, _Traits, _Alloc>  __stringbuf_type;
  typedef basic_istream<char_type, traits_type> __istream_type;

  // cppreference (3)
  explicit constexpr
  basic_istringstream(const __string_type& __str,
    ios_base::openmode __mode = ios_base::in)
  : __istream_type(), _M_string(__str)
  { /*this->init(&_M_stringbuf);*/ }

  // cppreference (6)
  /*explicit constexpr
  basic_istringstream(__string_type&& __str,
                      ios_base::openmode __mode = ios_base::in) {
  }*/

  constexpr ~basic_istringstream() {}

  //__stringbuf_type  _M_stringbuf;
  __string_type _M_string;
};

using  istringstream = basic_istringstream<char>;
using wistringstream = basic_istringstream<wchar_t>;

} // namespace cest

#endif // _CEST_SSTREAM_HPP_
