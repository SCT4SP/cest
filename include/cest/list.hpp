#ifndef _CEST_LIST_HPP_
#define _CEST_LIST_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/list_tests.hpp

#include <memory>
#include <algorithm>

namespace cest {

template <
  typename T,
  typename Allocator = std::allocator<T>
 >
struct list {
  struct       iter;
  struct const_iter;
  
  using value_type            = T;
  using allocator_type        = Allocator;
  using size_type             = std::size_t;
  using difference_type       = std::ptrdiff_t;
  using reference             =       value_type&;
  using const_reference       = const value_type&;
  using pointer               = std::allocator_traits<Allocator>::pointer;
  using const_pointer         = std::allocator_traits<Allocator>::const_pointer;
  using iterator              =       iter;
  using const_iterator        = const_iter;
  using reverse_iterator      = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  struct        node_base {
    constexpr void m_hook(node_base* const pos)   noexcept
    {
      this->next      = pos;
      this->prev      = pos->prev;
      pos->prev->next = this;
      pos->prev       = this;
    }

    constexpr void m_unhook()                     noexcept
    {
      node_base* const next_node = this->next;
      node_base* const prev_node = this->prev;
      prev_node->next            = next_node;
      next_node->prev            = prev_node;
    }

    node_base* next = this;
    node_base* prev = this;
  };

  struct node : node_base {
    constexpr node(const value_type &v) : value(v) {}
    constexpr node(const value_type &v, node_base* n, node_base* p)
                                               : node_base(n,p), value(v) {}
    value_type value;
  };

  struct iter
  {
    using difference_type   = std::ptrdiff_t;
    using value_type        = list::value_type;
    using reference         = value_type&;
    using pointer           = value_type*;
    using iterator_category = std::bidirectional_iterator_tag;

    constexpr reference operator*() const noexcept {
      return static_cast<node*>(m_node)->value;
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

    constexpr auto&     operator--()                noexcept {
      m_node = m_node->prev; return *this;
    }

    constexpr auto      operator--(int)             noexcept {
      auto tmp(m_node); --(*this); return tmp; 
    }

    constexpr bool      operator==(const iter& rhs) noexcept {
      return m_node == rhs.m_node;
    }
    
    node_base* m_node = nullptr;
  };

  struct const_iter
  {
    using difference_type   = std::ptrdiff_t;
    using value_type        = list::value_type;
    using reference         = const value_type&;
    using pointer           = const value_type*;
    using iterator_category = std::bidirectional_iterator_tag;

    constexpr const_iter(const node_base *n) : m_node(n)         {}
    constexpr const_iter(const iter&     it) : m_node(it.m_node) {}
    
    constexpr reference operator*()                 const noexcept {
      return static_cast<const node*>(m_node)->value;
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

    constexpr auto&     operator--()                      noexcept {
      m_node = m_node->prev; return *this;
    }

    constexpr auto      operator--(int)                   noexcept {
      auto tmp(m_node); --(*this); return tmp; 
    }

    constexpr bool      operator==(const const_iter& rhs) noexcept {
      return m_node == rhs.m_node;
    }
    
    const node_base* m_node = nullptr;
  };
  
  constexpr  list() = default;
  constexpr ~list() { clear(); }

  constexpr allocator_type get_allocator() const noexcept {
    return m_node_alloc;
  }
  
  constexpr       reference front()                { return *begin();        }
  constexpr const_reference front() const          { return *begin();        }

  constexpr       reference back()                 {
	        iterator tmp = end(); --tmp; return *tmp;
  }
  constexpr const_reference back()  const          {
	  const_iterator tmp = end(); --tmp; return *tmp;
  }
  
  constexpr iterator        begin()       noexcept { return {m_node.next};   }
  constexpr const_iterator  begin() const noexcept { return {m_node.next};   }
  constexpr const_iterator cbegin() const noexcept { return {m_node.next};   }

  constexpr       iterator    end()       noexcept { return {&m_node};       }
  constexpr const_iterator    end() const noexcept { return {&m_node};       }
  constexpr const_iterator   cend() const noexcept { return {&m_node};       }
  
  [[nodiscard]]
  constexpr bool            empty() const noexcept { return m_size == 0;     }
  
  constexpr size_type        size() const noexcept { return m_size;          }
  
  constexpr size_type    max_size() const noexcept {
    return std::allocator_traits<decltype(m_node_alloc)>::
             max_size(m_node_alloc);
  }
  
  constexpr void clear() noexcept
  {
    node_base* curr = m_node.next;
    while (curr != &m_node) {
      node* tmp = static_cast<node*>(curr);
      curr = curr->next;
      std::destroy_at(&tmp->value);
      m_node_alloc.deallocate(tmp, 1);
    }
    
    m_node.prev = m_node.next = &m_node;
    m_size = 0;
  }

  constexpr void push_back(const value_type &value)  { insert(end(), value);   }
  constexpr void push_back(value_type&& value)       {
    insert(end(), std::move(value));
  }
  constexpr void push_front(const value_type& value) { insert(begin(), value); }
  
  constexpr void push_front(value_type&& value)      {
    insert(begin(), std::move(value));
  }

  constexpr iterator insert(const_iterator pos, const T& value) {
    node_base*   p = const_cast<node_base*>(pos.m_node);
    node* new_node = m_node_alloc.allocate(1);
//    std::construct_at(new_node, value);//, new_node, p->prev);
//    new_node->m_hook(p);
    p->prev = p->prev->next = std::construct_at(new_node, value, p, p->prev);

    // List_node_base::_M_hook
    // __tmp->_M_hook(__position._M_const_cast()._M_node);
//    new_node->next = p;
//    new_node->prev = p->prev;
//    p->prev->next = new_node;
//    p->prev       = new_node;

    m_size++;
    return {new_node};
  }
  
  constexpr iterator insert(const_iterator pos, value_type&& value) {
    node_base*   p = const_cast<node_base*>(pos.m_node);
    node* new_node = m_node_alloc.allocate(1);
    //p->prev = std::construct_at(new_node, std::move(value), new_node, p->prev);
    //std::construct_at(new_node, std::move(value));//, new_node, p->prev);
    p->prev = p->prev->next =
      std::construct_at(new_node, std::move(value), p, p->prev);

    // List_node_base::_M_hook
    /*new_node->next = p;
    new_node->prev = p->prev;
    p->prev->next = new_node;
    p->prev       = new_node;
*/

    m_size++;
    return {new_node};
  }

  constexpr iterator erase(const_iterator pos)
  {
    m_size--;

    iterator ret{pos.m_node->next};
    node_base* const p = const_cast<node_base*>(pos.m_node);

    /*node_base* const next_node = p->next;
    node_base* const prev_node = p->prev;
    prev_node->next = next_node;
    next_node->prev = prev_node;
*/

    p->m_unhook();

    node* tmp = static_cast<node*>(p);
    std::destroy_at(&tmp->value);
    m_node_alloc.deallocate(tmp, 1);

    return ret;
  }

#if 0
  constexpr iterator insert(const_iterator pos, size_type count, 
                            const T& value) { 
  }
  
  template <class InputIt>
  constexpr iterator insert(const_iterator pos, InputIt first, InputIt last) {
  }
  
  constexpr iterator insert(const_iterator pos, 
                            std::initializer_list<T> ilist) {
  }

  // TODO: Deal with the edge case where the emplaced element is the first 
  // element in the list, or is it fine to assume that you cannot pass an empty
  // iterator..?
  template <class... Args>
  constexpr iterator emplace(const_iterator pos, Args&&... args) {
    node *new_node = m_node_alloc.allocate(1);
    node* prev = pos.curr_node->prev_node;
    node* next = pos.curr_node;
    std::construct_at(new_node, value_type(std::forward<Args>(args)...), next, 
                      prev);
    next->prev_node = new_node;
    
    if (next == m_node)
      m_node = new_node;
    else 
      prev->next_node = new_node;

    return iter(new_node);
  }

  // TODO/NOTE: There will likely be a lot of places where I need to check if
  // something is a nullptr... there's a lot of accessing of members without 
  // checking things, but perhaps its fine to assume a user is doing the correct
  // thing
  // It might be feasible to simplify this with some calls to pop_back/pop_front
  // if we know the element we're removing is in the back or front of the list
  // but that may just make the functon longer
  constexpr iterator erase(const_iterator pos) {
    node* tmp = pos.curr_node;
    node* next = tmp->next_node;
    node* prev = tmp->prev_node;
    node* ret = nullptr;
    
    if (next && prev) {
      next->prev_node = prev;
      prev->next_node = next;
      ret = next;
    } else if (!next && prev) { // end of list
      prev->next_node = nullptr;
      m_back = prev;
      ret = prev;
    } else if (next && !prev) { // front of list
      next->prev_node = nullptr;
      m_node = next;
      ret = next;
    } else { // empty list
      m_back = m_node = nullptr;
    }

    std::destroy_at(tmp);
    m_node_alloc.deallocate(tmp,1);
    return iter(ret);
  }

  // TODO
  constexpr iterator erase(const_iterator first, const_iterator last) {}
  
  constexpr void push_back(const T &value) {
    node *new_node = m_node_alloc.allocate(1);
    std::construct_at(new_node, value, nullptr, m_back);
    
    if (m_back) {
      m_back->next_node = new_node;
      m_back = new_node;
    } else
      m_node = m_back = new_node;
  }
  
  // The difference between push_back and emplace new seems to be that one 
  // forwards and the other does a move beforehand... you can see an example of 
  // this in the cppreference for lists emplace_back
  constexpr void push_back(T &&value)      {
    node *new_node = m_node_alloc.allocate(1);
    std::construct_at(new_node, std::move(value), nullptr, m_back);
    
    // TODO: Can this and its other equivalents be encapsulated in someway?
    if (m_back) {
      m_back->next_node = new_node;
      m_back = new_node;
    } else
      m_node = m_back = new_node;
  }
  
  template<class... Args>
  constexpr reference emplace_back(Args&&... args) {
    node *new_node = m_node_alloc.allocate(1);
    std::construct_at(new_node, value_type(std::forward<Args>(args)...), 
                      nullptr, m_back);

    if (m_back) {
      m_back->next_node = new_node;
      m_back = new_node;
    } else
      m_node = m_back = new_node;
  }

  // TODO: Handle case where we pop off the last or second last element..  
  constexpr void pop_back() {
    node* tmp = m_back;
    
    if (m_back->prev_node) {
      m_back = m_back->prev_node;
      m_back->next_node = nullptr;
    } else { 
      // assumption that this is the last element in the list or somethings 
      // went wrong and we've lost a node
      m_node = m_back = nullptr;
    }
    
    std::destroy_at(tmp);
    m_node_alloc.deallocate(tmp,1);
  }
  
  constexpr void push_front(const T &value) {
    node *new_node = m_node_alloc.allocate(1);
    std::construct_at(new_node, value, m_node, nullptr);
    
    if (m_node) {
      m_node->prev_node = new_node;
      m_node = new_node;
    } else
      m_node = m_back = new_node;
  }
  
  constexpr void push_front(T &&value)      {
    node *new_node = m_node_alloc.allocate(1);
    std::construct_at(new_node, std::move(value), m_node, nullptr);
    
    if (m_node) {
      m_node->prev_node = new_node;
      m_node = new_node;
    } else
      m_node = m_back = new_node;
  }

  template<class... Args>
  constexpr reference emplace_front(Args&&... args) { 
    node *new_node = m_node_alloc.allocate(1);
    std::construct_at(new_node, value_type(std::forward<Args>(args)...), 
                      m_node, nullptr);
    m_node->prev_node = new_node;
    m_node = new_node;
    
    if (m_node) {
      m_node->prev_node = new_node;
      m_node = new_node;
    } else
      m_node = m_back = new_node;
  }
  
  constexpr void pop_front() {
    node* tmp = m_node;
    
    if (m_node->next_node) {
      m_node = m_node->next_node;
      m_node->prev_node = nullptr;
    } else
      // assumption that this is the last element in the list or somethings 
      // went wrong and we've lost a node
      m_node = m_back = nullptr;
    

    std::destroy_at(tmp);
    m_node_alloc.deallocate(tmp,1);
  }
  
  // resize
  
  // swap 
  /*
  check this in list for an idea of how to implement it, although technically
  there is no need to implement everything...
  template <class _Tp, class _Alloc>
void
__list_imp<_Tp, _Alloc>::swap(__list_imp& __c)
  */
  constexpr void swap(list& other) noexcept {
  }
#endif
  
  node_base m_node;
  size_type m_size = 0;
  std::allocator_traits<allocator_type>::template rebind_alloc<node> m_node_alloc;
};

} // namespace cest

#endif // _CEST_LIST_HPP_
