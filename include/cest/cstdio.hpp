#ifndef _CEST_CSTDIO_HPP_
#define _CEST_CSTDIO_HPP_

#include <cstdio>

namespace cest {

using FILE = std::FILE; // In libstdc++ FILE is a typedef of _IO_FILE struct

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

constexpr FILE* fmemopen(void* buf, size_t size, const char* mode)
{
  if (!std::is_constant_evaluated())
  {
    return ::fmemopen(buf, size, mode);
  }

  return stdout;
}

constexpr std::size_t fread(void* buffer, std::size_t size,
                            std::size_t count, FILE* stream)
{
  if (!std::is_constant_evaluated())
  {
    return std::fread(buffer, size, count, stream);
  }

  return 0;
}

constexpr int fclose(FILE* stream)
{
  if (!std::is_constant_evaluated())
  {
    return std::fclose(stream);
  }

  delete stream;

  return 0;
}

} // namespace cest

#endif // _CEST_CSTDIO_HPP_
