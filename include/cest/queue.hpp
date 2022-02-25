#ifndef _CEST_QUEUE_HPP_
#define _CEST_QUEUE_HPP_

#include "cest/deque.hpp"
#include <utility> // std::move

namespace cest {

template <class T, class Container = deque<T>> class queue {
public:
  using container_type = Container;
  using value_type = typename Container::value_type;
  using size_type = typename Container::size_type;
  using reference = typename Container::reference;
  using const_reference = typename Container::const_reference;

  constexpr queue() : c() {}
  explicit constexpr queue(const container_type &cont) : c(cont) {}
  explicit constexpr queue(container_type &&cont) : c(std::move(cont)) {}

  [[nodiscard]] constexpr bool empty() const { return c.empty(); }
  constexpr size_type size() const { return c.size(); }

  constexpr reference front() { return c.front(); }
  constexpr const_reference front() const { return c.front(); }
  constexpr reference back() { return c.back(); }
  constexpr const_reference back() const { return c.back(); }

  constexpr void push(const T &value) { c.push_back(value); }
  constexpr void push(T &&value) { c.push_back(std::move(value)); }
  constexpr void pop() { c.pop_front(); }

protected:
  container_type c;
};

} // namespace cest

#endif // _CEST_QUEUE_HPP_
