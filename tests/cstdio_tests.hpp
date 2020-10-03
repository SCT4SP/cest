#ifndef _CEST_CSTDIO_TESTS_HPP_
#define _CEST_CSTDIO_TESTS_HPP_

#include "cest/cstdio.hpp"
#include "cest/cstdlib.hpp" // cest::size_t
#include "cest/string.hpp"  // cest::string

constexpr bool cstdio_test1()
{
  cest::printf(__func__);
  const char format[] = " (%d %s)";
  int r1 = cest::printf(format, 1789, "ok");
  int r2 = cest::fprintf(cest::stderr, format, 1789, "ok");
  cest::printf("\n");
  bool b = false;
  if (std::is_constant_evaluated())
    b = r1==0;
  else
    b = r1==10;

  return b && r1==r2;
}

constexpr bool cstdio_test2()
{
  const unsigned sz = 128;
  char buffer[sz];
  cest::string str = "Hello!";

  cest::FILE* in        = cest::fmemopen(str.data(), str.size(), "rb");
  cest::size_t num_read = cest::fread(buffer, sizeof buffer[0], sz, in);
  int ret               = cest::fclose(in);

  return 0==ret && 6==num_read && str[0]==buffer[0];
}

void cstdio_tests()
{
#if CONSTEXPR_CEST == 1
  static_assert(cstdio_test1());
  static_assert(cstdio_test2());
#endif

  assert(cstdio_test1());
  assert(cstdio_test2());

  const unsigned sz = 128;
  static_assert(sizeof(short)==(2*sizeof(char)));

  char buffer[sz];
  std::string str = "Hello!";
  std::FILE* in        = fmemopen(str.data(), str.size(), "rb");
  std::size_t num_read = std::fread(buffer, sizeof buffer[0], sz, in);
  int ret              = std::fclose(in);
  assert(0==ret && 6==num_read && str[0]==buffer[0]);
}

#endif // _CEST_CSTDIO_TESTS_HPP_
