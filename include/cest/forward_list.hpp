#ifndef _CEST_FORWARD_LIST_HPP_
#define _CEST_FORWARD_LIST_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/forward_list_tests.hpp

#include <memory>

namespace cest {

// constexpr reinterpret_cast is prohibited in C++20 (Section 7.7 (5.15))
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=49171

template <typename T, typename U>
constexpr T const_cast2(U *p) {
  return p ? const_cast<T>(p) : nullptr;
}

template <typename T, typename U>
constexpr T const_cast2(U &&x) {
  return const_cast<T>(std::forward<decltype(x)>(x));
}

template <typename T, typename U>
constexpr T static_cast2(U *p) {
  return p ? static_cast<T>(p) : nullptr;
}

template <typename T, typename U>
constexpr T static_cast2(U &&x) {
  return static_cast<T>(std::forward<decltype(x)>(x));
}

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

  struct        node_base { node_base* next = nullptr; };
  struct node : node_base {
    constexpr node(const value_type &v, node_base* n = nullptr)
                                                  : node_base(n), value(v) {}
    value_type value;
  };

  struct iter
  {
    using difference_type   = std::ptrdiff_t;
    using value_type        = forward_list::value_type;
    using reference         = value_type&;
    using pointer           = value_type*;
    using iterator_category = std::forward_iterator_tag;

    constexpr reference operator*()           const noexcept {
      return  static_cast<node*>(m_node)->value;
    }

    constexpr pointer   operator->()          const noexcept {
      return &static_cast<node*>(m_node)->value;
    }

    constexpr auto&     operator++()                noexcept { // pre-incr
      m_node = m_node->next; return *this;
    }

    constexpr auto      operator++(int)             noexcept { // post-incr
      auto tmp(m_node); ++(*this); return tmp; 
    }

    constexpr bool      operator==(const iter& rhs) noexcept {
      return m_node == rhs.m_node;
    }

    node_base* m_node = nullptr;
  };

  struct const_iter
  {
    using value_type        = forward_list::value_type;
    using difference_type   = std::ptrdiff_t;
    using reference         = const value_type&;
    using pointer           = const value_type*;
    using iterator_category = std::forward_iterator_tag;

    constexpr const_iter(const node_base *n) : m_node(n)         {}
    constexpr const_iter(const iter&     it) : m_node(it.m_node) {}

    constexpr reference operator*()                 const noexcept {
      return  static_cast<const node*>(m_node)->value;
    }

    constexpr pointer   operator->()                const noexcept {
      return &static_cast<const node*>(m_node)->value;
    }

    constexpr auto&     operator++()                      noexcept {
      m_node = m_node->next; return *this;
    }

    constexpr auto      operator++(int)                   noexcept {
      auto tmp(m_node); ++(*this); return tmp; 
    }

    constexpr bool      operator==(const const_iter& rhs) noexcept {
      return m_node == rhs.m_node;
    }

    const node_base *m_node = nullptr;
  };

  constexpr forward_list() = default;
  constexpr forward_list(const forward_list& x)
  {
    const node_base* from = &x.m_front;
          node_base*   to = &this->m_front;
    while (from->next) {
      const node *nextf = static_cast<node*>(from->next);
      to->next = std::construct_at(m_node_alloc.allocate(1), nextf->value);
      from     = from->next;
        to     =   to->next;
    }
  }

  constexpr forward_list& operator=(const forward_list& x)
  {
    forward_list tmp(x);
    this->swap(tmp);
    return *this;
  }

  constexpr ~forward_list() { erase_after(before_begin(), end()); }

  constexpr void swap(forward_list& x) {
    std::swap(this->m_front.next, x.m_front.next);
  }

  constexpr allocator_type get_allocator() const noexcept {
    return m_node_alloc;
  }
  constexpr iterator        begin()       noexcept { return {m_front.next};   }
  constexpr const_iterator  begin() const noexcept { return {m_front.next};   }
  constexpr const_iterator cbegin() const noexcept { return {m_front.next};   }
  constexpr iterator          end()       noexcept { return {nullptr};        }
  constexpr const_iterator    end() const noexcept { return {nullptr};        }
  constexpr const_iterator   cend() const noexcept { return {nullptr};        }
  constexpr iterator before_begin()       noexcept { return {&m_front};       }

  [[nodiscard]]
  constexpr bool            empty() const noexcept { return begin() == end(); }

  constexpr
  iterator insert_after(const_iterator pos, const value_type& value) {
    node_base*   p = const_cast<node_base*>(pos.m_node);
    node* new_node = m_node_alloc.allocate(1);
    p->next        = std::construct_at(new_node,           value, p->next);
    return {new_node};
  }

  constexpr
  iterator insert_after(const_iterator pos,      value_type&& value) {
    node_base*   p  = const_cast<node_base*>(pos.m_node);
    node* new_node  = m_node_alloc.allocate(1);
    p->next         = std::construct_at(new_node, std::move(value), p->next);
    return {new_node};
  }

  constexpr void clear() noexcept { erase_after(before_begin(), end()); }

  constexpr iterator erase_after(const_iterator first, const_iterator last)
  {
    node_base* p = const_cast<node_base*>(first.m_node);
    node_base* l = const_cast2<node_base*>(last.m_node);

    node* curr = static_cast2<node*>(p->next);
    while (curr != l) {
      node* tmp  = curr;
      curr = static_cast2<node*>(curr->next);
      std::destroy_at(&tmp->value);
      m_node_alloc.deallocate(tmp, 1);
    }

    p->next = l;
    return {l};
  }

  constexpr iterator erase_after(const_iterator pos)
  {
    node_base* p = const_cast<node_base*>(pos.m_node);
    node*   curr = static_cast<node*>(p->next);

    p->next = curr->next;
    std::destroy_at(&curr->value);
    m_node_alloc.deallocate(curr, 1);

    return {p->next};
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
    return static_cast<node*>(m_front.next)->value;
  }

  node_base m_front;
  std::allocator_traits<allocator_type>::template rebind_alloc<node> m_node_alloc;
};

} // namespace cest

#endif // _CEST_FORWARD_LIST_HPP_
