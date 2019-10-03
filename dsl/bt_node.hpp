#ifndef DSL_BT_NODE_HPP
#define DSL_BT_NODE_HPP

#include <stdexcept> // for std::domain_error

namespace dsl {
/**
 * @addtogroup dsl
 * @{
 */

/** Class that will be thrown when invalid binary tree is found */
class bad_bt_tree : public std::domain_error {
public:
  explicit bad_bt_tree() : std::domain_error("bad bt_tree") {}
};

/** Iterator (const or non-const) for bt_node_base.
 * Traverse binary tree in post-fix (bottom-up) order.
 *
 * Reference: A. D. Robison. Considering Recursion: How harmful is
 * it? Dr. Dobb's Journal March 2000
 */
template <class _Ref, class _Ptr = _Ref> class bt_node_base_iterator {
  // typedef forward_iterator_tag iterator_category;
  typedef bt_node_base_iterator<_Ref, _Ptr> _Self;

public:
  /** Default constructor */
  bt_node_base_iterator() : _p(0) {}

  /** Construct an iteration by the node pointer x */
  bt_node_base_iterator(_Ptr *x) : _p(x) {}

  bt_node_base_iterator(const _Self &rhs) : _p(rhs._p) {}

  _Self &operator=(const _Self &rhs) {
    _p = rhs._p;
    return *this;
  }

  /** @return the node pointer */
  _Ptr *value() const { return _p; }

  /** Dereference */
  _Ref &operator*() const { return *_p; }

  /** Reference */
  _Ptr *operator->() const { return _p; } //?

  /** Prefix increment: advance one node (post-order, i.e. bottom-up) */
  _Self &operator++() {
    _Ptr *m = _p;
    _Ptr *p = m->parent();

    if (p == 0 || m == p->right() || p->right() == 0) {
      _p = p;
      return *this;
    }

    _Ptr *n = p->right();
    // Jump to leftmost node of subtree rooted at n.
    while ((m = n->left()) != 0)
      n = m;
    _p = n;
    return *this;
  }

  /** Postfix increment: advance one node (post-order, i.e. bottom-up) */
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

/** Binary tree node */
class bt_node_base {
public:
  typedef bt_node_base _Self;
  typedef bt_node_base_iterator<bt_node_base> iterator;
  typedef bt_node_base_iterator<const bt_node_base> const_iterator;

public:
  _Self *_parent; /**< pointer to parent node */
  _Self *_left;   /**< pointer to left node */
  _Self *_right;  /**< pointer to right node */

  /** Default constructor */
  inline bt_node_base() : _parent(nullptr), _left(nullptr), _right(nullptr) {}

  // Compiler generated copy constructor and operator=() are fine

  /** @return whether this node is root */
  inline bool is_root() const { return parent() == nullptr; }

  /** @return whether this node is leaf */
  inline bool is_leaf() const {
    return left() == nullptr && right() == nullptr;
  }

  /** @return left pointer */
  inline _Self *left() { return _left; }

  /** @return right pointer */
  inline _Self *right() { return _right; }

  /** @return parent pointer */
  inline _Self *parent() { return _parent; }

  /** @return left pointer */
  inline const _Self *left() const { return _left; }

  /** @return right pointer */
  inline const _Self *right() const { return _right; }

  /** @return parent pointer */
  inline const _Self *parent() const { return _parent; }

  /** Left rotation. Pre-condition: right child of this node is not null.
      Note that rotation operations preserves the inorder numbering
      of the nodes */
  void rotate_left();

  /** Right rotation. Pre-condition: left child of this node is not null.
      Note that rotation operations preserves the inorder numbering
      of the nodes. */
  void rotate_right();

  /** Check if it is a valid tree (recursive function).
      Throw exception bad_bt_tree() if it is not. */
  inline void check_valid() const {
    check_valid_subtree(left());
    check_valid_subtree(right());
  }

  /** Equality */
  inline bool operator==(const _Self &rhs) const {
    if (this == &rhs)
      return true;
    return parent() == rhs.parent() && left() == rhs.left() &&
           right() == rhs.right();
  }

private:
  void check_valid_subtree(const _Self *child) const;
};

class bgt_node;

/** Iterator for Singly linked node. */
template <typename _Ref, typename _Ptr = _Ref> class bgt_node_iterator {
  // typedef forward_iterator_tag iterator_category;
  typedef bgt_node _Node;
  typedef bgt_node_iterator<_Ref, _Ptr> _Self;
  typedef bt_node_base_iterator<_Node> _Base;

public:
  bgt_node_iterator(_Node *x) : _it(x) {}
  bgt_node_iterator() : _it(nullptr) {}

  // Compiler generated copy constructor and operator=() are fine

  _Node *operator*() { return &(*_it); } // Return node pointer.
  _Node *operator->() { return &(*_it); }
  _Self &operator++() {
    ++_it;
    return *this;
  }
  _Self operator++(int) {
    _Self old(*this);
    ++*this;
    return old;
  }
  bool operator==(const _Self &x) const { return _it == x._it; }
  bool operator!=(const _Self &x) const { return _it != x._it; }

private:
  _Base _it; /**< base iterator */
};

/**
 * Binary tree node (for generating all binary trees by rotatins).
 * Reference: J. Lucas, D. Roelants van Baronaigien and F. Ruskey,
 * Generating Binary Trees by Rotations, J. Algorithms, 15 (1993)
 *  343-366.
 */
class bgt_node : public bt_node_base {
  friend class bin_gen_tree;
  friend class slicing_flrpln;

  typedef bgt_node _Self;
  typedef bt_node_base _Base;

public:
  typedef bgt_node_iterator<_Self> iterator;
  typedef bgt_node_iterator<const _Self> const_iterator;

private:
  _Self *_next;         /**< pointer to next node */
  mutable bool _direct; /**< whether direction is UP */
  mutable char _rank;

  /** Default constructor */
  inline bgt_node() : _Base(), _next(nullptr), _direct(false), _rank(0) {}

  // Compiler generated copy constructor and operator=() are fine

  /** @return left pointer */
  inline _Self *left() { return (_Self *)_left; }

  /** @return right pointer */
  inline _Self *right() { return (_Self *)_right; }

  /** @return parent pointer */
  inline _Self *parent() { return (_Self *)_parent; }

  /** @return left pointer */
  inline const _Self *left() const { return (_Self *)_left; }

  /** @return right pointer */
  inline const _Self *right() const { return (_Self *)_right; }

  /** @return parent pointer */
  inline const _Self *parent() const { return (_Self *)_parent; }

  /** @return whether it is on up stream. */
  inline bool is_up() const { return _direct; }

  /** Rotate this node in appropriate direction. */
  inline void rotate() {
    if (is_up())
      parent()->rotate_left();
    else
      parent()->rotate_right();
  }

  /** @return whether this node is done in its direction. */
  inline bool done_in_direction() const {
    return (!is_up() && left() == nullptr) ||
           (is_up() && parent() == nullptr) ||
           (is_up() && parent()->left() == this);
  }
};

// Forward declaration
template <typename _Tp> class bt_node;

/** Iterator for Singly linked node. */
template <typename _Tp, typename _Ref, typename _Ptr> class bt_node_iterator {
  // typedef forward_iterator_tag iterator_category;
  typedef bt_node<_Tp> _Node;
  typedef bt_node_iterator<_Tp, _Ref, _Ptr> _Self;
  typedef bt_node_base_iterator<bt_node<_Tp> > _Base;

public:
  bt_node_iterator(_Node *x) : _it(x) {}
  bt_node_iterator() : _it(0) {}

  _Node *operator*() { return &(*_it); }
  _Node *operator->() { return &(*_it); }

  _Self &operator++() {
    ++_it;
    return *this;
  }
  _Self operator++(int) {
    const _Self old(*this);
    ++*this;
    return old;
  }
  bool operator==(const _Self &x) const { return _it == x._it; }
  bool operator!=(const _Self &x) const { return _it != x._it; }

private:
  _Base _it; /**< base iterator */
};

/** Binary tree node with data. */
template <typename _Tp> class bt_node {
  typedef bt_node<_Tp> _Self;

public:
  typedef bt_node_iterator<_Tp, _Tp &, _Tp *> iterator;
  typedef bt_node_iterator<_Tp, const _Tp &, const _Tp *> const_iterator;

public:
  _Self *_parent; /**< pointer to parent node */
  _Self *_left;   /**< pointer to left node */
  _Self *_right;  /**< pointer to right node */
  _Tp _data;      /**< data that this node carries */

  /** Default constructor */
  inline bt_node()
      : _parent(nullptr), _left(nullptr), _right(nullptr), _data() {}

  // Compiler generated copy constructor and operator=() are fine

  /** @return data */
  inline _Tp &data() { return _data; }

  /** @return left pointer */
  inline _Self *left() { return (_Self *)_left; }

  /** @return right pointer */
  inline _Self *right() { return (_Self *)_right; }

  /** @return parent pointer */
  inline _Self *parent() { return (_Self *)_parent; }

  /** @return data */
  inline const _Tp &data() const { return _data; }

  /** @return left pointer */
  inline const _Self *left() const { return (_Self *)_left; }

  /** @return right pointer */
  inline const _Self *right() const { return (_Self *)_right; }

  /** @return parent pointer */
  inline const _Self *parent() const { return (_Self *)_parent; }

  /** @return whether this node is a root. */
  inline bool is_root() const { return parent() == nullptr; }

  /** @return whether this node is leaf */
  inline bool is_leaf() const {
    return left() == nullptr && right() == nullptr;
  }

  /** Equality */
  inline bool operator==(const _Self &rhs) const {
    if (this == &rhs)
      return true;
    return parent() == rhs.parent() && _data == rhs._data &&
           left() == rhs.left() && right() == rhs.right();
  }
};

/** @} */
}

#endif
