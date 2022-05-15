#ifndef CONSTEXPR_CEST
#define CONSTEXPR_CEST 1 // default: yes, test cest's constexpr functionality
#endif

// Copyright (c) 2020-2022 Paul Keir, University of the West of Scotland.

#include "algorithm_tests.hpp"
#include "allocator_tests.hpp"
#include "array_tests.hpp"
//#include "cctype_tests.hpp"
//#include "cmath_tests.hpp"
//#include "complex_tests.hpp"
//#include "cstdio_tests.hpp"
//#include "cstdlib_tests.hpp"
#include "deque_tests.hpp"
//#include "exception_tests.hpp"
#include "forward_list_tests.hpp"
//#include "functional_tests.hpp"
#include "iostream_tests.hpp"
//#include "iterator_tests.hpp"
#include "list_tests.hpp"
#include "map_tests.hpp"
//#include "memory_tests.hpp"
#include "numeric_tests.hpp"
#include "queue_tests.hpp"
#include "set_tests.hpp"
#include "string_tests.hpp"
//#include "stringstream_tests.hpp"
#include "utility_tests.hpp"
#include "vector_tests.hpp"

int main(int argc, char *argv[]) {
  vector_tests();
  forward_list_tests();
  list_tests();
  set_tests();
  map_tests();
  string_tests();
//  cctype_tests();
  deque_tests();
  queue_tests();
//  iterator_tests();
  algorithm_tests();
  numeric_tests();
  iostream_tests();
  allocator_tests();
//  stringstream_tests();
  array_tests();
//  cstdio_tests();
//  cstdlib_tests();
//  memory_tests();
  utility_tests();
//  cmath_tests();
//  complex_tests();
//  functional_tests();
//  exception_tests();

  return 0;
}

/*
Expected runtime output from the program above:

Zod
Zod
Hello World!
Fire and Brimstone!
42 42
42 42
Hello World!
Fire and Brimstone!
42 42
42 42
cstdio_test1 (1789 ok) (1789 ok)
*/
