#ifndef DSL_DCLIST_HPP
#define DSL_DCLIST_HPP

#include "dl_base.hpp"
#include <iosfwd> // for declaration of std::istream and std::ostream
#include <iostream>

namespace dsl {
/**
 * @addtogroup dsl
 * @{
 */

/** Doubly Linked Circular List. */
template <typename _Tp> class dclist : public dlist_base<_Tp> {
  typedef dclist<_Tp> _Self;
  typedef dlist_base<_Tp> _Base;

public:
  typedef dnode<_Tp> node;
  typedef dnode_base node_base;
  typedef typename node::iterator iterator;
  typedef typename node::const_iterator const_iterator;

  /** Create a circular doubly linked list with size n. */
  inline explicit dclist(size_t n)
      : _Base(n), _tail(_Base::_sa + n - 1), _num_nodes(n) {
    assert(n > 0);
    _Base::make_list();
    node *head = _Base::_sa;
    _tail->_next = head;
    head->_prev = _tail;
  }

  /** Destructor */
  ~dclist() override = default;

  /** Copy constructor */
  inline dclist(const _Self &copy)
      : _Base(copy),
        //_tail(copy._tail - copy._sa + _Base::_sa),
        _num_nodes(copy._num_nodes) {
    _tail = copy._tail - copy._sa + _Base::_sa;
  }

  /** Equality */
  inline bool operator==(const _Self &rhs) const {
    if (this == &rhs)
      return true;
    if (_tail != rhs._tail - rhs._sa + _Base::_sa)
      return false;
    return _Base::operator==(rhs);
  }

  /** @return whether this is a valid circular list */
  inline bool is_valid() const {
    if (!_tail->loop_back())
      return false;

    const node *cur = _tail;
    do {
      if (cur->_next->_prev != cur)
        return false;
      if (cur->_prev->_next != cur)
        return false;
      cur = cur->_next;
    } while (cur != _tail);

    return true;
  }

  /** Perform (n,m)-Josephus permutation.
      Time complexity: O(n), Space complexity: O(1) */
  inline void josephus_permutate(int m) {
    _tail = _tail->josephus_permutate(m);
  }

  /** Reverse */
  void reverse() { _tail->reverse(); }

  /** @return iterator that points to the head of the list */
  iterator begin() { return iterator(_tail); }

  /** @return iterator that points to the tail of the list */
  iterator end() { return iterator(_tail); }

  /** @return iterator that points to the head of the list */
  const_iterator begin() const { return const_iterator(_tail); }

  /** @return iterator that points to the tail of the list */
  const_iterator end() const { return const_iterator(_tail); }

  /** @return the tail of the list */
  const node *tail() const { return _tail; }

  /** @return the tail of the list */
  node *tail() { return _tail; }

  /** Set the tail of the list. Precondition tail is on the list. */
  void set_tail(node *tail) { _tail = tail; }

  /** @return the actual number of nodes connected */
  size_t num_nodes() const { return _num_nodes; }

  /** Apply function to the carrying data */
  template <class _Function> void apply(_Function f) {
    node *first = _Base::head_ptr();
    node *last = first + _Base::size();
    for (node *p = first; p != last; ++p)
      f(p->_data);
  }

  void detach(node *s) {
    s->detach();
    --_num_nodes;
  }

  /** Remove duplicates of consecutive elements */
  void unique() {
    if (_tail == _tail->_next)
      return; // only one node
    node *s = _tail;
    do {
      node *t = s->_next;
      while (s->_data == t->_data) {
        std::cout << s->_prev->_data << std::endl;
        std::cout << s->_data << std::endl;
        std::cout << t->_data << std::endl;
        std::cout << t->_next->_data << std::endl;

        detach(t);
        t = s->_next;
        if (t == _tail) {
          _tail = s;
          return;
        }
      }
      s = s->_next;
    } while (s != _tail);
  }

  /** Reconnect the doubly linked list from singly list */
  inline void connect() {
    node *a = _tail;
    do {
      node *t = a->_next;
      t->_prev = a;
      a = t;
    } while (a != _tail);
  }

private:
  // Degenerate assignment operator
  _Self &operator=(const _Self &) = delete;

private:
  node *_tail;       /**< point to the tail of the circular list */
  size_t _num_nodes; /**< number of nodes */
};

// Non-member funtions

/** Print out the order of the circular list cs */
template <typename _Tp>
inline std::ostream &operator<<(std::ostream &os, const dclist<_Tp> &cs) {
  return cs.print_clist(os, (const typename dclist<_Tp>::node_base *)cs.tail());
}

/** @} */ // end of dsl
}

#endif
