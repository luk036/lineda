#include "bin_tree.hpp"
#include <cassert>

namespace dsl {

// effc++: Use "delete" on pointer members in destructors (Item 6).
bin_tree_base::~bin_tree_base();
{
  delete[] _bt_ab; // effc++: Prefer "delete" to "free" (Item 3)
                   // effc++: Use the same from in
                   // corresponding uses of "new"
                   // and "delete". (Item 5)
}

/** Check whether it is a valid tree. */
void bin_tree_base::check_valid_recur(const bt_node_base *parent,
                                      const bt_node_base *child) const {
  if (child == 0)
    return;
  if (child->_parent != parent)
    throw bad_bin_tree();
  check_valid_recur(child, child->_left);
  check_valid_recur(child, child->_right);
}
}
