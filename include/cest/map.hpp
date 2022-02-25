#ifndef _CEST_MAP_HPP_
#define _CEST_MAP_HPP_

#include "swap.hpp"
#include <functional> // std::less
#include <memory>

namespace cest {

template <class Key, class T, class Compare = std::less<Key>,
          class Allocator = std::allocator<std::pair<const Key, T>>>
class map {
public:
  struct node;
  struct tree_iter;
  struct const_tree_iter;

  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Compare;
  using allocator_type = Allocator;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer =
      typename std::allocator_traits<Allocator>::const_pointer;
  using iterator = tree_iter;
  using const_iterator = const_tree_iter;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using node_type = node;

  struct tree_iter {
    using value_type = map::value_type;
    using reference = value_type &;
    using pointer = value_type *;
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = ptrdiff_t;

    constexpr tree_iter() noexcept {}
    explicit constexpr tree_iter(node *np) noexcept : curr_node(np) {}

    constexpr reference operator*() const { return curr_node->x; }
    constexpr pointer operator->() const { return &curr_node->x; }

    constexpr tree_iter &operator++() // pre-increment
    {
      if (curr_node->r) {
        curr_node = curr_node->r;
        while (curr_node->l)
          curr_node = curr_node->l;
      } else {
        node *pn = curr_node->p;
        while (pn && pn->r && curr_node->x.first == pn->r->x.first) {
          curr_node = pn;
          pn = pn->p;
        }
        curr_node = pn;
      }
      return *this;
    }

    constexpr tree_iter operator++(int) // post-increment
    {
      tree_iter tmp(curr_node);
      ++(*this);
      return tmp;
    }

    friend constexpr bool operator==(const tree_iter &x, const tree_iter &y) {
      return x.curr_node == y.curr_node;
    }

    friend constexpr bool operator!=(const tree_iter &x, const tree_iter &y) {
      return x.curr_node != y.curr_node;
    }

    node *curr_node = nullptr;
  };

  struct const_tree_iter {
    using value_type = map::value_type;
    using reference = const value_type &;
    using pointer = const value_type *;
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = ptrdiff_t;

    constexpr const_tree_iter() noexcept {}
    explicit constexpr const_tree_iter(node *np) noexcept : curr_node(np) {}
    constexpr const_tree_iter(const map::iterator &it) noexcept
        : curr_node(it.curr_node) {}

    constexpr reference operator*() const { return curr_node->x; }
    constexpr pointer operator->() const { return &curr_node->x; }

    constexpr const_tree_iter &operator++() // pre-increment
    {
      if (curr_node->r) {
        curr_node = curr_node->r;
        while (curr_node->l)
          curr_node = curr_node->l;
      } else {
        node *pn = curr_node->p;
        while (pn && pn->r && curr_node->x.first == pn->r->x.first) {
          curr_node = pn;
          pn = pn->p;
        }
        curr_node = pn;
      }
      return *this;
    }

    constexpr const_tree_iter operator++(int) // post-increment
    {
      const_tree_iter tmp(curr_node);
      ++(*this);
      return tmp;
    }

    friend constexpr bool operator==(const const_tree_iter &x,
                                     const const_tree_iter &y) {
      return x.curr_node == y.curr_node;
    }

    friend constexpr bool operator!=(const const_tree_iter &x,
                                     const const_tree_iter &y) {
      return x.curr_node != y.curr_node;
    }

    node *curr_node = nullptr;
  };

  enum eCol { RED, BLACK };

  struct node {
    constexpr node(value_type x, node *l, node *r, node *p, eCol c)
        : x(x), l(l), r(r), p(p), c(c) {}
    value_type x;
    node *l, *r, *p;
    eCol c;
  };

  constexpr map() : m_root{}, m_begin{}, m_size{} {}
  constexpr ~map() { clear(); }

  constexpr map(const map &other) : map() {
    for (auto it = other.begin(); it != other.end(); ++it)
      insert(*it);
  }

  constexpr map &operator=(const map &other) {
    clear();
    for (auto it = other.begin(); it != other.end(); ++it)
      insert(*it);
    return *this;
  }

  // This seems to exist in cppreference.com; but GCC's std::set doesn't have
  // it, and it results in the iterator's type failing std::weakly_incrementable
  // etc.
  //  constexpr     iterator  begin()       noexcept { return iterator{m_begin};
  //  }
  constexpr const_iterator begin() const noexcept {
    return const_iterator{m_begin};
  }
  constexpr const_iterator cbegin() const noexcept {
    return const_iterator{m_begin};
  }
  constexpr iterator end() noexcept { return iterator(); }
  constexpr const_iterator end() const noexcept { return const_iterator(); }
  constexpr const_iterator cend() const noexcept { return const_iterator(); }
  constexpr size_type size() const noexcept { return m_size; }
  [[nodiscard]] constexpr bool empty() const noexcept { return 0 == m_size; }

  constexpr void clear() noexcept {
    auto dd = [this](node *n, auto &dd_rec) -> void {
      if (n) {
        dd_rec(n->l, dd_rec);
        dd_rec(n->r, dd_rec);
        std::destroy_at(n);
        m_node_alloc.deallocate(n, 1);
      }
    };
    dd(m_root, dd);

    m_root = nullptr;
    m_begin = nullptr;
    m_size = 0;
  }

  constexpr iterator find(const Key &key) {
    if (empty())
      return end();

    node *n = m_root;
    while (n) {
      if (m_comp(key, n->x.first)) {
        n = n->l;
        continue;
      } else if (m_comp(n->x.first, key)) {
        n = n->r;
        continue;
      } else {
        break;
      }
    }

    return iterator(n);
  }

  constexpr const_iterator find(const Key &key) const {
    if (empty())
      return end();

    node *n = m_root;
    while (n) {
      if (m_comp(key, n->x.first)) {
        n = n->l;
        continue;
      } else if (m_comp(n->x.first, key)) {
        n = n->r;
        continue;
      } else {
        break;
      }
    }

    return const_iterator(n);
  }

  template <class K> constexpr iterator find(const K &key) {
    if (empty())
      return end();

    node *n = m_root;
    while (n) {
      if (m_comp(key, n->x.first)) {
        n = n->l;
        continue;
      } else if (m_comp(n->x.first, key)) {
        n = n->r;
        continue;
      } else {
        break;
      }
    }

    return {n};
  }

  template <class K> constexpr const_iterator find(const K &key) const {
    if (empty())
      return end();

    node *n = m_root;
    while (n) {
      if (m_comp(key, n->x.first)) {
        n = n->l;
        continue;
      } else if (m_comp(n->x.first, key)) {
        n = n->r;
        continue;
      } else {
        break;
      }
    }

    return {n};
  }

  constexpr void rotate_left(node *&n) {
    node *nr = n->r;
    node *&nrlp = n->r->l ? n->r->l->p : nr;
    nary::swap(n, n->r, n->r->l, n->r->p, n->p, nrlp);
  };

  constexpr void rotate_right(node *&n) {
    node *nl = n->l;
    node *&nlrp = n->l->r ? n->l->r->p : nl;
    nary::swap(n, n->l, n->l->r, n->l->p, n->p, nlrp);
  };

  constexpr std::pair<iterator, bool> insert(const value_type &value) {
    bool added = false;
    node *ret_node = nullptr; // node added; or the one that prevents insertion
    auto ins = [this, &added, &value, &ret_node](node *&n, node *p,
                                                 auto &ins_rec) {
      if (!n) {
        n = ret_node = m_node_alloc.allocate(1);
        std::construct_at(ret_node, value, nullptr, nullptr, p, RED);
        m_begin =
            (!m_begin || m_comp(value.first, m_begin->x.first)) ? n : m_begin;
        added = true;
        m_size++;
        return;
      }
      if (m_comp(value.first, n->x.first)) {
        ins_rec(n->l, n, ins_rec);
      } else if (m_comp(n->x.first, value.first)) {
        ins_rec(n->r, n, ins_rec);
      } else {
        ret_node = n; // the node that prevents insertion
        return;
      }

      if (n->c == RED)
        return;

      if (n->l && n->l->c == RED && n->l->l && n->l->l->c == RED) {
        n->l->l->c = BLACK;
        rotate_right(n);
      } else if (n->l && n->l->c == RED && n->l->r && n->l->r->c == RED) {
        n->l->c = BLACK;
        rotate_left(n->l);
        rotate_right(n);
      } else if (n->r && n->r->c == RED && n->r->l && n->r->l->c == RED) {
        n->r->c = BLACK;
        rotate_right(n->r);
        rotate_left(n);
      } else if (n->r && n->r->c == RED && n->r->r && n->r->r->c == RED) {
        n->r->r->c = BLACK;
        rotate_left(n);
      }
    };

    ins(m_root, nullptr, ins);
    m_root->c = BLACK; // make_black

    return {iterator(ret_node), added};
  }

  node *m_root;
  node *m_begin;
  size_type m_size;
  allocator_type m_alloc;
  key_compare m_comp;
  typename std::allocator_traits<allocator_type>::template rebind_alloc<node>
      m_node_alloc;
};

} // namespace cest

#endif // _CEST_MAP_HPP_
