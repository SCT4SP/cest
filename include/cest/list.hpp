#ifndef _CEST_LIST_HPP_
#define _CEST_LIST_HPP_

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
  using pointer               = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer         = typename std::allocator_traits<Allocator>::const_pointer;
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
    // Is this constructor used?
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

    constexpr bool      operator==(const iter& rhs) const noexcept {
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

    constexpr bool      operator==(const const_iter& rhs) const noexcept {
      return m_node == rhs.m_node;
    }
    
    const node_base* m_node = nullptr;
  };
  
  constexpr list() : m_size{} {}
  constexpr ~list() { clear(); }

  constexpr list(const list& other) : list()
  {
    for (auto it = other.cbegin(); it != other.cend(); ++it)
      push_back(*it);
  }

  constexpr list& operator=(const list& other)
  {
    clear();
    for (auto it = other.cbegin(); it != other.cend(); ++it)
      push_back(*it);
    return *this;
  }

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

  constexpr iterator insert(const_iterator pos, const T& value)
  {
    node_base*   p = const_cast<node_base*>(pos.m_node);
    node* new_node = m_node_alloc.allocate(1);

    // As an implicit-lifetime type (P0593R6), node construction isn't needed
    //std::construct_at(&new_node->value, value);
    std::construct_at(new_node, value);
    // ...actually...it is: that exemption doesn't apply for constexpr.

    // List_node_base::_M_hook
    // __tmp->_M_hook(__position._M_const_cast()._M_node);
    // new_node->m_hook(p);
    new_node->next = p;
    new_node->prev = p->prev;
    p->prev->next = new_node;
    p->prev       = new_node;

    m_size++;
    return {new_node};
  }
  
  constexpr iterator insert(const_iterator pos, value_type&& value) {
    return emplace(pos, std::move(value));
  }

  template <class... Args>
  constexpr iterator emplace(const_iterator pos, Args&&... args) {
    node_base*   p = const_cast<node_base*>(pos.m_node);
    node* new_node = m_node_alloc.allocate(1);

    // Could a move constructor for node reduce this to one construct_at call?
    std::construct_at(new_node, value_type(std::forward<Args>(args)...));
    //std::construct_at(&new_node->value, std::forward<Args>(args)...);

    // List_node_base::_M_hook
    new_node->next = p;
    new_node->prev = p->prev;
    p->prev->next = new_node;
    p->prev       = new_node;

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

  template <typename... Args >
  constexpr reference emplace_front(Args&&... args) {
    insert(begin(), value_type(std::forward<Args>(args)...));
    return front();
  }

  template <typename... Args >
  constexpr reference emplace_back(Args&&... args) {
    insert(end(), value_type(std::forward<Args>(args)...));
    return back();
  }

  constexpr void pop_back()  { erase({m_node.prev}); }
  constexpr void pop_front() { erase(begin());       }

  node_base m_node;
  size_type m_size;
  typename std::allocator_traits<allocator_type>::template rebind_alloc<node> m_node_alloc;
};

} // namespace cest

#endif // _CEST_LIST_HPP_
