#include "tri_tree.hpp"
#include <cassert>

namespace dsl {

/* Check whether it is a valid tree. */
void tri_tree_base::check_valid_recur(const tt_node_base *parent,
                                      const tt_node_base *child) const {
  if (child == nullptr)
    return;
  if (child->_parent != parent)
    throw bad_tri_tree();
  check_valid_recur(child, child->_left);
  check_valid_recur(child, child->_right);
  check_valid_recur(child, child->_middle);
}

/** Minimum spanning tree */
double tri_tree_base::mst(size_t idx) {
  double res = sl_base::mst(idx);
  _root = _tri_ab[idx];

  // Connect all pointers
  for (size_t i = 0; i < num_nodes(); ++i) {
    if (i == idx)
      continue;
    tt_node_base *const s1 = _tri_ab[i];
    tt_node_base *const parent = s1->_parent;
    assert(parent->_middle == 0); // at most three children
    if (parent->_left == nullptr)
      parent->_left = s1;
    else if (parent->_right == nullptr)
      parent->_right = s1;
    else
      parent->_middle = s1;
  }

  assert(is_valid());
  return res;
}

// Explicit instantiations
template class tri_tree<int>;
}
