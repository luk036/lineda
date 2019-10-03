#include "point.hpp"
#include <boost/static_assert.hpp>
#include <interval.hpp> // for testing only
#include <svgstream.hpp>

using numeric::interval;
using std::ostream;

namespace recti {

// Don't increase the size.
BOOST_STATIC_ASSERT(sizeof(Point<int>) <= 2 * sizeof(int));

template <typename _Tp> const Point<_Tp> Point<_Tp>::_origin = Point<_Tp>();

template <typename _Tp>
const char *Point<_Tp>::_svg_style = "fill:red;stroke:black";

template <typename _Tp> int Point<_Tp>::_svg_radius = 8;

template <typename _Tp>
svgstream &operator<<(svgstream &osvgs, const Point<_Tp> &p) {
  osvgs << "<circle"
        << " cx=\"" << p.x() << "\""
        << " cy=\"" << p.y() << "\""
        << " r=\"" << Point<int>::_svg_radius << "\""
        << " style=\"" << Point<int>::_svg_style << "\""
        << "/>";
  return osvgs;
}

void example_duPoint() {
  Point<int> p(3, 4);
  duPoint<int> q = p;
  // Point<int> r = q;
}

/** Explicit instantiation */
template class Point<int>;
template ostream &operator<<(ostream &os, const Point<int> &p);
template class Point<interval<int>>;
template ostream &operator<<(ostream &os, const Point<interval<int>> &p);
// xxx template const char* Point<int>::_svg_style;
// xxx template int Point<int>::_svg_radius;
template const char *Point<double>::_svg_style;
template int Point<double>::_svg_radius;

template svgstream &operator<<(svgstream &os, const Point<int> &p);
template svgstream &operator<<<double>(svgstream &os, const Point<double> &p);
}
