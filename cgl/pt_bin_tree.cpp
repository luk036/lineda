#include "pt_bin_tree.hpp"
#include <svgstream.hpp>

using namespace dsl;
using namespace recti;

namespace cgl {

/** @return the cost between two points. */
double pt_bin_tree::cost(const bt_node_base &a, const bt_node_base &b) const {
  const point pa(((const bt_node<point> &)a)._data);
  const point pb(((const bt_node<point> &)b)._data);
  return distance(pa, pb);
}

/** Print out the tree in .svg format */
svgstream &operator<<(svgstream &os, const pt_bin_tree &cs) {
  // Draw node in circle
  for (size_t i = 0; i < cs.num_nodes(); ++i) {
    os << cs.get_node(i)._data << "\n";
  }

  // Draw root with different color
  const char *point_style = point::_svg_style;
  point::_svg_style = "fill:green;stroke:blue";
  os << cs.get_root()._data << "\n";
  point::_svg_style = point_style;

  // Draw the line between nodes
  for (size_t i = 0; i < cs.num_nodes(); ++i) {
    const dsl::bt_node<point> &s = cs.get_node(i);
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
