#ifndef _CEST_CSTDIO_HPP_
#define _CEST_CSTDIO_HPP_

#include <cstdio>
#include "cest/iostream.hpp"
#include "cest/string.hpp"

namespace cest {

using FILE = istream; // In libstdc++ FILE is a typedef of _IO_FILE struct

constexpr FILE *stdin  = nullptr;
constexpr FILE *stdout = nullptr;
constexpr FILE *stderr = nullptr;

// avoid warning: format string is not a string literal (-Wformat-security)
constexpr int printf(const char* format)
{
  if (!std::is_constant_evaluated())
  {
    return std::printf("%s", format);
  }

  return 0;
}

template <typename... Ts>
constexpr int printf(const char* format, Ts&&... xs)
{
  if (!std::is_constant_evaluated())
  {
    return std::printf(format, std::forward<Ts>(xs)...);
  }

  return 0; // positive; not an error; 0 characters output (constexpr)
}

// avoid warning: format string is not a string literal (-Wformat-security)
constexpr int fprintf(FILE* stream, const char* format)
{
  if (!std::is_constant_evaluated())
  {
    return std::printf("%s", format);
  }

  return 0;
}

template <typename... Ts>
constexpr int fprintf(FILE* stream, const char* format, Ts&&... xs)
{
  if (!std::is_constant_evaluated())
  {
    return std::fprintf(stream, format, std::forward<Ts>(xs)...);
  }

  return 0;
}

constexpr FILE* fmemopen(char* buf, size_t size, const char* mode)
{
  /*if (!std::is_constant_evaluated())
  {
    return ::fmemopen(buf, size, mode);
  }*/

//  string str("ok");
  istringstream* piss = new istringstream(string(buf,size));
//  istringstream* piss = new istringstream(str);
  return piss;
}

// buffer is char* because:
// 1) A void* can't be arg #1 of std::basic_istream<CharT,Traits>::read
// 2) A T* template parameter implies types larger than char can be used, but
//    with constexpr, they can't.
constexpr std::size_t fread(char* buffer, std::size_t size,
                            std::size_t count, FILE* stream)
{
  /*if (!std::is_constant_evaluated())
  {
    return std::fread(buffer, size, count, stream);
  }*/
  assert(size==sizeof(char));
  stream->read(buffer, size*count);
  return stream->gcount() / size;
}

constexpr int fclose(FILE* stream)
{
  /*if (!std::is_constant_evaluated())
  {
    return std::fclose(stream);
  }*/

  delete stream;

  return 0;
}

} // namespace cest

#endif // _CEST_CSTDIO_HPP_
