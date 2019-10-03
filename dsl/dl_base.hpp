#ifndef DSL_DL_BASE_HPP
#define DSL_DL_BASE_HPP

#ifndef DSL_DNODE_HPP
#include "dnode.hpp"
#endif
#include <cassert>
#include <iosfwd> // for declaration of std::istream and std::ostream

namespace dsl {
/**
 * @addtogroup dsl
 * @{
 */

/** Base class for Doubly linked list or Doubly linked tree. */
class dl_base {
  typedef dl_base _Self;
  typedef dnode_base node;

public:
  /** Create a doubly linked list. */
  dl_base(size_t n);

  /** Destructor */
  virtual ~dl_base();

  /** @return the number of nodes */
  inline size_t num_nodes() const { return _size; }

  /** @return the number of nodes */
  inline size_t size() const { return _size; }

  /** @return the cost between two nodes. Default is a unit cost. */
  virtual double cost(const node *, const node *) const;

  /** @return the total edge cost. */
  inline double total_cost() const;

  /** Print out the structure in .dot format to os
      Pre-condition: the underlying structure is a circular list. */
  std::ostream &print_clist(std::ostream &os, const node *tail) const;

private:
  // Degenerate copy and assignment
  dl_base(const _Self &) = delete;
  _Self &operator=(const _Self &) = delete;

private:
  size_t _size; /** size of array */

protected:
  node **const _sab; /** array of pointer of node's */
};

/** @return the total length cost. */
inline double dl_base::total_cost() const {
  double res = 0.0;
  for (size_t i = 0; i < num_nodes(); ++i) {
    const node *s = _sab[i];
    if (s->_next == nullptr)
      continue;
    res += cost(s->_next, s);
  }
  return res;
}

/** Base class for Doubly linked list or circular Doubly linked list. */
template <typename _Tp> class dlist_base : public dl_base {
  typedef dlist_base<_Tp> _Self;
  typedef dl_base _Base;
  typedef dnode<_Tp> node;
  typedef dnode_base node_base;

public:
  /** Create a doubly linked list. */
  inline dlist_base(size_t n) : _Base(n), _sa(new node[n]) {
    assert(n > 0);
    copy_base();
  }

  /** Destructor */
  inline ~dlist_base() override { delete[] _sa; }

  /** Copy constructor */
  inline dlist_base(const _Self &copy)
      : _Base(copy.num_nodes()), _sa(new node[num_nodes()]) {
    copy_base();
    for (size_t i = 0; i < num_nodes(); ++i) {
      if (copy._sa[i]._next != nullptr)
        _sa[i]._next = copy._sa[i]._next - copy._sa + _sa;
      else
        _sa[i]._next = nullptr;
      if (copy._sa[i]._prev != nullptr)
        _sa[i]._prev = copy._sa[i]._prev - copy._sa + _sa;
      else
        _sa[i]._prev = nullptr;
      _sa[i]._data = copy._sa[i]._data;
    }
  }

  /** @return a pointer to the array of nodes */
  inline const node *head_ptr() const { return _sa; }

  /** @return a pointer to the array of nodes */
  inline node *head_ptr() { return _sa; }

  /** Equality */
  inline bool operator==(const _Self &rhs) const {
    if (this == &rhs)
      return true;
    if (num_nodes() != rhs.num_nodes())
      return false;
    for (size_t i = 0; i < num_nodes(); ++i) {
      if (_sa[i]._next == nullptr) {
        if (rhs._sa[i]._next != nullptr)
          return false;
      } else {
        if (_sa[i]._next - _sa != rhs._sa[i]._next - rhs._sa)
          return false;
      }
      if (_sa[i]._prev == nullptr) {
        if (rhs._sa[i]._prev != nullptr)
          return false;
      } else {
        if (_sa[i]._prev - _sa != rhs._sa[i]._prev - rhs._sa)
          return false;
      }
      if (!(_sa[i]._data == rhs._sa[i]._data))
        return false;
    }
    return true;
  }

  /** Make as a list */
  inline void make_list() {
    node *s = _sa;
    for (size_t i = 0; i < num_nodes(); ++i, ++s) {
      s->_next = s + 1;
      s->_prev = s - 1;
    }
  }

  /** @return the cost between node a and node b */
  double cost(const node_base *, const node_base *) const override {
    return 1.0;
  }

private:
  // Unimplement
  _Self &operator=(const _Self &) = delete;

  inline void copy_base() {
    node *s = _sa;
    for (size_t i = 0; i < num_nodes(); ++i) {
      _sab[i] = (node_base *)s++;
    }
  }

protected:
  node *const _sa; /** array of pointer of node's */
};

/** @} */ // end of dsl
}

#endif
