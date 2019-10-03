#include "rectangle.hpp"
#include <svgstream.hpp>

using numeric::interval;
using std::max;
using std::min;
using std::ostream;
/* sdfds sdfsf  */
namespace recti {
template <typename _Tp>
const Rectangle<_Tp> Rectangle<_Tp>::_zero = Rectangle<_Tp>();

template <typename _Tp>
std::ostream &operator<<(std::ostream &os, const Rectangle<_Tp> &r) {
  os << "(" << r.ll() << " " << r.ur() << ")";
  return os;
}

// Specialization
template <> const char *Rectangle<int>::_svg_style = "fill:none;stroke:black";

// Print out in SVG format
template <> svgstream &operator<<(svgstream &osvgs, const Rectangle<int> &r) {
  osvgs << "<rect"
        << " x=\"" << r.llx() << "\""
        << " y=\"" << r.lly() << "\""
        << " width=\"" << r.width() << "\""
        << " height=\"" << r.height() << "\""
        << " style=\"" << rectangle::_svg_style << "\""
        << "/>";
  return osvgs;
}

/** Explicit instantiation */
template class Rectangle<int>;
template class Rectangle<interval<int>>;
template ostream &operator<<(ostream &os, const Rectangle<int> &r);
template ostream &operator<<(ostream &os, const Rectangle<interval<int>> &r);
}
