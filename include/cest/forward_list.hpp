#ifndef _CEST_FORWARD_LIST_HPP_
#define _CEST_FORWARD_LIST_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/forward_list_tests.hpp

#include <memory>

namespace cest {

template <
  class T,
  class Allocator = std::allocator<T>
>
class forward_list {
public:
  struct       iter;
  struct const_iter;

  using value_type      = T;
  using allocator_type  = Allocator;
  using size_type       = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference       =       value_type&;
  using const_reference = const value_type&;
  using pointer         = std::allocator_traits<Allocator>::pointer;
  using const_pointer   = std::allocator_traits<Allocator>::const_pointer;
  using iterator        =       iter;
  using const_iterator  = const_iter;

  struct node {
    value_type value;
    node      *next_node;
  };

  struct iter
  {
    using difference_type   = std::ptrdiff_t;
    using value_type        = forward_list::value_type;
    using reference         = value_type&;
    using pointer           = value_type*;
    using iterator_category = std::forward_iterator_tag;

    constexpr reference operator*()     { return curr_node->value;  }
    constexpr auto&     operator++()    {        // pre-increment
      curr_node = curr_node->next_node;
      return *this;
    }
    constexpr auto      operator++(int) {        // post-increment
      auto tmp(curr_node);
      ++(*this);
      return tmp; 
    }
    constexpr bool      operator==(const iter &other) {
      return this->curr_node == other.curr_node;
    }
    node *curr_node;
  };

  struct const_iter
  {
    using value_type        = forward_list::value_type;
    using difference_type   = std::ptrdiff_t;
    using reference         = const value_type&;
    using pointer           = const value_type*;
    using iterator_category = std::forward_iterator_tag;

    constexpr const_iter(      node  *n) : curr_node(n)            {}
    constexpr const_iter(const iter &it) : curr_node(it.curr_node) {}
    constexpr reference operator*()     { return curr_node->value;  }
    constexpr auto&     operator++()    {        // pre-increment
      curr_node = curr_node->next_node;
      return *this;
    }
    constexpr auto      operator++(int) {        // post-increment
      auto tmp(curr_node);
      ++(*this);
      return tmp; 
    }
    constexpr bool      operator==(const const_iter &other) {
      return this->curr_node == other.curr_node;
    }
    node *curr_node;
  };

  constexpr  forward_list()  = default;
  constexpr ~forward_list() {
    node *curr_node = m_front;
    while (curr_node) {
      node *next_node = curr_node->next_node;
      std::destroy_at(curr_node);
      m_node_alloc.deallocate(curr_node, 1);
      curr_node = next_node;
    };
  }

  constexpr allocator_type get_allocator() const   { return  m_alloc;        }
  constexpr       iterator  begin()       noexcept { return {m_front};       }
  constexpr const_iterator  begin() const noexcept { return {m_front};       }
  constexpr const_iterator cbegin() const noexcept { return {m_front};       }
  constexpr       iterator    end()       noexcept { return {nullptr};       }
  constexpr const_iterator    end() const noexcept { return {nullptr};       }
  constexpr const_iterator   cend() const noexcept { return {nullptr};       }

  [[nodiscard]] bool empty()        const noexcept { return begin() == end(); }

  constexpr void push_front(const T &value) {
    node *new_node = m_node_alloc.allocate(1);
    std::construct_at(new_node, value, m_front);
    m_front = new_node;
  }

  constexpr void push_front(T &&value)      {
    node *new_node = m_node_alloc.allocate(1);
    std::construct_at(new_node, std::move(value), m_front);
    m_front = new_node;
  }

  constexpr reference       front()         { return m_front->value; }
  constexpr const_reference front() const   { return m_front->value; }

  node *m_front = nullptr;
  allocator_type m_alloc;
  std::allocator_traits<allocator_type>::template rebind_alloc<node> m_node_alloc;
};

} // namespace cest

#endif // _CEST_FORWARD_LIST_HPP_
