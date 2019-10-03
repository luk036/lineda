#include "bt_node.hpp"
#include <boost/static_assert.hpp>
#include <cassert>

namespace dsl {

// Don't increase the size of bt_node_base as it is used
// as a base in many classes.
BOOST_STATIC_ASSERT(sizeof(bt_node_base) <= 3 * sizeof(void *));

void bt_node_base::check_valid_subtree(const bt_node_base *child) const {
  if (child == nullptr)
    return;
  if (child->parent() != this)
    throw bad_bt_tree();
  child->check_valid_subtree(child->left());
  child->check_valid_subtree(child->right());
}

// See the classic book "Introduction to Algorithms, MIT"
// section 14.2 by T.H.Cormen, C.E.Leiserson and R.L.Rivest, 1990
void bt_node_base::rotate_left() {
  assert(_right != 0);

  bt_node_base *y = _right; // set y
  _right = y->_left;        // turn y's left subtree into this
  // node's right subtree
  if (_right != nullptr)
    _right->_parent = this;
  y->_parent = _parent; // Link this node's parent to y
  if (_parent != nullptr) {
    if (_parent->_left == this)
      _parent->_left = y;
    else
      _parent->_right = y;
  }
  y->_left = this; // put this node on y's left.
  _parent = y;
}

// See the classic book "Introduction to Algorithm, MIT"
// section 14.2 by T.H.Cormen, C.E.Leiserson and R.L.Rivest, 1990
void bt_node_base::rotate_right() {
  assert(_left != 0);

  bt_node_base *x = _left; // set x
  _left = x->_right;       // turn x's right subtree into this
  // node's left subtree
  if (_left != nullptr)
    _left->_parent = this;
  x->_parent = _parent; // Link this node's parent to x
  if (_parent != nullptr) {
    if (_parent->_right == this)
      _parent->_right = x;
    else
      _parent->_left = x;
  }
  x->_right = this; // put this node on x's right.
  _parent = x;
}

// Explicit instantiations
template class bt_node<int>;
}
