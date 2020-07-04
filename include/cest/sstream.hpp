#ifndef _CEST_SSTREAM_HPP_
#define _CEST_SSTREAM_HPP_

#include "istream.hpp"
#include "string.hpp"

namespace cest {

// for streambuf.hpp
template<
  class _CharT,
  class _Traits = std::char_traits<_CharT>
> class basic_streambuf {
public:
  typedef _CharT   char_type;
  typedef _Traits  traits_type;
  typedef typename traits_type::int_type    int_type;
  typedef typename traits_type::pos_type    pos_type;
  typedef typename traits_type::off_type    off_type;

protected:
  char_type*    _M_in_beg;     ///< Start of get area.
  char_type*    _M_in_cur;     ///< Current read area.
  char_type*    _M_in_end;     ///< End of get area.
  char_type*    _M_out_beg;    ///< Start of put area.
  char_type*    _M_out_cur;    ///< Current put area.
  char_type*    _M_out_end;    ///< End of put area.
  //  locale      _M_buf_locale;

public:
  constexpr // virtual
  ~basic_streambuf()
  {}

protected:
  constexpr basic_streambuf()
  : _M_in_beg(0), _M_in_cur(0), _M_in_end(0),
  _M_out_beg(0), _M_out_cur(0), _M_out_end(0)//,
//  _M_buf_locale(locale())
  { }

  constexpr basic_streambuf(const basic_streambuf& rhs) {}

  constexpr void                                                       
  setg(char_type* __gbeg, char_type* __gnext, char_type* __gend)
  {                                                        
    _M_in_beg = __gbeg;                                                    
    _M_in_cur = __gnext;
    _M_in_end = __gend;                                          
  }

  constexpr void pbump(int __n) { _M_out_cur += __n; }

  constexpr void setp(char_type* __pbeg, char_type* __pend)
  {
    _M_out_beg = _M_out_cur = __pbeg;
    _M_out_end = __pend;
  }
};

template<
  class _CharT,
  class _Traits = std::char_traits<_CharT>,
  class _Alloc = std::allocator<_CharT>
> class basic_stringbuf : public basic_streambuf<_CharT, _Traits> {
public:

  typedef _CharT  char_type;
  typedef _Traits traits_type;
  typedef _Alloc  allocator_type;

  typedef typename traits_type::int_type    int_type;
  typedef typename traits_type::pos_type    pos_type;
  typedef typename traits_type::off_type    off_type;

  typedef basic_streambuf<char_type, traits_type>   __streambuf_type;
  typedef basic_string<char_type, _Traits, _Alloc>  __string_type;
  typedef typename __string_type::size_type   __size_type;

protected:
  ios_base::openmode _M_mode;
  __string_type      _M_string;

public:

  constexpr basic_stringbuf()
  : __streambuf_type(), _M_mode(ios_base::in | ios_base::out), _M_string()
  { }

  constexpr explicit
  basic_stringbuf(ios_base::openmode __mode)
  : __streambuf_type(), _M_mode(__mode), _M_string()
  { }

  explicit constexpr
  basic_stringbuf(const __string_type& __str,
    ios_base::openmode __mode = ios_base::in | ios_base::out)
  : __streambuf_type(), _M_mode(),
    _M_string(__str.data(), __str.size(), __str.get_allocator())
  { _M_stringbuf_init(__mode); }

protected:
  // Common initialization code goes here.
  constexpr void _M_stringbuf_init(ios_base::openmode __mode)
  {
    _M_mode = __mode;
    __size_type __len = 0;
    if (_M_mode & (ios_base::ate | ios_base::app))
      __len = _M_string.size();
    _M_sync(const_cast<char_type*>(_M_string.data()), 0, __len);
  }

  constexpr void _M_sync(char_type* __base, __size_type __i, __size_type __o);
  constexpr void _M_pbump(char_type* __pbeg, char_type* __pend, off_type __off);
};

  template <class _CharT, class _Traits, class _Alloc>
    constexpr void
    basic_stringbuf<_CharT, _Traits, _Alloc>::
    _M_sync(char_type* __base, __size_type __i, __size_type __o)
    {
      const bool __testin = _M_mode & ios_base::in;
      const bool __testout = _M_mode & ios_base::out;
      char_type* __endg = __base + _M_string.size();
      char_type* __endp = __base + _M_string.capacity();

      if (__base != _M_string.data())
  {
    // setbuf: __i == size of buffer area (_M_string.size() == 0).
    __endg += __i;
    __i = 0;
    __endp = __endg;
  }

      if (__testin)
  this->setg(__base, __base + __i, __endg);
      if (__testout)
  {
    _M_pbump(__base, __endp, __o);
    // egptr() always tracks the string end.  When !__testin,
    // for the correct functioning of the streambuf inlines
    // the other get area pointers are identical.
    if (!__testin)
      this->setg(__endg, __endg, __endg);
  }
    }

  template <class _CharT, class _Traits, class _Alloc>
    constexpr void
    basic_stringbuf<_CharT, _Traits, _Alloc>::
    _M_pbump(char_type* __pbeg, char_type* __pend, off_type __off)
    {
      this->setp(__pbeg, __pend);
      while (__off > __gnu_cxx::__numeric_traits<int>::__max)
  {
    this->pbump(__gnu_cxx::__numeric_traits<int>::__max);
    __off -= __gnu_cxx::__numeric_traits<int>::__max;
  }
      this->pbump(__off);
    }

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
  typedef basic_stringbuf<_CharT, _Traits, _Alloc>  __stringbuf_type;
  typedef basic_istream<char_type, traits_type> __istream_type;

  // cppreference (3)
//  explicit constexpr
//  basic_istringstream(const __string_type& __str,
//    ios_base::openmode __mode = ios_base::in)
//  : __istream_type() { }

  explicit constexpr
  basic_istringstream(const __string_type& __str,
    ios_base::openmode __mode = ios_base::in)
  : __istream_type(), _M_stringbuf(__str, __mode | ios_base::in)
  { /*this->init(&_M_stringbuf);*/ }

  // cppreference (6)
  // doesn't exist in libstdc++?
  /*explicit constexpr
  basic_istringstream(__string_type&& __str,
    ios_base::openmode __mode = ios_base::in)
  : __istream_type() { }*/

  constexpr ~basic_istringstream() {}

  __stringbuf_type  _M_stringbuf;
  //  __string_type _M_string;
};

using  istringstream = basic_istringstream<char>;
using wistringstream = basic_istringstream<wchar_t>;

} // namespace cest

#endif // _CEST_SSTREAM_HPP_
