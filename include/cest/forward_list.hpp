#ifndef _CEST_FORWARD_LIST_HPP_
#define _CEST_FORWARD_LIST_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/forward_list_tests.hpp

#include <memory>

namespace cest {

template <
  class T,
  class Allocator = std::allocator<T>
>
struct forward_list {
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

  struct        node_base { node_base *next = nullptr; };
  struct node : node_base { value_type value;          };

  struct iter
  {
    using difference_type   = std::ptrdiff_t;
    using value_type        = forward_list::value_type;
    using reference         = value_type&;
    using pointer           = value_type*;
    using iterator_category = std::forward_iterator_tag;

    constexpr reference operator*() const {
      return static_cast<node*>(m_node)->value;
    }

    constexpr auto&     operator++()    {        // pre-increment
      m_node = m_node->next; return *this;
    }

    constexpr auto      operator++(int) {        // post-increment
      auto tmp(m_node); ++(*this); return tmp; 
    }

    constexpr bool      operator==(const iter &other) {
      return m_node == other.m_node;
    }

    node_base *m_node;
  };

  struct const_iter
  {
    using value_type        = forward_list::value_type;
    using difference_type   = std::ptrdiff_t;
    using reference         = const value_type&;
    using pointer           = const value_type*;
    using iterator_category = std::forward_iterator_tag;

    constexpr const_iter(node_base  *n)  : m_node(n)         {}

    constexpr const_iter(const iter &it) : m_node(it.m_node) {}

    constexpr reference operator*() const {
      return static_cast<node*>(m_node)->value;
    }

    constexpr auto&     operator++()    {        // pre-increment
      m_node = m_node->next; return *this;
    }

    constexpr auto      operator++(int) {        // post-increment
      auto tmp(m_node); ++(*this); return tmp; 
    }

    constexpr bool      operator==(const const_iter &other) {
      return m_node == other.m_node;
    }

    node_base *m_node;
  };

  constexpr forward_list() { m_front.next = nullptr; }
  constexpr forward_list(const forward_list& x) : m_front()
  {
    const node_base* from = &x.m_front;
          node_base*   to = &this->m_front;
    while (from->next) {
      const node *nextf = static_cast<node*>(from->next);
      to->next = new node{nullptr,*nextf};
      from = from->next;
        to =   to->next;
    }
  }

  constexpr forward_list& operator=(const forward_list& x) {
    if (&x != this) { // test this
      forward_list tmp(x);
      this->swap(tmp);
    }
    return *this;
  }

  constexpr ~forward_list() {
    node_base *p = m_front.next;
    while (p) {
      node *tmp = static_cast<node*>(p);
      p = p->next;
      m_node_alloc.deallocate(tmp, 1);
    }
  }

  constexpr void swap(forward_list& x) {
    std::swap(this->m_front.next, x.m_front.next);
  }

  constexpr allocator_type get_allocator() const noexcept { return m_alloc;   }
  constexpr iterator       begin()        noexcept { return {m_front.next};   }
  constexpr const_iterator begin()  const noexcept { return {m_front.next};   }
  constexpr const_iterator cbegin() const noexcept { return {m_front.next};   }
  constexpr iterator       end()          noexcept { return {nullptr};        }
  constexpr const_iterator end()    const noexcept { return {nullptr};        }
  constexpr const_iterator cend()   const noexcept { return {nullptr};        }
  constexpr iterator       before_begin() noexcept { return {&m_front};       }

  [[nodiscard]] bool empty()        const noexcept { return begin() == end(); }

  constexpr iterator insert_after(iterator it, const T& value) {
    node *new_node  = m_node_alloc.allocate(1);
    new_node->next  = it.m_node->next;
    new_node->value = value;
    it.m_node->next = new_node;
    return {new_node};
  }

  constexpr iterator insert_after(iterator it, T&& value) {
    node *new_node  = m_node_alloc.allocate(1);
    new_node->next  = it.m_node->next;
    new_node->value = std::move(value);
    it.m_node->next = new_node;
    return {new_node};
  }

  constexpr iterator erase_after(iterator it) {
    node* p = static_cast<node*>(it.node->next);
    it.node->next = p->next;
    delete *p;
    return {it.node->next};
  }

  constexpr void push_front(const value_type &value) {
    insert_after(before_begin(), value);
  }

  constexpr void push_front(value_type  &&value)      {
    insert_after(before_begin(), std::move(value));
  }

  constexpr reference       front()         {
    return static_cast<node*>(m_front.next)->value;
  }

  constexpr const_reference front() const   {
    return static_cast<node *>(m_front.next)->value;
  }

  node_base m_front;
  allocator_type m_alloc;
  std::allocator_traits<allocator_type>::template rebind_alloc<node> m_node_alloc;
};

} // namespace cest

#endif // _CEST_FORWARD_LIST_HPP_
