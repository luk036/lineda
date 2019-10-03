#ifndef DSL_GLIST_HPP
#define DSL_GLIST_HPP

#include <cassert>
#include <cstdlib> // for rand()
#include <iosfwd>

namespace dsl {
/** @addtogroup dsl
 *  @{
 */

/* Forward declaration */
template <class _Node> class glist;
template <class _Node>

/** ??? */
std::ostream &operator<<(std::ostream &, const glist<_Node> &);

/**
 * A Doubly-linked List class. This class simply contains a link of
 * node's. By adding a dummy node (sentinel) nil, deleting a node is
 * extremely fast (see "Introduction to Algorithm"). This class does
 * not keep the length information as it is not necessary for the FM
 * algorithm. This saves memory and run-time to update the length
 * information. Note that this class does not own the list node. They
 * are supplied by the caller in order to better reuse the nodes.
 *
 * From "Introduction to Algorithm":
 *
 * A sentinel is a dummy object that allows us to simplify boundary
 * conditions. For example, suppose that we provide with list L an object
 * nil[L] that represents NIL but has all the fields of the other list
 * elements. Wherever we have a reference to NIL in list code, we replace
 * it by a reference to the sentinel nil[L]. As shown in Figure 11.4,
 * this turns a regular doubly linked list into a circular list, with the
 * sentinel nil[L] placed between the head and tail; the field
 * next[nil[L]] points to the head of the list, and prev[nil[L]] points
 * to the tail. Similarly, both the next field of the tail and the prev
 * field of the head point to nil[L]. Since next[nil[L]] points to the
 * head, we can eliminate the attribute head[L] altogether, replacing
 * references to it by references to next[nil[L]]. An empty list consists
 * of just the sentinel, since both next[nil[L]] and prev[nil[L]] can be
 * set to nil[L].
 *
 */
template <class _Node> class glist {
  typedef glist<_Node> _Self;

public:
  typedef typename _Node::iterator iterator;
  typedef typename _Node::const_iterator const_iterator;
  friend std::ostream &operator<<<>(std::ostream &, const glist<_Node> &);

  /** Constructor */
  glist() : _head(nil()), _tail(_head) {}

  /** @return true if list is empty. */
  bool empty() const { return _head == nil(); }

  /** Reset to empty list. */
  void clear() { _head = _tail = nil(); }

  /** Reverse */
  void reverse() { nil()->reverse(); }

  /** @return iterator that points to the head of the list */
  iterator begin() { return iterator(_head); }

  /** @return iterator that points to the tail of the list */
  iterator end() { return iterator(nil()); }

  /** @return iterator that points to the head of the list */
  const_iterator begin() const { return const_iterator(_head); }

  /** @return iterator that points to the tail of the list */
  const_iterator end() const { return const_iterator(nil()); }

  /** @return the first item. Precondition: list is not empty */
  _Node &first() {
    assert(!empty());
    return *_head;
  }

  /** @return the last item. Precondition: list is not empty */
  _Node &last() {
    assert(!empty());
    return *_tail;
  }

  /** @return the first item. Precondition: list is not empty */
  const _Node &first() const {
    assert(!empty());
    return *_head;
  }

  /** @return the last item. Precondition: list is not empty */
  const _Node &last() const {
    assert(!empty());
    return *_tail;
  }

  /** Detach item from this list. Precondition: list contains the node */
  static void detach(_Node &ptr) { ptr.detach(); }

  /** Insert item at the front of this list.
      Precondition: list does not contain the node */
  void push_front(_Node &ptr) {
    ptr._next = _head;
    _head->_prev = &ptr;
    _head = &ptr;
    ptr._prev = nil();
  }

  /** Insert item at the end of this list
      Precondtion: list does not contain the node */
  void push_back(_Node &ptr) {
    ptr._prev = _tail;
    _tail->_next = &ptr;
    _tail = &ptr;
    ptr._next = nil();
  }

  /** Arbitrarily put item at the front or at the end of this list
      Precondition: list does not contain the node */
  void push_random(_Node &ptr) {
    if (rand() & 1)
      push_back(ptr);
    else
      push_front(ptr);
  }

  /** Pop an item from the front of the list.
      Precondition: list is not empty */
  void pop_front() {
    assert(!empty());
    _head = _head->_next;
    _head->_prev = nil();
  }

  /** Pop an item from the end of the list.
      Precondition: list is not empty */
  void pop_back() {
    assert(!empty());
    _tail = _tail->_prev;
    _tail->_next = nil();
  }

  /** Concatenate L to the end of this list. L becomes an empty list
      after the concatenation. */
  void concat(glist<_Node> &L) {
    if (L.empty())
      return;
    _tail->_next = L._head;
    L.first()._prev = _tail;
    L.last()._next = nil();
    _tail = L._tail;
    L.clear();
  }

  /** @return true if the list is valid. Take O(n) time */
  bool is_valid() const {
    const _Node *cur = nil();
    do {
      if (cur->_next->_prev != cur)
        return false;
      if (cur->_prev->_next != cur)
        return false;
      cur = cur->_next;
    } while (cur != nil());
    return true;
  }

  /** (for restricted use only) Make the list act like a non-empty list
      (Function empty() always returns false). */
  void make_dummy() { _head = 0; }

private:
  _Node *nil() { return reinterpret_cast<_Node *>(this); }
  const _Node *nil() const { return reinterpret_cast<const _Node *>(this); }

  // Unimplemented (no way to implement these)
  glist(const glist<_Node> &);
  glist<_Node> &operator=(const glist<_Node> &);

private:
  _Node *_head; /**< pointer of the head */
  _Node *_tail; /**< pointer of the tail */
};

/** @} */ // end of dsl
}

#endif
