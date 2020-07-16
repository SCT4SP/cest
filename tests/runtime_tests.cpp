#include "vector_tests.hpp"
#include "forward_list_tests.hpp"
#include "list_tests.hpp"
#include "set_tests.hpp"
#include "map_tests.hpp"
#include "string_tests.hpp"
#include "cctype_tests.hpp"
#include "deque_tests.hpp"
//#include "iterator_tests.hpp"
/*#include "queue_tests.hpp"
#include "algorithm_tests.hpp"
#include "iostream_tests.hpp"
#include "allocator_tests.hpp"
#include "stringstream_tests.hpp"
*/

int main(int argc, char *argv[])
{
  vector_tests();
  forward_list_tests();
  list_tests();
  set_tests();
  map_tests();
  string_tests();
  cctype_tests();
  deque_tests();
  //iterator_tests();
  /*queue_tests();
  algorithm_tests();
  iostream_tests();
  allocator_tests();
  stringstream_tests();
*/
  return 0;
}
