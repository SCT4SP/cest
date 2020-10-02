#ifndef _CEST_CSTDIO_TESTS_HPP_
#define _CEST_CSTDIO_TESTS_HPP_

#include "cest/cstdio.hpp"
#include "cest/cstdlib.hpp" // cest::size_t
#include "cest/string.hpp"  // cest::string

constexpr bool cstdio_test1()
{
  cest::printf(__func__);
  cest::printf(": %d %s\n", 1789, "ok");
  cest::fprintf(cest::stderr, "An error stream test message.\n");

  return true;
}

constexpr bool cstdio_test2()
{
  const unsigned sz = 128;
  char buffer[sz];
  cest::string str = "Hello!";

  cest::FILE* in        = cest::fmemopen(str.data(), str.size(), "rb");
  cest::size_t num_read = cest::fread(buffer, sizeof buffer[0], sz, in);
  int ret               = cest::fclose(in);

  // cest::cout << buffer << " " << num_read << " " << ret << '\n';

  return 0==ret && 6==num_read;
  //return true;
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
  char buffer[sz];
  std::string str = "Hello!";

  std::FILE* in        = fmemopen(str.data(), str.size(), "rb");
  std::size_t num_read = std::fread(buffer, sizeof buffer[0], sz, in);
  int ret              = std::fclose(in);

  // std::cout << buffer << " " << num_read << " " << ret << '\n';
}

#endif // _CEST_CSTDIO_TESTS_HPP_
