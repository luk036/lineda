#ifndef RECTI_PATH_HPP
#define RECTI_PATH_HPP

#include "point_set.hpp"
#include <algorithm>
#include <cassert>
#include <functional>
#include <iosfwd>
#include <vector>

/** Forward declarations */
class svgstream;

namespace recti {
/**
 * @addtogroup recti
 * @{
 */

/* Forward declaration */
template <typename _Tp> class Path;
template <typename _Tp> class Polygon;
template <typename _Tp> class RPolygon;
template <typename _Tp> class Rectangle;

template <typename _Tp>
std::ostream &operator<<(std::ostream &os, const Path<_Tp> &p);

typedef Path<int> path;

/**
 * Path Class. A rectilinearly oriented path. Implemented by a
 * vector of center points [p0, p1, p2, ... pN-1] and width. Simple
 * path (i.e. no crossing line segments) is assumed.
 */
template <typename _Tp> class Path : public Point_set<_Tp> {
  typedef std::vector<Point<_Tp>> pt_vec_type;
  typedef Path<_Tp> _Self;
  typedef Point_set<_Tp> _Base;

  friend std::ostream &operator<<<>(std::ostream &os, const Path<_Tp> &p);

public:
  /** Default constructor */
  inline Path() : _Base(), _width(0) {}

  /** Create a Path by a vector of points and width. */
  inline Path(const pt_vec_type &v, const _Tp &width)
      : _Base(v), _width(width) {}

  /** Create a Path by an array of points and width. */
  inline Path(const Point<_Tp> *a, size_t s, const _Tp &width)
      : _Base(a, s), _width(width) {}

  /** Create a Path by converting a rectangle r. (not implemented yet) */
  Path(const Rectangle<_Tp> &r);

  /** Create a rectilinear path with n vertices randomly.
      Time complexity O(n). */
  static _Self random(size_t n);

  // Compiler generated copy constructor and assignment operator are fine.

  /** @return the width of this Path */
  inline const _Tp &width() const { return _width; }

  /** @return the bounding box of this Path. (Not implemented yet).
      Time complexity: O(n). */
  Rectangle<_Tp> bounding_box() const;

  /** @return the length of this path (not implemented yet)
      Time complexity: O(n) */
  _Tp length() const;

  /** @return the area of this path (not implemented yet)
      Assume rectilinear path and no extension at the two end points.
      Time complexity: O(n) */
  inline _Tp area() const { return length() * width() * 2; }

  /** Minkowski sum with a point p and assign.
      Time complexity: O(n). */
  template <typename _Up> inline _Self &operator+=(const Point<_Up> &p) {
    _Base::operator+=(p);
    return *this;
  }

  /** Minkowski difference with a point p and assign
      Time complexity: O(n). */
  template <typename _Up> inline _Self &operator-=(const Point<_Up> &p) {
    _Base::operator-=(p);
    return *this;
  }

  // Comparison operators

  /** Test for equality.
      Pre-condition: two paths are in canonical form.
      Time complexity: O(n) */
  template <typename _Up> inline bool operator==(const Path<_Up> &p) const {
    return _Base::operator==(p) && width() == p.width();
  }

  /** @return whether the point q is inside this path.
      (not implemented yet) */
  template <typename _Up> bool contain(const Point<_Up> &q) const;

  /** @return whether the rectangle r overlaps with this path.
      (not implemented yet) */
  template <typename _Up> bool overlap(const Rectangle<_Up> &r) const;

  /** @return whether the path p overlaps with this path.
      (not implemented yet) */
  template <typename _Up> bool overlap(const Path<_Up> &p) const;

  /** @return pointer of a new polygon that represents the outline of
      this path. Client is responsible to delete the polygon
      Assume rectilinear path and no extension at the two end points.
      Time complexity is O(n). */
  RPolygon<_Tp> *create_outline() const;

private:
  _Tp _width; /**< width of the path */
};

/** Minkowski sum with a point. Time complexity: O(n). @see operator+=() */
template <typename _Tp>
inline Path<_Tp> operator+(Path<_Tp> lhs, const Point<_Tp> &p) {
  return lhs += p;
}

/** Minkowski diff with a point. Time complexity: O(n). @see operator-=() */
template <typename _Tp>
inline Path<_Tp> operator-(Path<_Tp> lhs, const Point<_Tp> &p) {
  return lhs -= p;
}

/** Test for inequality.
    Pre-condition: two paths are in canonical form.
    Time complexity: O(n)
    @see operator==()  */
template <typename _Tp, typename _Up>
inline bool operator!=(const Path<_Tp> &lhs, const Path<_Up> &rhs) {
  return !(lhs == rhs);
}

/** Print the path ph in SVG format.*/
template <typename _Tp>
svgstream &operator<<(svgstream &osvgs, const Path<_Tp> &ph);

/** @} */
}

#endif // RECTI_PATH_HPP
