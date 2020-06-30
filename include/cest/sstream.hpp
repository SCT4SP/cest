#ifndef _CEST_SSTREAM_HPP_
#define _CEST_SSTREAM_HPP_

namespace cest {

template <
  class _CharT,
  class _Traits = std::char_traits<_CharT>,
  class _Alloc = std::allocator<_CharT>
> class basic_istringstream // : public basic_iostream<_CharT, _Traits>
{
public:
  using char_type   = _CharT;
  using traits_type = _Traits;
  using int_type    = typename traits_type::int_type;
  using pos_type    = typename traits_type::pos_type;
  using off_type    = typename traits_type::off_type;
  using allocator_type = _Alloc;

  using __string_type = basic_string<_CharT, _Traits, _Alloc>;

  // cppreference (3)
  explicit constexpr
  basic_istringstream(const __string_type& str,
                      std::ios_base::openmode mode = std::ios_base::in) {
  }

  // cppreference (6)
  explicit constexpr
  basic_istringstream(__string_type&& str,
                      std::ios_base::openmode mode = std::ios_base::in) {
  }

  __string_type m_str;
};

using  istringstream = basic_istringstream<char>;
using wistringstream = basic_istringstream<wchar_t>;

} // namespace cest

#endif // _CEST_SSTREAM_HPP_
