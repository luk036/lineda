// Sample header file to illustrate the coding policy. The rules are
// based on the books "Large-Scale C++ Software Design" and "Effective
// C++, 2nd Ed"
//
//  DEF - definition
//  MDR - major design rule
//  mdr - minor design rule
//  GL  - guideline
//  P   - principle
//
//  effc++ - rules from Effective C++.
//

#ifndef DSL_BIN_TREE_HPP
#define DSL_BIN_TREE_HPP

#ifndef DSL_BT_NODE_HPP
#include "bt_node.hpp"
#endif

#include <cassert>
#include <iosfwd>
#include <stdexcept>

namespace dsl {

/** for throwing exception */
class bad_bin_tree : public std::domain_error {
public:
  explicit bad_bin_tree() : std::domain_error("bad bin_tree") {}
};

class bin_tree_base {
  typedef bin_tree_base _Self;

protected:
  bin_tree_base(size_t n)
      : // effc++: List members in an initialization list in the order
        // in which they are declared (Item 13).
        _size(n),
        _bt_ab(
            new bt_node_base *[n]), // effc++: Prefer "new" to "malloc" (Item 3)
        _root(0)                    // effc++: Prefer initialization to
                                    // assignment in constructors (Item 12)
  {
    assert(n > 1);
  }

  // effc++: Make sure base classes have vitual destructors (Item 14)
  // mdr: In every class that declares or is derived from a class
  // that declares a virtual function, explicitly declare the
  // destructor as the first virtual function in the class and
  // define it out of the line.
  virtual ~bin_tree_base();

  inline bool is_valid() const;
  inline void left_rotate(bt_node_base *x);
  inline void right_rotate(bt_node_base *y);

  virtual int cost(const bt_node_base &a, const bt_node_base &b) const;

private:
  // effc++: Explicitly disallow use of implicitly generated member
  // functions you don't want (Item 27)
  bin_tree_base(const _Self &);
  _Self &operator=(const _Self &rhs);

  void check_valid_recur(const bt_node_base *parent,
                         const bt_node_base *child) const;

protected:
  size_t _size;
  bt_node_base **const _bt_ab;
  bt_node_base *_root;
};

inline bool bin_tree_base::is_valid() const {
  //
  // effc++: Prefer C++-style comments for commenting out codes
  // (Item 4)
  //
  // xxx if (_root == 0) return true;
  // xxx if (_root->_parent != 0) return false;
  // xxx
  // xxx try { _root->check_valid(); }
  // xxx catch (bad_bt_node&) { return false; }
  return true;
}

inline void bin_tree_base::left_rotate(bt_node_base *x) {
  assert(x->_right != 0);
  if (x->_parent != 0)
    _root = x->_right;
  x->left_rotate();
}

inline void bin_tree_base::right_rotate(bt_node_base *y) {
  assert(y->_left != 0);
  if (y->_parent != 0)
    _root = y->_left;
  y->right_rotate();
}

template <typename _Tp> class bin_tree : public bin_tree_base {
  typedef bin_tree<_Tp> _Self;

public:
  bin_tree(size_t n) : bin_tree_base(n), _bt_a(new bt_node<_Tp>[n]) {
    assert(n > 1);
    bt_node<_Tp> *s = _bt_a;
    for (size_t i = 0; i < n; ++i) {
      _bt_ab[i] = (bt_node_base *)s++;
    }
  }

  ~bin_tree() { delete[] _bt_a; }

  // effc++: Declare a copy constructor and an assignment operator
  // for classes with dynamically allocated memory (Item 11).
  bin_tree(const _Self &);

  _Self &operator=(const _Self &rhs) {
    // effc++: Check for assignmentto self in operator= (Item 17)
    if (*this == rhs)
      return *this;
    delete[] _bt_a;
    delete[] _bt_ab;

    // effc++: Assign to all data members in operator= (Item 16)
    _size = rhs._size;
    _bt_a = new bt_node<_Tp>[_size];
    _bt_ab = new bt_node_base *[_size];

    // ...

    // effc++: Have operator= return a reference to *this (Item 15)
    return *this;
  }

protected:
  bt_node<_Tp> *const _bt_a;
};
}

#endif
