#include "pt_stree.hpp"
#include <iostream>

namespace cgl {

/** Print out the tree in .svg format */
std::ostream &operator<<(std::ostream &os, const pt_stree &cs) {
  for (size_t i = 0; i < cs.num_nodes(); ++i) {
    os << "<circle"
       << " cx=\"" << cs._sa[i]._data.x() << "\""
       << " cy=\"" << cs._sa[i]._data.y() << "\""
       << " r=\"10\" style=\"fill:red;stroke:black\""
       << "/>\n";
  }
  for (size_t i = 1; i < cs.num_nodes(); ++i) {
    os << "<line"
       << " x1=\"" << cs._sa[i]._data.x() << "\""
       << " y1=\"" << cs._sa[i]._data.y() << "\""
       << " x2=\"" << cs._sa[i]._next->_data.x() << "\""
       << " y2=\"" << cs._sa[i]._next->_data.y() << "\""
       << " style=\"fill:none;stroke:black\""
       << "/>\n";
  }

  return os;
}
}
