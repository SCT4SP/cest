#ifndef _CEST_STREAMBUF_HPP_
#define _CEST_STREAMBUF_HPP_

namespace cest {

#define _IsUnused __attribute__ ((__unused__))

template<
  class _CharT,
  class _Traits = std::char_traits<_CharT>
> class basic_streambuf
{
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

  constexpr int
  pubsync() { return this->sync(); }

  constexpr int_type
  snextc()
  {
    int_type __ret = traits_type::eof();
    if (__builtin_expect(!traits_type::eq_int_type(this->sbumpc(),
                   __ret), true))
      __ret = this->sgetc();
    return __ret;
  }

  constexpr int_type
  sbumpc()
  {
    int_type __ret;
    if (__builtin_expect(this->gptr() < this->egptr(), true))
    {                                                 
      __ret = traits_type::to_int_type(*this->gptr());
      this->gbump(1);
    }
    else
      __ret = this->uflow();
    return __ret;
  }

  constexpr int_type
  sgetc()
  {
    int_type __ret;
    if (__builtin_expect(this->gptr() < this->egptr(), true))
      __ret = traits_type::to_int_type(*this->gptr());
    else
      __ret = this->underflow();
    return __ret;
  }

  constexpr int_type
  sungetc()
  {
    int_type __ret;
    if (__builtin_expect(this->eback() < this->gptr(), true))
    {
      this->gbump(-1);
      __ret = traits_type::to_int_type(*this->gptr());
    }
    else
      __ret = this->pbackfail();
    return __ret;
  }

protected:
  constexpr basic_streambuf()
  : _M_in_beg(0), _M_in_cur(0), _M_in_end(0),
  _M_out_beg(0), _M_out_cur(0), _M_out_end(0)//,
//  _M_buf_locale(locale())
  { }

  constexpr basic_streambuf(const basic_streambuf& rhs) {}

  constexpr char_type*
  eback() const { return _M_in_beg; }

  constexpr char_type*
  gptr()  const { return _M_in_cur;  }

  constexpr char_type*
  egptr() const { return _M_in_end; }

  constexpr void
  gbump(int __n) { _M_in_cur += __n; }

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

  virtual constexpr int
  sync() { return 0; }

  virtual constexpr int_type
  underflow()
  { return traits_type::eof(); }

  virtual constexpr int_type
  uflow()
  {
    int_type __ret = traits_type::eof();
    const bool __testeof = traits_type::eq_int_type(this->underflow(),
              __ret);
    if (!__testeof)
    {
      __ret = traits_type::to_int_type(*this->gptr());
      this->gbump(1);
    }
    return __ret;
  }

  virtual constexpr int_type
  pbackfail(int_type __c _IsUnused  = traits_type::eof())
  { return traits_type::eof(); }

};

} // namespace cest

#endif // _CEST_STREAMBUF_HPP_
