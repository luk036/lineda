#ifndef DSL_SNODE_HPP
#define DSL_SNODE_HPP

#include <cassert>

namespace dsl {
/**
 * @addtogroup dsl
 * @{
 */

/** Iterator (const or non-const) for snode_base. */
template <class _Ref, class _Ptr = _Ref> class snode_base_iterator {
  // typedef forward_iterator_tag iterator_category;
  typedef snode_base_iterator<_Ref, _Ptr> _Self;

public:
  /** Default constructor */
  snode_base_iterator() : _p(nullptr) {}

  /** Construct an iteration by the node pointer x */
  snode_base_iterator(_Ptr *x) : _p(x) {}

  /** Copy constructor */
  snode_base_iterator(const _Self &copy) : _p(copy._p) {}

  /** Assignment */
  _Self &operator=(const _Self &rhs) {
    _p = rhs._p;
    return *this;
  }

  /** @return the node pointer */
  _Ptr *value() const { return _p; }

  /** Dereference */
  _Ref &operator*() const { return *_p; }

  /** Reference */
  _Ptr *operator->() const { return _p; }

  /** @return the next iterator */
  _Self next() const { return _Self(_p->next()); }

  /** Prefix increment: advance one node */
  _Self &operator++() {
    _p = _p->_next;
    return *this;
  }

  /** Postfix increment: advance one node */
  _Self operator++(int) {
    _Self old(*this);
    ++*this;
    return old;
  }

  /** Equal */
  bool operator==(const _Self &x) const { return _p == x._p; }

  /** Not equal */
  bool operator!=(const _Self &x) const { return _p != x._p; }

  /** Equal */
  bool operator==(const _Ptr *p) const { return _p == p; }

  /** Not Equal */
  bool operator!=(const _Ptr *p) const { return _p != p; }

private:
  _Ptr *_p; /**< the node pointer */
};

/** Base class of Singly-linked node. */
struct snode_base {
  typedef snode_base _Self;
  typedef snode_base_iterator<_Self> iterator;
  typedef snode_base_iterator<const _Self> const_iterator;

  _Self *_next; /**< pointer to the next node */

  /** Default constructor */
  snode_base() : _next(nullptr) {}

  /** Compiler generated copy constructor
      and assignment operator are fine. */

  /** @return the next pointer */
  inline const _Self *next() const { return _next; }

  /** @return the next pointer */
  inline _Self *next() { return _next; }

  /**
   * Attach b after this node.
   *   Before: -> this -> ...
   *   After:  -> this -> b -> ...
   */
  inline void attach_after(_Self *b) {
    b->_next = _next;
    _next = b;
  }

  /**
   * Detach the node after this node
   *   Before: -> this -> b-> ...
   *   After:  -> this -> ...
   * Precondition: there exists a node next to this node.
   * Note: if this node points to itself, this operation takes no effect.
   */
  inline void detach_after() {
    assert(_next != nullptr);
    _next = _next->_next;
  }

  /**
   * @return whether it contains cycle on its path.
   * Time compliexity: O(n). Space complexity O(1).
   */
  inline _Self *has_cycle();

  inline _Self *has_cycle2();

  /**
   * @return whether it will loops back to itself.
   * Time compliexity: O(n). Space complexity O(1).
   */
  inline bool loop_back() const;

  /** Equality */
  inline bool operator==(const _Self &rhs) const {
    return next() == rhs.next();
  }

  /**
   * Reverse a singly linked list in which this node is the head.
   * @return the pointer to the head of the reversed list.
   * Time complexity: O(n). Space complexity: O(1).
   */
  inline _Self *reverse();

  /**
   * Perform (n,m)-Josephus permutation. Assume that this node
   * is the tail of a circular singly lined list.
   * @return  pointer to the tail of the permutated circular linked list.
   * Pre-conditions: m > 0 and m <= n, where n is the length of list
   * Time complexity: O(n*m), Space complexity: O(1)
   */
  _Self *josephus_permutate(int m);
};

/**
 * @return a pointer to a snode_base. If a link contains cycle on
 * its path, return pointer to a node that is inside the cycle.
 * Otherwise, return a null pointer.
 * The time complexity is O(n) where n is the length of the path.
 * The storage requirement is O(1). The trick is to emit two
 * pointers, say n1 and n2, from the head of the "list".
 * Each time n2 advances two steps ahead, while n1 advances
 * only one step. If the two pointers meet again, then
 * the "list" has a cycle. If n2 encounters a null pointer,
 * then the list has no cycle.
 */
inline snode_base *snode_base::has_cycle() {
  iterator n1(this), n2(n1);
  while (n2 != nullptr && ++n2 != nullptr && ++n2 != ++n1)
    ;
  return n2.value();
}

/**
 * @return a pointer to a snode_base. If a link contains cycle on
 * its path, return pointer to the node that could break the cycle.
 * Otherwise, return a null pointer.
 * The time complexity is O(n) where n is the length of the path.
 * The storage requirement is O(1).
 *
 */
inline snode_base *snode_base::has_cycle2() {
  iterator n2(has_cycle());
  if (n2 == nullptr)
    return nullptr;
  iterator n3(this), n1(n2);
  while (++n1 != n2)
    ++n3; // advance n3 (len-1) steps
  n2 = this;
  while (n2 != n3->_next)
    ++n2, ++n3;
  return n3.value(); // to break the cycle, assign 0 to n3->_next
}

/**
 * @return whether it will loops back to itself.
 * @see has_cycle().
 */
inline bool snode_base::loop_back() const {
  const_iterator n1(this), n2(this);
  while (n2 != nullptr) {
    if (++n2 == this)
      return true;
    else if (n2 == nullptr)
      return false;
    if (++n2 == this)
      return true;
    else if (n2 == ++n1)
      return false;
  }
  return false;
}

/**
 * Reverse a linked list in which this node is the head.
 * @return the pointer to the head of the reversed list.
 * Time complexity: O(n). Space complexity: O(1).
 */
inline snode_base *snode_base::reverse() {
  _Self *res = nullptr;
  _Self *head = this;
  while (head != nullptr) {
    _Self *const tmp = head->_next;
    head->_next = res;
    res = head;
    head = tmp;
  }
  return res;
}

// Forward declaration
template <typename _Tp> class snode;

/** Iterator for Singly linked node. */
template <typename _Tp, typename _Ref, typename _Ptr> class snode_iterator {
  // typedef forward_iterator_tag iterator_category;
  typedef snode_iterator<_Tp, _Ref, _Ptr> _Self;
  typedef snode_base_iterator<snode<_Tp> > _Base;

public:
  typedef _Tp value_type;

  /** Default constructor */
  snode_iterator(snode<_Tp> *x) : _it(x) {}

  /** Construct an iteration by the node pointer x */
  snode_iterator() : _it(nullptr) {}

  /* Default copy constructor and assignment operator are fine. */

  /** Dereference */
  _Ref operator*() const { return _it->_data; }

  /** Reference */
  _Ptr operator->() const { return &_it->_data; }

  /** @return the next iterator */
  _Self next() const { return _Self(_it.next().value()); }

  /** Prefix increment: advance one node */
  _Self &operator++() {
    ++_it;
    return *this;
  }

  /** Postfix increment: advance one node */
  _Self operator++(int) {
    _Self old(*this);
    ++*this;
    return old;
  }

  /** Equal */
  bool operator==(const _Self &x) const { return _it == x._it; }

  /** Not equal */
  bool operator!=(const _Self &x) const { return _it != x._it; }

private:
  _Base _it; /**< base iterator */
};

/** Singly linked node */
template <typename _Tp> class snode {
  typedef snode<_Tp> _Self;

public:
  typedef snode_base _Base;
  typedef snode_iterator<_Tp, _Tp &, _Tp *> iterator;
  typedef snode_iterator<_Tp, const _Tp &, const _Tp *> const_iterator;

  _Self *_next; /**< pointer to next node */
  _Tp _data;    /**< data that this node carries */

  /** Default constructor */
  snode() : _next(nullptr), _data() {}

  /** Copy constructor */
  snode<_Tp>(const _Self &s) : _next(s._next), _data(s._data) {}

  /** @return the next pointer */
  const _Self *next() const { return _next; }

  /** @return the next pointer */
  _Self *next() { return _next; }

  /** @return the data */
  const _Tp &data() const { return _data; }

  /** @return the next pointer */
  _Tp &data() { return _data; }

  /** Assignment */
  _Self &operator=(const _Self &s) {
    _next = s._next, _data = s._data;
    return *this;
  }

  /**
   * @return whether it contains cycle on its path.
   * Time compliexity: O(n). Space complexity O(1).
   */
  inline _Self *has_cycle() { return (_Self *)((_Base *)this)->has_cycle(); }

  /**
   * @return whether it will loops back to itself.
   * Time compliexity: O(n). Space complexity O(1).
   */
  inline bool loop_back() const { return ((const _Base *)this)->loop_back(); }

  /** Equality */
  inline bool operator==(const _Self &rhs) const {
    return next() == rhs.next() && data() == rhs.data();
  }

  /**
   * Reverse a linked list in which this node is the head.
   * @return the pointer to the head of the reversed list.
   * Time complexity: O(n). Space complexity: O(1).
   */
  inline _Self *reverse() { return (_Self *)((_Base *)this)->reverse(); }

  /**
   * Perform (n,m)-Josephus permutation. Assume that this node
   * is the tail of a circular singly lined list.
   * @return  pointer to the tail of the permutated circular linked list.
   * Pre-conditions: m > 0 and m <= n, where n is the length of list
   * Time complexity: O(n), Space complexity: O(1)
   */
  inline _Self *josephus_permutate(int m) {
    return (_Self *)((_Base *)this)->josephus_permutate(m);
  }
};

/** @} */ // end of dsl
}

#endif
