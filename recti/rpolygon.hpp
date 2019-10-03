#ifndef RECTI_RPOLYGON_HPP
#define RECTI_RPOLYGON_HPP

#ifndef RECTI_POLYGON_HPP
#include "polygon.hpp"
#endif

namespace recti {
/**
 * @addtogroup recti
 * @{
 */

/* Forward declarations */
template <typename _Tp> class RPolygon;

/** Define integer coordinate type polygon */
typedef RPolygon<int> rpolygon;

/**
 * Rectilinear Polygon Class. A rectilinearly oriented polygon.
 * Implemented by a vector of points [p0, p1, p2, ... pN-1].
 * Simple polygon (i.e. no crossing line segments) is assumed.
 */
template <typename _Tp> class RPolygon : public Polygon<_Tp> {
  typedef RPolygon<_Tp> _Self;
  typedef Polygon<_Tp> _Base;

public:
  typedef typename _Base::pt_set_type pt_set_type;
  typedef typename pt_set_type::node pnode;
  typedef typename pt_set_type::iterator iterator;
  typedef typename pt_set_type::const_iterator const_iterator;

public:
  static const char *_svg_style; /**< SVG style */

  /** Create a simple rectilinear polygon with n vertices randomly.
      Reference: ???
      Time complexity O(n^5). */
  static RPolygon<_Tp> random(size_t n);

  /** Create a polygon by a vector of points */
  inline RPolygon(const pt_set_type &v) : Polygon<_Tp>(v) {}

  /** Create a polygon by an array of points. */
  RPolygon(const Point<_Tp> *a, size_t s) : Polygon<_Tp>(a, s) {}

  /** Create a polygon by converting a rectangle r. */
  RPolygon(const Rectangle<_Tp> &r) : Polygon<_Tp>(r) {}

  // Compiler generated copy constructor and operator=() are fine.

  /** @return signed area.
      Preconditon: polygon is in canonical form.
      Time complexity: O(n). */
  inline _Tp area() const;

  /** @name Comparison and Relations */
  //@{

  /** Polygon cutting. Cut this polygon into a set of rectangles.
      (not implemented yet) */
  template <typename _Up> void cut(std::vector<Rectangle<_Up>> &rect_set);

  /** Enlarge the polygon by delta. Precondition: polygon is
      rectilinearly convex and in canonical form, delta >=0 */
  // xxx void enlarge(int delta);

  /** Shrink the polygon by delta. Precondition: polygon is
      rectilinearly and in canonical form, delta >=0 */
  void shrink(const _Tp &delta);

  /** Create a rectilinear polygon with n vertices randomly.
      Time complexity O(n). */
  static pt_set_type random1(size_t n);
};

/** @} */
}

#endif // RECTI_RPOLYGON_HPP
