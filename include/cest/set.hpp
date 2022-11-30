#ifndef _CEST_SET_HPP_
#define _CEST_SET_HPP_

#include "bits/tree.hpp"
#include <functional> // std::less
#include <memory>

namespace cest {

template <class Key, class Compare = std::less<Key>,
          class Allocator = std::allocator<Key>>
class set {
public:

  using key_type = Key;
  using value_type = Key;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Compare;
  using value_compare = Compare;
  using allocator_type = Allocator;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer =
      typename std::allocator_traits<Allocator>::const_pointer;
  using tree_type = tree<Key, Compare, Allocator>;
  // cppreference notes that these "may be aliases to the same type".
  using iterator = typename tree_type::const_tree_iter;
  using const_iterator = typename tree_type::const_tree_iter;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using node_type = typename tree_type::node_type;

  constexpr set() : m_tree{} {}

  constexpr set(const set &other) : set{} {
    for (auto it = other.begin(); it != other.end(); ++it)
      insert(*it);
  }

  constexpr ~set() { clear(); }

  constexpr set &operator=(const set &other) {
    clear();
    for (auto it = other.begin(); it != other.end(); ++it)
      insert(*it);
    return *this;
  }

  constexpr iterator begin() noexcept { return m_tree.begin(); }
  constexpr const_iterator begin() const noexcept { return m_tree.begin(); }
  constexpr const_iterator cbegin() const noexcept { return m_tree.cbegin(); }
  constexpr iterator end() noexcept { return m_tree.end(); }
  constexpr const_iterator end() const noexcept { return m_tree.end(); }
  constexpr const_iterator cend() const noexcept { return m_tree.cend(); }
  constexpr size_type size() const noexcept { return m_tree.size(); }
  [[nodiscard]] constexpr bool empty() const noexcept { return m_tree.empty(); }

  constexpr void clear() noexcept { m_tree.clear(); }

  constexpr iterator find(const Key &key) {
    return m_tree.find(key);
  }
  constexpr const_iterator find(const Key &key) const {
    return m_tree.find(key);
  }
  template <class K> constexpr iterator find(const K &key) {
    return m_tree.find(key);
  }
  template <class K> constexpr const_iterator find(const K &key) const {
    return find(key);
  }

  constexpr std::pair<iterator, bool> insert(const value_type &value) {
    return m_tree.insert(value);
  }

  //  std::pair<iterator,bool> insert( value_type&& value );

  // this currently ignores the hint argument (pos)
  constexpr iterator insert(const_iterator pos, const value_type &value) {
    return this->insert(value).first;
  }

  tree_type m_tree;
};

template <class Key, class Compare = std::less<Key>,
          class Allocator = std::allocator<Key>>
class multiset {
public:

  using key_type = Key;
  using value_type = Key;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Compare;
  using value_compare = Compare;
  using allocator_type = Allocator;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer =
      typename std::allocator_traits<Allocator>::const_pointer;
  using tree_type = tree<Key, Compare, Allocator, true>;
  // cppreference notes that these "may be aliases to the same type".
  using iterator = typename tree_type::const_tree_iter;
  using const_iterator = typename tree_type::const_tree_iter;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using node_type = typename tree_type::node_type;

  constexpr multiset() : m_tree{} {}

  constexpr multiset(const multiset &other) : multiset{} {
    for (auto it = other.begin(); it != other.end(); ++it)
      insert(*it);
  }

  constexpr ~multiset() { clear(); }

  constexpr multiset &operator=(const multiset &other) {
    clear();
    for (auto it = other.begin(); it != other.end(); ++it)
      insert(*it);
    return *this;
  }

  constexpr iterator begin() noexcept { return m_tree.begin(); }
  constexpr const_iterator begin() const noexcept { return m_tree.begin(); }
  constexpr const_iterator cbegin() const noexcept { return m_tree.cbegin(); }
  constexpr iterator end() noexcept { return m_tree.end(); }
  constexpr const_iterator end() const noexcept { return m_tree.end(); }
  constexpr const_iterator cend() const noexcept { return m_tree.cend(); }
  constexpr size_type size() const noexcept { return m_tree.size(); }
  [[nodiscard]] constexpr bool empty() const noexcept { return m_tree.empty(); }

  constexpr void clear() noexcept { m_tree.clear(); }

  constexpr iterator find(const Key &key) {
    return m_tree.find(key);
  }
  constexpr const_iterator find(const Key &key) const {
    return m_tree.find(key);
  }
  template <class K> constexpr iterator find(const K &key) {
    return m_tree.find(key);
  }
  template <class K> constexpr const_iterator find(const K &key) const {
    return find(key);
  }

  constexpr iterator insert(const value_type &value) {
    return m_tree.insert(value).first;
  }

  // iterator insert(value_type&& value);

  // this currently ignores the hint argument (pos)
  constexpr iterator insert(const_iterator pos, const value_type &value) {
    return this->insert(value);
  }

  tree_type m_tree;
};

} // namespace cest

#endif // _CEST_SET_HPP_
