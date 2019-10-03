#include "pt_tri_tree.hpp"
#include <iostream>

using recti::point;

namespace cgl {

/** Print out the tree in .svg format */
std::ostream &operator<<(std::ostream &os, const pt_tri_tree &cs) {
  for (size_t i = 0; i < cs.num_nodes(); ++i) {
    os << "<circle"
       << " cx=\"" << cs._tri_a[i]._data.x() << "\""
       << " cy=\"" << cs._tri_a[i]._data.y() << "\""
       << " r=\"10\" style=\"fill:red;stroke:black\""
       << "/>\n";
  }
  for (size_t i = 0; i < cs.num_nodes(); ++i) {
    const dsl::tt_node<point> &s = cs._tri_a[i];
    if (s._parent == nullptr)
      continue;
    os << "<line"
       << " x1=\"" << s._data.x() << "\""
       << " y1=\"" << s._data.y() << "\""
       << " x2=\"" << s._parent->_data.x() << "\""
       << " y2=\"" << s._parent->_data.y() << "\""
       << " style=\"fill:none;stroke:black\""
       << "/>\n";
  }

  return os;
}
}
