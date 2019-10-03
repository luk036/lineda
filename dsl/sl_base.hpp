#ifndef DSL_SL_BASE_HPP
#define DSL_SL_BASE_HPP

#ifndef DSL_SNODE_HPP
#include "snode.hpp"
#endif
#include <cassert>
#include <iosfwd> // for declaration of std::istream and std::ostream

namespace dsl {
/**
 * @addtogroup dsl
 * @{
 */

/** Base class for Singly linked list or Singly linked tree. */
class sl_base {
  typedef sl_base _Self;

public:
  typedef snode_base node;

  /** Create a singly linked list. */
  sl_base(size_t n);

  /** Destructor */
  virtual ~sl_base();

  /** @return the number of nodes */
  inline size_t num_nodes() const { return _size; }

  /** @return the number of nodes */
  inline size_t size() const { return _size; }

  /** @return the cost between two nodes. Default is a unit cost. */
  virtual double cost(const node *, const node *) const;

  /** @return the total edge cost. */
  inline double total_cost() const;

  /**
   * Contruct a minimum spanning tree using Prim's algorithm.
   * @return the total edge cost of the MST.
   * Precondtions:
   *   1. Number of nodes > 1.
   *   2. cost() is symmetric, i.e., cost(a, b) == cost(b, a).
   *      However, it could be negative value and doesn't need to satisfy
   *      the triangle inequality: cost(a, c) <= cost(a, b) + cost(b, c).
   * Post-condition: total cost of MST <= total cost of initial tree
   * Time complexity: O(n^2). Space complexity: O(n).
   */
  double minimum_spanning_tree();

  /** Construct a minimum spanning tree using node i as the root.
      @return the total edge cost of the MST.
      @see minmum_spanning_tree(). */
  inline double mst(size_t i = 0);

  /** Print out the structure in .dot format to os.
      Pre-condition: the underlying structure is a tree. */
  std::ostream &print_tree(std::ostream &os) const;

  /** Print out the structure in .dot format to os
      Pre-condition: the underlying structure is a circular list. */
  std::ostream &print_clist(std::ostream &os, const node *tail) const;

protected:
  /** Make as a tree. */
  inline void make_tree() {
    node *const root = _sab[0];
    root->_next = nullptr;
    for (size_t i = 1; i < num_nodes(); ++i) {
      _sab[i]->_next = root;
    }
  }

private:
  // Degenerate copy and assignment
  sl_base(const _Self &) = delete;
  _Self &operator=(const _Self &) = delete;

private:
  size_t _size; /** size of array */

protected:
  node **const _sab; /** array of pointer of node's */
};

/** Construct a minimum spanning tree using node i as the root.
    @see minmum_spanning_tree(). */
inline double sl_base::mst(size_t i) {
  double total_cost = minimum_spanning_tree();
  if (i != 0)
    _sab[i]->reverse();
  return total_cost;
}

/** @return the total length cost. */
inline double sl_base::total_cost() const {
  double res = 0.0;
  for (size_t i = 0; i < num_nodes(); ++i) {
    const node *s = _sab[i];
    if (s->_next == nullptr)
      continue;
    res += cost(s->_next, s);
  }
  return res;
}

/** Base class for Singly linked list or circular Singly linked list. */
template <typename _Tp> class slist_base : public sl_base {
  typedef slist_base<_Tp> _Self;

public:
  typedef sl_base _Base;
  typedef snode<_Tp> node;
  typedef snode_base node_base;

  /** Create a singly linked list. */
  inline slist_base(size_t n) : _Base(n), _sa(new node[n]) {
    assert(n > 0);
    copy_base();
  }

  /** Destructor */
  inline ~slist_base() override { delete[] _sa; }

  /** Copy constructor */
  inline slist_base(const _Self &copy)
      : _Base(copy.num_nodes()), _sa(new node[num_nodes()]) {
    copy_base();
    for (size_t i = 0; i < num_nodes(); ++i) {
      if (copy._sa[i]._next != nullptr)
        _sa[i]._next = copy._sa[i]._next - copy._sa + _sa;
      else
        _sa[i]._next = nullptr;
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
      if (_sa[i]._data != rhs._sa[i]._data)
        return false;
    }
    return true;
  }

  /** Make as a list */
  inline void make_list() {
    node *s = _sa;
    for (size_t i = 0; i < num_nodes(); ++i, ++s) {
      s->_next = s + 1;
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
