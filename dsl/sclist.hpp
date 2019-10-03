#ifndef DSL_SCLIST_HPP
#define DSL_SCLIST_HPP

#ifndef DSL_SL_BASE_HPP
#include "sl_base.hpp"
#endif
#include <iosfwd> // for declaration of std::istream and std::ostream

namespace dsl {
/**
 * @addtogroup dsl
 * @{
 */

/** Singly Linked Circular List. */
template <typename _Tp> class sclist : public slist_base<_Tp> {
public:
  typedef sclist<_Tp> _Self;
  typedef slist_base<_Tp> _Base;
  typedef snode<_Tp> node;
  typedef snode_base node_base;
  typedef typename node::iterator iterator;
  typedef typename node::const_iterator const_iterator;

  /** Create a circular singly linked list with size n. */
  inline sclist(size_t n) : _Base(n), _tail(_Base::_sa + n - 1), _num_nodes(n) {
    assert(n > 0);
    _Base::make_list();
    _tail->_next = _Base::_sa;
  }

  /** Destructor */
  ~sclist() override = default;

  /** Copy constructor */
  inline sclist(const _Self &copy)
      : _Base(copy), _tail(copy._tail - copy._sa + _Base::_sa),
        _num_nodes(copy._num_nodes) {}

  /** Equality */
  inline bool operator==(const _Self &rhs) const {
    if (this == &rhs)
      return true;
    if (_tail != rhs._tail - rhs._sa + _Base::_sa)
      return false;
    return _Base::operator==(rhs);
  }

  /** @return whether this is a valid circular list */
  inline bool is_valid() const { return _tail->loop_back(); }

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

  /** Apply function to the carrying data */
  template <class _Function> void apply(_Function f) {
    node *sa = _Base::head_ptr();
    for (node *p = sa; p != sa + _Base::size(); ++p)
      f(p->_data);
  }

  /** Remove duplicates of consecutive elements */
  void unique() {
    if (_tail == _tail->_next)
      return; // only one node
    node *s = _tail;
    do {
      node *t = s->_next;
      while (s->_data == t->_data) {
        s->_next = t->_next;
        t = s->_next;
        --_num_nodes;
        if (t == _tail) {
          _tail = s;
          return;
        }
      }
      s = s->_next;
    } while (s != _tail);
  }

private:
  _Self &operator=(const _Self &) = delete;

private:
  node *_tail;       /**< point to the tail of the circular list */
  size_t _num_nodes; /**< number of nodes */
};

// Non-member funtions

/** Print out the order of the circular list cs */
template <typename _Tp>
inline std::ostream &operator<<(std::ostream &os, const sclist<_Tp> &cs) {
  return cs.print_clist(os, (const typename sclist<_Tp>::node_base *)cs.tail());
}

/** @} */ // end of dsl
}

#endif
