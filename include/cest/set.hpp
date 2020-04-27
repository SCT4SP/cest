#ifndef _CEST_SET_HPP_
#define _CEST_SET_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/set_tests.hpp

#include "cest/swap.hpp"
#include <functional> // std::less
#include <memory>

namespace cest {

template <
  class Key,
  class Compare   = std::less<Key>,
  class Allocator = std::allocator<Key>
>
class set
{
public:
  struct node;
  struct const_tree_iter;

  using key_type              = Key;
  using value_type            = Key;
  using size_type             = std::size_t;
  using difference_type       = std::ptrdiff_t;
  using key_compare           = Compare;
  using value_compare         = Compare;
  using allocator_type        = Allocator;
  using reference             =       value_type&;
  using const_reference       = const value_type&;
  using pointer               = std::allocator_traits<Allocator>::pointer;
  using const_pointer         = std::allocator_traits<Allocator>::const_pointer;
  using iterator              = const_tree_iter;
  using const_iterator        = const_tree_iter;
  using reverse_iterator      = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using node_type             = node;

  struct const_tree_iter {
    using reference         = const value_type&;
    using pointer           = const value_type*;
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type   = ptrdiff_t;

    constexpr reference operator*()  const { return  curr_node->x; }
    constexpr pointer   operator->() const { return &curr_node->x; }

    constexpr const_tree_iter&     operator++()    // pre-increment
    {
      if (curr_node->r)
      {
        curr_node = curr_node->r;
        while (curr_node->l)
          curr_node = curr_node->l;
      }
      else 
      {
        node *pn = curr_node->p;
        while (pn && pn->r && !m_comp(curr_node->x,pn->r->x) &&
                              !m_comp(pn->r->x,curr_node->x))
        {
          curr_node = pn;
          pn = pn->p;
        }
        curr_node = pn;
      }
      return *this;
    }

    constexpr const_tree_iter      operator++(int) // post-increment
    {
      const_tree_iter tmp(curr_node);
      ++(*this);
      return tmp; 
    }

    friend constexpr bool operator==(const const_tree_iter &x,
                                     const const_tree_iter &y) {
      return x.curr_node == y.curr_node; // this should work constexpr. Test.
    }

    friend constexpr bool operator!=(const const_tree_iter &x,
                                     const const_tree_iter& y) {
      return x.curr_node != y.curr_node;
    }

    node *curr_node = nullptr;
    key_compare m_comp; // needed above due to illegal constexpr ptr compare
  };

  enum eCol { RED, BLACK };

  struct node {
    value_type x;
    node      *l, *r, *p;
    eCol       c;
  };

  constexpr  set() : m_root(nullptr), m_begin(nullptr), m_size(0) {}
  constexpr ~set() {
    auto dd = [this](node *n, auto &dd_rec) -> void {
      if (n) {
        dd_rec(n->l,dd_rec);
        dd_rec(n->r,dd_rec);
        std::destroy_at(n);
        m_node_alloc.deallocate(n,1);
      }
    };
    dd(m_root,dd);
  }

// This seems to exist in cppreference.com; but GCC doesn't have it, and
// it results in the iterator's type failing std::weakly_incrementable etc.
//  constexpr     iterator  begin()       noexcept { return {m_begin}; }
  constexpr const_iterator  begin() const noexcept { return {m_begin}; }
  constexpr const_iterator cbegin() const noexcept { return {m_begin}; }
  constexpr       iterator    end()       noexcept { return {nullptr}; }
  constexpr const_iterator    end() const noexcept { return {nullptr}; }
  constexpr const_iterator   cend() const noexcept { return {nullptr}; }
  constexpr size_type        size() const noexcept { return m_size;    }
  [[nodiscard]]
  constexpr bool            empty() const noexcept { return 0==m_size; }

  constexpr       iterator find(const Key &key) {
    if (empty()) return end();

    node *n = m_root;
    while (n) {
      if      (m_comp(key,n->x)) { n = n->l; continue; }
      else if (m_comp(n->x,key)) { n = n->r; continue; }
      else                       {              break; }
    }

    return {n};
  }

  constexpr const_iterator find(const Key &key) const {
    if (empty()) return end();

    node *n = m_root;
    while (n) {
      if      (m_comp(key,n->x)) { n = n->l; continue; }
      else if (m_comp(n->x,key)) { n = n->r; continue; }
      else                       {              break; }
    }

    return {n};
  }

  template <class K>
  constexpr       iterator find(const K &key) {
    if (empty()) return end();

    node *n = m_root;
    while (n) {
      if      (m_comp(key,n->x)) { n = n->l; continue; }
      else if (m_comp(n->x,key)) { n = n->r; continue; }
      else                       {              break; }
    }

    return {n};
  }

  template <class K>
  constexpr const_iterator find(const K &key) const {
    if (empty()) return end();

    node *n = m_root;
    while (n) {
      if      (m_comp(key,n->x)) { n = n->l; continue; }
      else if (m_comp(n->x,key)) { n = n->r; continue; }
      else                       {              break; }
    }

    return {n};
  }

  constexpr void rotate_left(node *&n) {
    node  *nr   = n->r;
    node *&nrlp = n->r->l ? n->r->l->p : nr;
    nary::swap(n,n->r,n->r->l, n->r->p,n->p,nrlp);
  };

  constexpr void rotate_right(node *&n) {
    node  *nl   = n->l;
    node *&nlrp = n->l->r ? n->l->r->p : nl;
    nary::swap(n,n->l,n->l->r, n->l->p,n->p,nlrp);
  };

  constexpr std::pair<iterator,bool> insert(const value_type &value) {
    bool added = false;
    node *ret_node = nullptr; // node added; or the one that prevents insertion
    auto ins = [this,&added,&value,&ret_node](node *&n, node *p, auto &ins_rec) {
      if (!n) {
        n = ret_node = m_node_alloc.allocate(1);
        std::construct_at(ret_node,value,nullptr,nullptr,p,RED);
        m_begin = (!m_begin || m_comp(value,m_begin->x)) ? n : m_begin;
        added = true;
        m_size++;
        return;
      }
      if      (m_comp(value,n->x)) {
        ins_rec(n->l,n,ins_rec);
      }
      else if (m_comp(n->x,value)) {
        ins_rec(n->r,n,ins_rec);
      }
      else {
        ret_node = n; // the node that prevents insertion
        return;
      }

      if (n->c == RED) return;

      if        (n->l && n->l->c == RED && n->l->l && n->l->l->c == RED) {
        n->l->l->c = BLACK;
        rotate_right(n);
      } else if (n->l && n->l->c == RED && n->l->r && n->l->r->c == RED) {
        n->l->c    = BLACK;
        rotate_left(n->l);
        rotate_right(n);
      } else if (n->r && n->r->c == RED && n->r->l && n->r->l->c == RED) {
        n->r->c    = BLACK;
        rotate_right(n->r);
        rotate_left(n);
      } else if (n->r && n->r->c == RED && n->r->r && n->r->r->c == RED) {
        n->r->r->c = BLACK;
        rotate_left(n);
      }
    };

    ins(m_root,nullptr,ins);
    m_root->c = BLACK; // make_black

    return {iterator(ret_node),added};
  }
//  std::pair<iterator,bool> insert( value_type&& value );

  // this currently ignores the hint argument
  constexpr iterator insert(const_iterator hint, const value_type &value) {
    return this->insert(value).first;
  }

  node *m_root;
  node *m_begin;
  size_type m_size;
  allocator_type m_alloc;
  key_compare m_comp;
  std::allocator_traits<allocator_type>::template rebind_alloc<node> m_node_alloc;
};

} // namespace cest

#endif // _CEST_SET_HPP_
