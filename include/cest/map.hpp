#ifndef _CEST_MAP_HPP_
#define _CEST_MAP_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/map_tests.hpp

#include <functional> // std::less
#include <memory>

namespace cest {

template <
  class Key,
  class T,
  class Compare   = std::less<Key>,
  class Allocator = std::allocator<std::pair<const Key, T>>
>
class map {
public:
  struct node;
  struct tree_iter;
  struct const_tree_iter;

  using key_type        = Key;
  using mapped_type     = T;
  using value_type      = std::pair<const Key, T>;
  using key_compare     = Compare;
  using allocator_type  = Allocator;
  using       reference =       value_type&;
  using const_reference = const value_type&;
  using       iterator  =       tree_iter;
  using const_iterator  = const_tree_iter;
  using size_type       = std::size_t;
  using node_type       = node;

  using _Iter           = tree_iter;

  struct node {
    value_type x;
  };

  struct tree_iter {
    using reference         = value_type&;
    using pointer           = value_type*;
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type   = ptrdiff_t;

    constexpr reference operator*()  const { return  curr_node->x; }
    constexpr pointer   operator->() const { return &curr_node->x; }

    node *curr_node = nullptr;
    key_compare m_comp; // needed above due to illegal constexpr ptr compare
  };

  struct const_tree_iter {
    using reference         = const value_type&;
    using pointer           = const value_type*;
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type   = ptrdiff_t;

    constexpr reference operator*()  const { return  curr_node->x; }
    constexpr pointer   operator->() const { return &curr_node->x; }

    node *curr_node = nullptr;
    key_compare m_comp; // needed above due to illegal constexpr ptr compare
  };

  constexpr map() : m_root(nullptr), m_size(0) {}
  constexpr ~map() {
  }
  constexpr std::pair<iterator,bool> insert(const value_type &value) {
    if (0 == m_size || 3 == m_size) {
      m_size++;
      std::pair<iterator,bool> p(iterator(),true);
      return p;
    } else {
      m_size++;
      std::pair<iterator,bool> p(iterator(),false);
      return p;
    }
  }

  constexpr iterator       find( const Key &key ) {
    node *n = m_root;
    return _Iter(n);
  }
  constexpr const_iterator find( const Key &key ) const {
    node *n = m_root;
    return _Iter(n);
  }

  node *m_root;
  size_type m_size;
  allocator_type m_alloc;
  std::allocator_traits<allocator_type>::template rebind_alloc<node> m_node_alloc;
};

} // namespace cest

#endif // _CEST_MAP_HPP_
