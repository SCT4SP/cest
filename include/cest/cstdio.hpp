#ifndef _CEST_CSTDIO_HPP_
#define _CEST_CSTDIO_HPP_

#include <cstdio>
#include "cest/iostream.hpp"
#include "cest/string.hpp"

namespace cest {

// Here, we have istream, not iostream, as iostream misleadingly only inherits
// from istream due to the prohibition of constexpr virtual inheritance.
// (In libstdc++ FILE is a typedef of _IO_FILE struct.)
using FILE = istream;

constexpr FILE* stdin = &cest::cin;
constexpr ostream* stdout = &cest::cout;
constexpr ostream* stderr = &cest::cerr;

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

constexpr int fprintf(ostream* stream, const char* format)
{
  if (!std::is_constant_evaluated())
  {
    (*stream) << format;
  }

  return 0;
}

// FILE* for param 1 would itroduce the issues described above. 
template <typename... Ts>
constexpr int fprintf(ostream* stream, const char* format, Ts&&... xs)
{
  if (!std::is_constant_evaluated())
  {
    int nbytes = std::snprintf(nullptr, 0, format, std::forward<Ts>(xs)...);
    char *psz = new char[nbytes+1];
    std::snprintf(psz, nbytes+1, format, std::forward<Ts>(xs)...);
    (*stream) << psz;
    delete [] psz;
    return nbytes;
  }

  return 0;
}

constexpr FILE* fmemopen(char* buf, size_t size, const char* mode)
{
  return new stringstream(string(buf,size));
}

// Param #1 of fmemopen and fread is char* (instead of void*) because:
// 1) A void* can't be arg #1 of std::basic_istream<CharT,Traits>::read
// 2) A T* template parameter alternative, would imply types larger than char
//    can be used, but with constexpr, they can't.
// 3) Also note C-style "FILE" type returned: istream (i.e. basic_istream<char>)
constexpr std::size_t fread(char* buffer, std::size_t size,
                            std::size_t count, FILE* stream)
{
  assert(size==sizeof(char));
  stream->read(buffer, size*count);
  return stream->gcount() / size;
}

constexpr int fclose(FILE* stream)
{
  delete stream;
  return 0;
}

} // namespace cest

#endif // _CEST_CSTDIO_HPP_
