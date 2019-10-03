#ifndef RECTI_GRPOLYGON_HPP
#define RECTI_GRPOLYGON_HPP

#include "point.hpp"
#include <algorithm>
#include <cassert>
#include <dclist.hpp>
#include <functional>
#include <iosfwd>
#include <list>

class svgstream;

namespace recti {
/**
 * @addtogroup recti
 * @{
 */

/* Forward declarations */
template <typename _Tp> class GRPolygon;
template <typename _Tp>
svgstream &operator<<(svgstream &os, const GRPolygon<_Tp> &p);

/** Define integer coordinate type polygon */
typedef GRPolygon<int> grpolygon;

/**
 * Rectilinear Polygon Class. A rectilinearly oriented polygon.
 * Implemented by a vector of points [p0, p1, p2, ... pN-1].
 * Simple polygon (i.e. no crossing line segments) is assumed.
 */
template <typename _Tp> class GRPolygon {
  typedef GRPolygon<_Tp> _Self;
  typedef typename dsl::dclist<Point<_Tp>> pt_set_type;
  typedef typename pt_set_type::node pnode;
  friend svgstream &operator<<<>(svgstream &os, const GRPolygon<_Tp> &p);

public:
  static const char *_svg_style; /**< SVG style */

  /** Create randomly a set of a simple rectilinear polygon with
      total n vertices.
      Reference: ???
      Time complexity O(n^5). */
  GRPolygon(size_t n);

  // Compiler generated copy constructor and operator=() are fine.

  /** @return the number of vertices. */
  inline size_t num_vertices() const { return _pt_set.size(); }

private:
  pt_set_type _pt_set;           /**< point set */
  std::list<pnode *> _tail_list; /**< list of polygons */
};

/** @} */
}

#endif // RECTI_GRPOLYGON_HPP
