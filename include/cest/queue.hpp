#ifndef _CEST_QUEUE_HPP_
#define _CEST_QUEUE_HPP_

#include "cest/deque.hpp"
#include <utility> // std::move

namespace cest {

template<
  class T,
  class Container = deque<T>
>
class queue {
public:

  using container_type  = Container;
  using value_type      = Container::value_type;
  using size_type       = Container::size_type;
  using reference       = Container::reference;
  using const_reference = Container::const_reference;

           constexpr queue()                           : m_cont()           { }
  explicit constexpr queue(const container_type& cont) : m_cont(cont)       { }
  explicit constexpr queue(container_type&& cont) : m_cont(std::move(cont)) { }

  [[nodiscard]] constexpr bool      empty() const { return m_cont.empty(); }
                constexpr size_type  size() const { return m_cont.size();  }

  constexpr reference       front()       { return m_cont.front(); }
  constexpr const_reference front() const { return m_cont.front(); }
  constexpr reference        back()       { return m_cont.back();  }
  constexpr const_reference  back() const { return m_cont.back();  }

  constexpr void push(const T& value) { m_cont.push_back(value);            }
  constexpr void push(T&& value)      { m_cont.push_back(std::move(value)); }
  constexpr void pop()                { m_cont.pop_front();                 }

private:
  container_type m_cont;
};

} // namespace cest

#endif // _CEST_QUEUE_HPP_
