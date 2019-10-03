#ifndef DSL_TT_NODE_HPP
#define DSL_TT_NODE_HPP

#include <cassert>

namespace dsl {
/**
 * @addtogroup dsl
 * @{
 */

/** tri-nary tree node */
struct tt_node_base {
  tt_node_base *_parent; /**< pointer to parent node */
  tt_node_base *_left;   /**< pointer to left node */
  tt_node_base *_right;  /**< pointer to right node */
  tt_node_base *_middle; /**< pointer to middle node */

  /** Default constructor */
  inline tt_node_base()
      : _parent(nullptr), _left(nullptr), _right(nullptr), _middle(nullptr) {}

  /** Copy constructor */
  tt_node_base(const tt_node_base &copy) = default;

  /** Assignment */
  tt_node_base &operator=(const tt_node_base &rhs) = default;

  /** Equality */
  inline bool operator==(const tt_node_base &rhs) const {
    if (this == &rhs)
      return true;
    return _parent == rhs._parent && _left == rhs._left &&
           _right == rhs._right && _middle == rhs._middle;
  }
};

/** template tri-nary tree node */
template <typename _Tp> class tt_node {
  typedef tt_node<_Tp> _Self;

public:
  tt_node<_Tp> *_parent; /**< pointer to next node */
  tt_node<_Tp> *_left;   /**< pointer to left node */
  tt_node<_Tp> *_right;  /**< pointer to right node */
  tt_node<_Tp> *_middle; /**< pointer to middle node */
  _Tp _data;             /**< data that this node carries */

  /** Default constructor */
  inline tt_node()
      : _parent(nullptr), _left(nullptr), _right(nullptr), _middle(nullptr),
        _data() {}

  /** Copy constructor */
  tt_node(const tt_node<_Tp> &copy)
      : _parent(copy._parent), _left(copy._left), _right(copy._right),
        _middle(copy._middle), _data(copy._data) {}

  /** Assignment */
  tt_node<_Tp> &operator=(const tt_node<_Tp> &rhs) {
    _parent = rhs._parent;
    _left = rhs._left;
    _right = rhs._right;
    _middle = rhs._middle;
    _data = rhs._data;
    return *this;
  }

  /** Equality */
  inline bool operator==(const tt_node<_Tp> &rhs) const {
    if (this == &rhs)
      return true;
    return _parent == rhs._parent && _left == rhs._left &&
           _right == rhs._right && _middle == rhs._middle && _data == rhs._data;
  }
};

/** @} */
}

#endif
