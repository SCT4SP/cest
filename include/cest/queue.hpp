#ifndef _CEST_QUEUE_HPP_
#define _CEST_QUEUE_HPP_

#include <type_traits>

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/queue_tests.hpp

namespace cest {

template <
  class T,
  class Container = cest::list<T> // default to list for now
>
struct queue
{
  using container_type  = Container;
  using value_type      = typename Container::value_type;
  using size_type       = typename Container::size_type;
  using reference       = typename Container::reference;
  using const_reference = typename Container::const_reference;

//  template <
//    typename _Seq = Container,
//    typename _Requires = typename
//	       enable_if<std::is_default_constructible<_Seq>::value>::type>
//	queue() : m_c() { }

//  void push( const value_type& value );
//  void push( value_type&& value );
//  void pop();
//  [[nodiscard]] bool empty() const;
//  reference front();
//  const_reference front() const;

  container_type m_c;
};

} // namespace cest

#endif // _CEST_QUEUE_HPP_
