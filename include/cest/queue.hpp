#ifndef _CEST_QUEUE_HPP_
#define _CEST_QUEUE_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/queue_tests.hpp

namespace cest {

template <
  class T,
  class Container = cest::list<T> // default to list for now
>
struct queue
{
  using value_type      = typename Container::value_type;
  using reference       = typename Container::reference;
  using const_reference = typename Container::const_reference;
  void push( const value_type& value );
  void push( value_type&& value );
  void pop();
  [[nodiscard]] bool empty() const;
  reference front();
  const_reference front() const;
};

} // namespace cest

#endif // _CEST_QUEUE_HPP_
