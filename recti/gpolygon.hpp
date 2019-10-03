#ifndef RECTI_GPOLYGON_HPP
#define RECTI_GPOLYGON_HPP

#ifndef RECTI_POINT_HPP
#include "point.hpp"
#endif
#ifndef DSL_SCLIST_HPP
#include <sclist.hpp>
#endif

#include <algorithm>
#include <cassert>
#include <functional>
#include <iosfwd>
#include <list>
#include <vector>

class svgstream;

namespace recti {
/**
 * @addtogroup recti
 * @{
 */

/* Forward declarations */
template <typename _Tp> class GPolygon;
template <typename _Tp> class Rectangle;
template <typename _Tp>
svgstream &operator<<(svgstream &os, const GPolygon<_Tp> &p);

/** Define integer coordinate type gpolygon */
typedef GPolygon<int> gpolygon;

/**
 * GPolygon Class. A gpolygon.
 * Implemented by a vector of points [p0, p1, p2, ... pN-1].
 * Simple gpolygon (i.e. no crossing line segments) is assumed.
 */
template <typename _Tp> class GPolygon {
  typedef GPolygon<_Tp> _Self;
  typedef dsl::sclist<Point<_Tp>> pt_set_type;
  friend svgstream &operator<<<>(svgstream &os, const _Self &p);

public:
  static const char *_svg_style; /**< SVG style */

  /** Create a simple gpolygon with n vertices randomly. Reference: ???
      Time complexity O(n^5). */
  GPolygon(size_t n);

  // Compiler generated copy constructor and operator=() are fine.

  /** @return the number of vertices. */
  inline size_t num_vertices() const { return _pt_set.size(); }

  /** @return perimeter. (not implemented yet)
      Preconditon: gpolygon is in canonical form.
      Time complexity: O(n). */
  inline _Tp perimeter() const;

  /** @return signed area.
      Preconditon: gpolygon is in canonical form.
      Time complexity: O(n). */
  inline _Tp area() const;

  /** @return the bounding box of this gpolygon
      (not implemented yet) */
  Rectangle<_Tp> bounding_box() const;

  /**
   * @name Geometry transformations
   */
  //@{

  /** Translate this point_set by (delta_x, delta_y). */
  inline void translate(int delta_x, int delta_y);

  /** Rotate 90 degree anti-clockwise. */
  inline void rotate090();

  /** Rotate 180 degree anti-clockwise. */
  inline void rotate180();

  /** Rotate 270 degree anti-clockwise. */
  inline void rotate270();

  /** Flip against y-axis. */
  inline void flip_y();

  /** Rotate 90 degree anti-clockwise and then flip against y-axis. */
  inline void rotate090_then_flip_y();

  /** Rotate 180 degree anti-clockwise and then flip against y-axis. */
  inline void rotate180_then_flip_y();

  /** Rotate 270 degree anti-clockwise and then flip against y-axis. */
  inline void rotate270_then_flip_y();

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate270() { rotate090_then_flip_y(); }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate180() { rotate180_then_flip_y(); }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate090() { rotate270_then_flip_y(); }

  /** Flip against x-axis */
  inline void flip_x() { rotate180_then_flip_y(); }
  //@}

  /** @name Arithmetic */
  //@{
  /** Minkowski sum with a point p and assign.
      Time complexity: O(n). */
  template <typename _Up> inline _Self &operator+=(const Point<_Up> &p);

  /** Minkowski difference with a point p and assign.
      Time complexity: O(n). */
  template <typename _Up> inline _Self &operator-=(const Point<_Up> &p);

  /** Minkowski sum with a rectangle and assign.
      Precondition: gpolygon is convex and in canonical
      form. Time complexity: O(n). */
  template <typename _Up> _Self &operator+=(const Rectangle<_Up> &r);

  /** Minkowski sum with a rectangle.
      Precondition: gpolygon is convex and in canonical
      form. Time complexity: O(n).
      @see operator+=(). */
  // inline _Self operator+(const Rectangle<_Tp>& r) const
  // { return _Self(*this) += r; }

  //@}

  /** @name Comparison and Relations */
  //@{

  /** Test for equality. Assume two gpolygons are in canonical form. */
  template <typename _Up> inline bool operator==(const GPolygon<_Up> &p) const;

  /** Test for inequality. @see operator==() */
  template <typename _Up> inline bool operator!=(const GPolygon<_Up> &p) const;

  /** @return whether it is below p. */
  template <class geom_obj> inline bool below(const geom_obj &p) const {
    return bounding_box().below(p.bounding_box());
  }

  /** @return whether it is above p. */
  template <class geom_obj> inline bool above(const geom_obj &p) const {
    return bounding_box().above(p.bounding_box());
  }

  /** @return whether it is left of p */
  template <class geom_obj> inline bool left_of(const geom_obj &p) const {
    return bounding_box().left_of(p.bounding_box());
  }

  /** @return whether it is right of r */
  template <class geom_obj> inline bool right_of(const geom_obj &p) const {
    return bounding_box().right_of(p.bounding_box());
  }

  /** @return whether it is left-of and below p. */
  template <class geom_obj>
  inline bool left_and_below(const geom_obj &p) const {
    return bounding_box().left_and_below(p.bounding_box());
  }

  /** @return whether it is left-of or below p. */
  template <class geom_obj> inline bool left_or_below(const geom_obj &p) const {
    return bounding_box().left_or_below(p.bounding_box());
  }

  /** @return whether the point q is inside this gpolygon.
      Use ray intersection method. (not implemented yet) */
  template <typename _Up> bool contain(const Point<_Up> &q) const;

  /** @return whether the rectangle r overlaps with this gpolygon */
  template <typename _Up> bool overlap(const Rectangle<_Up> &r) const;

  /** @return whether the gpolygon poly overlaps with this gpolygon */
  template <typename _Up> bool overlap(const GPolygon<_Up> &poly) const;

  /** Convert a gpolygon in canonical form: i.e. the first point is
      left-bottom most and the sequence of points is in
      anti-clockwise order */
  void normalize();

  /** @return whether a gpolygon is in canonical form: i.e. the first point is
      left-bottom most and the sequence of points is in
      anti-clockwise order */
  bool is_normal() const;

  /** GPolygon cutting. Cut this gpolygon into a set of rectangles.
      (not implemented yet) */
  template <typename _Up> void cut(std::vector<Rectangle<_Up>> &rect_set);

protected:
  pt_set_type _pt_set;                            /**< point set */
  std::list<dsl::snode<Point<_Tp>> *> _tail_list; /**< list of polygons */
};

/** @} */

// Non-member inline functions
/** Translate this gpolygon by (delta_x, delta_y). */
template <typename _Tp>
inline void GPolygon<_Tp>::translate(int delta_x, int delta_y) {
  typedef dsl::sclist<Point<_Tp>> pt_set_type;
  typename pt_set_type::iterator it = _pt_set.begin();
  do {
    it->translate(delta_x, delta_y);
    ++it;
  } while (it != _pt_set.begin()); // assume circular list
}

/** Rotate 90 degree anti-clockwise.
    Post-condition: _ll is left below _ur */
template <typename _Tp> inline void GPolygon<_Tp>::rotate090() {
  _pt_set.apply(std::mem_fun_ref(&Point<_Tp>::rotate090));
}

/** Rotate 180 degree anti-clockwise.
    Post-condition: _ll is left below _ur */
template <typename _Tp> inline void GPolygon<_Tp>::rotate180() {
  _pt_set.apply(std::mem_fun_ref(&Point<_Tp>::rotate180));
}

/** Rotate 270 degree anti-clockwise.
    Post-condition: _ll is left below _ur */
template <typename _Tp> inline void GPolygon<_Tp>::rotate270() {
  _pt_set.apply(std::mem_fun_ref(&Point<_Tp>::rotate270));
}

/** Flip against y-axis.
    Post-condition: _ll is left below _ur */
template <typename _Tp> inline void GPolygon<_Tp>::flip_y() {
  _pt_set.apply(std::mem_fun_ref(&Point<_Tp>::flip_y));
}

/** Rotate 90 degree anti-clockwise and then flip against y-axis.
    Post-condition: _ll is left below _ur */
template <typename _Tp> inline void GPolygon<_Tp>::rotate090_then_flip_y() {
  _pt_set.apply(std::mem_fun_ref(&Point<_Tp>::rotate090_then_flip_y));
}

/** Rotate 180 degree anti-clockwise and then flip against y-axis.
    Post-condition: _ll is left below _ur */
template <typename _Tp> inline void GPolygon<_Tp>::rotate180_then_flip_y() {
  _pt_set.apply(std::mem_fun_ref(&Point<_Tp>::rotate180_then_flip_y));
}

/** Rotate 270 degree anti-clockwise and then flip against y-axis.
    Post-condition: _ll is left below _ur */
template <typename _Tp> inline void GPolygon<_Tp>::rotate270_then_flip_y() {
  _pt_set.apply(std::mem_fun_ref(&Point<_Tp>::rotate270_then_flip_y));
}

/** Minkowski sum with a point q and assign */
template <typename _Tp>
template <typename _Up>
inline GPolygon<_Tp> &GPolygon<_Tp>::operator+=(const Point<_Up> &q) {
  typedef dsl::sclist<Point<_Tp>> pt_set_type;
  typename pt_set_type::iterator it = _pt_set.begin();
  do {
    *it += q;
  } while (++it != _pt_set.begin()); // assume circular list
  return *this;
}

/** Minkowski difference with a point q and assign */
template <typename _Tp>
template <typename _Up>
inline GPolygon<_Tp> &GPolygon<_Tp>::operator-=(const Point<_Up> &q) {
  typedef dsl::sclist<Point<_Tp>> pt_set_type;
  typename pt_set_type::iterator it = _pt_set.begin();
  do {
    *it -= q;
  } while (++it != _pt_set.begin()); // assume circular list
  return *this;
}

/** Minkowski sum with a point. Time complexity: O(n). @see operator+=(). */
template <typename _Tp>
inline GPolygon<_Tp> operator+(GPolygon<_Tp> lhs, const Point<_Tp> &p) {
  return lhs += p;
}

/** Minkowski diff with a point. Time complexity: O(n). @see operator-=(). */
template <typename _Tp>
inline GPolygon<_Tp> operator-(GPolygon<_Tp> lhs, const Point<_Tp> &p) {
  return lhs -= p;
}

/** Test for equality. Precondition: two gpolygons are in canonical form. */
template <typename _Tp, typename _Up>
inline bool operator==(const GPolygon<_Tp> &p, const GPolygon<_Up> &q) {
  return p.get_pt_set() == q.get_pt_set();
}

/** Not equal. @see operator==() */
template <typename _Tp, typename _Up>
inline bool operator!=(const GPolygon<_Tp> &p, const GPolygon<_Up> &q) {
  return !(p == q);
}
}

#endif // RECTI_GPOLYGON_HPP
